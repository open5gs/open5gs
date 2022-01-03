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

#include "ogs-core.h"

typedef struct ogs_hash_entry_t ogs_hash_entry_t;
struct ogs_hash_entry_t {
    ogs_hash_entry_t    *next;
    unsigned int        hash;
    const void          *key;
    int                 klen;
    const void          *val;
};

struct ogs_hash_index_t {
    ogs_hash_t          *ht;
    ogs_hash_entry_t    *this, *next;
    unsigned int        index;
};

struct ogs_hash_t {
    ogs_hash_entry_t    **array;
    ogs_hash_index_t    iterator;  /* For ogs_hash_first(NULL, ...) */
    unsigned int        count, max, seed;
    ogs_hashfunc_t      hash_func;
    ogs_hash_entry_t    *free;  /* List of recycled entries */
};

#define INITIAL_MAX 15 /* tunable == 2^n - 1 */

static ogs_hash_entry_t **alloc_array(ogs_hash_t *ht, unsigned int max)
{
    ogs_hash_entry_t **ptr = ogs_calloc(1, sizeof(*ht->array) * (max + 1));
    ogs_assert(ptr);
    return ptr;
}

ogs_hash_t *ogs_hash_make()
{
    ogs_hash_t *ht;
    ogs_time_t now = ogs_get_monotonic_time();

    ht = ogs_malloc(sizeof(ogs_hash_t));
    ogs_expect_or_return_val(ht, NULL);

    ht->free = NULL;
    ht->count = 0;
    ht->max = INITIAL_MAX;
    ht->seed = (unsigned int)((now >> 32) ^ now ^ 
                              (uintptr_t)ht ^ (uintptr_t)&now) - 1;
    ht->array = alloc_array(ht, ht->max);
    ht->hash_func = NULL;

    return ht;
}

ogs_hash_t *ogs_hash_make_custom(ogs_hashfunc_t hash_func)
{
    ogs_hash_t *ht = ogs_hash_make();
    ogs_expect_or_return_val(ht, NULL);
    ht->hash_func = hash_func;
    return ht;
}

void ogs_hash_destroy(ogs_hash_t *ht)
{
    ogs_hash_entry_t *he = NULL, *next_he = NULL;

    ogs_assert(ht);
    ogs_assert(ht->array);

    ogs_hash_clear(ht);

    he = ht->free;
    while(he) {
        next_he = he->next;

        ogs_free(he);
        he = next_he;
    }

    ogs_free(ht->array);
    ogs_free(ht);
}

ogs_hash_index_t *ogs_hash_next(ogs_hash_index_t *hi)
{
    ogs_assert(hi);

    hi->this = hi->next;
    while (!hi->this) {
        if (hi->index > hi->ht->max)
            return NULL;

        hi->this = hi->ht->array[hi->index++];
    }
    hi->next = hi->this->next;
    return hi;
}

ogs_hash_index_t *ogs_hash_first(ogs_hash_t *ht)
{
    ogs_hash_index_t *hi;

    ogs_assert(ht);

    hi = &ht->iterator;

    hi->ht = ht;
    hi->index = 0;
    hi->this = NULL;
    hi->next = NULL;
    return ogs_hash_next(hi);
}

void ogs_hash_this(ogs_hash_index_t *hi,
        const void **key, int *klen, void **val)
{
    ogs_assert(hi);

    if (key)  *key  = hi->this->key;
    if (klen) *klen = hi->this->klen;
    if (val)  *val  = (void *)hi->this->val;
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

static void expand_array(ogs_hash_t *ht)
{
    ogs_hash_index_t *hi;
    ogs_hash_entry_t **new_array;
    unsigned int new_max;

    new_max = ht->max * 2 + 1;
    new_array = alloc_array(ht, new_max);
    for (hi = ogs_hash_first(ht); hi; hi = ogs_hash_next(hi)) {
        unsigned int i = hi->this->hash & new_max;
        hi->this->next = new_array[i];
        new_array[i] = hi->this;
    }
    ogs_free(ht->array);
    ht->array = new_array;
    ht->max = new_max;
}

static unsigned int hashfunc_default(
        const char *char_key, int *klen, unsigned int hash)
{
    const unsigned char *key = (const unsigned char *)char_key;
    const unsigned char *p;
    int i;
    
    /*
     * This is the popular `times 33' hash algorithm which is used by
     * perl and also appears in Berkeley DB. This is one of the best
     * known hash functions for strings because it is both computed
     * very fast and distributes very well.
     *
     * The originator may be Dan Bernstein but the code in Berkeley DB
     * cites Chris Torek as the source. The best citation I have found
     * is "Chris Torek, Hash function for text in C, Usenet message
     * <27038@mimsy.umd.edu> in comp.lang.c , October, 1990." in Rich
     * Salz's USENIX 1992 paper about INN which can be found at
     * <http://citeseer.nj.nec.com/salz92internetnews.html>.
     *
     * The magic of number 33, i.e. why it works better than many other
     * constants, prime or not, has never been adequately explained by
     * anyone. So I try an explanation: if one experimentally tests all
     * multipliers between 1 and 256 (as I did while writing a low-level
     * data structure library some time ago) one detects that even
     * numbers are not useable at all. The remaining 128 odd numbers
     * (except for the number 1) work more or less all equally well.
     * They all distribute in an acceptable way and this way fill a hash
     * table with an average percent of approx. 86%.
     *
     * If one compares the chi^2 values of the variants (see
     * Bob Jenkins ``Hashing Frequently Asked Questions'' at
     * http://burtleburtle.net/bob/hash/hashfaq.html for a description
     * of chi^2), the number 33 not even has the best value. But the
     * number 33 and a few other equally good numbers like 17, 31, 63,
     * 127 and 129 have nevertheless a great advantage to the remaining
     * numbers in the large set of possible multipliers: their multiply
     * operation can be replaced by a faster operation based on just one
     * shift plus either a single addition or subtraction operation. And
     * because a hash function has to both distribute good _and_ has to
     * be very fast to compute, those few numbers should be preferred.
     *
     *                  -- Ralf S. Engelschall <rse@engelschall.com>
     */

    if (*klen == OGS_HASH_KEY_STRING) {
        for (p = key; *p; p++) {
            hash = hash * 33 + *p;
        }
        *klen = p - key;
    }
    else {
        for (p = key, i = *klen; i; i--, p++) {
            hash = hash * 33 + *p;
        }
    }

    return hash;
}

unsigned int ogs_hashfunc_default(const char *char_key, int *klen)
{
    return hashfunc_default(char_key, klen, 0);
}

static ogs_hash_entry_t **find_entry(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line)
{
    ogs_hash_entry_t **hep, *he;
    unsigned int hash;

    if (ht->hash_func)
        hash = ht->hash_func(key, &klen);
    else
        hash = hashfunc_default(key, &klen, ht->seed);

    /* scan linked list */
    for (hep = &ht->array[hash & ht->max], he = *hep;
         he; hep = &he->next, he = *hep) {
        if (he->hash == hash
            && he->klen == klen
            && memcmp(he->key, key, klen) == 0)
            break;
    }
    if (he || !val)
        return hep;

    /* add a new entry for non-NULL values */
    if ((he = ht->free) != NULL)
        ht->free = he->next;
    else {
        he = ogs_malloc(sizeof(*he));
        ogs_assert(he);
    }
    he->next = NULL;
    he->hash = hash;
    he->key  = key;
    he->klen = klen;
    he->val  = val;
    *hep = he;
    ht->count++;
    return hep;
}

void *ogs_hash_get_debug(ogs_hash_t *ht,
        const void *key, int klen, const char *file_line)
{
    ogs_hash_entry_t *he;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen);

    he = *find_entry(ht, key, klen, NULL, file_line);
    if (he)
        return (void *)he->val;
    else
        return NULL;
}

void ogs_hash_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line)
{
    ogs_hash_entry_t **hep;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen);

    hep = find_entry(ht, key, klen, val, file_line);
    if (*hep) {
        if (!val) {
            /* delete entry */
            ogs_hash_entry_t *old = *hep;
            *hep = (*hep)->next;
            old->next = ht->free;
            ht->free = old;
            --ht->count;
        } else {
            /* replace entry */
            (*hep)->val = val;
            /* check that the collision rate isn't too high */
            if (ht->count > ht->max) {
                expand_array(ht);
            }
        }
    }
    /* else key not present and val==NULL */
}

void *ogs_hash_get_or_set_debug(ogs_hash_t *ht,
        const void *key, int klen, const void *val, const char *file_line)
{
    ogs_hash_entry_t **hep;

    ogs_assert(ht);
    ogs_assert(key);
    ogs_assert(klen);

    hep = find_entry(ht, key, klen, val, file_line);
    if (*hep) {
        val = (*hep)->val;
        /* check that the collision rate isn't too high */
        if (ht->count > ht->max) {
            expand_array(ht);
        }
        return (void *)val;
    }
    /* else key not present and val==NULL */
    return NULL;
}

unsigned int ogs_hash_count(ogs_hash_t *ht)
{
    ogs_assert(ht);
    return ht->count;
}

void ogs_hash_clear(ogs_hash_t *ht)
{
    ogs_hash_index_t *hi;

    ogs_assert(ht);

    for (hi = ogs_hash_first(ht); hi; hi = ogs_hash_next(hi))
        ogs_hash_set(ht, hi->this->key, hi->this->klen, NULL);
}

/* This is basically the following...
 * for every element in hash table {
 *    comp elemeny.key, element.value
 * }
 *
 * Like with table_do, the comp callback is called for each and every
 * element of the hash table.
 */
int ogs_hash_do(ogs_hash_do_callback_fn_t *comp,
                             void *rec, const ogs_hash_t *ht)
{
    ogs_hash_index_t  hix;
    ogs_hash_index_t *hi;
    int rv, dorv  = 1;

    hix.ht    = (ogs_hash_t *)ht;
    hix.index = 0;
    hix.this  = NULL;
    hix.next  = NULL;

    if ((hi = ogs_hash_next(&hix))) {
        /* Scan the entire table */
        do {
            rv = (*comp)(rec, hi->this->key, hi->this->klen, hi->this->val);
        } while (rv && (hi = ogs_hash_next(hi)));

        if (rv == 0) {
            dorv = 0;
        }
    }
    return dorv;
}
