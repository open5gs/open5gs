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

abts_suite *test_hash(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, hash_make_test, NULL);
    abts_run_test(suite, hash_set_test, NULL);
    abts_run_test(suite, hash_get_or_set_test, NULL);
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
