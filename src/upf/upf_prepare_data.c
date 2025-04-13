/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
//
// Created by Fatemeh Shafiei Ardestani on 20/11/24.
//

//#include "upf-ee/types/types.h"
//#define DEFINE_UPF_SHARED
//#include "upf-ee/storage/shared_variables.h"
//#undef DEFINE_UPF_SHARED
//
#define DEFINE_UPF_STORAGE
#include "ee_event.h"
#undef DEFINE_UPF_STORAGE
//
#define STB_DS_IMPLEMENTATION
#include "stb_ds.h"
#undef STB_DS_IMPLEMENTATION
//#include "upf/upf-ee/EE-init.h"
#include "upf_prepare_data.h"
#define DEFINE_UPF_SHARED
#include "shared_variables.h"
#include "btree.h"
#undef DEFINE_UPF_SHARED
//#include "xxhash.h"

#define TCP_PROTOCOL 6
#define UDP_PROTOCOL 17
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
  while (p == endp) { /* Jumbo Frame */
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
void make_str_key(flow_key * key, char * str_key){
  fatemeh_log("in the make_str_key function.");
  memset(str_key, 0, 230);
  int str_key_zie = 0;
  char * seid = malloc(10);
  sprintf(seid, "%llu", key->seid);
  char * src_port = malloc(10);
  sprintf(src_port, "%hu", key->src_port);
  char * dst_port = malloc(10);
  sprintf(dst_port, "%hu", key->dst_port);
  char * proto = malloc(10);
  sprintf(proto, "%hu", key->proto);
  fatemeh_log("[DEBUG] key->sub_id is %s ", key->usubid);
  fatemeh_log("[DEBUG] seid is %s ", seid);
  fatemeh_log("[DEBUG] key->ue_ip is %s ", key->ue_ip);
  fatemeh_log("[DEBUG] key->dst_ip is %s ", key->dst_ip);
  fatemeh_log("[DEBUG] src_port is %s ", src_port);
  fatemeh_log("[DEBUG] dst_port is %s ", dst_port);
  fatemeh_log("[DEBUG] proto is %s ", proto);  

  sprintf(str_key,"%s%s%s%s%s%s%s", key->usubid, seid, key->ue_ip, key->dst_ip, src_port, dst_port, proto);
  fatemeh_log("the created hash key is %s", str_key);
  return;

}

void convert_ipv4_uint32_to_string(uint32_t src_addr, char *key_src) {
  fatemeh_log("the ip in int is %d", src_addr);
  uint8_t byte1 = (src_addr >> 24) & 0xFF;
  uint8_t byte2 = (src_addr >> 16) & 0xFF;
  uint8_t byte3 = (src_addr >> 8) & 0xFF;
  uint8_t byte4 = src_addr & 0xFF;

  snprintf(key_src, INET_ADDRSTRLEN, "%u.%u.%u.%u", byte4, byte3, byte2, byte1);

}
void convert_ipv6_to_string(uint8_t src_addr[16], char *key_src) {
  fatemeh_log("trying to convert 1p v6 to int ");
  struct in6_addr ip6_addr;
  memcpy(ip6_addr.s6_addr, src_addr, 16);

  if (inet_ntop(AF_INET6, &ip6_addr, key_src, INET6_ADDRSTRLEN) != NULL) {
  } else {
    fatemeh_log("inet_ntop");
  }
}
void prepare_data_ee(bool Uplink, uint64_t seid, ogs_pkbuf_t *pkbuf){

  if (mymap == NULL) {
    mymap = new_str_btree();
  }
  pthread_mutex_lock(&subID_lock);
  int len_sub = cvector_size(subIDList);
  for(int i=0;i<len_sub;i++){
    flow_key * key = malloc(sizeof (flow_key));
    struct ip *ip_h = NULL;
    struct ip6_hdr *ip6_h = NULL;
    uint32_t *src_addr = NULL;
    uint32_t *dst_addr = NULL;
    uint64_t p_len = 0;
    uint8_t proto = 0;
    uint16_t ip_hlen = 0;
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->len);
    ogs_assert(pkbuf->data);
    ip_h = (struct ip *)pkbuf->data;
    key->usubid = malloc(8);
    strcpy(key->usubid , subIDList[i]);
    if (ip_h->ip_v == 4) {
      ip_h = (struct ip *)pkbuf->data;
      ip6_h = NULL;
      proto = ip_h->ip_p;
      ip_hlen = (ip_h->ip_hl)*4;
      src_addr = (void *)&ip_h->ip_src.s_addr;
      dst_addr = (void *)&ip_h->ip_dst.s_addr;
      key->dst_ip = malloc(16 * sizeof(char));
      key->ue_ip = malloc(16 * sizeof(char));
      if(Uplink){
        convert_ipv4_uint32_to_string(*src_addr,key->ue_ip);
        convert_ipv4_uint32_to_string(*dst_addr,key->dst_ip);
      }
      else{
        convert_ipv4_uint32_to_string(*src_addr,key->dst_ip);
        convert_ipv4_uint32_to_string(*dst_addr,key->ue_ip);

      }
      p_len = ip_h->ip_len;
    }else if (ip_h->ip_v == 6) {
      ip_h = NULL;
      ip6_h = (struct ip6_hdr *)pkbuf->data;
      decode_ipv6_header(ip6_h, &proto, &ip_hlen);
      src_addr = (void *)ip6_h->ip6_src.s6_addr;
      dst_addr = (void *)ip6_h->ip6_dst.s6_addr;
      key->dst_ip = malloc( 40 * sizeof(char));
      key->ue_ip = malloc(40 * sizeof(char));
      if(Uplink){
        convert_ipv6_to_string((uint8_t *)src_addr,  key->ue_ip);
        convert_ipv6_to_string((uint8_t *)dst_addr,  key->dst_ip);
      }
      else{
        convert_ipv6_to_string((uint8_t *)src_addr,  key->dst_ip);
        convert_ipv6_to_string((uint8_t *)dst_addr,  key->ue_ip);
      }
      p_len = ip6_h->ip6_ctlun.ip6_un1.ip6_un1_plen + 40;

    } else {
      ogs_error("[upf_prepare_data]Invalid packet [IP version:%d, Packet Length:%d]",
                ip_h->ip_v, pkbuf->len);
      ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
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
      struct udphdr *udph = (struct udphdr *)((char *)pkbuf->data + ip_h->ip_hl * 4);
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
    }
    else{
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
    }

  }
  pthread_mutex_unlock(&subID_lock);

}
//void prepare_data_ee_opt(bool Uplink, uint64_t seid, ogs_pkbuf_t *pkbuf){
//  if (mymap == NULL) {
//    mymap = new_u64_btree();
//  }
//  flow_id  key;
//  struct ip *ip_h = NULL;
//  struct ip6_hdr *ip6_h = NULL;
//  uint32_t *src_addr = NULL;
//  uint32_t *dst_addr = NULL;
//  uint64_t p_len = 0;
//  uint8_t proto = 0;
//  uint16_t ip_hlen = 0;
//  ogs_assert(pkbuf);
//  ogs_assert(pkbuf->len);
//  ogs_assert(pkbuf->data);
//  ip_h = (struct ip *)pkbuf->data;
//  if (ip_h->ip_v == 4) {
//    ip_h = (struct ip *)pkbuf->data;
//    ip6_h = NULL;
//    proto = ip_h->ip_p;
//    ip_hlen = (ip_h->ip_hl)*4;
//    src_addr = (void *)&ip_h->ip_src.s_addr;
//    dst_addr = (void *)&ip_h->ip_dst.s_addr;
//    if(Uplink){
//      key.ue_ip = *src_addr;
//      key.dst_ip = *dst_addr;
//    }
//    else{
//      key.dst_ip = *src_addr;
//      key.ue_ip = *dst_addr;
//    }
//    p_len = ip_h->ip_len;
//  }else if (ip_h->ip_v == 6) {
//    return;
//    ip_h = NULL;
//    ip6_h = (struct ip6_hdr *)pkbuf->data;
//    decode_ipv6_header(ip6_h, &proto, &ip_hlen);
//    src_addr = (void *)ip6_h->ip6_src.s6_addr;
//    dst_addr = (void *)ip6_h->ip6_dst.s6_addr;
//    if(Uplink){
//      key.ue_ip = *src_addr;
//      key.dst_ip = *dst_addr;
//    }
//    else{
//      key.dst_ip = *src_addr;
//      key.ue_ip = *dst_addr;
//    }
//    p_len = ip6_h->ip6_ctlun.ip6_un1.ip6_un1_plen + 40;
//
//  } else {
//    ogs_error("[upf_prepare_data]Invalid packet [IP version:%d, Packet Length:%d]",
//              ip_h->ip_v, pkbuf->len);
//    ogs_log_hexdump(OGS_LOG_ERROR, pkbuf->data, pkbuf->len);
//    return;
//  }
//  key.proto = proto;
//  if(key.proto == TCP_PROTOCOL){
//    struct tcphdr *tcph = (struct tcphdr *)((char *)pkbuf->data + ip_hlen);
//    if(Uplink){
//      key.src_port = be16toh(tcph->th_sport);
//      key.dst_port = be16toh(tcph->th_dport);
//    }else{
//      key.dst_port = be16toh(tcph->th_sport);
//      key.src_port = be16toh(tcph->th_dport);
//    }
//  }
//  else if (key.proto == IPPROTO_UDP) {
//    struct udphdr *udph = (struct udphdr *)((char *)pkbuf->data + ip_h->ip_hl * 4);
//    if(Uplink){
//      key.src_port = ntohs(udph->uh_sport);
//      key.dst_port = ntohs(udph->uh_dport);
//    }else{
//      key.dst_port = ntohs(udph->uh_sport);
//      key.src_port = ntohs(udph->uh_dport);
//    }
//  }
//  else{
//    key.src_port = 0;
//    key.dst_port = 0;
//  }
//  key.seid = seid;
//
//  uint64_t hash = XXH64(&key, sizeof(flow_id), 0);
//  flow_report * rep =  btreemap_get(mymap,&hash);
//
//  if(rep == NULL){
//    rep = malloc(sizeof (flow_report));
//    rep->key = key;
//    if(Uplink){
//      rep->ul_bytes = p_len;
//      rep->ul_pkts = 1;
//      rep->ul_bytes_rate = p_len;
//      rep->ul_pkts_rate = 1;
//      rep->dl_bytes = 0;
//      rep->dl_pkts = 0;
//      rep->dl_bytes_rate = 0;
//      rep->dl_pkts_rate = 0;
//      rep->peak_dl_bytes = 0;
//      rep->peak_dl_pkts = 0;
//      rep->peak_ul_bytes = 0;
//      rep->peak_ul_pkts = 0;
//      time_t current_time;
//      time(&current_time);
//      rep->start_time = current_time;
//      rep->timestamp = current_time;
//
//    } else{
//      rep->dl_bytes = p_len;
//      rep->dl_pkts = 1;
//      rep->dl_bytes_rate = p_len;
//      rep->dl_pkts_rate = 1;
//      rep->ul_bytes = 0;
//      rep->ul_pkts = 0;
//      rep->ul_bytes_rate = 0;
//      rep->ul_pkts_rate = 0;
//      rep->peak_dl_bytes = 0;
//      rep->peak_dl_pkts = 0;
//      rep->peak_ul_bytes = 0;
//      rep->peak_ul_pkts = 0;
//      time_t current_time;
//      time(&current_time);
//      rep->start_time = current_time;
//      rep->timestamp = current_time;
//    }
//    btreemap_insert_u64(mymap,&hash, rep);
//  }
//  else{
//    if(Uplink){
//      rep->ul_bytes += p_len;
//      rep->ul_pkts += 1;
//      rep->ul_bytes_rate += p_len;
//      rep->ul_pkts_rate += 1;
//
//    } else{
//      rep->dl_bytes += p_len;
//      rep->dl_pkts += 1;
//      rep->dl_bytes_rate += p_len;
//      rep->dl_pkts_rate += 1;
//    }
//  }
//}