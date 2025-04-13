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
// Created by Fatemeh Shafiei Ardestani on 2024-08-20.
//

#include "ee_server.h"
#include <microhttpd.h>
#include <time.h>

#define SERVER_PORT 4355

struct MHD_Daemon *ee_daemon = NULL;
void request_completed_callback(void *cls, struct MHD_Connection *connection,
                                void **con_cls, enum MHD_RequestTerminationCode toe) {
  fatemeh_log("freeing the request memory");
  struct PostData *post_data = *con_cls;

  if (post_data) {
    if (post_data->data) {
      free(post_data->data);
    }
    free(post_data);
  }
  fatemeh_log("freed the request memory");
}


//static void poll_http_server(void) {
//  if (ee_daemon != NULL) {
////    fatemeh_log("[server_info] got request. \n");
//    MHD_run(ee_daemon);
//  }
//}

void* ee_http_server(void *arg) {

  fatemeh_log("[server_info] Starting server to get requests \n");
  ee_daemon = MHD_start_daemon(MHD_USE_INTERNAL_POLLING_THREAD, SERVER_PORT, NULL, NULL,
                            &default_handler, NULL, MHD_OPTION_NOTIFY_COMPLETED, request_completed_callback, NULL,MHD_OPTION_END);
  if (!ee_daemon) {
    fatemeh_log("[server_info] Starting server failed. Errno: %d - %s\n", errno, strerror(errno));
    return NULL;
  }

  while (1) {
    struct timespec ts = {0, 100000000};
    nanosleep(&ts, NULL);
//    poll_http_server();
  }

  if (ee_daemon != NULL) {
    MHD_stop_daemon(ee_daemon);
    ee_daemon = NULL;
  }
  return NULL;
}


