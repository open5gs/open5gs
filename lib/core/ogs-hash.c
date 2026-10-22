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

/*
 * Robin Hood insertion, backward-shift deletion and reverse iteration
 * follow PostgreSQL src/include/lib/simplehash.h. PostgreSQL notice:
 *
 * PostgreSQL Database Management System
 * (also known as Postgres, formerly known as Postgres95)
 *
 * Portions Copyright (c) 1996-2026, PostgreSQL Global Development Group
 *
 * Portions Copyright (c) 1994, The Regents of the University of California
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without a written agreement
 * is hereby granted, provided that the above copyright notice and this
 * paragraph and the following two paragraphs appear in all copies.
 *
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING
 * LOST PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
 * DOCUMENTATION, EVEN IF THE UNIVERSITY OF CALIFORNIA HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATIONS TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

/*
 * SipHash-2-4 adapted from https://github.com/veorq/SipHash, using its
 * MIT license option. Original implementation:
 * Copyright (c) 2012-2022 Jean-Philippe Aumasson
 * Copyright (c) 2012-2014 Daniel J. Bernstein
 * Upstream MIT notice:
 *
 * Copyright 2012-2024 JP Aumasson
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "ogs-core.h"

#include <limits.h>

#define KEY_INLINE_SIZE 24
#define INITIAL_CAPACITY 16

typedef struct ogs_hash_slot_t ogs_hash_slot_t;
struct ogs_hash_slot_t {
    const void          *val;   /* NULL marks an empty slot */
    uint32_t            hash;
    int                 klen;
    union {
        char            bytes[KEY_INLINE_SIZE];
        char            *ptr;
    } key;
};

struct ogs_hash_index_t {
    ogs_hash_t          *ht;
    unsigned int        index;
    unsigned int        next;
    unsigned int        remaining;
};

struct ogs_hash_t {
    ogs_hash_slot_t     *slots;
    ogs_hash_index_t    iterator;
    unsigned int        count;
    unsigned int        capacity;
};

/* Initialized once, before publishing any table; lookups need no lock. */
static uint8_t hash_secret[16];

#ifdef _WIN32
static INIT_ONCE hash_once = INIT_ONCE_STATIC_INIT;

static BOOL CALLBACK init_hash_secret(
        PINIT_ONCE once, PVOID parameter, PVOID *context)
{
    ogs_random(hash_secret, sizeof(hash_secret));
    return TRUE;
}
#else
static pthread_once_t hash_once = PTHREAD_ONCE_INIT;

static void init_hash_secret(void)
{
    ogs_random(hash_secret, sizeof(hash_secret));
}
#endif

static uint64_t load64_le(const uint8_t *p)
{
    return (uint64_t)p[0] | ((uint64_t)p[1] << 8) |
        ((uint64_t)p[2] << 16) | ((uint64_t)p[3] << 24) |
        ((uint64_t)p[4] << 32) | ((uint64_t)p[5] << 40) |
        ((uint64_t)p[6] << 48) | ((uint64_t)p[7] << 56);
}

static uint64_t rotl64(uint64_t v, unsigned int bits)
{
    return (v << bits) | (v >> (64 - bits));
}

#define HASH_SIPROUND() do { \
    v0 += v1; v1 = rotl64(v1, 13); v1 ^= v0; v0 = rotl64(v0, 32); \
    v2 += v3; v3 = rotl64(v3, 16); v3 ^= v2; \
    v0 += v3; v3 = rotl64(v3, 21); v3 ^= v0; \
    v2 += v1; v1 = rotl64(v1, 17); v1 ^= v2; v2 = rotl64(v2, 32); \
} while (0)

/* Byte loads make both unaligned keys and host endianness immaterial. */
static uint64_t siphash24(
        const void *key, size_t length, const uint8_t secret[16])
{
    const uint8_t *p = key;
    uint64_t k0 = load64_le(secret);
    uint64_t k1 = load64_le(secret + 8);
    uint64_t v0 = UINT64_C(0x736f6d6570736575) ^ k0;
    uint64_t v1 = UINT64_C(0x646f72616e646f6d) ^ k1;
    uint64_t v2 = UINT64_C(0x6c7967656e657261) ^ k0;
    uint64_t v3 = UINT64_C(0x7465646279746573) ^ k1;
    uint64_t last = (uint64_t)(length & 0xff) << 56;
    size_t remaining = length;
    unsigned int i;

    while (remaining >= 8) {
        uint64_t m = load64_le(p);

        v3 ^= m;
        HASH_SIPROUND();
        HASH_SIPROUND();
        v0 ^= m;
        p += 8;
        remaining -= 8;
    }
    for (i = 0; i < remaining; i++)
        last |= (uint64_t)p[i] << (8 * i);

    v3 ^= last;
    HASH_SIPROUND();
    HASH_SIPROUND();
    v0 ^= last;
    v2 ^= 0xff;
    HASH_SIPROUND();
    HASH_SIPROUND();
    HASH_SIPROUND();
    HASH_SIPROUND();
    return v0 ^ v1 ^ v2 ^ v3;
}

#undef HASH_SIPROUND

static uint32_t hash_key(const void *key, int *klen)
{
    if (*klen == OGS_HASH_KEY_STRING) {
        size_t length = strlen(key);

        ogs_assert(length <= INT_MAX);
        *klen = (int)length;
    }
    return (uint32_t)siphash24(key, (size_t)*klen, hash_secret);
}

static const void *slot_key(const ogs_hash_slot_t *slot)
{
    if (slot->klen < KEY_INLINE_SIZE)
        return slot->key.bytes;
    return slot->key.ptr;
}

static void free_key(ogs_hash_slot_t *slot)
{
    if (slot->klen >= KEY_INLINE_SIZE)
        ogs_free(slot->key.ptr);
}

static void prepare_slot(ogs_hash_slot_t *slot,
        const void *key, int klen, const void *val, uint32_t hash)
{
    char *copy;

    memset(slot, 0, sizeof(*slot));
    slot->hash = hash;
    slot->klen = klen;
    if (klen < KEY_INLINE_SIZE)
        copy = slot->key.bytes;
    else {
        copy = ogs_malloc((size_t)klen + 1);
        ogs_assert(copy);
        slot->key.ptr = copy;
    }
    memcpy(copy, key, klen);
    copy[klen] = '\0';
    slot->val = val;
}

static unsigned int probe_distance(
        uint32_t hash, unsigned int index, unsigned int mask)
{
    return (index - (hash & mask)) & mask;
}

/* A resident closer to home proves this key cannot occur further on. */
static ogs_hash_slot_t *find_slot(
        ogs_hash_t *ht, const void *key, int klen, uint32_t hash)
{
    unsigned int index, distance = 0, mask;

    if (!ht->count)
        return NULL;

    mask = ht->capacity - 1;
    index = hash & mask;
    for (;;) {
        ogs_hash_slot_t *slot = &ht->slots[index];

        if (!slot->val ||
                probe_distance(slot->hash, index, mask) < distance)
            return NULL;
        if (slot->hash == hash && slot->klen == klen &&
                memcmp(slot_key(slot), key, klen) == 0)
            return slot;

        index = (index + 1) & mask;
        distance++;
    }
}

/*
 * The caller has already excluded duplicates and left an empty slot.
 * Swapping whole slots transfers long-key pointers without copying or
 * freeing their allocations; inline keys travel with the slot itself.
 */
static void insert_slot(ogs_hash_slot_t *slots,
        unsigned int capacity, ogs_hash_slot_t entry)
{
    unsigned int mask = capacity - 1;
    unsigned int index = entry.hash & mask;
    unsigned int distance = 0;

    for (;;) {
        ogs_hash_slot_t *slot = &slots[index];
        unsigned int resident_distance;

        if (!slot->val) {
            *slot = entry;
            return;
        }
        resident_distance = probe_distance(slot->hash, index, mask);
        if (resident_distance < distance) {
            ogs_hash_slot_t displaced = *slot;

            *slot = entry;
            entry = displaced;
            distance = resident_distance;
        }
        index = (index + 1) & mask;
        distance++;
    }
}

static void grow_slots(ogs_hash_t *ht)
{
    ogs_hash_slot_t *slots;
    unsigned int capacity, i;

    ogs_assert(ht->capacity <= UINT_MAX / 2);
    capacity = ht->capacity ? ht->capacity * 2 : INITIAL_CAPACITY;
    ogs_assert(sizeof(*slots) <= SIZE_MAX / (size_t)capacity);
    slots = ogs_calloc((size_t)capacity, sizeof(*slots));
    ogs_assert(slots);

    for (i = 0; i < ht->capacity; i++) {
        if (ht->slots[i].val)
            insert_slot(slots, capacity, ht->slots[i]);
    }
    if (ht->slots)
        ogs_free(ht->slots);
    ht->slots = slots;
    ht->capacity = capacity;
}

static void add_entry(ogs_hash_t *ht,
        const void *key, int klen, const void *val, uint32_t hash)
{
    ogs_hash_slot_t entry;

    /*
     * key may point into the old array, or be a prefix of a stored key.
     * Finish its independent copy before growth or Robin Hood swaps.
     */
    prepare_slot(&entry, key, klen, val, hash);
    if (!ht->capacity ||
            ht->count >= (uint64_t)ht->capacity * 9 / 10)
        grow_slots(ht);
    insert_slot(ht->slots, ht->capacity, entry);
    ht->count++;
}

static void remove_slot(ogs_hash_t *ht, ogs_hash_slot_t *slot)
{
    unsigned int mask = ht->capacity - 1;
    unsigned int index = (unsigned int)(slot - ht->slots);

    free_key(slot);
    for (;;) {
        unsigned int next = (index + 1) & mask;

        /*
         * This stop-at-home rule relies on Robin Hood insertion.
         * A plain linear-probing table would need a different test.
         */
        if (!ht->slots[next].val ||
                !probe_distance(ht->slots[next].hash, next, mask))
            break;
        ht->slots[index] = ht->slots[next];
        index = next;
    }
    memset(&ht->slots[index], 0, sizeof(ht->slots[index]));
    ht->count--;
}

ogs_hash_t *ogs_hash_make(void)
{
    ogs_hash_t *ht;

#ifdef _WIN32
    BOOL initialized = InitOnceExecuteOnce(
            &hash_once, init_hash_secret, NULL, NULL);
    ogs_assert(initialized);
#else
    int rv = pthread_once(&hash_once, init_hash_secret);
    ogs_assert(rv == 0);
#endif

    ht = ogs_calloc(1, sizeof(*ht));
    if (!ht)
        ogs_error("ogs_calloc() failed");
    return ht;
}

void ogs_hash_clear(ogs_hash_t *ht)
{
    unsigned int i;

    ogs_assert(ht);
    if (!ht->count)
        return;
    for (i = 0; i < ht->capacity; i++) {
        if (ht->slots[i].val)
            free_key(&ht->slots[i]);
    }
    memset(ht->slots, 0, (size_t)ht->capacity * sizeof(*ht->slots));
    ht->count = 0;
}

void ogs_hash_destroy(ogs_hash_t *ht)
{
    ogs_assert(ht);
    ogs_hash_clear(ht);
    if (ht->slots)
        ogs_free(ht->slots);
    ogs_free(ht);
}

void *ogs_hash_get_debug(ogs_hash_t *ht,
        const void *key, int klen, const char *file_line)
{
    ogs_hash_slot_t *slot;
    uint32_t hash;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen == OGS_HASH_KEY_STRING || klen >= 0);

    if (!ht->count)
        return NULL;
    hash = hash_key(key, &klen);
    slot = find_slot(ht, key, klen, hash);
    return slot ? (void *)slot->val : NULL;
}

void ogs_hash_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line)
{
    ogs_hash_slot_t *slot;
    uint32_t hash;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen == OGS_HASH_KEY_STRING || klen >= 0);

    if (!ht->count && !val)
        return;
    hash = hash_key(key, &klen);
    slot = find_slot(ht, key, klen, hash);
    if (slot) {
        if (val)
            slot->val = val;
        else
            remove_slot(ht, slot);
    } else if (val)
        add_entry(ht, key, klen, val, hash);
}

void *ogs_hash_get_or_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line)
{
    ogs_hash_slot_t *slot;
    uint32_t hash;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen == OGS_HASH_KEY_STRING || klen >= 0);

    if (!ht->count && !val)
        return NULL;
    hash = hash_key(key, &klen);
    slot = find_slot(ht, key, klen, hash);
    if (slot)
        return (void *)slot->val;
    if (val)
        add_entry(ht, key, klen, val, hash);
    return (void *)val;
}

bool ogs_hash_unset_if_owner_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *owner, const char *file_line)
{
    ogs_hash_slot_t *slot;
    uint32_t hash;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen == OGS_HASH_KEY_STRING || klen >= 0);
    ogs_assert(owner);

    hash = hash_key(key, &klen);
    slot = find_slot(ht, key, klen, hash);
    if (slot && slot->val == owner) {
        remove_slot(ht, slot);
        return true;
    }
    ogs_warn("%s: hash unset skipped: entry not owned by expected context",
            file_line);
    return false;
}

unsigned int ogs_hash_count(ogs_hash_t *ht)
{
    ogs_assert(ht);
    return ht->count;
}

static void init_iterator(ogs_hash_t *ht, ogs_hash_index_t *hi)
{
    unsigned int boundary;

    memset(hi, 0, sizeof(*hi));
    hi->ht = ht;
    if (!ht->count)
        return;

    /* There is always an empty slot at the configured maximum load. */
    for (boundary = 0; ht->slots[boundary].val; boundary++)
        ;
    hi->next = boundary;
    hi->remaining = ht->capacity;
}

ogs_hash_index_t *ogs_hash_next(ogs_hash_index_t *hi)
{
    ogs_assert(hi);

    while (hi->remaining) {
        unsigned int index = hi->next;

        /* Save the next position before the caller can delete current. */
        hi->next = (index - 1) & (hi->ht->capacity - 1);
        hi->remaining--;
        if (hi->ht->slots[index].val) {
            hi->index = index;
            return hi;
        }
    }
    return NULL;
}

ogs_hash_index_t *ogs_hash_first(ogs_hash_t *ht)
{
    ogs_assert(ht);
    init_iterator(ht, &ht->iterator);
    return ogs_hash_next(&ht->iterator);
}

void ogs_hash_this(ogs_hash_index_t *hi,
        const void **key, int *klen, void **val)
{
    ogs_hash_slot_t *slot;

    ogs_assert(hi);
    ogs_assert(hi->ht);
    ogs_assert(hi->index < hi->ht->capacity);
    slot = &hi->ht->slots[hi->index];
    ogs_assert(slot->val);

    if (key)
        *key = slot_key(slot);
    if (klen)
        *klen = slot->klen;
    if (val)
        *val = (void *)slot->val;
}

const void *ogs_hash_this_key(ogs_hash_index_t *hi)
{
    const void *key;

    ogs_hash_this(hi, &key, NULL, NULL);
    return key;
}

int ogs_hash_this_key_len(ogs_hash_index_t *hi)
{
    int klen;

    ogs_hash_this(hi, NULL, &klen, NULL);
    return klen;
}

void *ogs_hash_this_val(ogs_hash_index_t *hi)
{
    void *val;

    ogs_hash_this(hi, NULL, NULL, &val);
    return val;
}

int ogs_hash_do(ogs_hash_do_callback_fn_t *comp,
        void *rec, const ogs_hash_t *ht)
{
    ogs_hash_index_t hi;

    ogs_assert(comp);
    ogs_assert(ht);
    init_iterator((ogs_hash_t *)ht, &hi);
    while (ogs_hash_next(&hi)) {
        ogs_hash_slot_t *slot = &hi.ht->slots[hi.index];

        if (!comp(rec, slot_key(slot), slot->klen, slot->val))
            return 0;
    }
    return 1;
}
