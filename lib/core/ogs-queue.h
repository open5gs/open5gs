/* Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_QUEUE_H
#define OGS_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_queue_s ogs_queue_t;

ogs_queue_t *ogs_queue_create(unsigned int capacity);
void ogs_queue_destroy(ogs_queue_t *queue);

int ogs_queue_push(ogs_queue_t *queue, void *data);
int ogs_queue_pop(ogs_queue_t *queue, void **data);

int ogs_queue_trypush(ogs_queue_t *queue, void *data);
int ogs_queue_trypop(ogs_queue_t *queue, void **data);

int ogs_queue_timedpush(ogs_queue_t *queue, void *data, ogs_time_t timeout);
int ogs_queue_timedpop(ogs_queue_t *queue, void **data, ogs_time_t timeout);

unsigned int ogs_queue_size(ogs_queue_t *queue);

int ogs_queue_interrupt_all(ogs_queue_t *queue);
int ogs_queue_term(ogs_queue_t *queue);


#ifdef __cplusplus
}
#endif

#endif /* OGS_QUEUE_H */
