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
// Created by Fatemeh Shafiei Ardestani on 2024-08-18.
//
#include "response_builder.h"
#include "ee_event.h"
#include "manage_collection.h"
HTTP_response create_subscription(const char *body, bool *created, char **newSubId) {

  if (body == NULL) {
    return (HTTP_response) {
            .body = simple_message("No body provided."),
            .status = BAD_REQUEST
    };

  }
  fatemeh_log("[server_info] creating subscription");
  CreatedSubscription *createdSubscription = malloc(sizeof(CreatedSubscription));
  createdSubscription->subscription = parse_subscription_request(body);
  size_t sub_len = cvector_size(createdSubscription->subscription->EventList);

  cvector_vector_type(int)to_remove = NULL;
  for (int i = 0; i < sub_len; ++i) {
    int supported_events_length = sizeof(supported_events) / sizeof(EventType);

    bool isAvailable = false;
    for (int j = 0; j < supported_events_length; ++j) {
      fatemeh_log("[server_info] creating subscription %d %d\n", createdSubscription->subscription->EventList[i].type, supported_events[j]);

      if (createdSubscription->subscription->EventList[i].type == supported_events[j]) {
        isAvailable = true;
        break;
      }
    }
    if (!isAvailable) {
      cvector_push_back(to_remove, i);
    }
  }

  int* tri;
  cvector_for_each_in(tri, to_remove) {
    printf("\nTO REMOVE %d\n", *tri);
    cvector_erase(createdSubscription->subscription->EventList, *tri);
  }

  cvector_push_back(subscriptionList, *createdSubscription);
  sub_len = cvector_size(createdSubscription->subscription->EventList);
  for(int i = 0; i < sub_len; i++){
    cvector_vector_type(CreatedSubscription *) v = NULL;
    pthread_mutex_lock(&subHash_lock);
    if(subHash == NULL){
      subHash = new_str_btree();
      fatemeh_log("the subhash is created");
    }
    v = btreemap_get(subHash, getEventTypeString(createdSubscription->subscription->EventList[i].type));
    cvector_push_back(v,createdSubscription);
    btreemap_insert_str(subHash, getEventTypeString(createdSubscription->subscription->EventList[i].type), v);
  }
  pthread_mutex_unlock(&subHash_lock);

  int subId = cvector_size(subscriptionList) + 10;
  *newSubId = malloc(7 + 1);
  sprintf(*newSubId, "%d", subId);
  *(newSubId + 7) = 0;
  createdSubscription->subscription_id = malloc(7 + 1);
  sprintf(createdSubscription->subscription_id, "%d", subId);
  *created = true;
  is_active = true;
  fatemeh_log("\nthe length of subscribers  %lu\n",cvector_size(subscriptionList));
  pthread_mutex_lock(&subID_lock);
  cvector_push_back(subIDList,createdSubscription->subscription_id);
  //cvector_push_back(subIDList,*newSubId);
  pthread_mutex_unlock(&subID_lock);

  return (HTTP_response) {
          .body = json_dumps(serialize_created_response(createdSubscription->subscription, *newSubId), JSON_ENCODE_ANY),
          .status = CREATED
  };
  return (HTTP_response) {
          .body = simple_message("Not implemented 2"),
          .status = CREATED
  };
}

HTTP_response subscription_router(const char *url, const char *method, const char *body, char *subscription_id, bool *created,
                                  char **newSubId) {
  if (*subscription_id == 0) {
    if (validate_method(method, "POST")) {
      fatemeh_log("the subscription is created");
      return create_subscription(body, created, newSubId);

    }
  }
  //To DO: change the subscription
//  else{
//    subscription_id += strlen("/");
//
//  }

  return (HTTP_response) {
          .body = simple_message("Not implemented 2"),
          .status = NOT_IMPLEMENTED
  };


}
RSPX HTTP_build_response_JSON(const char *message) {
  struct MHD_Response *response;
  fatemeh_log("the enum is %d\n",MHD_RESPMEM_PERSISTENT);
  void * fa = (void *)message;
  int x = strlen(message);
  response = MHD_create_response_from_buffer_static(x, fa);


  if (!response)
    return NULL;

  MHD_add_response_header(response, "Content-Type", "application/json");
  return response;
}
RSPX HTTP_build_created_response_JSON(const char *message, const char* newSubId, char * url_str) {
  struct MHD_Response *response;
  fatemeh_log("[FATEMEH] in the create response");
  response = MHD_create_response_from_buffer(strlen(message), (void *)message, MHD_RESPMEM_PERSISTENT);
  size_t url_len = strlen(url_str);
  size_t sub_id_len = strlen(newSubId);
  size_t total_len = url_len + sub_id_len + 1;

  char *location = (char *)malloc(total_len);

  strcpy(location, url_str);
  strcat(location, newSubId);

  if (!response)
    return NULL;
  MHD_add_response_header(response,"Location",location);
  MHD_add_response_header(response, "Content-Type", "application/json");
  fatemeh_log("[FATEMEH] in the create response, End of function");
  return response;
}