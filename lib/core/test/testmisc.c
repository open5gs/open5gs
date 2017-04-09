#include "core_lib.h"
#include "core_debug.h"

#include "testutil.h"

static void misc_test1(abts_case *tc, void *data)
{
    unsigned char c[42];
    ABTS_INT_EQUAL(tc, CORE_OK, core_generate_random_bytes(c, sizeof c));
}

static void misc_test2(abts_case *tc, void *data)
{
#define K "4   6  5B5    CE8   B199B49FAA5F0A2EE238A6BC   "
#define OP "5F1D289C  5D354D0A 140C2548   F5F3E3BA"
#define OPc "E8ED2 89D EBA9 52E4 283B 54E8 8E61 83CA"
#define AMF "8000"
    c_uint8_t k[16] = "\x46\x5B\x5C\xE8\xB1\x99\xB4\x9F\xAA\x5F\x0A\x2E\xE2\x38\xA6\xBC";
    c_uint8_t op[16] = "\x5F\x1D\x28\x9C\x5D\x35\x4D\x0A\x14\x0C\x25\x48\xF5\xF3\xE3\xBA";
    c_uint8_t opc[16] = "\xE8\xED\x28\x9D\xEB\xA9\x52\xE4\x28\x3B\x54\xE8\x8E\x61\x83\xCA";
    c_uint8_t amf[2] = { 0x80, 0x00 };
#define LOWER "    12abcdE   F"
    c_uint8_t lower[4] = "\x12\xab\xcd\xef";

    char buffer[16];

    ABTS_TRUE(tc, memcmp(k, CORE_HEX(K, strlen(K), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(op, CORE_HEX(OP, strlen(OP), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(opc, CORE_HEX(OPc, strlen(OPc), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(amf, CORE_HEX(AMF, strlen(AMF), buffer), 2) == 0);

    ABTS_TRUE(tc, memcmp(lower, 
                CORE_HEX(LOWER, strlen(LOWER), buffer), 4) == 0);
}

static void misc_test3(abts_case *tc, void *data)
{
#define MAX_SIZE 8
    c_uint8_t tmp[MAX_SIZE] = "\x01\x23\x45\x67\x89\xab\xcd\xef";
    c_uint8_t buf[MAX_SIZE];
    c_uint64_t num;

    num = 0x0123456789abcdef;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 8, buf), 8) == 0);
    num = 0x0123456789abcd;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 7, buf), 7) == 0);
    num = 0x0123456789ab;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 6, buf), 6) == 0);
    num = 0x0123456789;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 5, buf), 5) == 0);
    num = 0x01234567;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 4, buf), 4) == 0);
    num = 0x012345;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 3, buf), 3) == 0);
    num = 0x0123;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 2, buf), 2) == 0);
    num = 0x01;
    ABTS_TRUE(tc, memcmp(tmp, core_uint64_to_buffer(num, 1, buf), 1) == 0);
}

static void misc_test4(abts_case *tc, void *data)
{
    ABTS_TRUE(tc, 0x0123456789abcdef ==
            core_buffer_to_uint64("\x01\x23\x45\x67\x89\xab\xcd\xef", 8));
    ABTS_TRUE(tc, 0x0123456789abcd ==
            core_buffer_to_uint64("\x01\x23\x45\x67\x89\xab\xcd", 7));
    ABTS_TRUE(tc, 0x0123456789ab ==
            core_buffer_to_uint64("\x01\x23\x45\x67\x89\xab", 6));
    ABTS_TRUE(tc, 0x0123456789 ==
            core_buffer_to_uint64("\x01\x23\x45\x67\x89", 5));
    ABTS_TRUE(tc, 0x01234567 ==
            core_buffer_to_uint64("\x01\x23\x45\x67", 4));
    ABTS_TRUE(tc, 0x012345 ==
            core_buffer_to_uint64("\x01\x23\x45", 3));
    ABTS_TRUE(tc, 0x0123 ==
            core_buffer_to_uint64("\x01\x23", 2));
    ABTS_TRUE(tc, 0x01 ==
            core_buffer_to_uint64("\x01", 1));
}

static void misc_test5(abts_case *tc, void *data)
{
    char out[16];
    int out_len;
#define MSISDN "491725670014"
    core_bcd_to_buffer(MSISDN, strlen(MSISDN), out, &out_len);
    ABTS_TRUE(tc, memcmp("\x94\x71\x52\x76\x00\x41", out, out_len) == 0);
#define MEI "3516020019874800"
    core_bcd_to_buffer(MEI, strlen(MEI), out, &out_len);
    ABTS_TRUE(tc, 
            memcmp("\x53\x61\x20\x00\x91\x78\x84\x00", out, out_len) == 0);
}

abts_suite *testmisc(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, misc_test1, NULL);
    abts_run_test(suite, misc_test2, NULL);
    abts_run_test(suite, misc_test3, NULL);
    abts_run_test(suite, misc_test4, NULL);
    abts_run_test(suite, misc_test5, NULL);

    return suite;
}
