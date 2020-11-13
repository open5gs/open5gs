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

#ifndef OGS_HASH_H
#define OGS_HASH_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_HASH_KEY_STRING     (-1)

typedef struct ogs_hash_t ogs_hash_t;
typedef struct ogs_hash_index_t ogs_hash_index_t;
typedef unsigned int (*ogs_hashfunc_t)(const char *key, int *klen);
unsigned int ogs_hashfunc_default(const char *key, int *klen);

ogs_hash_t *ogs_hash_make(void);
ogs_hash_t *ogs_hash_make_custom(ogs_hashfunc_t ogs_hash_func);
void ogs_hash_destroy(ogs_hash_t *ht);

void ogs_hash_set(ogs_hash_t *ht, const void *key, int klen, const void *val);
void *ogs_hash_get(ogs_hash_t *ht, const void *key, int klen);
void *ogs_hash_get_or_set(ogs_hash_t *ht, 
        const void *key, int klen, const void *val);

ogs_hash_index_t *ogs_hash_first(ogs_hash_t *ht);
ogs_hash_index_t *ogs_hash_next(ogs_hash_index_t *hi);
void ogs_hash_this(ogs_hash_index_t *hi, 
        const void **key, int *klen, void **val);

const void* ogs_hash_this_key(ogs_hash_index_t *hi);
int ogs_hash_this_key_len(ogs_hash_index_t *hi);
void* ogs_hash_this_val(ogs_hash_index_t *hi);
unsigned int ogs_hash_count(ogs_hash_t *ht);
void ogs_hash_clear(ogs_hash_t *ht);

typedef int (ogs_hash_do_callback_fn_t)(
        void *rec, const void *key, int klen, const void *value);

int ogs_hash_do(ogs_hash_do_callback_fn_t *comp,
        void *rec, const ogs_hash_t *ht);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OGS_HASH_H */
