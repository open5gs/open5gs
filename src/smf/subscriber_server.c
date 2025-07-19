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
// Created by Fatemeh Shafiei Ardestani on 2024-12-10.
//

#include "subscriber_server.h"
#include "subscriber_client.h"
#include "ogs-gtp.h"
#include "context.h"
#include "pfcp-path.h"

#define SERVER_PORT 8081
struct MHD_Daemon *ee_daemon = NULL;
jmp_buf exceptionBuffer;
#define TRY if (setjmp(exceptionBuffer) == 0)
#define CATCH else
#include <time.h>
void get_relase_time(char *buffer, size_t buffer_size) {

  struct timeval tv;
  struct tm local_tm;
  gettimeofday(&tv, NULL);
  localtime_r(&tv.tv_sec, &local_tm);
  int millisec = tv.tv_usec / 1000;
  snprintf(buffer, buffer_size, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
           local_tm.tm_year + 1900, local_tm.tm_mon + 1, local_tm.tm_mday,
           local_tm.tm_hour, local_tm.tm_min, local_tm.tm_sec, millisec);

}
uint32_t convert_ipv4_string_to_uint32(const char *key_src) {

  uint32_t src_addr = 0;
  uint8_t byte1, byte2, byte3, byte4;

  // Parse the string into four separate bytes
  if (sscanf(key_src, "%hhu.%hhu.%hhu.%hhu", &byte1, &byte2, &byte3, &byte4) == 4) {
    src_addr = (byte4 << 24) | (byte3 << 16) | (byte2 << 8) | byte1;
  } else {
//    fprintf(stderr, "Invalid IPv4 address format: %s\n", key_src);
    return 0;
  }
  fatemeh_log("the provided ip is %s, %d",key_src, src_addr);

  return src_addr;
}
void parse_supi(const char *supi, char **ue_Ip, int *seid) {
  char *supi_copy = strdup(supi);
  char *token = strtok(supi_copy, "-");

  if (token != NULL) {
    *ue_Ip = strdup(token);
    token = strtok(NULL, "-");

    if (token != NULL) {
      *seid = atoi(token);
    } else {
      *seid = -1;
    }
  } else {
    *ue_Ip = NULL;
    *seid = -1;
  }

  free(supi_copy);
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
char *simple_message(const char *message_str) {
  char *formatted_message = NULL;

  size_t formatted_message_size = strlen(message_str) + 20;
  formatted_message = (char *)malloc(formatted_message_size);

  if (formatted_message) {
    snprintf(formatted_message, formatted_message_size, "{\"message\": \"%s\"}", message_str);
  }

  return formatted_message;
}

void log_api(const char *url, const char *method) {
  fatemeh_log("[server_info] We got request[%s] %s\n", method, url);
}
static void poll_http_server(void) {
  if (ee_daemon != NULL) {
//    fatemeh_log("[server_info] got request. \n");
    MHD_run(ee_daemon);
  }
}
HTTP_response evaluate_notification(const char *body){
  fatemeh_log("in the evaluate notification\n");
  if (body == NULL) {
    fatemeh_log("the body is null\n");
    return (HTTP_response) {
            .body = simple_message("No body provided."),
            .status = BAD_REQUEST
    };

  }
  json_error_t error;
  json_t *root = json_loads(body, 0, &error);
  if (!root) {
    fatemeh_log("Error parsing JSON\n");
    return (HTTP_response) {
            .body = simple_message("body cannot be parsed."),
            .status = BAD_REQUEST
    };
  }
  char r_time[50];
  get_relase_time(r_time, sizeof(r_time));
  ogs_warn("[Notif_Latency] notification received %s\n", r_time);
  json_t *abnorBehavrs = json_object_get(root, "abnorBehavrs");
  if (abnorBehavrs && json_is_array(abnorBehavrs)) {
    fatemeh_log("The JSON contains an array named abnorBehavrs.\n");
    size_t index;
    json_t *abnorBehavr;
    UEPduRatioPair ueRatioList[100];
    int ueRatioCount = 0;
    json_array_foreach(abnorBehavrs, index, abnorBehavr) {
      json_t *supis = json_object_get(abnorBehavr, "supis");
      if (supis && json_is_array(supis)) {
        size_t entryIndex;
        json_t *entry;
        json_array_foreach(supis, entryIndex, entry) {
          fatemeh_log("looping the supis entries.\n");
          if(json_is_string(entry)){
            const char *supi = json_string_value(entry);
            fatemeh_log("SUPI[%zu]: %s", index, supi);
            char *ue_Ip = NULL;
            int seId = 0;
            parse_supi(supi, &ue_Ip, &seId);

            if (seId != -1) {
              fatemeh_log("in the if trying to copy.\n");
              strncpy(ueRatioList[ueRatioCount].ueIP, ue_Ip, sizeof(ueRatioList[ueRatioCount].ueIP));
              fatemeh_log("ue ip copid.\n");
              ueRatioList[ueRatioCount].seId = seId;
//            strncpy(ueRatioList[ueRatioCount].seId, seId, sizeof(ueRatioList[ueRatioCount].seId));
              fatemeh_log("seid copied. the seId is %d\n", seId);
              uint32_t ueIP = convert_ipv4_string_to_uint32(ue_Ip);

              smf_sess_t * sess_ip = smf_sess_find_by_ipv4(ueIP);
              smf_sess_t * sess_id = smf_sess_find_by_seid(seId);
              if(sess_ip == NULL){
                fatemeh_log("the sess_ip is NULL");
              }
              if(sess_id == NULL){
                fatemeh_log("the sess_id is NULL");
              }
              fatemeh_log("calling the remove function.\n");
//            smf_sess_remove(sess);
              if(sess_id != NULL){
                smf_5gc_pfcp_send_session_deletion_request(sess_id,NULL,5);
                char c_time[50];
                get_relase_time(c_time, sizeof(c_time));
                ogs_warn("[Report_Latency]the session releas request is sent and the time is %s\n", c_time);

              }
              else{
                fatemeh_log("the session is already removed.\n");
              }

              fatemeh_log("after calling the remove function.\n");
              ueRatioCount++;
            }
          }


        }
        return (HTTP_response) {
                .body = simple_message("DDoS reported."),
                .status = OK
        };
      }else{
        return (HTTP_response) {
                .body = simple_message("No ddos array"),
                .status = OK
        };

      }
    }
//    manage_suspicious_session(ueRatioList, ueRatioCount); TODO
  }
  return (HTTP_response) {
          .body = simple_message(""),
          .status = OK
  };
}
enum MHD_Result default_handler(void *cls, struct MHD_Connection *connection, const char *url,
                                const char *method, const char *version, const char *upload_data,
                                size_t *upload_data_size, void **con_cls){
  fatemeh_log("in the default handler");
  if (NULL == *con_cls) {
    struct PostData *post_data = malloc(sizeof(struct PostData));
    post_data->data = NULL;
    post_data->size = 0;
    post_data->capacity = 0;
    *con_cls = post_data;
    return MHD_YES;
  }
  struct PostData *post_data = *con_cls;

  char *url_str = (char *)url;
  char *method_str = (char *)method;
  int ret;

  struct MHD_Response *response;
  HTTP_response response_api;

  log_api(url_str, method_str);

  TRY {

    if (strcmp(method, "POST") == 0) {
      if (*upload_data_size != 0) {
        // Ensure enough capacity
        if (post_data->size + *upload_data_size >= post_data->capacity) {
          post_data->capacity = post_data->size + *upload_data_size + 1;
          post_data->data = realloc(post_data->data, post_data->capacity);
        }

        // Copy the data
        memcpy(post_data->data + post_data->size, upload_data, *upload_data_size);
        post_data->size += *upload_data_size;
        post_data->data[post_data->size] = '\0';

        // Reset upload_data_size to signal that the data has been processed
        *upload_data_size = 0;

        return MHD_YES;
      }
    }
//    notification
    fatemeh_log("received subscription request!");
    char* is_subscribe_trigger = strstr(url_str, "/subscri");
    char* is_noitf = strstr(url_str, "/notificati");
    if (is_subscribe_trigger != NULL) {
      fatemeh_log("receiving sub req.");
      send_subscription_request();

      response_api = (HTTP_response){
              .body = simple_message("Subscribed"),
              .status = 200
      };

    }else if(is_noitf != NULL){
      fatemeh_log("receiving notif req.");
      response_api = evaluate_notification(post_data->data);
    }
    else{
      response_api = (HTTP_response){
              .body = simple_message( "The URL is wrong."),
              .status = BAD_REQUEST
      };
    }
//    /notification
  } CATCH {
    response_api = (HTTP_response){
            .body = simple_message("Internal server error"),
            .status = INTERNAL_SERVER_ERROR
    };

    fatemeh_log("Internal server error");
  }
  response = HTTP_build_response_JSON(response_api.body);
  if (!response){
    fatemeh_log("[FATEMEH] the response is null");
    return MHD_NO;
  }
  ret = MHD_queue_response(connection, response_api.status, response);
  MHD_destroy_response(response);
  fflush(0);
  return ret;
}

void * subscriber_server(void*){
  fatemeh_log("[smf_subscriber_server] Starting server to get requests \n");
  ee_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, SERVER_PORT, NULL, NULL,
                               &default_handler, NULL, MHD_OPTION_END);
  if (!ee_daemon) {
    fatemeh_log("[smf_subscriber_server] Starting server failed. \n");
    return NULL;
  }

  while (1) {
    struct timespec ts = {0, 100000000}; // 0.1 seconds
    nanosleep(&ts, NULL); // Sleep for 0.1 seconds to mimic wait time
    poll_http_server();
  }

  if (ee_daemon != NULL) {
    MHD_stop_daemon(ee_daemon);
    ee_daemon = NULL;
  }
  return NULL;

}