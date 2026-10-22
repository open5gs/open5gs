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

static void same_value_binary(abts_case *tc, void *data)
{
    ogs_hash_t *h = NULL;
    char *result = NULL;

    h = ogs_hash_make();
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

    /* Growing the table moves entries; every key stays intact. */
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
 * ogs_hash_clear() and ogs_hash_destroy() release key storage. Slots
 * reused after clear take a key of any length, followed by a NUL where
 * a previous, longer key had other bytes.
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
        /* Slots that held a long key take a short one, and back. */
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

    /* Cleared slots take 4-byte keys after 23-byte ones. */
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
        for (j = 0; j < 13; j++)
            ogs_hash_set(h, &j, sizeof(j), &owner[2]);
        for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
            if (ogs_hash_this_val(hi) == &owner[0])
                break;
        }
        ABTS_PTR_NOTNULL(tc, hi);
        if (hi) {
            stored = ogs_hash_this_key(hi);
            /* The fifteenth insertion exceeds 90% of sixteen slots. */
            ogs_hash_set(h, stored, 5, &owner[1]);
        }
        ABTS_INT_EQUAL(tc, 15, ogs_hash_count(h));
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

/* Repeated replacement and removal must preserve every other entry. */
static void hash_set_many_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
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

#define HASH_MODEL_KEYS 251

static uint32_t hash_test_random(uint32_t *state)
{
    *state = *state * 1664525u + 1013904223u;
    return *state;
}

/* Unique binary keys exercise both sides of the inline boundary. */
static int hash_model_key(uint8_t *key, uint32_t id)
{
    static const int lengths[] = { 4, 23, 24, 40, 108 };
    int len = lengths[id % OGS_ARRAY_SIZE(lengths)];

    fill_key(key, len, id);
    memcpy(key, &id, sizeof(id));
    return len;
}

static void hash_check_model(abts_case *tc, ogs_hash_t *h, void **model)
{
    ogs_hash_index_t *hi;
    uint8_t key[108];
    unsigned int seen[HASH_MODEL_KEYS] = { 0 };
    unsigned int i, count = 0, visited = 0;

    for (i = 0; i < HASH_MODEL_KEYS; i++) {
        int len = hash_model_key(key, i);

        ABTS_PTR_EQUAL(tc, model[i], ogs_hash_get(h, key, len));
        if (model[i])
            count++;
    }
    ABTS_INT_EQUAL(tc, count, ogs_hash_count(h));
    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        uint32_t id;
        int len;

        if (visited++ >= count) {
            ABTS_TRUE(tc, false);
            break;
        }
        ABTS_TRUE(tc, ogs_hash_this_key_len(hi) >= (int)sizeof(id));
        if (ogs_hash_this_key_len(hi) < (int)sizeof(id))
            break;
        memcpy(&id, ogs_hash_this_key(hi), sizeof(id));
        ABTS_TRUE(tc, id < HASH_MODEL_KEYS);
        if (id >= HASH_MODEL_KEYS)
            break;
        ABTS_INT_EQUAL(tc, 0, seen[id]++);
        len = hash_model_key(key, id);
        ABTS_TRUE(tc, key_is(hi, key, len));
        ABTS_PTR_EQUAL(tc, model[id], ogs_hash_this_val(hi));
    }
    ABTS_INT_EQUAL(tc, count, visited);
}

/* Empty tables do not allocate a slot array, even after unsuccessful calls. */
static void hash_lazy_allocation_test(abts_case *tc, void *data)
{
    size_t before = core_blocks();
    ogs_hash_t *h = ogs_hash_make();
    uint32_t key = 1;
    int owner;

    ABTS_SIZE_EQUAL(tc, before + 1, core_blocks());
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_get(h, &key, sizeof(key)));
    ABTS_PTR_EQUAL(tc, NULL,
            ogs_hash_get_or_set(h, &key, sizeof(key), NULL));
    ogs_hash_set(h, &key, sizeof(key), NULL);
    ABTS_TRUE(tc, !ogs_hash_unset_if_owner(h, &key, sizeof(key), &owner));
    ABTS_PTR_EQUAL(tc, NULL, ogs_hash_first(h));
    ogs_hash_clear(h);
    ABTS_SIZE_EQUAL(tc, before + 1, core_blocks());
    ogs_hash_set(h, &key, sizeof(key), &owner);
    ABTS_SIZE_EQUAL(tc, before + 2, core_blocks());
    ABTS_PTR_EQUAL(tc, &owner, ogs_hash_get(h, &key, sizeof(key)));
    ogs_hash_destroy(h);
    ABTS_SIZE_EQUAL(tc, before, core_blocks());
}

/* Check lookup and iteration immediately before and after 90% boundaries. */
static void hash_growth_boundaries_test(abts_case *tc, void *data)
{
    static const unsigned int boundaries[] = { 14, 28, 57, 115, 230, 460, 921 };
    ogs_hash_t *h = ogs_hash_make();
    int owner[923];
    unsigned int i, boundary = 0;

    for (i = 0; i < OGS_ARRAY_SIZE(owner); i++) {
        uint32_t key = i;

        owner[i] = i;
        ogs_hash_set(h, &key, sizeof(key), &owner[i]);
        if (boundary < OGS_ARRAY_SIZE(boundaries) &&
                (i + 1 == boundaries[boundary] || i == boundaries[boundary])) {
            ogs_hash_index_t *hi;
            unsigned int j, visited = 0;

            ABTS_INT_EQUAL(tc, i + 1, ogs_hash_count(h));
            for (j = 0; j <= i; j++) {
                key = j;
                ABTS_PTR_EQUAL(tc, &owner[j],
                        ogs_hash_get(h, &key, sizeof(key)));
            }
            for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
                if (visited++ > i) {
                    ABTS_TRUE(tc, false);
                    break;
                }
                memcpy(&key, ogs_hash_this_key(hi), sizeof(key));
                ABTS_TRUE(tc, key <= i);
                if (key <= i)
                    ABTS_PTR_EQUAL(tc, &owner[key], ogs_hash_this_val(hi));
            }
            ABTS_INT_EQUAL(tc, i + 1, visited);
            if (i == boundaries[boundary])
                boundary++;
        }
    }
    ABTS_INT_EQUAL(tc, OGS_ARRAY_SIZE(boundaries), boundary);
    ogs_hash_destroy(h);
}

/* Compare mutations with a separate map model across growth and repeated reuse. */
static void hash_randomized_model_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    void *model[HASH_MODEL_KEYS] = { NULL };
    int owner[2][HASH_MODEL_KEYS];
    uint8_t key[108];
    uint32_t state = 0x5a17c9e3u;
    unsigned int i, step;

    for (i = 0; i < HASH_MODEL_KEYS; i++)
        owner[0][i] = owner[1][i] = i;
    for (i = 0; i < 230; i++) {
        int len = hash_model_key(key, i);

        model[i] = &owner[0][i];
        ogs_hash_set(h, key, len, model[i]);
    }
    for (step = 0; step < 30000; step++) {
        uint32_t choice = hash_test_random(&state);
        unsigned int id = (choice >> 8) % HASH_MODEL_KEYS;
        void *candidate = &owner[(choice >> 24) & 1][id];
        int len = hash_model_key(key, id);

        switch ((hash_test_random(&state) >> 16) % 8) {
        case 0:
        case 6:
            ogs_hash_set(h, key, len, candidate);
            model[id] = candidate;
            break;
        case 1:
            ABTS_PTR_EQUAL(tc, model[id] ? model[id] : candidate,
                    ogs_hash_get_or_set(h, key, len, candidate));
            if (!model[id])
                model[id] = candidate;
            break;
        case 2:
            ogs_hash_set(h, key, len, NULL);
            model[id] = NULL;
            break;
        case 3:
            ABTS_INT_EQUAL(tc, model[id] == candidate,
                    ogs_hash_unset_if_owner(h, key, len, candidate));
            if (model[id] == candidate)
                model[id] = NULL;
            break;
        case 4:
            ABTS_PTR_EQUAL(tc, model[id], ogs_hash_get(h, key, len));
            break;
        case 5:
            ABTS_PTR_EQUAL(tc, model[id],
                    ogs_hash_get_or_set(h, key, len, NULL));
            break;
        case 7:
            if (step % 1024 == 0) {
                ogs_hash_clear(h);
                memset(model, 0, sizeof(model));
            }
            break;
        }
        if (step % 127 == 0)
            hash_check_model(tc, h, model);
    }
    hash_check_model(tc, h, model);
    ogs_hash_destroy(h);
}

/* Backward-shift deletion must not skip or repeat unvisited entries. */
static void hash_iteration_delete_test(abts_case *tc, void *data)
{
    uint32_t state = 0xf1538a27u;
    int owner[HASH_MODEL_KEYS];
    unsigned int round, i;

    for (i = 0; i < HASH_MODEL_KEYS; i++)
        owner[i] = i;
    for (round = 0; round < 2048; round++) {
        ogs_hash_t *h = ogs_hash_make();
        ogs_hash_index_t *hi;
        unsigned int seen[HASH_MODEL_KEYS] = { 0 };
        bool removed[HASH_MODEL_KEYS] = { false };
        uint8_t key[108];
        unsigned int count = 14 + (hash_test_random(&state) >> 8) % 217;
        unsigned int visited = 0, remaining = count;

        for (i = 0; i < count; i++) {
            int len = hash_model_key(key, i);

            ogs_hash_set(h, key, len, &owner[i]);
        }
        for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
            unsigned int id = *(int *)ogs_hash_this_val(hi);

            if (visited++ >= count) {
                ABTS_TRUE(tc, false);
                break;
            }
            ABTS_TRUE(tc, id < count);
            if (id >= count)
                break;
            ABTS_INT_EQUAL(tc, 0, seen[id]++);
            if (round % 3 == 0 || (round % 3 == 1 && id % 2 == 0) ||
                    (round % 3 == 2 && (hash_test_random(&state) >> 31))) {
                ogs_hash_set(h, ogs_hash_this_key(hi),
                        ogs_hash_this_key_len(hi), NULL);
                removed[id] = true;
                remaining--;
            }
        }
        ABTS_INT_EQUAL(tc, count, visited);
        ABTS_INT_EQUAL(tc, remaining, ogs_hash_count(h));
        for (i = 0; i < count; i++) {
            int len = hash_model_key(key, i);

            ABTS_INT_EQUAL(tc, 1, seen[i]);
            ABTS_PTR_EQUAL(tc, removed[i] ? NULL : &owner[i],
                    ogs_hash_get(h, key, len));
        }
        ogs_hash_destroy(h);
    }
}

#define HASH_DO_KEYS 115

typedef struct hash_do_record_s {
    abts_case *tc;
    ogs_hash_t *h;
    int *owner;
    unsigned int seen[HASH_DO_KEYS];
    unsigned int count, stop_after;
    bool remove, nested;
} hash_do_record_t;

static int hash_do_callback(void *data,
        const void *key, int klen, const void *value)
{
    hash_do_record_t *rec = data;
    uint32_t id;

    ABTS_INT_EQUAL(rec->tc, sizeof(id), klen);
    if (klen != (int)sizeof(id))
        return 0;
    memcpy(&id, key, sizeof(id));
    ABTS_TRUE(rec->tc, id < HASH_DO_KEYS);
    if (id >= HASH_DO_KEYS)
        return 0;
    ABTS_INT_EQUAL(rec->tc, 0, rec->seen[id]++);
    ABTS_PTR_EQUAL(rec->tc, &rec->owner[id], value);
    ABTS_INT_EQUAL(rec->tc, 0, ((const char *)key)[klen]);
    rec->count++;
    if (rec->nested && rec->count == 1) {
        hash_do_record_t child;

        memset(&child, 0, sizeof(child));
        child.tc = rec->tc;
        child.h = rec->h;
        child.owner = rec->owner;
        ABTS_INT_EQUAL(rec->tc, 1,
                ogs_hash_do(hash_do_callback, &child, rec->h));
        ABTS_INT_EQUAL(rec->tc, HASH_DO_KEYS, child.count);
    }
    if (rec->remove)
        ogs_hash_set(rec->h, key, klen, NULL);
    return !rec->stop_after || rec->count < rec->stop_after;
}

/* hash_do owns its iterator, supports nesting, early stop and current deletion. */
static void hash_do_test(abts_case *tc, void *data)
{
    ogs_hash_t *h = ogs_hash_make();
    ogs_hash_index_t *hi;
    hash_do_record_t rec;
    unsigned int outer_seen[HASH_DO_KEYS] = { 0 };
    int owner[HASH_DO_KEYS];
    unsigned int i, visited = 0;

    for (i = 0; i < HASH_DO_KEYS; i++) {
        uint32_t key = i;

        owner[i] = i;
        ogs_hash_set(h, &key, sizeof(key), &owner[i]);
    }
    /* A full callback traversal must preserve an in-progress first/next loop. */
    for (hi = ogs_hash_first(h); hi; hi = ogs_hash_next(hi)) {
        unsigned int id = *(int *)ogs_hash_this_val(hi);

        if (visited++ >= HASH_DO_KEYS) {
            ABTS_TRUE(tc, false);
            break;
        }
        ABTS_TRUE(tc, id < HASH_DO_KEYS);
        if (id >= HASH_DO_KEYS)
            break;
        ABTS_INT_EQUAL(tc, 0, outer_seen[id]++);
        memset(&rec, 0, sizeof(rec));
        rec.tc = tc;
        rec.h = h;
        rec.owner = owner;
        ABTS_INT_EQUAL(tc, 1, ogs_hash_do(hash_do_callback, &rec, h));
        ABTS_INT_EQUAL(tc, HASH_DO_KEYS, rec.count);
    }
    ABTS_INT_EQUAL(tc, HASH_DO_KEYS, visited);

    memset(&rec, 0, sizeof(rec));
    rec.tc = tc;
    rec.h = h;
    rec.owner = owner;
    rec.stop_after = 7;
    ABTS_INT_EQUAL(tc, 0, ogs_hash_do(hash_do_callback, &rec, h));
    ABTS_INT_EQUAL(tc, 7, rec.count);

    memset(&rec, 0, sizeof(rec));
    rec.tc = tc;
    rec.h = h;
    rec.owner = owner;
    rec.nested = true;
    ABTS_INT_EQUAL(tc, 1, ogs_hash_do(hash_do_callback, &rec, h));
    ABTS_INT_EQUAL(tc, HASH_DO_KEYS, rec.count);

    memset(&rec, 0, sizeof(rec));
    rec.tc = tc;
    rec.h = h;
    rec.owner = owner;
    rec.remove = true;
    ABTS_INT_EQUAL(tc, 1, ogs_hash_do(hash_do_callback, &rec, h));
    ABTS_INT_EQUAL(tc, HASH_DO_KEYS, rec.count);
    ABTS_INT_EQUAL(tc, 0, ogs_hash_count(h));
    rec.count = 0;
    ABTS_INT_EQUAL(tc, 1, ogs_hash_do(hash_do_callback, &rec, h));
    ABTS_INT_EQUAL(tc, 0, rec.count);
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
    abts_run_test(suite, hash_set_many_test, NULL);
    abts_run_test(suite, hash_set_arguments_test, NULL);
    abts_run_test(suite, hash_reset, NULL);
    abts_run_test(suite, same_value, NULL);
    abts_run_test(suite, same_value_binary, NULL);
    abts_run_test(suite, key_space, NULL);
    abts_run_test(suite, delete_key, NULL);

    abts_run_test(suite, hash_count_0, NULL);
    abts_run_test(suite, hash_count_1, NULL);
    abts_run_test(suite, hash_count_5, NULL);

    abts_run_test(suite, hash_clear_test, NULL);
    abts_run_test(suite, hash_traverse, NULL);
    abts_run_test(suite, summation_test, NULL);
    abts_run_test(suite, hash_lazy_allocation_test, NULL);
    abts_run_test(suite, hash_growth_boundaries_test, NULL);
    abts_run_test(suite, hash_randomized_model_test, NULL);
    abts_run_test(suite, hash_iteration_delete_test, NULL);
    abts_run_test(suite, hash_do_test, NULL);

    return suite;
}
