//
// Created by f2shafie on 20/11/24.
//

#define DEFINE_UPF_STORAGE
#include "ee_event.h"
#undef DEFINE_UPF_STORAGE
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION
#include "upf_prepare_data.h"
#define DEFINE_UPF_SHARED
#include "shared_variables.h"
#include "btree.h"
#include "delta_queue.h"
#undef DEFINE_UPF_SHARED

#include <arpa/inet.h>
#include <netinet/in.h>

#include <endian.h>
#include <netinet/ip.h>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
#define DELTA_FLUSH_INTERVAL_NS 900000000ULL

static cvector_vector_type(usage_report_per_flow_t *) dirty_reports = NULL;
static pthread_once_t delta_flush_once = PTHREAD_ONCE_INIT;

static char *dup_string(const char *src) {
  if (!src) return NULL;
  size_t len = strlen(src) + 1;
  char *dst = malloc(len);
  if (!dst) return NULL;
  memcpy(dst, src, len);
  return dst;
}

static bool publish_flow_delta(usage_report_per_flow_t *rep) {
  if (!rep) return true;
  if (rep->ul_bytes_rate == 0 && rep->dl_bytes_rate == 0 &&
      rep->ul_pkts_rate == 0 && rep->dl_pkts_rate == 0) {
    rep->pending_flush = false;
    return true;
  }

  flow_delta_entry *delta = calloc(1, sizeof(*delta));
  if (!delta) return false;

  delta->str_key = dup_string(rep->str_key);
  if (!delta->str_key) {
    flow_delta_entry_free(delta);
    return false;
  }

  delta->ul_bytes_delta += rep->ul_bytes_rate;
  delta->ul_pkts_delta += rep->ul_pkts_rate;
  delta->dl_bytes_delta += rep->dl_bytes_rate;
  delta->dl_pkts_delta += rep->dl_pkts_rate;

  if (!rep->announced) {
    delta->is_new = true;
    delta->usubid = dup_string(rep->key->usubid);
    delta->ue_ip = dup_string(rep->key->ue_ip);
    delta->dst_ip = dup_string(rep->key->dst_ip);
    if (!delta->usubid || !delta->ue_ip || !delta->dst_ip) {
      flow_delta_entry_free(delta);
      return false;
    }
    delta->seid = rep->key->seid;
    delta->src_port = rep->key->src_port;
    delta->dst_port = rep->key->dst_port;
    delta->proto = rep->key->proto;
    delta->start_time = rep->start_time;
    delta->timestamp = rep->timestamp;
  }

  if (!flow_delta_enqueue(delta)) {
    flow_delta_entry_free(delta);
    return false;
  }

  rep->pending_flush = false;
  rep->announced = true;
  rep->ul_bytes_rate = 0;
  rep->ul_pkts_rate = 0;
  rep->dl_bytes_rate = 0;
  rep->dl_pkts_rate = 0;
  return true;
}

static void flush_dirty_reports_locked(void) {
  size_t count = cvector_size(dirty_reports);
  if (count == 0) return;
  size_t keep = 0;
  for (size_t i = 0; i < count; ++i) {
    usage_report_per_flow_t *rep = dirty_reports[i];
    if (!publish_flow_delta(rep)) {
      dirty_reports[keep++] = rep;
    }
  }
  cvector_set_size(dirty_reports, keep);
}

static void *delta_flush_worker(void *arg) {
  (void)arg;
  const struct timespec interval = {
      .tv_sec = DELTA_FLUSH_INTERVAL_NS / 1000000000ULL,
      .tv_nsec = DELTA_FLUSH_INTERVAL_NS % 1000000000ULL,
  };
  while (true) {
    nanosleep(&interval, NULL);
    pthread_mutex_lock(&subID_lock);
    flush_dirty_reports_locked();
    pthread_mutex_unlock(&subID_lock);
  }
  return NULL;
}

static void start_delta_flush_worker(void) {
  pthread_t tid;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  if (pthread_create(&tid, &attr, delta_flush_worker, NULL) != 0) {
    ogs_error("delta_flush_worker: failed to start");
  }
  pthread_attr_destroy(&attr);
}

static void ensure_delta_flush_worker(void) {
  pthread_once(&delta_flush_once, start_delta_flush_worker);
}

static void schedule_report_for_flush(usage_report_per_flow_t *rep) {
  if (!rep || rep->pending_flush) return;
  cvector_push_back(dirty_reports, rep);
  rep->pending_flush = true;
}
static int decode_ipv6_header(
        struct ip6_hdr *ip6_h, uint8_t *proto, uint16_t *hlen)
{
  int done = 0;
  uint8_t *p, *jp, *endp;
  uint8_t nxt;          /* Next Header */

  ogs_assert(ip6_h);
  ogs_assert(proto);
  ogs_assert(hlen);

  nxt = ip6_h->ip6_nxt;
  p = (uint8_t *)ip6_h + sizeof(*ip6_h);
  endp = p + be16toh(ip6_h->ip6_plen);

  jp = p + sizeof(struct ip6_hbh);
  if (p == endp) { /* Jumbo Frame */
    uint32_t jp_len = 0;
    struct ip6_opt_jumbo *jumbo = NULL;

    ogs_assert(nxt == 0);

    jumbo = (struct ip6_opt_jumbo *)jp;
    memcpy(&jp_len, jumbo->ip6oj_jumbo_len, sizeof(jp_len));
    jp_len = be32toh(jp_len);
    switch (jumbo->ip6oj_type) {
      case IP6OPT_JUMBO:
        endp = p + jp_len;
        break;
      case 0:
        jp++;
        break;
      default:
        jp += (sizeof(struct ip6_opt) + jp_len);
        break;
    }
  }

  while (p < endp) {
    struct ip6_ext *ext = (struct ip6_ext *)p;
    switch (nxt) {
      case IPPROTO_HOPOPTS:
      case IPPROTO_ROUTING:
      case IPPROTO_DSTOPTS:
      case 135: /* mobility */
      case 139: /* host identity, experimental */
      case 140: /* shim6 */
      case 253: /* testing, experimental */
      case 254: /* testing, experimental */
        p += ((ext->ip6e_len << 3) + 8);
        break;
      case IPPROTO_FRAGMENT:
        p += sizeof(struct ip6_frag);
        break;
      case IPPROTO_AH:
        p += ((ext->ip6e_len + 2) << 2);
        break;
      default: /* Upper Layer */
        done = 1;
        break;

    }
    if (done)
      break;

    nxt = ext->ip6e_nxt;
  }

  *proto = nxt;
  *hlen = p - (uint8_t *)ip6_h;

  return OGS_OK;
}

void make_str_key(flow_key *key, char *str_key) {
  snprintf(str_key, 230, "%s|%llu|%s|%s|%hu|%hu|%u",
           key->usubid,
           (unsigned long long)key->seid,
           key->ue_ip, key->dst_ip,
           key->src_port, key->dst_port, key->proto);
}

void convert_ipv4_uint32_to_string(uint32_t src_addr, char *key_src) {
  fatemeh_log("the ip in int is %d", src_addr);
  uint8_t byte1 = (src_addr >> 24) & 0xFF;
  uint8_t byte2 = (src_addr >> 16) & 0xFF;
  uint8_t byte3 = (src_addr >> 8) & 0xFF;
  uint8_t byte4 = src_addr & 0xFF;

  snprintf(key_src, INET_ADDRSTRLEN, "%u.%u.%u.%u", byte4, byte3, byte2, byte1);

}
void prepare_data_ee(bool Uplink, uint64_t seid, ogs_pkbuf_t *pkbuf){
  pthread_mutex_lock(&subID_lock);
  ensure_delta_flush_worker();
  if (mymap == NULL) {
    mymap = new_str_btree();
  }

  int len_sub = cvector_size(subIDList);
  for(int i=0;i<len_sub;i++){
    flow_key * key = malloc(sizeof (flow_key));
    struct ip *ip_h = NULL;
    struct ip6_hdr *ip6_h = NULL;
    uint64_t p_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);
    ip_h = (struct ip *)pkbuf->data;
    key->usubid = malloc(strlen(subIDList[i]) + 1);
    strcpy(key->usubid , subIDList[i]);
    if (ip_h->ip_v == 4) {
      ip_h = (struct ip *)pkbuf->data;
      ip6_h = NULL;
      proto = ip_h->ip_p;
      ip_hlen = (ip_h->ip_hl)*4;
      key->dst_ip = malloc(INET_ADDRSTRLEN);
      key->ue_ip = malloc(INET_ADDRSTRLEN);
      if(Uplink){
        inet_ntop(AF_INET, &ip_h->ip_src, key->ue_ip, INET_ADDRSTRLEN);
        inet_ntop(AF_INET, &ip_h->ip_dst, key->dst_ip, INET_ADDRSTRLEN);
      }
      else{
          inet_ntop(AF_INET, &ip_h->ip_src, key->dst_ip, INET_ADDRSTRLEN);
          inet_ntop(AF_INET, &ip_h->ip_dst, key->ue_ip, INET_ADDRSTRLEN);

      }
      p_len = ntohs(ip_h->ip_len);
    }else if (ip_h->ip_v == 6) {
      ip_h = NULL;
      ip6_h = (struct ip6_hdr *)pkbuf->data;
      decode_ipv6_header(ip6_h, &proto, &ip_hlen);
      key->dst_ip = malloc( INET6_ADDRSTRLEN);
      key->ue_ip = malloc(INET6_ADDRSTRLEN);
      if(Uplink){
        inet_ntop(AF_INET6, &ip6_h->ip6_src, key->ue_ip,  INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &ip6_h->ip6_dst, key->dst_ip, INET6_ADDRSTRLEN);
      }
      else{
        inet_ntop(AF_INET6, &ip6_h->ip6_src, key->dst_ip, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &ip6_h->ip6_dst, key->ue_ip,  INET6_ADDRSTRLEN);
      }
      p_len = ntohs(ip6_h->ip6_ctlun.ip6_un1.ip6_un1_plen) + 40;

    } else {
      ogs_error("[upf_prepare_data]Invalid packet [IP version:%d, Packet Length:%d]",
                ip_h->ip_v, pkbuf->len);
      ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
      pthread_mutex_unlock(&subID_lock);
      return;
    }

    key->proto = proto;
    if(key->proto == TCP_PROTOCOL){
      struct tcphdr *tcph = (struct tcphdr *)((char *)pkbuf->data + ip_hlen);
      if(Uplink){
        key->src_port = be16toh(tcph->th_sport);
        key->dst_port = be16toh(tcph->th_dport);
      }else{
        key->dst_port = be16toh(tcph->th_sport);
        key->src_port = be16toh(tcph->th_dport);
      }


    }
    else if (key->proto == IPPROTO_UDP) {
      struct udphdr *udph = (struct udphdr *)((char *)pkbuf->data + ip_hlen);
      if(Uplink){
        key->src_port = ntohs(udph->uh_sport);
        key->dst_port = ntohs(udph->uh_dport);

      }else{
        key->dst_port = ntohs(udph->uh_sport);
        key->src_port = ntohs(udph->uh_dport);
      }

    }
    else{
      key->src_port = 0;
      key->dst_port = 0;
    }
    key->seid = seid;

    char * str_key = malloc(230);
    memset(str_key, 0, 230);
    make_str_key(key, str_key);
    usage_report_per_flow_t * rep =  btreemap_get(mymap,str_key);
    if(rep == NULL){
      rep = malloc(sizeof (usage_report_per_flow_t));
      rep->str_key = str_key;
      rep->key = key;
      if(Uplink){
        rep->ul_bytes = p_len;
        rep->ul_pkts = 1;
        rep->ul_bytes_rate = p_len;
        rep->ul_pkts_rate = 1;
        rep->dl_bytes = 0;
        rep->dl_pkts = 0;
        rep->dl_bytes_rate = 0;
        rep->dl_pkts_rate = 0;
        rep->peak_dl_bytes = 0;
        rep->peak_dl_pkts = 0;
        rep->peak_ul_bytes = 0;
        rep->peak_ul_pkts = 0;
        time_t current_time;
        time(&current_time);
        rep->start_time = current_time;
        rep->timestamp = current_time;

      } else{
        rep->dl_bytes = p_len;
        rep->dl_pkts = 1;
        rep->dl_bytes_rate = p_len;
        rep->dl_pkts_rate = 1;
        rep->ul_bytes = 0;
        rep->ul_pkts = 0;
        rep->ul_bytes_rate = 0;
        rep->ul_pkts_rate = 0;
        rep->peak_dl_bytes = 0;
        rep->peak_dl_pkts = 0;
        rep->peak_ul_bytes = 0;
        rep->peak_ul_pkts = 0;
        time_t current_time;
        time(&current_time);
        rep->start_time = current_time;
        rep->timestamp = current_time;
      }
      btreemap_insert_str(mymap,str_key, rep);
      rep->pending_flush = false;
      rep->announced = false;
      schedule_report_for_flush(rep);
    }
    else{
      free(str_key);
      free(key->usubid);
      free(key->ue_ip);
      free(key->dst_ip);
      free(key);
      if(Uplink){
        rep->ul_bytes += p_len;
        rep->ul_pkts += 1;
        rep->ul_bytes_rate += p_len;
        rep->ul_pkts_rate += 1;

      } else{
        rep->dl_bytes += p_len;
        rep->dl_pkts += 1;
        rep->dl_bytes_rate += p_len;
        rep->dl_pkts_rate += 1;
      }
      schedule_report_for_flush(rep);
    }

  }
  pthread_mutex_unlock(&subID_lock);

}
