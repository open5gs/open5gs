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
