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
// Created by Fatemeh Shafiei Ardestani on 2024-07-07.
//

#ifndef REST_API_C__EE_EVENT_H
#define REST_API_C__EE_EVENT_H

#ifdef DEFINE_UPF_STORAGE
#define EXTERN
#define INITIALIZER(...) = __VA_ARGS__
#else
#define EXTERN extern
#define INITIALIZER(...)
#endif

#include "cvector.h"
#include "types.h"
#include "stb_ds.h"
#include "btree.h"

EXTERN cvector_vector_type(CreatedSubscription) subscriptionList INITIALIZER(NULL);
EXTERN EventType supported_events[2] INITIALIZER({USER_DATA_USAGE_MEASURES, USER_DATA_USAGE_TRENDS});
EXTERN BTreeMap subHash INITIALIZER(NULL);
EXTERN pthread_mutex_t subHash_lock;
EXTERN pthread_mutex_t subID_lock;
EXTERN cvector_vector_type(char*) subIDList INITIALIZER(NULL);

//int initialize_hashmap(const unsigned initial_size){
//  return hashmap_create(initial_size, &hashmap);;
//}

#endif //REST_API_C__EE_EVENT_H