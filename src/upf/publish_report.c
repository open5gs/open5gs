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
////
//// Created by Fatemeh Shafiei Ardestani on 2025-03-09.
////
//
//#include "publish_report.h"
////
//// Created by Fatemeh Shafiei Ardestani on 2024-07-13.
////
////userDataMeasurements
//// Created by Fatemeh Shafiei Ardestani on 2024-07-13.
////
//#include <stdio.h>
//#include <string.h>
//#include <zmq.h>
//#include "publish_report.h"
//
//void create_publish_report(char* sub_id, UpfEventSubscription upfSub,EventType type, void *router){
//  if(type == USER_DATA_USAGE_MEASURES){
//    ogs_warn("[USER_DATA_USAGE_MEASURES] in create_custom_report");
//    cvector_vector_type(NotificationItem *) Notifvec = NULL;
//    cvector_vector_type(UpfEvent) EventList = upfSub.EventList;
//
//    // the len of EventList might be > 1, it should be handled
//    int len = cvector_size(EventList);
//    for(int e = 0; e < len; e++){
//      if (EventList[e].type != type) continue;
//      int len_mtypes = cvector_size(EventList[e].measurementTypes);
//      for (int m = 0; m < len_mtypes; m++){
//        if(EventList[e].granularityOfMeasurement == PER_FLOW && EventList[e].measurementTypes[m] == VOLUME_MEASUREMENT){
//          fillNotificationItemPerFlow(sub_id,upfSub, &Notifvec, type, VOLUME_MEASUREMENT);
//        }
//        else if(EventList[e].granularityOfMeasurement == PER_SESSION && EventList[e].measurementTypes[m] == VOLUME_MEASUREMENT){
//          fillNotificationItemPerSess(sub_id, upfSub, &Notifvec, type, VOLUME_MEASUREMENT);
//        }
//        else if(EventList[e].granularityOfMeasurement == PER_FLOW && EventList[e].measurementTypes[m] == THROUGHPUT_MEASUREMENT){
//          fillNotificationItemPerFlow(sub_id,upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
//          // TODO: the logic should be checked
//        }
//        else if(EventList[e].granularityOfMeasurement == PER_SESSION && EventList[e].measurementTypes[m] == THROUGHPUT_MEASUREMENT){
//          fillNotificationItemPerSess(sub_id,upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
//          // TODO: the logic should be checked
//        }
//      }
//    }
//    if(Notifvec == NULL){
//      ogs_warn("[create_publish_report] There is no data to report");
//      return;
//    }
//    ogs_warn("[create_publish_report] fillNotificationItem, the Noitve_size %lu\n", cvector_size(Notifvec));
//    for(size_t i = 0; i < cvector_size(Notifvec); i++){
//
//      json_t* callBack_Report = serialize_callBack(Notifvec[i], upfSub.notifyCorrelationId, 0);
//      if (!callBack_Report) {
//        ogs_warn("The json_t object is NULL.\n");
//      }
//      char *json_str = json_dumps(callBack_Report, JSON_INDENT(2));
//      publish (router,json_str, upfSub, type);
//    }
//    // to do: free the memory
//  }
//  if(type == USER_DATA_USAGE_TRENDS){
//    ogs_warn("[USER_DATA_USAGE_MEASURES] in create_custom_report");
//    cvector_vector_type(NotificationItem *) Notifvec = NULL;
//    cvector_vector_type(UpfEvent) EventList = upfSub.EventList;
//
//    int len = cvector_size(EventList);
//
//    for(int e = 0; e < len; e++){
//      if(EventList[e].type != type) continue;
//      if(EventList[e].granularityOfMeasurement == PER_FLOW){
//        ogs_warn("[USER_DATA_USAGE_MEASURES] in create_custom_report for PER_FLOW");
//        fillNotificationItemPerFlow(sub_id, upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
//        // TODO: the logic should be checked
//      }
//      else if(EventList[e].granularityOfMeasurement == PER_SESSION){
//        ogs_warn("[USER_DATA_USAGE_MEASURES] in create_custom_report for PER_FLOW");
//        fillNotificationItemPerSess(sub_id, upfSub, &Notifvec, type, THROUGHPUT_MEASUREMENT);
//        // TODO: the logic should be checked
//      }
//    }
//    if(Notifvec == NULL){
//      ogs_warn("[create_publish_report] There is no data to report");
//      return;
//    }
//    ogs_warn("[create_publish_report] fillNotificationItem, the Noitve_size %lu\n", cvector_size(Notifvec));
//    for(size_t i = 0; i < cvector_size(Notifvec); i++){
//
//      json_t* callBack_Report = serialize_callBack(Notifvec[i], upfSub.notifyCorrelationId, 0);
//      if (!callBack_Report) {
//        ogs_warn("The json_t object is NULL.\n");
//      }
//      char *json_str = json_dumps(callBack_Report, JSON_INDENT(2));
//      publish (router,json_str, upfSub, type);
//    }
//    // to do: free the memory
//
//  }
//}
//void publish(void * router,char *json_data,UpfEventSubscription upfSub,EventType type){
//  zmq_send(router, upfSub.notifyCorrelationId, strlen(upfSub.notifyCorrelationId), ZMQ_SNDMORE);
//  zmq_send(router, json_data, strlen(json_data), 0);
//
//}
//void* Publish_EventReport(void*) {
//
//  int length = sizeof(supported_events) / sizeof(EventType);
//  for (int e = 0; e < length; e++){
//    time_t current_time;
//    time(&current_time);
//    pthread_mutex_lock(&subHash_lock);
//    if(subHash == NULL) subHash = new_str_btree();
//    cvector_vector_type(CreatedSubscription *) retrieved_vec = btreemap_get(subHash, getEventTypeString(supported_events[e]));
//    if (retrieved_vec!=NULL) {
//      ogs_warn("the retrieved_vec is not null");
//      for (int i = 0; i < cvector_size(retrieved_vec); i++) {
//        ogs_warn("Index i = %d", i);
//        ogs_warn("retrieved_vec pointer: %p", (void *)retrieved_vec);
//        ogs_warn("retrieved_vec[%d] pointer: %p", i, (void *)retrieved_vec[i]);
//        UpfEventSubscription  * upfEventSubscription = (retrieved_vec[i]->subscription);
//        if(upfEventSubscription->eventReportingMode.trigger == PERIODIC){
//          if(upfEventSubscription->eventReportingMode.TimeOfLastReport == 0){
//            if (current_time - upfEventSubscription->eventReportingMode.TimeOfSubscription >=upfEventSubscription->eventReportingMode.repPeriod){
//              create_send_report(retrieved_vec[i]->subscription_id,*upfEventSubscription, supported_events[e]); //change
//              upfEventSubscription->eventReportingMode.TimeOfLastReport = current_time;
//              upfEventSubscription->eventReportingMode.sent_reports += 1;
//            }
//          }
//          else if (current_time - upfEventSubscription->eventReportingMode.TimeOfLastReport >=upfEventSubscription->eventReportingMode.repPeriod){
//            create_send_report(retrieved_vec[i]->subscription_id,*upfEventSubscription, supported_events[e]); //change
//            upfEventSubscription->eventReportingMode.TimeOfLastReport = current_time;
//            upfEventSubscription->eventReportingMode.sent_reports += 1;
//          }
//        }
//
//      }
//    }
//    pthread_mutex_unlock(&subHash_lock);
//
//  }
//
//  return NULL;
//}
//
