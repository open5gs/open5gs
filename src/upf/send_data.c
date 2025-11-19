//
// Created by Fatemeh Shafiei Ardestani on 2024-07-13.
//
//userDataMeasurements
// Created by Fatemeh Shafiei Ardestani on 2024-07-13.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "send_data.h"
#include "delta_queue.h"

static pthread_mutex_t reporting_lock = PTHREAD_MUTEX_INITIALIZER;
static BTreeMap reporting_map = NULL;

static char *dup_string_local(const char *src) {
  if (!src) return NULL;
  size_t len = strlen(src) + 1;
  char *dst = malloc(len);
  if (!dst) return NULL;
  memcpy(dst, src, len);
  return dst;
}

static char *own_or_dup(char **src) {
  if (src && *src) {
    char *tmp = *src;
    *src = NULL;
    return tmp;
  }
  return dup_string_local("");
}

void ensure_reporting_map_exists(void) {
  if (!reporting_map) {
    reporting_map = new_str_btree();
  }
}

usage_report_per_flow_t *create_report_from_delta(flow_delta_entry *delta) {
  if (!delta->str_key || !delta->usubid || !delta->ue_ip || !delta->dst_ip) {
    return NULL;
  }
  usage_report_per_flow_t *rep = calloc(1, sizeof(*rep));
  if (!rep) return NULL;
  rep->key = calloc(1, sizeof(flow_key));
  if (!rep->key) {
    free(rep);
    return NULL;
  }
  rep->str_key = delta->str_key;
  delta->str_key = NULL;
  rep->key->usubid = own_or_dup(&delta->usubid);
  rep->key->ue_ip = own_or_dup(&delta->ue_ip);
  rep->key->dst_ip = own_or_dup(&delta->dst_ip);
  rep->key->seid = delta->seid;
  rep->key->src_port = delta->src_port;
  rep->key->dst_port = delta->dst_port;
  rep->key->proto = delta->proto;
  rep->start_time = delta->start_time ? delta->start_time : time(NULL);
  rep->timestamp = delta->timestamp ? delta->timestamp : rep->start_time;
  rep->pending_flush = false;
  rep->announced = true;
  return rep;
}

void apply_delta_to_report(usage_report_per_flow_t *rep, flow_delta_entry *delta) {
  rep->ul_bytes += delta->ul_bytes_delta;
  rep->ul_pkts += delta->ul_pkts_delta;
  rep->dl_bytes += delta->dl_bytes_delta;
  rep->dl_pkts += delta->dl_pkts_delta;
  rep->ul_bytes_rate += delta->ul_bytes_delta;
  rep->ul_pkts_rate += delta->ul_pkts_delta;
  rep->dl_bytes_rate += delta->dl_bytes_delta;
  rep->dl_pkts_rate += delta->dl_pkts_delta;
}

void apply_pending_deltas_locked(void) {
  ensure_reporting_map_exists();
  flow_delta_entry *delta;
  while ((delta = flow_delta_try_dequeue()) != NULL) {
    if (!delta->str_key) {
      fatemeh_log("[delta_queue] drop delta without key");
      flow_delta_entry_free(delta);
      continue;
    }
    usage_report_per_flow_t *rep = btreemap_get(reporting_map, delta->str_key);
    if (!rep) {
      rep = create_report_from_delta(delta);
      if (!rep) {
        fatemeh_log("[delta_queue] drop delta for %s (missing payload)", delta->str_key);
        flow_delta_entry_free(delta);
        continue;
      }
      btreemap_insert_str(reporting_map, rep->str_key, rep);
    }
    apply_delta_to_report(rep, delta);
    flow_delta_entry_free(delta);
  }
}

void get_current_time_send(char *buffer, size_t buffer_size) {

    struct timeval tv;
    struct tm local_tm;
    gettimeofday(&tv, NULL);
    localtime_r(&tv.tv_sec, &local_tm);
    int millisec = tv.tv_usec / 1000;
    snprintf(buffer, buffer_size, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
             local_tm.tm_year + 1900, local_tm.tm_mon + 1, local_tm.tm_mday,
             local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec, millisec);

}
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  printf("%.*s", (int)realsize, (char *)contents); // Print the response data
  return realsize;
}

void parse_time(const char* date_time, struct  tm* tm){
  int year, month, day, hour, minute, second;
  memset(tm, 0, sizeof(struct tm));

  if (sscanf(date_time, "%d-%d-%dT%d:%d:%dZ", &year, &month, &day, &hour, &minute, &second) != 6) {
    fatemeh_log("Failed to parse date-time string\n");
    return;
  }

  tm->tm_year = year - 1900;
  tm->tm_mon = month - 1;
  tm->tm_mday = day;
  tm->tm_hour = hour;
  tm->tm_min = minute;
  tm->tm_sec = second;
  tm->tm_isdst = -1;
  fatemeh_log("[parse_time] succeed to parse date-time string\n");

  return;
}
char* key_to_string(flow_key* key){
  json_t *obj = json_object();
  json_object_set_new(obj,"SeId", json_integer(key->seid));
  json_object_set_new(obj,"SrcIp", json_string(key->ue_ip));
  json_object_set_new(obj,"DstIp", json_string(key->dst_ip));
  json_object_set_new(obj,"SrcPort", json_integer(key->src_port));
  json_object_set_new(obj,"DstPort", json_integer(key->dst_port));
  char *s = json_dumps(obj, JSON_INDENT(2));
  json_decref(obj);
  return s;
}
char* sumStrings(char* str1, char* str2) {
  if (str1 == NULL || str2 == NULL) {
    return NULL;
  }

  char *endptr;

  long num1 = strtol(str1, &endptr, 10);
  if (endptr == str1 || *endptr != 'B') {
    fprintf(stderr, "Invalid format for first string: %s\n", str1);
    return NULL;
  }

  long num2 = strtol(str2, &endptr, 10);
  if (endptr == str2 || *endptr != 'B') {
    fprintf(stderr, "Invalid format for second string: %s\n", str2);
    return NULL;
  }

  long sum = num1 + num2;

  char* result = malloc(32);
  if (result == NULL) {
    fprintf(stderr, "Memory allocation failed.\n");
    return NULL;
  }

  sprintf(result, "%ldB", sum);
  return result;
}
void fillNotifVec(MapNode root, cvector_vector_type(NotificationItem **) Notifvec){
  if(root == NULL){
    fatemeh_log("the root is null");
    return;
  }

  fillNotifVec(root->right, Notifvec);
  cvector_push_back(*Notifvec, root->value);
  fillNotifVec(root->left, Notifvec);

}
void fillNotificationOperation(char * sub_id,BTreeMap ue_to_notif,MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType){
  usage_report_per_flow_t * usage_report_per_flow = root->value;
  fatemeh_log("the key->sub_id is %s, the report is for %s ",usage_report_per_flow->key->usubid,sub_id);
  if(strcmp(usage_report_per_flow->key->usubid, sub_id) != 0){
    fatemeh_log("the report is not for this subscription");
    return;
  }
  fatemeh_log("the line 69 in fillNotificationItemPerPacketHelper");
  UserDataUsageMeasurements *usage = malloc(sizeof (UserDataUsageMeasurements));
  usage->flowInfo = malloc(sizeof (FlowInformation));
  usage->flowInfo->flowDescription = NULL;
  usage->flowInfo->ethFlowDescription = NULL;
  usage->flowInfo->flowLabel = NULL;
  usage->flowInfo->packetFilterUsage = false;
  usage->flowInfo->fDir = BIDIRECTIONAL;
  usage->flowInfo->spi = NULL;
  usage->flowInfo->tosTrafficClass = NULL;
  usage->appID = NULL;
  usage->throughputStatisticsMeasurement = NULL;
  usage->applicationRelatedInformation = NULL;
  usage->flowInfo->packFiltId = key_to_string(usage_report_per_flow->key);
  fatemeh_log("[EventReport_UDUT] in fillNotificationOperation");

  if(measurementType == VOLUME_MEASUREMENT){
    fatemeh_log("[EventReport_UDUT] fill the VOLUME_MEASUREMENT");
    usage->volumeMeasurement = malloc(sizeof (VolumeMeasurement));
    usage->volumeMeasurement->totalNbOfPackets = usage_report_per_flow->dl_pkts + usage_report_per_flow->ul_pkts;
    usage->volumeMeasurement->totalVolume = malloc(21);
    usage->volumeMeasurement->ulVolume    = malloc(21);
    usage->volumeMeasurement->dlVolume    = malloc(21);
    uint64_t totalV = usage_report_per_flow->ul_bytes
                      + usage_report_per_flow->dl_bytes;
    sprintf(usage->volumeMeasurement->totalVolume, "%lluB", totalV);
    sprintf(usage->volumeMeasurement->ulVolume,    "%lluB", usage_report_per_flow->ul_bytes);
    sprintf(usage->volumeMeasurement->dlVolume,    "%lluB", usage_report_per_flow->dl_bytes);
    usage->volumeMeasurement->totalNbOfPackets = usage_report_per_flow->dl_pkts + usage_report_per_flow->ul_pkts;
    usage->volumeMeasurement->ulNbOfPackets = usage_report_per_flow->ul_pkts;
    usage->volumeMeasurement->dlNbOfPackets = usage_report_per_flow->dl_pkts;
    usage->throughputMeasurement = NULL;

  }
  else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_MEASURES){
    fatemeh_log("[EventReport_UDUT] fill the THROUGHPUT_MEASUREMENT for USER_DATA_USAGE_MEASURES");
    time_t now;
    time(&now);
    time_t diff = now - usage_report_per_flow->timestamp;
    if(diff == 0){
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free(usage);
      return;
    }
    fatemeh_log("[EventReport_UDUT] time diff calculated");
    usage->volumeMeasurement = NULL;
    usage->throughputMeasurement = malloc(sizeof(ThroughputMeasurement));
    usage->throughputMeasurement->ulThroughput = malloc(21);
    usage->throughputMeasurement->dlThroughput = malloc(21);
    usage->throughputMeasurement->ulPacketThroughput = malloc(21);
    usage->throughputMeasurement->dlPacketThroughput = malloc(21);
    sprintf(usage->throughputMeasurement->ulThroughput,"%llubps", ((usage_report_per_flow->ul_bytes_rate * 8) / diff));
    sprintf(usage->throughputMeasurement->dlThroughput,"%llubps", ((usage_report_per_flow->dl_bytes_rate * 8) / diff));
    sprintf(usage->throughputMeasurement->ulPacketThroughput,"%llupps", ((usage_report_per_flow->ul_pkts_rate) / diff));
    sprintf(usage->throughputMeasurement->dlPacketThroughput,"%llupps", ((usage_report_per_flow->dl_pkts_rate) / diff));
    // update for the next round
    usage_report_per_flow->peak_ul_bytes = (usage_report_per_flow->peak_ul_bytes > (usage_report_per_flow->ul_bytes_rate * 8) / diff) ? usage_report_per_flow->peak_ul_bytes : (usage_report_per_flow->ul_bytes_rate * 8) / diff;
    usage_report_per_flow->peak_dl_bytes = (usage_report_per_flow->peak_dl_bytes > (usage_report_per_flow->dl_bytes_rate * 8) / diff) ? usage_report_per_flow->peak_dl_bytes : (usage_report_per_flow->dl_bytes_rate * 8) / diff;

    usage_report_per_flow->peak_ul_pkts = (usage_report_per_flow->peak_ul_pkts > (usage_report_per_flow->ul_pkts_rate) / diff) ? usage_report_per_flow->peak_ul_pkts : (usage_report_per_flow->ul_pkts_rate) / diff;
    usage_report_per_flow->peak_dl_pkts = (usage_report_per_flow->peak_dl_pkts > (usage_report_per_flow->dl_pkts_rate) / diff) ? usage_report_per_flow->peak_dl_pkts : (usage_report_per_flow->dl_pkts_rate) / diff;

    usage_report_per_flow->timestamp = now;
    usage_report_per_flow->ul_bytes_rate = 0;
    usage_report_per_flow->dl_bytes_rate = 0;
    usage_report_per_flow->ul_pkts_rate = 0;
    usage_report_per_flow->dl_pkts_rate = 0;
    fatemeh_log("[EventReport_UDUT] end of fill the THROUGHPUT_MEASUREMENT for USER_DATA_USAGE_MEASURES");
  }
  else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_TRENDS){
    time_t now;
    time(&now);
    time_t diff = now - usage_report_per_flow->timestamp;
    if(diff == 0){
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free(usage);
      return;
    }
    time_t from_start = now - usage_report_per_flow->start_time;
    if (from_start == 0){
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free(usage);
      return;
    }
    u_int64_t  ulThroughput = (usage_report_per_flow->ul_bytes_rate * 8) / diff;
    u_int64_t dlThroughput = (usage_report_per_flow->dl_bytes_rate * 8) / diff;
    u_int64_t ulPacketThroughput = usage_report_per_flow->ul_pkts_rate / diff;
    u_int64_t dlPacketThroughput = usage_report_per_flow->dl_pkts_rate / diff;

    usage_report_per_flow->peak_ul_bytes = (usage_report_per_flow->peak_ul_bytes > ulThroughput) ? usage_report_per_flow->peak_ul_bytes : ulThroughput;
    usage_report_per_flow->peak_dl_bytes = (usage_report_per_flow->peak_dl_bytes > dlThroughput) ? usage_report_per_flow->peak_dl_bytes : dlThroughput;

    usage_report_per_flow->peak_ul_pkts = (usage_report_per_flow->peak_ul_pkts > ulPacketThroughput) ? usage_report_per_flow->peak_ul_pkts : ulPacketThroughput;
    usage_report_per_flow->peak_dl_pkts = (usage_report_per_flow->peak_dl_pkts > dlPacketThroughput) ? usage_report_per_flow->peak_dl_pkts : dlPacketThroughput;
    usage_report_per_flow->timestamp = now;

    usage_report_per_flow->ul_bytes_rate = 0;
    usage_report_per_flow->dl_bytes_rate = 0;
    usage_report_per_flow->ul_pkts_rate = 0;
    usage_report_per_flow->dl_pkts_rate = 0;

    usage->volumeMeasurement = NULL;
    usage->throughputMeasurement = NULL;
    usage->throughputStatisticsMeasurement = malloc(sizeof(ThroughputStatisticsMeasurement));

    usage->throughputStatisticsMeasurement->dlPeakThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->ulPeakThroughput = malloc(21);

    usage->throughputStatisticsMeasurement->ulPeakPacketThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlPeakPacketThroughput = malloc(21);

    usage->throughputStatisticsMeasurement->ulAverageThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlAverageThroughput =  malloc(21);

    usage->throughputStatisticsMeasurement->ulAveragePacketThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlAveragePacketThroughput = malloc(21);

    //    sprintf(usage->throughputMeasurement->ulThroughput,"%llubps", ((usage_report_per_flow->ul_bytes_rate * 8) / diff));
    sprintf(usage->throughputStatisticsMeasurement->dlPeakThroughput, "%llubps", usage_report_per_flow->peak_dl_bytes);
    sprintf(usage->throughputStatisticsMeasurement->ulPeakThroughput, "%llubps",  usage_report_per_flow->peak_ul_bytes);

    sprintf(usage->throughputStatisticsMeasurement->ulPeakPacketThroughput, "%llupps",  usage_report_per_flow->peak_ul_pkts);
    sprintf(usage->throughputStatisticsMeasurement->dlPeakPacketThroughput, "%llupps",  usage_report_per_flow->peak_dl_pkts);

    sprintf(usage->throughputStatisticsMeasurement->ulAverageThroughput, "%llubps", (usage_report_per_flow->ul_bytes * 8) / from_start);
    sprintf(usage->throughputStatisticsMeasurement->dlAverageThroughput, "%llubps",(usage_report_per_flow->dl_bytes * 8) / from_start);

    sprintf(usage->throughputStatisticsMeasurement->ulAveragePacketThroughput, "%llupps",usage_report_per_flow->ul_pkts / from_start);
    sprintf(usage->throughputStatisticsMeasurement->dlAveragePacketThroughput, "%llupps",usage_report_per_flow->dl_pkts / from_start);

  }

  NotificationItem *item = btreemap_get(ue_to_notif,usage_report_per_flow->key->ue_ip);
  if(item == NULL) {
    item = malloc(sizeof(NotificationItem));
    item->ueIpv4Addr = usage_report_per_flow->key->ue_ip;
    fatemeh_log("item->ueIpv4Addr %s", item->ueIpv4Addr);
    item->type = type;
    time_t current_time;
    time(&current_time);
    item->timeStamp = current_time;
    item->startTime = upfSub.eventReportingMode.TimeOfSubscription;
    item->snssai.sst = 2;
    strncpy(item->snssai.sd, "00002", sizeof(item->snssai.sd));
    item->dnn = NULL;
    item->gpsi = NULL;
    item->supi = NULL;
    item->ueMacAddr = NULL;
    item->ueIpv6Prefix = NULL;
    cvector(UserDataUsageMeasurements *)userDataMeasurements = NULL;
    cvector_push_back(userDataMeasurements, usage);
    item->userDataUsageMeasurements = userDataMeasurements;
    btreemap_insert_str(ue_to_notif, usage_report_per_flow->key->ue_ip, item);
  }else{
      cvector_push_back(item->userDataUsageMeasurements, usage);
    }
}
void fillNotificationItemPerFlowHelper(char * sub_id,BTreeMap ue_to_notif, MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType){
  if(root == NULL){
    return;
  }
  fillNotificationItemPerFlowHelper(sub_id,ue_to_notif,root->left, upfSub, type, measurementType);
  fillNotificationOperation(sub_id,ue_to_notif,root, upfSub, type, measurementType);
  fillNotificationItemPerFlowHelper(sub_id,ue_to_notif,root->right, upfSub, type, measurementType);

}

void  fillNotificationItemPerFlow(char * sub_id,UpfEventSubscription upfSub,cvector_vector_type(NotificationItem **) Notifvec,EventType type, MeasurementType measurementType){
  if(type==USER_DATA_USAGE_MEASURES || type== USER_DATA_USAGE_TRENDS){
    pthread_mutex_lock(&reporting_lock);
    apply_pending_deltas_locked();
    if (reporting_map == NULL || reporting_map->root == NULL){
      fatemeh_log("[EventReport_UDUT] There is no data to report");
      pthread_mutex_unlock(&reporting_lock);
      return;
    }
    BTreeMap ue_to_notif = new_str_btree();
    fillNotificationItemPerFlowHelper(sub_id,ue_to_notif,reporting_map->root,upfSub,type, measurementType);
    fatemeh_log("size of ue_to_notif hash is %zu", ue_to_notif->size);
    fillNotifVec(ue_to_notif->root,Notifvec);
    btreemap_free(ue_to_notif, true, false, NULL);
    pthread_mutex_unlock(&reporting_lock);

  }
}

void fillNotificationOperationPerSess(char * sub_id,BTreeMap ue_to_notif,MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType, BTreeMap ue_to_throughput){
  usage_report_per_flow_t * usage_report_per_flow = root->value;
  fatemeh_log("the key->sub_id is %s, the report is for %s ",usage_report_per_flow->key->usubid,sub_id);
  if(strcmp(usage_report_per_flow->key->usubid, sub_id) != 0){
    fatemeh_log("the report is not for this subscription");
    return;
  }

  UserDataUsageMeasurements *usage = malloc(sizeof (UserDataUsageMeasurements)); // TODO: how memory is leaked?
  usage->flowInfo = malloc(sizeof (FlowInformation));
  usage->flowInfo->flowDescription = NULL;
  usage->flowInfo->ethFlowDescription = NULL;
  usage->flowInfo->flowLabel = NULL;
  usage->flowInfo->packetFilterUsage = false;
  usage->flowInfo->spi = NULL;
  usage->flowInfo->tosTrafficClass = NULL;
  usage->appID = NULL;
  usage->throughputMeasurement = NULL;
  usage->appID = NULL;
  usage->throughputStatisticsMeasurement = NULL;
  usage->applicationRelatedInformation = NULL;
  usage->flowInfo->packFiltId = NULL; //key_to_string(usage_report_per_flow->key);
  fatemeh_log("in fillNotificationOperation the packetfilterID is %s", usage->flowInfo->packFiltId);
  if(measurementType == VOLUME_MEASUREMENT){
    usage->volumeMeasurement = malloc(sizeof (VolumeMeasurement));
    usage->volumeMeasurement->totalNbOfPackets = usage_report_per_flow->dl_pkts + usage_report_per_flow->ul_pkts;
    usage->volumeMeasurement->totalVolume = malloc(21);
    usage->volumeMeasurement->ulVolume    = malloc(21);
    usage->volumeMeasurement->dlVolume    = malloc(21);

    uint64_t totalV = usage_report_per_flow->ul_bytes
                      + usage_report_per_flow->dl_bytes;

    sprintf(usage->volumeMeasurement->totalVolume, "%lluB", totalV);
    sprintf(usage->volumeMeasurement->ulVolume,    "%lluB", usage_report_per_flow->ul_bytes);
    sprintf(usage->volumeMeasurement->dlVolume,    "%lluB", usage_report_per_flow->dl_bytes);

    usage->volumeMeasurement->ulNbOfPackets = usage_report_per_flow->ul_pkts;
    usage->volumeMeasurement->dlNbOfPackets = usage_report_per_flow->dl_pkts;
    usage->flowInfo->fDir = BIDIRECTIONAL;

  }
  else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_MEASURES){
    usage->volumeMeasurement = NULL;

    time_t now;
    time(&now);
    time_t diff = now - usage_report_per_flow->timestamp;
    if(diff == 0){
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free(usage);
      return;
    }
    UeVolumeTime * ueVT = btreemap_get(ue_to_throughput,usage_report_per_flow->key->ue_ip);
    fatemeh_log("getting the ueVT");
    if (ueVT == NULL){
      ueVT = malloc(sizeof (UeVolumeTime));
      ueVT->ulVolume = usage_report_per_flow->ul_bytes_rate;
      ueVT->dlVolume = usage_report_per_flow->dl_bytes_rate;
      ueVT->ulNbOfPackets = usage_report_per_flow->ul_pkts_rate;
      ueVT->dlNbOfPackets = usage_report_per_flow->dl_pkts_rate;
      ueVT->time = diff;
      btreemap_insert_str(ue_to_throughput, usage_report_per_flow->key->ue_ip, ueVT);
      fatemeh_log("allocating memory to ueVT");
    }
    else{
      ueVT->ulVolume += usage_report_per_flow->ul_bytes_rate;
      ueVT->dlVolume += usage_report_per_flow->dl_bytes_rate;
      ueVT->ulNbOfPackets += usage_report_per_flow->ul_pkts_rate;
      ueVT->dlNbOfPackets += usage_report_per_flow->dl_pkts_rate;
      ueVT->time = (diff > ueVT->time) ? diff : ueVT->time;
      fatemeh_log("updated the ueVT");

    }

    usage->throughputMeasurement = malloc(sizeof(ThroughputMeasurement));
    usage->throughputMeasurement->ulThroughput = malloc(21);
    usage->throughputMeasurement->dlThroughput = malloc(21);
    usage->throughputMeasurement->ulPacketThroughput = malloc(21);
    usage->throughputMeasurement->dlPacketThroughput = malloc(21);
    sprintf(usage->throughputMeasurement->ulThroughput,"%llubps", ((ueVT->ulVolume  * 8) / ueVT->time));
    sprintf(usage->throughputMeasurement->dlThroughput,"%llubps", ((ueVT->dlVolume * 8) / ueVT->time));
    sprintf(usage->throughputMeasurement->ulPacketThroughput,"%llupps", ((ueVT->ulNbOfPackets) / ueVT->time));
    sprintf(usage->throughputMeasurement->dlPacketThroughput,"%llupps", ((ueVT->dlNbOfPackets) / ueVT->time));
    fatemeh_log("filed the throughputMeasurement");
    // update for the next round
    usage_report_per_flow->peak_ul_bytes = (usage_report_per_flow->peak_ul_bytes > (ueVT->ulVolume  * 8) / ueVT->time) ? usage_report_per_flow->peak_ul_bytes : (ueVT->ulVolume  * 8) / ueVT->time;
    usage_report_per_flow->peak_dl_bytes = (usage_report_per_flow->peak_dl_bytes > (ueVT->dlVolume * 8) / ueVT->time) ? usage_report_per_flow->peak_dl_bytes : (ueVT->dlVolume * 8) / ueVT->time;

    usage_report_per_flow->peak_ul_pkts = (usage_report_per_flow->peak_ul_pkts > (ueVT->ulNbOfPackets) / ueVT->time) ? usage_report_per_flow->peak_ul_pkts : (ueVT->ulNbOfPackets) / ueVT->time;
    usage_report_per_flow->peak_dl_pkts = (usage_report_per_flow->peak_dl_pkts > (ueVT->dlNbOfPackets) / ueVT->time) ? usage_report_per_flow->peak_dl_pkts : (ueVT->dlNbOfPackets) / ueVT->time;

    usage_report_per_flow->timestamp = now;
    usage_report_per_flow->ul_bytes_rate = 0;
    usage_report_per_flow->dl_bytes_rate = 0;
    usage_report_per_flow->ul_pkts_rate = 0;
    usage_report_per_flow->dl_pkts_rate = 0;
    fatemeh_log("updated for the next time");
  }
  else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_TRENDS){
    usage->volumeMeasurement = NULL;
    usage->throughputMeasurement = NULL;

    time_t now;
    time(&now);
    time_t diff = now - usage_report_per_flow->timestamp;
    if(diff == 0){
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free(usage);
      return;
    }
    time_t from_start = now - usage_report_per_flow->start_time;
    if (from_start == 0){
      return;
    }
    UeVolumeTime * ueVT = btreemap_get(ue_to_throughput,usage_report_per_flow->key->ue_ip);
    if (ueVT == NULL){
      ueVT = malloc(sizeof (UeVolumeTime));
      ueVT->ulVolume = usage_report_per_flow->ul_bytes_rate;
      ueVT->dlVolume = usage_report_per_flow->dl_bytes_rate;
      ueVT->ulNbOfPackets = usage_report_per_flow->ul_pkts_rate;
      ueVT->dlNbOfPackets = usage_report_per_flow->dl_pkts_rate;
      ueVT->time = diff;
      btreemap_insert_str(ue_to_throughput, usage_report_per_flow->key->ue_ip, ueVT);
    }
    else{
      ueVT->ulVolume += usage_report_per_flow->ul_bytes_rate;
      ueVT->dlVolume += usage_report_per_flow->dl_bytes_rate;
      ueVT->ulNbOfPackets += usage_report_per_flow->ul_pkts_rate;
      ueVT->dlNbOfPackets += usage_report_per_flow->dl_pkts_rate;
      ueVT->time = (diff > ueVT->time) ? diff : ueVT->time;

    }
    u_int64_t ulThroughput = ((ueVT->ulVolume  * 8) / ueVT->time);
    u_int64_t dlThroughput  = ((ueVT->dlVolume * 8) / ueVT->time);
    u_int64_t ulPacketThroughput = ((ueVT->ulNbOfPackets) / ueVT->time);
    u_int64_t dlPacketThroughput  = ((ueVT->dlNbOfPackets) / ueVT->time);
    usage->volumeMeasurement = NULL;
    usage->throughputMeasurement = NULL;
    usage->throughputStatisticsMeasurement = malloc(sizeof(ThroughputStatisticsMeasurement));

    usage->throughputStatisticsMeasurement->dlPeakThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->ulPeakThroughput = malloc(21);

    usage->throughputStatisticsMeasurement->ulPeakPacketThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlPeakPacketThroughput = malloc(21);

    usage->throughputStatisticsMeasurement->ulAverageThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlAverageThroughput =  malloc(21);

    usage->throughputStatisticsMeasurement->ulAveragePacketThroughput = malloc(21);
    usage->throughputStatisticsMeasurement->dlAveragePacketThroughput = malloc(21);



    // update for the next round
    usage_report_per_flow->peak_ul_bytes = (usage_report_per_flow->peak_ul_bytes > ulThroughput) ? usage_report_per_flow->peak_ul_bytes : ulThroughput;
    usage_report_per_flow->peak_dl_bytes = (usage_report_per_flow->peak_dl_bytes > dlThroughput) ? usage_report_per_flow->peak_dl_bytes : dlThroughput;

    usage_report_per_flow->peak_ul_pkts = (usage_report_per_flow->peak_ul_pkts > ulPacketThroughput) ? usage_report_per_flow->peak_ul_pkts : ulPacketThroughput;
    usage_report_per_flow->peak_dl_pkts = (usage_report_per_flow->peak_dl_pkts > dlPacketThroughput) ? usage_report_per_flow->peak_dl_pkts : dlPacketThroughput;
    // fill the report
    //sprintf(usage->throughputMeasurement->ulThroughput,"%llubps", ((ueVT->ulVolume  * 8) / ueVT->time));
    sprintf(usage->throughputStatisticsMeasurement->ulPeakThroughput ,"%llubps",usage_report_per_flow->peak_ul_bytes);
    sprintf(usage->throughputStatisticsMeasurement->dlPeakThroughput ,"%llubps", usage_report_per_flow->peak_dl_bytes);
    sprintf(usage->throughputStatisticsMeasurement->ulPeakPacketThroughput ,"%llupps", usage_report_per_flow->peak_ul_pkts);
    sprintf(usage->throughputStatisticsMeasurement->dlPeakPacketThroughput ,"%llupps", usage_report_per_flow->peak_dl_pkts);

    sprintf(usage->throughputStatisticsMeasurement->ulAverageThroughput, "%llubps", (usage_report_per_flow->ul_bytes * 8) / from_start);
    sprintf(usage->throughputStatisticsMeasurement->dlAverageThroughput, "%llubps",(usage_report_per_flow->dl_bytes * 8) / from_start);
    sprintf(usage->throughputStatisticsMeasurement->ulAveragePacketThroughput, "%llupps",usage_report_per_flow->ul_pkts / from_start);
    sprintf(usage->throughputStatisticsMeasurement->dlAveragePacketThroughput, "%llupps",usage_report_per_flow->dl_pkts / from_start);


    usage_report_per_flow->timestamp = now;
    usage_report_per_flow->ul_bytes_rate = 0;
    usage_report_per_flow->dl_bytes_rate = 0;
    usage_report_per_flow->ul_pkts_rate = 0;
    usage_report_per_flow->dl_pkts_rate = 0;


  }

  NotificationItem *item = btreemap_get(ue_to_notif,usage_report_per_flow->key->ue_ip);
  if(item == NULL) {
    item = malloc(sizeof(NotificationItem));
    item->ueIpv4Addr = usage_report_per_flow->key->ue_ip;
    fatemeh_log("item->ueIpv4Addr %s", item->ueIpv4Addr);
    item->type = type;
    time_t current_time;
    time(&current_time);
    item->timeStamp = current_time;
    item->startTime = upfSub.eventReportingMode.TimeOfSubscription;
    item->snssai.sst = 2;
    strncpy(item->snssai.sd, "00002", sizeof(item->snssai.sd));

    item->dnn = NULL;
    item->gpsi = NULL;
    item->supi = NULL;
    item->ueMacAddr = NULL;
    item->ueIpv6Prefix = NULL;
    cvector(UserDataUsageMeasurements *)userDataMeasurements = NULL;
    cvector_push_back(userDataMeasurements, usage);
    item->userDataUsageMeasurements = userDataMeasurements;
    btreemap_insert_str(ue_to_notif, usage_report_per_flow->key->ue_ip, item);
  }else{
    if(measurementType == VOLUME_MEASUREMENT){
      item->userDataUsageMeasurements[0]->volumeMeasurement->dlNbOfPackets += usage->volumeMeasurement->dlNbOfPackets;
      item->userDataUsageMeasurements[0]->volumeMeasurement->ulNbOfPackets += usage->volumeMeasurement->ulNbOfPackets;
      item->userDataUsageMeasurements[0]->volumeMeasurement->totalNbOfPackets += usage->volumeMeasurement->totalNbOfPackets;
      char *temp;
      char *old_dl = item->userDataUsageMeasurements[0]->volumeMeasurement->dlVolume;
      temp = sumStrings(old_dl,usage->volumeMeasurement->dlVolume);
      if (temp) {
        free(old_dl);
        item->userDataUsageMeasurements[0]->volumeMeasurement->dlVolume = temp;
      }
      char *old_ul = item->userDataUsageMeasurements[0]->volumeMeasurement->ulVolume;
      temp = sumStrings( old_ul,usage->volumeMeasurement->ulVolume);
      if (temp) {
        free(old_ul);
        item->userDataUsageMeasurements[0]->volumeMeasurement->ulVolume = temp;
      }
      char *old_total = item->userDataUsageMeasurements[0]->volumeMeasurement->totalVolume;
      temp = sumStrings(old_total, usage->volumeMeasurement->totalVolume);
      if (temp){
        free(old_total);
        item->userDataUsageMeasurements[0]->volumeMeasurement->totalVolume = temp;
      }

      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      free_volume_measurement(usage->volumeMeasurement);
      free(usage);
      usage = NULL;
    }
    else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_MEASURES){
      // TODO: works for now but should be changed
      if (item->userDataUsageMeasurements[0]->throughputMeasurement) {
        free_throughput_measurement(item->userDataUsageMeasurements[0]->throughputMeasurement);
      }
      item->userDataUsageMeasurements[0]->throughputMeasurement = usage->throughputMeasurement;
      usage->throughputMeasurement = NULL;
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      if (usage->volumeMeasurement) { free_volume_measurement(usage->volumeMeasurement); usage->volumeMeasurement = NULL; }
      if (usage->throughputStatisticsMeasurement) { free_throughput_statistics_measurement(usage->throughputStatisticsMeasurement); usage->throughputStatisticsMeasurement = NULL; }
      free(usage);
      usage = NULL;
    }
    else if(measurementType == THROUGHPUT_MEASUREMENT && type == USER_DATA_USAGE_TRENDS ){
      // TODO: works for now but should be changed
      if (item->userDataUsageMeasurements[0]->throughputStatisticsMeasurement) {
        free_throughput_statistics_measurement(item->userDataUsageMeasurements[0]->throughputStatisticsMeasurement);
      }
      item->userDataUsageMeasurements[0]->throughputStatisticsMeasurement = usage->throughputStatisticsMeasurement;
      usage->throughputStatisticsMeasurement = NULL;
      free_flow_information(usage->flowInfo);
      usage->flowInfo = NULL;
      if (usage->volumeMeasurement) { free_volume_measurement(usage->volumeMeasurement); usage->volumeMeasurement = NULL; }
      if (usage->throughputMeasurement) { free_throughput_measurement(usage->throughputMeasurement); usage->throughputMeasurement = NULL; }
      free(usage);
      usage = NULL;
    }
  }
}
void fillNotificationItemPerSessHelper(char * sub_id,BTreeMap ue_to_notif, MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType, BTreeMap ue_to_throughput){
  if(root == NULL){
    return;
  }
  fillNotificationItemPerSessHelper(sub_id,ue_to_notif,root->left, upfSub, type, measurementType, ue_to_throughput);
  fillNotificationOperationPerSess(sub_id,ue_to_notif,root, upfSub, type, measurementType, ue_to_throughput);
  fillNotificationItemPerSessHelper(sub_id,ue_to_notif,root->right, upfSub, type, measurementType, ue_to_throughput);

}

void fillNotificationItemPerSess(char * sub_id,UpfEventSubscription upfSub,cvector_vector_type(NotificationItem **) Notifvec,EventType type, MeasurementType measurementType){
  if(type==USER_DATA_USAGE_MEASURES || type== USER_DATA_USAGE_TRENDS){
    pthread_mutex_lock(&reporting_lock);
    apply_pending_deltas_locked();
    if (reporting_map == NULL || reporting_map->root == NULL){
      fatemeh_log("[EventReport_UDUT] There is no data to report");
      pthread_mutex_unlock(&reporting_lock);
      return;
    }
    BTreeMap ue_to_notif = new_str_btree();
    BTreeMap ue_to_throughput = new_str_btree();
    fillNotificationItemPerSessHelper(sub_id,ue_to_notif,reporting_map->root,upfSub,type, measurementType, ue_to_throughput);
    fatemeh_log("size of ue_to_notif hash is %zu", ue_to_notif->size);
    fillNotifVec(ue_to_notif->root,Notifvec);
    btreemap_free(ue_to_notif, true, false, NULL);
    btreemap_free(ue_to_throughput, true, true, free_ue_volume_time);
    pthread_mutex_unlock(&reporting_lock);

  }
}
// TODO: consider having two measurement types
// TODO: consider having two Event for 1 subscription types
void free_flow_information(FlowInformation *flowInfo) {
  if (!flowInfo) return;

  if (flowInfo->flowDescription) {
    free((void *)flowInfo->flowDescription);
    flowInfo->flowDescription = NULL;
  }
  if (flowInfo->packFiltId) {
    free((void *)flowInfo->packFiltId);
    flowInfo->packFiltId = NULL;
  }
  if (flowInfo->tosTrafficClass) {
    free((void *)flowInfo->tosTrafficClass);
    flowInfo->tosTrafficClass = NULL;
  }
  if (flowInfo->spi) {
    free((void *)flowInfo->spi);
    flowInfo->spi = NULL;
  }
  if (flowInfo->flowLabel) {
    free((void *)flowInfo->flowLabel);
    flowInfo->flowLabel = NULL;
  }
  if (flowInfo->ethFlowDescription) {
    free(flowInfo->ethFlowDescription);
    flowInfo->ethFlowDescription = NULL;
  }
  fatemeh_log("free the flowInfo");
  free(flowInfo);
}
void free_throughput_measurement(ThroughputMeasurement *tpMeasurement) {
  if (!tpMeasurement) return;

  free(tpMeasurement->ulThroughput);
  free(tpMeasurement->dlThroughput);
  free(tpMeasurement->ulPacketThroughput);
  free(tpMeasurement->dlPacketThroughput);
  fatemeh_log("free the tcp measures");
  free(tpMeasurement);
}

void free_throughput_statistics_measurement(ThroughputStatisticsMeasurement *tpStatsMeasurement) {
  if (!tpStatsMeasurement) return;

  free(tpStatsMeasurement->ulAverageThroughput);
  free(tpStatsMeasurement->dlAverageThroughput);
  free(tpStatsMeasurement->ulPeakThroughput);
  free(tpStatsMeasurement->dlPeakThroughput);
  free(tpStatsMeasurement->ulAveragePacketThroughput);
  free(tpStatsMeasurement->dlAveragePacketThroughput);
  free(tpStatsMeasurement->ulPeakPacketThroughput);
  free(tpStatsMeasurement->dlPeakPacketThroughput);
  fatemeh_log("free the tcp statistic measures");
  free(tpStatsMeasurement);
}
void free_volume_measurement(VolumeMeasurement *volumeMeasurement) {
  if (!volumeMeasurement) return;

  free(volumeMeasurement->totalVolume);
  free(volumeMeasurement->ulVolume);
  free(volumeMeasurement->dlVolume);
  fatemeh_log("free the volume measures");
  free(volumeMeasurement);
}

void free_application_related_information(ApplicationRelatedInformation *appInfo) {
  if (!appInfo) return;
  fatemeh_log("free the app info");
  free(appInfo);
}
void free_user_data_usage_measurements(UserDataUsageMeasurements *usageMeasurement) {
  if (!usageMeasurement) return;

  if (usageMeasurement->appID) {
    free(usageMeasurement->appID);
    usageMeasurement->appID = NULL;
  }

  if (usageMeasurement->flowInfo) {
    free_flow_information(usageMeasurement->flowInfo);
    usageMeasurement->flowInfo = NULL;
  }

  if (usageMeasurement->volumeMeasurement) {
    free_volume_measurement(usageMeasurement->volumeMeasurement);
    usageMeasurement->volumeMeasurement = NULL;
  }

  if (usageMeasurement->throughputMeasurement) {
    free_throughput_measurement(usageMeasurement->throughputMeasurement);
    usageMeasurement->throughputMeasurement = NULL;
  }

  if (usageMeasurement->applicationRelatedInformation) {
    free_application_related_information(usageMeasurement->applicationRelatedInformation);
    usageMeasurement->applicationRelatedInformation = NULL;
  }

  if (usageMeasurement->throughputStatisticsMeasurement) {
    free_throughput_statistics_measurement(usageMeasurement->throughputStatisticsMeasurement);
    usageMeasurement->throughputStatisticsMeasurement = NULL;
  }
  fatemeh_log("free the user measurement");
  free(usageMeasurement);
}

void free_notification_item(NotificationItem *item) {
  if (!item) return;


  int len = cvector_size(item->userDataUsageMeasurements);
  for (int i = 0; i < len; i++) {
    if (item->userDataUsageMeasurements[i]) {
      free_user_data_usage_measurements(item->userDataUsageMeasurements[i]);
      item->userDataUsageMeasurements[i] = NULL;
    }
  }
  cvector_free(item->userDataUsageMeasurements);
  item->userDataUsageMeasurements = NULL;
  fatemeh_log("free the notification");
  free(item);
}

void free_notification_memory(cvector_vector_type(NotificationItem *) *vec) {
  if (!vec || !*vec) return;

  cvector_vector_type(NotificationItem *) notifvec = *vec;
  int len = cvector_size(notifvec);
  for (int i = 0; i < len; i++) {
    if (notifvec[i]) {
      free_notification_item(notifvec[i]);
      notifvec[i] = NULL;
    }
  }
  cvector_free(notifvec);
  fatemeh_log("free the notification vector");
  *vec = NULL; // Optional: avoid dangling pointer
}
void free_ue_volume_time(void *p) {
  if (!p) return;
  UeVolumeTime *u = (UeVolumeTime *)p;
  free(u);
}

void create_send_report(char * sub_id,UpfEventSubscription upfSub,EventType type){
  if(type == USER_DATA_USAGE_MEASURES){
    fatemeh_log("[USER_DATA_USAGE_MEASURES] in create_custom_report");
    cvector_vector_type(NotificationItem *) Notifvec = NULL;
    cvector_vector_type(UpfEvent) EventList = upfSub.EventList;

    // the len of EventList might be > 1, it should be handled
    int len = cvector_size(EventList);
    for(int e = 0; e < len; e++){
      if (EventList[e].type != type) continue;
      int len_mtypes = cvector_size(EventList[e].measurementTypes);
      for (int m = 0; m < len_mtypes; m++){
        if(EventList[e].granularityOfMeasurement == PER_FLOW && EventList[e].measurementTypes[m] == VOLUME_MEASUREMENT){
          fillNotificationItemPerFlow(sub_id,upfSub, &Notifvec, type, VOLUME_MEASUREMENT);
        }
        else if(EventList[e].granularityOfMeasurement == PER_SESSION && EventList[e].measurementTypes[m] == VOLUME_MEASUREMENT){
          fillNotificationItemPerSess(sub_id,upfSub, &Notifvec, type, VOLUME_MEASUREMENT);
        }
        else if(EventList[e].granularityOfMeasurement == PER_FLOW && EventList[e].measurementTypes[m] == THROUGHPUT_MEASUREMENT){
          fillNotificationItemPerFlow(sub_id,upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
          // TODO: the logic should be checked
        }
        else if(EventList[e].granularityOfMeasurement == PER_SESSION && EventList[e].measurementTypes[m] == THROUGHPUT_MEASUREMENT){
          fillNotificationItemPerSess(sub_id,upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
          // TODO: the logic should be checked
        }
      }
    }
    if(Notifvec == NULL){
      return;
    }
    for(size_t i = 0; i < cvector_size(Notifvec); i++){

      json_t* callBack_Report = serialize_callBack(Notifvec[i], upfSub.notifyCorrelationId, 0);
      if (!callBack_Report) {
        fatemeh_log("The json_t object is NULL.\n");
      }
      char *json_str = json_dumps(callBack_Report, JSON_INDENT(2));
      send_report (json_str, upfSub, type);
      free(json_str);
      json_decref(callBack_Report);
    }
    free_notification_memory(&Notifvec);

  }
  if(type == USER_DATA_USAGE_TRENDS){
    fatemeh_log("[USER_DATA_USAGE_MEASURES] in create_custom_report");
    cvector_vector_type(NotificationItem *) Notifvec = NULL;
    cvector_vector_type(UpfEvent) EventList = upfSub.EventList;

    int len = cvector_size(EventList);

    for(int e = 0; e < len; e++){
      if(EventList[e].type != type) continue;
      if(EventList[e].granularityOfMeasurement == PER_FLOW){
        fatemeh_log("[USER_DATA_USAGE_MEASURES] in create_custom_report for PER_FLOW");
        fillNotificationItemPerFlow(sub_id,upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
        // TODO: the logic should be checked
      }
      else if(EventList[e].granularityOfMeasurement == PER_SESSION){
        fatemeh_log("[USER_DATA_USAGE_MEASURES] in create_custom_report for PER_FLOW");
        fillNotificationItemPerSess(sub_id, upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
        // TODO: the logic should be checked
      }
    }
    if(Notifvec == NULL){
      fatemeh_log("[create_send_report] There is no data to report");
      return;
    }
    fatemeh_log("[create_send_report] fillNotificationItem, the Noitve_size %lu\n", cvector_size(Notifvec));
    for(size_t i = 0; i < cvector_size(Notifvec); i++){

      json_t* callBack_Report = serialize_callBack(Notifvec[i], upfSub.notifyCorrelationId, 0);
      if (!callBack_Report) {
        fatemeh_log("The json_t object is NULL.\n");
      }
      char *json_str = json_dumps(callBack_Report, JSON_INDENT(2));
      send_report (json_str, upfSub, type);
      free(json_str);
      json_decref(callBack_Report);
    }

    free_notification_memory(&Notifvec);

  }
}
void send_report(char *json_data,UpfEventSubscription upfSub,EventType type){

  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);

  if (curl) {
    int report_num = upfSub.eventReportingMode.sent_reports;
    char report_num_str[10];
    char report_num_head[50] = "Report_number: ";
    snprintf(report_num_str, sizeof(report_num_str), "%d", report_num);
    strcat(report_num_head, report_num_str);
    curl_easy_setopt(curl, CURLOPT_URL, upfSub.eventNotifyUri);
    char c_time[50];
    get_current_time_send(c_time, sizeof(c_time));
    fatemeh_log("[Report_Latency]the report number sent and the time is %d, %s ,%s\n", report_num, report_num_str ,c_time);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L); // To set the size dependent to json

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Expect:"); // To Disable 100 continue
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, report_num_head);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    res = curl_easy_perform(curl);

    // Check for errors
    if (res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_global_cleanup();
//    let's see if it makse any problem.


  }
}
void* EventReport_UDUM(void*) {

    int length = sizeof(supported_events) / sizeof(EventType);
    for (int e = 0; e < length; e++){
      time_t current_time;
      time(&current_time);
      pthread_mutex_lock(&subHash_lock);
      if(subHash == NULL) subHash = new_str_btree();
      cvector_vector_type(CreatedSubscription *) retrieved_vec = btreemap_get(subHash, getEventTypeString(supported_events[e]));
      if (retrieved_vec!=NULL) {
        fatemeh_log("the retrieved_vec is not null");
        for (int i = 0; i < cvector_size(retrieved_vec); i++) {
          fatemeh_log("Index i = %d", i);
//          fatemeh_log("retrieved_vec pointer: %p", (void *)retrieved_vec);
//          fatemeh_log("retrieved_vec[%d] pointer: %p", i, (void *)retrieved_vec[i]);
          UpfEventSubscription  * upfEventSubscription = (retrieved_vec[i]->subscription);
//          fatemeh_log("upfEventSubscription[%d] pointer: %p", i, (void *)upfEventSubscription);
          fatemeh_log("the subscription_id is[%s]", retrieved_vec[i]->subscription_id);
          if(upfEventSubscription->eventReportingMode.trigger == PERIODIC){
            if(upfEventSubscription->eventReportingMode.TimeOfLastReport == 0){
              if (current_time - upfEventSubscription->eventReportingMode.TimeOfSubscription >=upfEventSubscription->eventReportingMode.repPeriod){
                fatemeh_log("time to send notification for the subcription ID %s",retrieved_vec[i]->subscription_id);
                create_send_report(retrieved_vec[i]->subscription_id,*upfEventSubscription, supported_events[e]); //change
                upfEventSubscription->eventReportingMode.TimeOfLastReport = current_time;
                upfEventSubscription->eventReportingMode.sent_reports += 1;
              }
            }
            else if (current_time - upfEventSubscription->eventReportingMode.TimeOfLastReport >=upfEventSubscription->eventReportingMode.repPeriod){
              create_send_report(retrieved_vec[i]->subscription_id,*upfEventSubscription, supported_events[e]); //change
              upfEventSubscription->eventReportingMode.TimeOfLastReport = current_time;
              upfEventSubscription->eventReportingMode.sent_reports += 1;
            }
          }

        }
      }
      pthread_mutex_unlock(&subHash_lock);

    }

  return NULL;
}
