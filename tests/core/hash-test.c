/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-core.h"
#include "core/abts.h"

#define MAX_LTH 256
#define MAX_DEPTH 11

static int comp_string(const void *str1, const void *str2)
{
    return strcmp(str1,str2);
}

static void dump_hash(ogs_hash_t *h, char str[][MAX_LTH]) 
{
    ogs_hash_index_t *hi;
    int i = 0;

    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        const char *key = ogs_hash_this_key(hi);
        int len = ogs_hash_this_key_len(hi);
        char *val = ogs_hash_this_val(hi);

        str[i][0]='\0';
        ogs_snprintf(str[i], MAX_LTH, "%sKey %s (%d) Value %s\n",
                 str[i], key, len, val);
        i++;
    }
    str[i][0]='\0';
    ogs_snprintf(str[i], MAX_LTH, "%s#entries %d\n", str[i], i);

    /* Sort the result strings so that they can be checked for expected results easily,
     * without having to worry about platform quirks
     */
    qsort(
        str, /* Pointer to elements */
        i,   /* number of elements */
        MAX_LTH, /* size of one element */
        comp_string /* Pointer to comparison routine */
    );
}

static void sum_hash(ogs_hash_t *h, int *pcount, int *keySum, int *valSum) 
{
    ogs_hash_index_t *hi;
    void *val, *key;
    int count = 0;

    *keySum = 0;
    *valSum = 0;
    *pcount = 0;
    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        ogs_hash_this(hi, (void*)&key, NULL, &val);
        *valSum += *(int *)val;
        *keySum += *(int *)key;
        count++;
    }
    *pcount=count;
}

static void hash_make_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_destroy(h);
}

static void hash_set_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, "value");
    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    ogs_hash_destroy(h);
}

static void hash_get_or_set_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    result = ogs_hash_get_or_set(h, "key", OGS_HASH_KEY_STRING, "value");
    ABTS_STR_EQUAL(tc, "value", result);

    result = ogs_hash_get_or_set(h, "key", OGS_HASH_KEY_STRING, "other");
    ABTS_STR_EQUAL(tc, "value", result);

    result = ogs_hash_get_or_set(h, "key", OGS_HASH_KEY_STRING, NULL);
    ABTS_STR_EQUAL(tc, "value", result);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, NULL);
    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = ogs_hash_get_or_set(h, "key", OGS_HASH_KEY_STRING, NULL);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = ogs_hash_get_or_set(h, "key", OGS_HASH_KEY_STRING, "other");
    ABTS_STR_EQUAL(tc, "other", result);

    ogs_hash_destroy(h);
}

static void hash_reset(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, "value");
    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, "new");
    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "new", result);

    ogs_hash_destroy(h);
}

static void same_value(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "same1", OGS_HASH_KEY_STRING, "same");
    result = ogs_hash_get(h, "same1", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "same", result);

    ogs_hash_set(h, "same2", OGS_HASH_KEY_STRING, "same");
    result = ogs_hash_get(h, "same2", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "same", result);

    ogs_hash_destroy(h);
}

static unsigned int hash_custom( const char *key, int *klen)
{
    unsigned int hash = 0;
    unsigned int len = *klen;
    while( len ) {
        (len) --;
        hash = hash * 33 + key[ len ];
    }
    return hash;
}

static void same_value_custom(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make_custom(hash_custom);
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "same1", 5, "same");
    result = ogs_hash_get(h, "same1", 5);
    ABTS_STR_EQUAL(tc, "same", result);

    ogs_hash_set(h, "same2", 5, "same");
    result = ogs_hash_get(h, "same2", 5);
    ABTS_STR_EQUAL(tc, "same", result);

    ogs_hash_destroy(h);
}

static void key_space(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key with space", OGS_HASH_KEY_STRING, "value");
    result = ogs_hash_get(h, "key with space", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    ogs_hash_destroy(h);
}

static void hash_clear_test(abts_case *tc, void *data)
{
    ogs_hash_t *h;
    int num[10], i;
    int c;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    for (i = 0; i < 10; i++)
    {
        num[i] = i;
        ogs_hash_set(h, &num[i], sizeof(num[i]), &num[i]);
    }
    c = ogs_hash_count(h);
    ABTS_INT_EQUAL(tc, 10, c);

    ogs_hash_clear(h);
    c = ogs_hash_count(h);
    ABTS_INT_EQUAL(tc, 0, c);

    ogs_hash_destroy(h);
}

/* This is kind of a hack, but I am just keeping an existing test.  This is
 * really testing ogs_hash_first, ogs_hash_next, and ogs_ogs_hash_this which 
 * should be tested in three separate tests, but this will do for now.
 */
static void hash_traverse(abts_case *tc, void *data)
{
    ogs_hash_t *h;
    char StrArray[MAX_DEPTH][MAX_LTH];

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "OVERWRITE", OGS_HASH_KEY_STRING, "should not see this");
    ogs_hash_set(h, "FOO3", OGS_HASH_KEY_STRING, "bar3");
    ogs_hash_set(h, "FOO3", OGS_HASH_KEY_STRING, "bar3");
    ogs_hash_set(h, "FOO1", OGS_HASH_KEY_STRING, "bar1");
    ogs_hash_set(h, "FOO2", OGS_HASH_KEY_STRING, "bar2");
    ogs_hash_set(h, "FOO4", OGS_HASH_KEY_STRING, "bar4");
    ogs_hash_set(h, "SAME1", OGS_HASH_KEY_STRING, "same");
    ogs_hash_set(h, "SAME2", OGS_HASH_KEY_STRING, "same");
    ogs_hash_set(h, "OVERWRITE", OGS_HASH_KEY_STRING, "Overwrite key");

    dump_hash(h, StrArray);

    ABTS_STR_EQUAL(tc, "Key FOO1 (4) Value bar1\n", StrArray[0]);
    ABTS_STR_EQUAL(tc, "Key FOO2 (4) Value bar2\n", StrArray[1]);
    ABTS_STR_EQUAL(tc, "Key FOO3 (4) Value bar3\n", StrArray[2]);
    ABTS_STR_EQUAL(tc, "Key FOO4 (4) Value bar4\n", StrArray[3]);
    ABTS_STR_EQUAL(tc, "Key OVERWRITE (9) Value Overwrite key\n", StrArray[4]);
    ABTS_STR_EQUAL(tc, "Key SAME1 (5) Value same\n", StrArray[5]);
    ABTS_STR_EQUAL(tc, "Key SAME2 (5) Value same\n", StrArray[6]);
    ABTS_STR_EQUAL(tc, "#entries 7\n", StrArray[7]);

    ogs_hash_destroy(h);
}

/* This is kind of a hack, but I am just keeping an existing test.  This is
 * really testing ogs_hash_first, ogs_hash_next, and ogs_ogs_hash_this which 
 * should be tested in three separate tests, but this will do for now.
 */
static void summation_test(abts_case *tc, void *data)
{
    ogs_hash_t *h;
    int sumKeys, sumVal, trySumKey, trySumVal;
    int i, j, val[100], key[100];

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    sumKeys = 0;
    sumVal = 0;
    trySumKey = 0;
    trySumVal = 0;

    for (i = 0; i < 100; i++) {
        j = i * 10 + 1;
        sumKeys += j;
        sumVal += i;
        key[i] = j;
        val[i] = i;
        ogs_hash_set(h, &key[i], sizeof(int), &val[i]);
    }

    sum_hash(h, &i, &trySumKey, &trySumVal);
    ABTS_INT_EQUAL(tc, 100, i);
    ABTS_INT_EQUAL(tc, sumVal, trySumVal);
    ABTS_INT_EQUAL(tc, sumKeys, trySumKey);

    ogs_hash_destroy(h);
}

static void delete_key(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, "value");
    ogs_hash_set(h, "key2", OGS_HASH_KEY_STRING, "value2");

    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    result = ogs_hash_get(h, "key2", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value2", result);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, NULL);

    result = ogs_hash_get(h, "key", OGS_HASH_KEY_STRING);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = ogs_hash_get(h, "key2", OGS_HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value2", result);

    ogs_hash_destroy(h);
}

static void hash_count_0(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    int count;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    count = ogs_hash_count(h);
    ABTS_INT_EQUAL(tc, 0, count);

    ogs_hash_destroy(h);
}

static void hash_count_1(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    int count;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key", OGS_HASH_KEY_STRING, "value");

    count = ogs_hash_count(h);
    ABTS_INT_EQUAL(tc, 1, count);

    ogs_hash_destroy(h);
}

static void hash_count_5(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    int count;

    h = ogs_hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    ogs_hash_set(h, "key1", OGS_HASH_KEY_STRING, "value1");
    ogs_hash_set(h, "key2", OGS_HASH_KEY_STRING, "value2");
    ogs_hash_set(h, "key3", OGS_HASH_KEY_STRING, "value3");
    ogs_hash_set(h, "key4", OGS_HASH_KEY_STRING, "value4");
    ogs_hash_set(h, "key5", OGS_HASH_KEY_STRING, "value5");

    count = ogs_hash_count(h);
    ABTS_INT_EQUAL(tc, 5, count);

    ogs_hash_destroy(h);
}

/*
 * Whether the key iteration hands out for hi is the klen bytes at expect,
 * with that length, followed by a NUL.
 */
static bool key_is(ogs_hash_index_t *hi, const void *expect, int klen)
{
    const char *key = ogs_hash_this_key(hi);

    return key && ogs_hash_this_key_len(hi) == klen &&
        memcmp(key, expect, klen) == 0 && key[klen] == '\0';
}

/* Blocks allocated with ogs_malloc() and friends, to see keys released. */
static size_t core_blocks(void)
{
    return talloc_total_blocks(__ogs_talloc_core);
}

static void fill_key(uint8_t *key, int len, int seed)
{
    int i;

    for (i = 0; i < len; i++)
        key[i] = (uint8_t)(seed * 31 + i);
}

/* An equal-key update replaces only the value; the table keeps its key. */
static void hash_set_keeps_stored_key_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    uint32_t key[2] = { 0x4001, 0x4001 };
    uint32_t lookup = 0x4001;
    int owner[2] = { 0, 1 };
    ogs_hash_index_t *hi;
    uint32_t i;
    int found = 0;

    ogs_assert(h);
    ogs_hash_set(h, &key[0], sizeof(key[0]), &owner[0]);
    ogs_hash_set(h, &key[1], sizeof(key[1]), &owner[1]);
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(h));
    ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, &lookup, sizeof(lookup)));

    /* Neither caller's buffer is the key. */
    key[0] = key[1] = 0;
    hi = ogs_hash_first(h);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi)
        ABTS_TRUE(tc, key_is(hi, &lookup, sizeof(lookup)));
    ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, &lookup, sizeof(lookup)));
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_get(h, &key[0], sizeof(key[0])));

    /* Growing the table relinks entries; every key stays intact. */
    for (i = 1; i <= 1000; i++)
        ogs_hash_set(h, &i, sizeof(i), &owner[0]);
    ABTS_INT_EQUAL(tc, 1001, ogs_hash_count(h));
    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        if (ogs_hash_this_val(hi) == &owner[1]) {
            ABTS_TRUE(tc, key_is(hi, &lookup, sizeof(lookup)));
            found++;
        } else {
            uint32_t n = 0;

            ABTS_INT_EQUAL(tc, sizeof(n), ogs_hash_this_key_len(hi));
            memcpy(&n, ogs_hash_this_key(hi), sizeof(n));
            ABTS_TRUE(tc, n >= 1 && n <= 1000);
            ABTS_TRUE(tc, key_is(hi, &n, sizeof(n)));
        }
    }
    ABTS_INT_EQUAL(tc, 1, found);
    ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, &lookup, sizeof(lookup)));
    ogs_hash_destroy(h);
}

/*
 * Lookup, iteration and removal after the caller's key buffer was changed
 * and freed: TEID, SEID and UE address sizes up to the longest key stored
 * in the entry (23 bytes), and longer keys (24 bytes, a 40-byte F-SEID,
 * a 108-byte DNN metric key) with storage of their own. Every key reads
 * back with its explicit length and followed by a NUL.
 */
static void hash_owns_binary_keys_test(abts_case *tc, void *data)
{
    static const int lengths[] = { 1, 4, 8, 15, 16, 20, 23, 24, 25, 40, 64, 108 };
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    uint8_t expect[108];
    int owner[OGS_ARRAY_SIZE(lengths)];
    size_t i;
    int found = 0;

    ogs_assert(h);
    for (i = 0; i < OGS_ARRAY_SIZE(lengths); i++) {
        uint8_t *key = ogs_malloc(lengths[i]);

        ogs_assert(key);
        fill_key(key, lengths[i], i);
        ogs_hash_set(h, key, lengths[i], &owner[i]);
        memset(key, 0xff, lengths[i]);
        ogs_free(key);
    }
    ABTS_INT_EQUAL(tc, OGS_ARRAY_SIZE(lengths), ogs_hash_count(h));

    for (i = 0; i < OGS_ARRAY_SIZE(lengths); i++) {
        fill_key(expect, lengths[i], i);
        ABTS_PTR_EQUAL(tc, &owner[i], ogs_hash_get(h, expect, lengths[i]));
    }

    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        int klen = ogs_hash_this_key_len(hi);

        for (i = 0; i < OGS_ARRAY_SIZE(lengths); i++) {
            if (lengths[i] != klen)
                continue;
            fill_key(expect, klen, i);
            ABTS_TRUE(tc, key_is(hi, expect, klen));
            ABTS_PTR_EQUAL(tc, &owner[i], ogs_hash_this_val(hi));
            found++;
        }
    }
    ABTS_INT_EQUAL(tc, OGS_ARRAY_SIZE(lengths), found);

    for (i = 0; i < OGS_ARRAY_SIZE(lengths); i++) {
        fill_key(expect, lengths[i], i);
        ogs_hash_set(h, expect, lengths[i], NULL);
        ABTS_PTR_EQUAL(tc, NULL, ogs_hash_get(h, expect, lengths[i]));
    }
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/*
 * A string key is copied with its NUL, whether its length is given as
 * OGS_HASH_KEY_STRING or as strlen() (as for SBI headers), so iteration
 * hands out a usable string, stored in the entry or not.
 */
static void hash_owns_string_keys_test(abts_case *tc, void *data)
{
    /* Even entries are set with strlen(), odd ones with KEY_STRING. */
    static const char *keys[] = {
        "k",
        "",
        "accept-encoding",
        "3gpp-sbi-binding",
        "imsi-001010000000001",
        "3gpp-sbi-target-apiroot",
        "3gpp-sbi-discovery-requester-nf-instance-id",
        "3gpp-sbi-discovery-target-nf-type",
        "12345678901234567890123",
        "123456789012345678901234",
    };
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    size_t i;
    int found = 0;

    ogs_assert(h);
    for (i = 0; i < OGS_ARRAY_SIZE(keys); i++) {
        char *key = ogs_strdup(keys[i]);

        ogs_assert(key);
        if (i % 2)
            ogs_hash_set(h, key, OGS_HASH_KEY_STRING, keys[i]);
        else
            ogs_hash_set(h, key, strlen(key), keys[i]);
        key[0] = 'X';
        ogs_free(key);
    }

    for (i = 0; i < OGS_ARRAY_SIZE(keys); i++) {
        ABTS_PTR_EQUAL(tc, keys[i],
                ogs_hash_get(h, keys[i], OGS_HASH_KEY_STRING));
        ABTS_PTR_EQUAL(tc, keys[i],
                ogs_hash_get(h, keys[i], strlen(keys[i])));
    }

    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        const char *val = ogs_hash_this_val(hi);

        ABTS_STR_EQUAL(tc, val, ogs_hash_this_key(hi));
        ABTS_INT_EQUAL(tc, strlen(val), ogs_hash_this_key_len(hi));
        found++;
    }
    ABTS_INT_EQUAL(tc, OGS_ARRAY_SIZE(keys), found);

    for (i = 0; i < OGS_ARRAY_SIZE(keys); i++)
        ogs_hash_set(h, keys[i], OGS_HASH_KEY_STRING, NULL);
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/* ogs_hash_get_or_set() copies the key of an entry it adds. */
static void hash_get_or_set_owns_key_test(abts_case *tc, void *data)
{
    static const char *name =
        "nf-instance-c9a3e5f0-8b6e-41ee-9fa2-0242ac120002";
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    uint64_t lookup = 0x1122334455667788ULL;
    uint64_t *seid = ogs_malloc(sizeof(*seid));
    char *copy = ogs_strdup(name);
    int owner[2];

    ogs_assert(h);
    ogs_assert(seid);
    ogs_assert(copy);
    *seid = lookup;
    ABTS_PTR_EQUAL(tc, &owner[0],
            ogs_hash_get_or_set(h, seid, sizeof(*seid), &owner[0]));
    ABTS_PTR_EQUAL(tc, &owner[1],
            ogs_hash_get_or_set(h, copy, OGS_HASH_KEY_STRING, &owner[1]));
    *seid = 0;
    ogs_free(seid);
    copy[0] = 'X';
    ogs_free(copy);

    ABTS_PTR_EQUAL(tc, &owner[0],
            ogs_hash_get_or_set(h, &lookup, sizeof(lookup), &owner[1]));
    copy = ogs_strdup(name);
    ogs_assert(copy);
    ABTS_PTR_EQUAL(tc, &owner[1],
            ogs_hash_get_or_set(h, copy, OGS_HASH_KEY_STRING, &owner[0]));
    copy[0] = 'X';
    ogs_free(copy);
    ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, name, OGS_HASH_KEY_STRING));
    ABTS_INT_EQUAL(tc, 2, ogs_hash_count(h));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/* Removing each entry by the key iteration hands out for it. */
static void hash_unset_by_stored_key_test(abts_case *tc, void *data)
{
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    char key[64];
    int owner = 0, i;

    ogs_assert(h);
    for (i = 0; i < 100; i++) {
        ogs_snprintf(key, sizeof(key), "%s%d",
                i % 2 ? "a-key-longer-than-the-entry-holds-" : "k", i);
        ogs_hash_set(h, key, OGS_HASH_KEY_STRING, &owner);
    }
    ABTS_INT_EQUAL(tc, 100, ogs_hash_count(h));

    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi))
        ogs_hash_set(h, ogs_hash_this_key(hi), ogs_hash_this_key_len(hi),
                NULL);
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_first(h));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/*
 * ogs_hash_clear() and ogs_hash_destroy() release key storage, and an
 * entry recycled by the free list takes a key of any length, followed
 * by a NUL where its previous, longer key had other bytes.
 */
static void hash_clear_releases_keys_test(abts_case *tc, void *data)
{
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    uint8_t full[23];
    char key[64];
    int owner = 0, i, round, found = 0;

    ogs_assert(h);
    for (round = 0; round < 3; round++) {
        /* Entries that held a long key take a short one, and back. */
        for (i = 0; i < 200; i++) {
            ogs_snprintf(key, sizeof(key), "%s%d", (i + round) % 2 ?
                    "a-key-longer-than-the-entry-holds-" : "k", i);
            ogs_hash_set(h, key, OGS_HASH_KEY_STRING, &owner);
        }
        ABTS_INT_EQUAL(tc, 200, ogs_hash_count(h));
        for (i = 0; i < 200; i++) {
            ogs_snprintf(key, sizeof(key), "%s%d", (i + round) % 2 ?
                    "a-key-longer-than-the-entry-holds-" : "k", i);
            ABTS_PTR_EQUAL(tc, &owner,
                    ogs_hash_get(h, key, OGS_HASH_KEY_STRING));
        }
        if (round < 2)
            ogs_hash_clear(h);
    }

    /* Recycled entries take 4-byte keys after 23-byte ones. */
    ogs_hash_clear(h);
    memset(full, 0xff, sizeof(full));
    for (i = 0; i < 200; i++) {
        memcpy(full, &i, sizeof(i));
        ogs_hash_set(h, full, sizeof(full), &owner);
    }
    ogs_hash_clear(h);
    for (i = 0; i < 200; i++)
        ogs_hash_set(h, &i, sizeof(i), &owner);
    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        int n = -1;

        ABTS_INT_EQUAL(tc, sizeof(n), ogs_hash_this_key_len(hi));
        memcpy(&n, ogs_hash_this_key(hi), sizeof(n));
        ABTS_TRUE(tc, n >= 0 && n < 200);
        ABTS_TRUE(tc, key_is(hi, &n, sizeof(n)));
        found++;
    }
    ABTS_INT_EQUAL(tc, 200, found);

    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/* Zero bytes is a valid key, identical to the empty string key. */
static void hash_empty_key_test(abts_case *tc, void *data)
{
    size_t blocks = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    char key = 'X';
    int owner[2] = { 0, 1 };

    ogs_assert(h);
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_get_or_set(h, &key, 0, NULL));
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ABTS_PTR_EQUAL(tc, &owner[0],
            ogs_hash_get_or_set(h, &key, 0, &owner[0]));
    key = 'Y';
    ABTS_PTR_EQUAL(tc, &owner[0], ogs_hash_get(h, &key, 0));
    ABTS_PTR_EQUAL(tc, &owner[0], ogs_hash_get(h, "", OGS_HASH_KEY_STRING));
    ABTS_PTR_EQUAL(tc, &owner[0],
            ogs_hash_get_or_set(h, "", OGS_HASH_KEY_STRING, &owner[1]));
    hi = ogs_hash_first(h);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi)
        ABTS_TRUE(tc, key_is(hi, "", 0));
    ogs_hash_set(h, "", OGS_HASH_KEY_STRING, &owner[1]);
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(h));
    ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, &key, 0));
    ABTS_TRUE(tc, !ogs_hash_unset_if_owner(h, &key, 0, &owner[0]));
    ABTS_TRUE(tc, ogs_hash_unset_if_owner(h, &key, 0, &owner[1]));
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ogs_hash_set(h, &key, 0, &owner[0]);
    ogs_hash_set(h, "", OGS_HASH_KEY_STRING, NULL);
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

/* A new key may be a prefix of a stored key, including during growth. */
static void hash_insert_from_stored_key_test(abts_case *tc, void *data)
{
    static const char *keys[] = {
        "imsi-001010000000001", "a-key-longer-than-the-entry-holds-0123456789"
    };
    size_t blocks = core_blocks(), i;
    int owner[3] = { 0, 1, 2 };

    for (i = 0; i < OGS_ARRAY_SIZE(keys); i++) {
        ogs_hash_t *h = ogs_hash_make();
        ogs_hash_index_t *hi;
        const char *stored;
        int j;

        ogs_assert(h);
        ogs_hash_set(h, keys[i], OGS_HASH_KEY_STRING, &owner[0]);
        for (j = 0; j < 14; j++)
            ogs_hash_set(h, &j, sizeof(j), &owner[2]);
        for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
            if (ogs_hash_this_val(hi) == &owner[0])
                break;
        }
        ABTS_PTR_NOTNULL(tc, hi);
        if (hi) {
            stored = ogs_hash_this_key(hi);
            /* The sixteenth insertion grows the initial bucket array. */
            ogs_hash_set(h, stored, 5, &owner[1]);
        }
        ABTS_INT_EQUAL(tc, 16, ogs_hash_count(h));
        ABTS_PTR_EQUAL(tc, &owner[0],
                ogs_hash_get(h, keys[i], OGS_HASH_KEY_STRING));
        ABTS_PTR_EQUAL(tc, &owner[1], ogs_hash_get(h, keys[i], 5));
        ogs_hash_destroy(h);
    }
    ABTS_SIZE_EQUAL(tc, blocks, core_blocks());
}

static void hash_set_owner_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    uint32_t *old_id = ogs_malloc(sizeof(*old_id));
    uint32_t new_id = 0x4001;
    int owner[2] = { 0, 1 };
    ogs_hash_index_t *hi;

    ogs_assert(h);
    ogs_assert(old_id);
    *old_id = new_id;
    ogs_hash_set(h, old_id, sizeof(*old_id), &owner[0]);
    ogs_hash_set(h, &new_id, sizeof(new_id), &owner[1]);
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(h));
    hi = ogs_hash_first(h);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi)
        ABTS_TRUE(tc, key_is(hi, &new_id, sizeof(new_id)));

    /* Old teardown must not delete the replacement; this skip is logged. */
    ABTS_TRUE(tc, !ogs_hash_unset_if_owner(
                h, old_id, sizeof(*old_id), &owner[0]));
    ABTS_PTR_EQUAL(tc, &owner[1],
            ogs_hash_get(h, &new_id, sizeof(new_id)));

    /* The old key storage can now go away without orphaning the new owner. */
    ogs_free(old_id);
    ABTS_PTR_EQUAL(tc, &owner[1],
            ogs_hash_get(h, &new_id, sizeof(new_id)));
    hi = ogs_hash_first(h);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi)
        ABTS_TRUE(tc, key_is(hi, &new_id, sizeof(new_id)));
    ogs_hash_set(h, &new_id, sizeof(new_id), &owner[1]);
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(h));
    ABTS_TRUE(tc, ogs_hash_unset_if_owner(
                h, &new_id, sizeof(new_id), &owner[1]));
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_get(h, &new_id, sizeof(new_id)));
    ogs_hash_destroy(h);
}

static void hash_set_string_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    char old_key[] = "same-id";
    char new_key[] = "same-id";
    int owner[2] = { 0, 1 };
    ogs_hash_index_t *hi;

    ogs_assert(h);
    ogs_hash_set(h, old_key, OGS_HASH_KEY_STRING, &owner[0]);
    ogs_hash_set(h, new_key, OGS_HASH_KEY_STRING, &owner[1]);
    hi = ogs_hash_first(h);
    ABTS_PTR_NOTNULL(tc, hi);
    if (hi) {
        ABTS_STR_EQUAL(tc, "same-id", ogs_hash_this_key(hi));
        ABTS_INT_EQUAL(tc, strlen(new_key), ogs_hash_this_key_len(hi));
    }
    old_key[0] = 'X';
    ABTS_PTR_EQUAL(tc, &owner[1],
            ogs_hash_get(h, new_key, OGS_HASH_KEY_STRING));

    /* A resolved string length denotes the same logical key. */
    ogs_hash_set(h, new_key, strlen(new_key), &owner[1]);
    ABTS_INT_EQUAL(tc, 1, ogs_hash_count(h));
    ogs_hash_destroy(h);
}

/* Force collisions so replacement also exercises lookups within a bucket. */
static unsigned int hash_constant(const char *key, int *klen)
{
    if (*klen == OGS_HASH_KEY_STRING)
        *klen = strlen(key);
    return 1;
}

static void hash_set_collision_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make_custom(hash_constant);
    uint32_t old_key[64], new_key[64];
    int owner[64], i;

    ogs_assert(h);
    for (i = 0; i < 64; i++) {
        old_key[i] = new_key[i] = i;
        owner[i] = i;
        ogs_hash_set(h, &old_key[i], sizeof(old_key[i]), &owner[i]);
    }
    for (i = 0; i < 64; i++) {
        ogs_hash_set(h, &new_key[i], sizeof(new_key[i]), &owner[i]);
        old_key[i] = i + 64;
    }
    ABTS_INT_EQUAL(tc, 64, ogs_hash_count(h));
    for (i = 0; i < 64; i++) {
        ABTS_PTR_EQUAL(tc, &owner[i],
                ogs_hash_get(h, &new_key[i], sizeof(new_key[i])));
        ABTS_TRUE(tc, ogs_hash_unset_if_owner(
                    h, &new_key[i], sizeof(new_key[i]), &owner[i]));
    }
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    ogs_hash_destroy(h);
}

static void hash_set_arguments_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_t *tables[2] = { h, h };
    uint32_t key = 0;
    int owner = 0;
    const void *keys[2] = { &key, &key };
    const void *values[2] = { &owner, &owner };
    int lengths[2] = { sizeof(key), sizeof(key) };
    int ti = 0, ki = 0, li = 0, vi = 0;

    ogs_assert(h);
    /* A function-backed macro must evaluate each argument only once. */
    ogs_hash_set(tables[ti++], keys[ki++], lengths[li++], values[vi++]);
    ABTS_INT_EQUAL(tc, 1, ti);
    ABTS_INT_EQUAL(tc, 1, ki);
    ABTS_INT_EQUAL(tc, 1, li);
    ABTS_INT_EQUAL(tc, 1, vi);
    ABTS_PTR_EQUAL(tc, &owner, ogs_hash_get(h, &key, sizeof(key)));
    ogs_hash_destroy(h);
}

abts_suite *test_hash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, hash_make_test, NULL);
    abts_run_test(suite, hash_set_test, NULL);
    abts_run_test(suite, hash_get_or_set_test, NULL);
    abts_run_test(suite, hash_set_keeps_stored_key_test, NULL);
    abts_run_test(suite, hash_owns_binary_keys_test, NULL);
    abts_run_test(suite, hash_owns_string_keys_test, NULL);
    abts_run_test(suite, hash_get_or_set_owns_key_test, NULL);
    abts_run_test(suite, hash_unset_by_stored_key_test, NULL);
    abts_run_test(suite, hash_clear_releases_keys_test, NULL);
    abts_run_test(suite, hash_empty_key_test, NULL);
    abts_run_test(suite, hash_insert_from_stored_key_test, NULL);
    abts_run_test(suite, hash_set_owner_test, NULL);
    abts_run_test(suite, hash_set_string_test, NULL);
    abts_run_test(suite, hash_set_collision_test, NULL);
    abts_run_test(suite, hash_set_arguments_test, NULL);
    abts_run_test(suite, hash_reset, NULL);
    abts_run_test(suite, same_value, NULL);
    abts_run_test(suite, same_value_custom, NULL);
    abts_run_test(suite, key_space, NULL);
    abts_run_test(suite, delete_key, NULL);

    abts_run_test(suite, hash_count_0, NULL);
    abts_run_test(suite, hash_count_1, NULL);
    abts_run_test(suite, hash_count_5, NULL);

    abts_run_test(suite, hash_clear_test, NULL);
    abts_run_test(suite, hash_traverse, NULL);
    abts_run_test(suite, summation_test, NULL);

    return suite;
}
