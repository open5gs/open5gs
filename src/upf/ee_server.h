//
// Created by Fatemeh Shafiei Ardestani on 2024-08-20.
//

#ifndef UPG_VPP_EE_SERVER_H
#define UPG_VPP_EE_SERVER_H
#include "ogs-gtp.h"
#include <microhttpd.h>
#include "handler.h"
#include <microhttpd.h>
#include "ee_event.h"
#include "stb_ds.h"
//static void poll_http_server();
void request_completed_callback(void *cls, struct MHD_Connection *connection,
                                void **con_cls, enum MHD_RequestTerminationCode toe);
void* ee_http_server(void *arg);
#endif //UPG_VPP_EE_SERVER_H
