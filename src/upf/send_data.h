#ifndef REST_API_C_SEND_DATA_H
#define REST_API_C_SEND_DATA_H

#define MAP_OK 0
#define MAP_MISSING -3
#define MAP_FULL -2
#define MAP_OMEM -1
#include "stb_ds.h"
#include "encoder.h"
#include "ee_event.h"
#include "btree.h"
#include "shared_variables.h"
#include <pthread.h>
#include <stdio.h>
#define _GNU_SOURCE
#define __USE_XOPEN
#define _XOPEN_SOURCE 700
#include <time.h>
#include <curl/curl.h>
#include "string.h"
#include <jansson.h>
void get_current_time_send(char *buffer, size_t buffer_size);
//static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
void parse_time(const char* date_time, struct  tm* tm);
char* key_to_string(flow_key* key);
char* sumStrings(char* str1, char* str2);
void fillNotifVec(MapNode root, cvector_vector_type(NotificationItem **) Notifvec);
void fillNotificationOperation(char * sub_id,BTreeMap ue_to_notif,MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType);
void fillNotificationItemPerFlowHelper(char * sub_id,BTreeMap ue_to_notif, MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType);
void fillNotificationItemPerFlow(char * sub_id,UpfEventSubscription upfSub,cvector_vector_type(NotificationItem **) Notifvec,EventType type, MeasurementType measurementType);
void fillNotificationOperationPerSess(char * sub_id,BTreeMap ue_to_notif,MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType, BTreeMap ue_to_throughput);
void fillNotificationItemPerSessHelper(char * sub_id,BTreeMap ue_to_notif, MapNode root, UpfEventSubscription upfSub,EventType type, MeasurementType measurementType, BTreeMap ue_to_throughput);
void fillNotificationItemPerSess(char * sub_id,UpfEventSubscription upfSub,cvector_vector_type(NotificationItem **) Notifvec,EventType type, MeasurementType measurementType);
void free_flow_information(FlowInformation *flowInfo);
void free_throughput_measurement(ThroughputMeasurement *tpMeasurement);
void free_throughput_statistics_measurement(ThroughputStatisticsMeasurement *tpStatsMeasurement);
void free_volume_measurement(VolumeMeasurement *volumeMeasurement);
void free_application_related_information(ApplicationRelatedInformation *appInfo);
void free_user_data_usage_measurements(UserDataUsageMeasurements *usageMeasurement);
void free_notification_item(NotificationItem *item);
void free_notification_memory(cvector_vector_type(NotificationItem *) *vec);
void create_send_report(char*sub_id,UpfEventSubscription upfSub,EventType type);
void send_report(char *json_data,UpfEventSubscription upfSub,EventType type);
void* EventReport_UDUM(void*);
#endif //REST_API_C_SEND_DATA_H