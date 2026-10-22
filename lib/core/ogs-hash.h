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

/* Slot storage is allocated on the first insertion. */
ogs_hash_t *ogs_hash_make(void);
void ogs_hash_destroy(ogs_hash_t *ht);

/*
 * The table keeps its own copy of every key. ogs_hash_set() and
 * ogs_hash_get_or_set() copy the key when they add an entry, so the
 * caller's key buffer may change or be freed right after the call. Setting
 * a key that is already present replaces only the value and keeps the
 * stored copy. The copy is released when the entry is removed
 * (ogs_hash_set(..., NULL)), cleared or destroyed.
 * key must be non-NULL. klen is a nonnegative byte count (including
 * zero), or OGS_HASH_KEY_STRING to use strlen(key).
 *
 * ogs_hash_this(), ogs_hash_this_key() and ogs_hash_do() hand out the
 * table's read-only copy, followed by a NUL even for an explicit length.
 * Hashing and comparison use only the klen key bytes. Do not modify or
 * free this copy. A new insertion, successful removal, clear() or
 * destroy() may invalidate ALL returned key pointers, including those
 * of other entries. Value replacement and unsuccessful removal do not
 * relocate entries. Use memcpy() when reading a key as a typed value.
 * Values are never copied or freed. Access must be externally serialized.
 *
 * Updating a context's key does not rename its entry: remove the old
 * mapping explicitly and insert the new key. Use unset_if_owner() when
 * another context may have taken over the old mapping.
 */
#define ogs_hash_set(ht, key, klen, val) \
    ogs_hash_set_debug(ht, key, klen, val, OGS_FILE_LINE)
void ogs_hash_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line);
#define ogs_hash_get(ht, key, klen) \
    ogs_hash_get_debug(ht, key, klen, OGS_FILE_LINE)
void *ogs_hash_get_debug(ogs_hash_t *ht,
        const void *key, int klen, const char *file_line);

/*
 * get_or_set() returns the existing value without changing the stored
 * key or value. If absent and val is non-NULL, it copies key, inserts
 * val and returns val; otherwise it returns NULL without inserting.
 */
#define ogs_hash_get_or_set(ht, key, klen, val) \
    ogs_hash_get_or_set_debug(ht, key, klen, val, OGS_FILE_LINE)
void *ogs_hash_get_or_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line);

/*
 * Clear an entry ONLY if it still points at the expected owner.
 *
 * Identity-keyed tables (SUPI/SUCI/GUTI/IMSI/...) can be re-claimed by a
 * freshly created context while an older context that holds the same key is
 * still being torn down (re-attach races, deferred removal). A blind
 * ogs_hash_set(ht, key, klen, NULL) in the old context's teardown would then
 * orphan the newer entry, leaving the live context unfindable.
 *
 * owner is the expected value pointer, independent of key ownership.
 * Returns true if the entry was unset (owner matched), false if absent
 * or if its value did not match.
 */
#define ogs_hash_unset_if_owner(ht, key, klen, owner) \
    ogs_hash_unset_if_owner_debug(ht, key, klen, owner, OGS_FILE_LINE)
bool ogs_hash_unset_if_owner_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *owner, const char *file_line);

/*
 * Iterate in unspecified order using one internal iterator per table.
 * Another first() on the same table replaces that iterator's state.
 * During iteration, only deletion of the current entry is supported;
 * after removal, call next() without inspecting the removed entry again.
 * Starting at an empty boundary and traversing backwards visits each
 * original entry once, including when current entries are removed.
 * Insertion invalidates the traversal; restart it with first(). Continuing
 * after insertion does not guarantee visiting every entry.
 */
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

/*
 * Uses separate traversal state without replacing first()'s iterator.
 * Return 1 after a full traversal (including an empty table), or 0 if
 * comp returns 0. Only the callback's current entry may be removed;
 * overlapping traversals that modify the table are not supported.
 */
int ogs_hash_do(ogs_hash_do_callback_fn_t *comp,
        void *rec, const ogs_hash_t *ht);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OGS_HASH_H */
