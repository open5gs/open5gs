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
//#include <zmq.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include "ee_event.h"
//#include "decoder.h"
//#include <string.h>
//#include <stdio.h>
//#include "encoder.h"
//#include <jansson.h>
//#include "cvector_utils.h"
//#include "utils.h"
//#include "types.h"
//#include "publish_report.h"
//#include "manage_collection.h"
//
//
//void create_publish_subscription(const char *body, bool *created, char **newSubId) {
//
//  if (body == NULL) {
//    *created = false;
//    return;
//  }
//  UpfEventSubscription *new_subscription = NULL;
//  new_subscription = parse_subscription_request(body);
//  size_t sub_len = cvector_size(new_subscription->EventList);
//
//  cvector_vector_type(int)to_remove = NULL;
//  for (int i = 0; i < sub_len; ++i) {
//    int supported_events_length = sizeof(supported_events) / sizeof(EventType);
//
//    bool isAvailable = false;
//    for (int j = 0; j < supported_events_length; ++j) {
//      ogs_warn("[server_info] creating subscription %d %d\n", new_subscription->EventList[i].type, supported_events[j]);
//
//      if (new_subscription->EventList[i].type == supported_events[j]) {
//        isAvailable = true;
//        break;
//      }
//    }
//    if (!isAvailable) {
//      cvector_push_back(to_remove, i);
//    }
//  }
//
//  int* tri;
//  cvector_for_each_in(tri, to_remove) {
//    printf("\nTO REMOVE %d\n", *tri);
//    cvector_erase(new_subscription->EventList, *tri);
//  }
//
//  CreatedSubscription * createdSubscription = malloc(sizeof(CreatedSubscription));
//  createdSubscription->subscription = new_subscription;
//  int subId = cvector_size(subscriptionList);
//  createdSubscription->subscription_id = malloc(7 + 1);
//
//  sprintf(createdSubscription->subscription_id, "%d", subId);
//  cvector_push_back(subscriptionList, createdSubscription);
//  pthread_mutex_lock(&subID_lock);
//  cvector_push_back(subIDList, createdSubscription->subscription_id);
//  pthread_mutex_unlock(&subID_lock);
//  sub_len = cvector_size(new_subscription->EventList);
//  for(int i = 0; i < sub_len; i++){
//    cvector_vector_type(CreatedSubscription *) v = NULL;
//    pthread_mutex_lock(&subHash_lock);
//    if(subHash == NULL){
//      subHash = new_str_btree();
//      ogs_warn("the subhash is created");
//    }
//    v = btreemap_get(subHash, getEventTypeString(new_subscription->EventList[i].type));
//    cvector_push_back(v,createdSubscription);
//    btreemap_insert_str(subHash, getEventTypeString(new_subscription->EventList[i].type), v);
//  }
//  pthread_mutex_unlock(&subHash_lock);
//
//  *created = true;
//  is_active = true;
//  ogs_warn("\nthe length of subscribers  %lu\n",cvector_size(subscriptionList));
//
//  body = json_dumps(serialize_created_response(new_subscription, *newSubId), JSON_ENCODE_ANY);
//  int status = 201;
//  return;
//
//}
//
//bool method_validation(const char *method, const char *valid_method) {
//  return strcmp(method, valid_method) == 0;
//}
//
//void* publisher(void*) {
//  void *context = zmq_ctx_new();
//  void *router = zmq_socket(context, ZMQ_ROUTER);
//  zmq_bind(router, "tcp://*:5555");
//
//  printf("Publisher started.\n");
//
//
//  while (1) {
//    zmq_pollitem_t items[] = { {router, 0, ZMQ_POLLIN, 0} };
//    zmq_poll(items, 1, 1000);
//
//    if (items[0].revents & ZMQ_POLLIN) {
//      zmq_msg_t id_msg;
//      zmq_msg_init(&id_msg);
//      zmq_msg_recv(&id_msg, router, 0);
//      size_t id_size = zmq_msg_size(&id_msg);
//      char *id = (char *)malloc(id_size + 1);
//      memcpy(id, zmq_msg_data(&id_msg), id_size);
//      id[id_size] = '\0'; // Null-terminate
//      zmq_msg_close(&id_msg);
//      zmq_msg_t request_msg;
//      zmq_msg_init(&request_msg);
//      zmq_msg_recv(&request_msg, router, 0);
//      size_t req_size = zmq_msg_size(&request_msg);
//      char *request = (char *)malloc(req_size + 1);
//      memcpy(request, zmq_msg_data(&request_msg), req_size);
//      request[req_size] = '\0'; // Null-terminate
//      zmq_msg_close(&request_msg);
//
//      printf("Received request:\n%s\n", request);
//
//      char *method = NULL, *request_url = NULL, *body = NULL;
//      method = strtok(request, "\n");
//      request_url = strtok(NULL, "\n");
//      body = strtok(NULL, ""); // Remaining part is the body
//
//      if (!method || !request_url || !body) {
//        printf("Invalid request format.\n");
//        zmq_send(router, id, strlen(id), ZMQ_SNDMORE);
//        zmq_send(router, "ERROR: Invalid request format", 29, 0);
//        free(id);
//        free(request);
//        continue;
//      }
//
//      method += 8;  // Skip "METHOD: "
//      request_url += 5;  // Skip "URL: "
//
//      if (strstr(request_url, "ee-subscriptions") == NULL) {
//        printf("❌ Invalid subscription request, ignoring.\n");
//        zmq_send(router, id, strlen(id), ZMQ_SNDMORE);
//        zmq_send(router, "ERROR: Invalid URL", 18, 0);
//        free(id);
//        free(request);
//        continue;
//      }
//
//      bool created = false;
//      char *newSubId = NULL;
//      if (method_validation(method, "POST")) {
//        create_publish_subscription(body, &created, &newSubId);
//        if (created) {
//          printf("✅ Subscription created with ID: %s\n", newSubId);
//          zmq_send(router, id, strlen(id), ZMQ_SNDMORE);
//          zmq_send(router, body, strlen(body), 0);
//
//          free(newSubId);
//        } else {
//          printf("Subscription creation failed.\n");
//          zmq_send(router, id, strlen(id), ZMQ_SNDMORE);
//          zmq_send(router, "ERROR: Subscription failed", 27, 0);
//        }
//      } else {
//        printf("❌ Unsupported method [%s], ignoring request.\n", method);
//        zmq_send(router, id, strlen(id), ZMQ_SNDMORE);
//        zmq_send(router, "ERROR: Method Not Allowed", 26, 0);
//      }
//
//      free(id);
//      free(request);
//    }
//
//    sleep(1);
//  }
//  EventReport(router);
//
//
//  zmq_close(router);
//  zmq_ctx_destroy(context);
//  return;
//}
