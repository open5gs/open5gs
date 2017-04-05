#include "testutil.h"
#include "core_hash.h"

#define MAX_LTH 256
#define MAX_DEPTH 11

static int comp_string(const void *str1, const void *str2)
{
    return strcmp(str1,str2);
}

static void dump_hash(hash_t *h, char str[][MAX_LTH]) 
{
    hash_index_t *hi;
    int i = 0;

    for (hi = hash_first(h); hi; hi = hash_next(hi)) {
        const char *key = hash_this_key(hi);
        int len = hash_this_key_len(hi);
        char *val = hash_this_val(hi);

        str[i][0]='\0';
        sprintf(str[i], "%sKey %s (%d) Value %s\n",
                 str[i], key, len, val);
        i++;
    }
    str[i][0]='\0';
    sprintf(str[i], "%s#entries %d\n", str[i], i);

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

static void sum_hash(hash_t *h, int *pcount, int *keySum, int *valSum) 
{
    hash_index_t *hi;
    void *val, *key;
    int count = 0;

    *keySum = 0;
    *valSum = 0;
    *pcount = 0;
    for (hi = hash_first(h); hi; hi = hash_next(hi)) {
        hash_this(hi, (void*)&key, NULL, &val);
        *valSum += *(int *)val;
        *keySum += *(int *)key;
        count++;
    }
    *pcount=count;
}

static void hash_make_test(abts_case *tc, void *data)
{
    hash_t *h = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_destroy(h);
}

static void hash_set_test(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key", HASH_KEY_STRING, "value");
    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    hash_destroy(h);
}

static void hash_get_or_set_test(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    result = hash_get_or_set(h, "key", HASH_KEY_STRING, "value");
    ABTS_STR_EQUAL(tc, "value", result);

    result = hash_get_or_set(h, "key", HASH_KEY_STRING, "other");
    ABTS_STR_EQUAL(tc, "value", result);

    result = hash_get_or_set(h, "key", HASH_KEY_STRING, NULL);
    ABTS_STR_EQUAL(tc, "value", result);

    hash_set(h, "key", HASH_KEY_STRING, NULL);
    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = hash_get_or_set(h, "key", HASH_KEY_STRING, NULL);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = hash_get_or_set(h, "key", HASH_KEY_STRING, "other");
    ABTS_STR_EQUAL(tc, "other", result);

    hash_destroy(h);
}

static void hash_reset(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key", HASH_KEY_STRING, "value");
    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    hash_set(h, "key", HASH_KEY_STRING, "new");
    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "new", result);

    hash_destroy(h);
}

static void same_value(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "same1", HASH_KEY_STRING, "same");
    result = hash_get(h, "same1", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "same", result);

    hash_set(h, "same2", HASH_KEY_STRING, "same");
    result = hash_get(h, "same2", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "same", result);

    hash_destroy(h);
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
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make_custom(hash_custom);
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "same1", 5, "same");
    result = hash_get(h, "same1", 5);
    ABTS_STR_EQUAL(tc, "same", result);

    hash_set(h, "same2", 5, "same");
    result = hash_get(h, "same2", 5);
    ABTS_STR_EQUAL(tc, "same", result);

    hash_destroy(h);
}

static void key_space(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key with space", HASH_KEY_STRING, "value");
    result = hash_get(h, "key with space", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    hash_destroy(h);
}

static void hash_clear_test(abts_case *tc, void *data)
{
    hash_t *h;
    int num[10], i;
    int c;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    for (i = 0; i < 10; i++)
    {
        num[i] = i;
        hash_set(h, &num[i], sizeof(num[i]), &num[i]);
    }
    c = hash_count(h);
    ABTS_INT_EQUAL(tc, 10, c);

    hash_clear(h);
    c = hash_count(h);
    ABTS_INT_EQUAL(tc, 0, c);

    hash_destroy(h);
}

/* This is kind of a hack, but I am just keeping an existing test.  This is
 * really testing hash_first, hash_next, and hash_this which 
 * should be tested in three separate tests, but this will do for now.
 */
static void hash_traverse(abts_case *tc, void *data)
{
    hash_t *h;
    char StrArray[MAX_DEPTH][MAX_LTH];

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "OVERWRITE", HASH_KEY_STRING, "should not see this");
    hash_set(h, "FOO3", HASH_KEY_STRING, "bar3");
    hash_set(h, "FOO3", HASH_KEY_STRING, "bar3");
    hash_set(h, "FOO1", HASH_KEY_STRING, "bar1");
    hash_set(h, "FOO2", HASH_KEY_STRING, "bar2");
    hash_set(h, "FOO4", HASH_KEY_STRING, "bar4");
    hash_set(h, "SAME1", HASH_KEY_STRING, "same");
    hash_set(h, "SAME2", HASH_KEY_STRING, "same");
    hash_set(h, "OVERWRITE", HASH_KEY_STRING, "Overwrite key");

    dump_hash(h, StrArray);

    ABTS_STR_EQUAL(tc, "Key FOO1 (4) Value bar1\n", StrArray[0]);
    ABTS_STR_EQUAL(tc, "Key FOO2 (4) Value bar2\n", StrArray[1]);
    ABTS_STR_EQUAL(tc, "Key FOO3 (4) Value bar3\n", StrArray[2]);
    ABTS_STR_EQUAL(tc, "Key FOO4 (4) Value bar4\n", StrArray[3]);
    ABTS_STR_EQUAL(tc, "Key OVERWRITE (9) Value Overwrite key\n", StrArray[4]);
    ABTS_STR_EQUAL(tc, "Key SAME1 (5) Value same\n", StrArray[5]);
    ABTS_STR_EQUAL(tc, "Key SAME2 (5) Value same\n", StrArray[6]);
    ABTS_STR_EQUAL(tc, "#entries 7\n", StrArray[7]);

    hash_destroy(h);
}

/* This is kind of a hack, but I am just keeping an existing test.  This is
 * really testing hash_first, hash_next, and hash_this which 
 * should be tested in three separate tests, but this will do for now.
 */
static void summation_test(abts_case *tc, void *data)
{
    hash_t *h;
    int sumKeys, sumVal, trySumKey, trySumVal;
    int i, j, val[100], key[100];

    h = hash_make();
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
        hash_set(h, &key[i], sizeof(int), &val[i]);
    }

    sum_hash(h, &i, &trySumKey, &trySumVal);
    ABTS_INT_EQUAL(tc, 100, i);
    ABTS_INT_EQUAL(tc, sumVal, trySumVal);
    ABTS_INT_EQUAL(tc, sumKeys, trySumKey);

    hash_destroy(h);
}

static void delete_key(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    char *result = NULL;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key", HASH_KEY_STRING, "value");
    hash_set(h, "key2", HASH_KEY_STRING, "value2");

    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value", result);

    result = hash_get(h, "key2", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value2", result);

    hash_set(h, "key", HASH_KEY_STRING, NULL);

    result = hash_get(h, "key", HASH_KEY_STRING);
    ABTS_PTR_EQUAL(tc, NULL, result);

    result = hash_get(h, "key2", HASH_KEY_STRING);
    ABTS_STR_EQUAL(tc, "value2", result);

    hash_destroy(h);
}

static void hash_count_0(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    int count;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    count = hash_count(h);
    ABTS_INT_EQUAL(tc, 0, count);

    hash_destroy(h);
}

static void hash_count_1(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    int count;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key", HASH_KEY_STRING, "value");

    count = hash_count(h);
    ABTS_INT_EQUAL(tc, 1, count);

    hash_destroy(h);
}

static void hash_count_5(abts_case *tc, void *data)
{
    hash_t *h = NULL;
    int count;

    h = hash_make();
    ABTS_PTR_NOTNULL(tc, h);

    hash_set(h, "key1", HASH_KEY_STRING, "value1");
    hash_set(h, "key2", HASH_KEY_STRING, "value2");
    hash_set(h, "key3", HASH_KEY_STRING, "value3");
    hash_set(h, "key4", HASH_KEY_STRING, "value4");
    hash_set(h, "key5", HASH_KEY_STRING, "value5");

    count = hash_count(h);
    ABTS_INT_EQUAL(tc, 5, count);

    hash_destroy(h);
}

abts_suite *testhash(abts_suite *suite)
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

