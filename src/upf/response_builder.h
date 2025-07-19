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
#include <string.h>
#include <stdio.h>
#include "ee_event.h"
#include "decoder.h"
#include "encoder.h"
#include <jansson.h>
#include "cvector_utils.h"
#include "utils.h"
#include <microhttpd.h>
#include "types.h"

typedef struct MHD_Response* RSPX;

HTTP_response create_subscription(const char *body, bool *created, char **newSubId);
HTTP_response subscription_router(const char *url, const char *method, const char *body, char *subscription_id, bool *created,
                                  char **newSubId);
RSPX HTTP_build_response_JSON(const char *message);
RSPX HTTP_build_created_response_JSON(const char *message, const char* newSubId, char * url_str);