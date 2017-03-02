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
#define K "465B5CE8B199B49FAA5F0A2EE238A6BC"
#define OP "5F1D289C5D354D0A140C2548F5F3E3BA"
#define OPc "E8ED289DEBA952E4283B54E88E6183CA"
#define AMF "8000"
    c_uint8_t k[16] = "\x46\x5B\x5C\xE8\xB1\x99\xB4\x9F\xAA\x5F\x0A\x2E\xE2\x38\xA6\xBC";
    c_uint8_t op[16] = "\x5F\x1D\x28\x9C\x5D\x35\x4D\x0A\x14\x0C\x25\x48\xF5\xF3\xE3\xBA";
    c_uint8_t opc[16] = "\xE8\xED\x28\x9D\xEB\xA9\x52\xE4\x28\x3B\x54\xE8\x8E\x61\x83\xCA";
    c_uint8_t amf[2] = { 0x80, 0x00 };

    char buffer[16];

    ABTS_TRUE(tc, memcmp(k, core_ascii_to_hex(K, strlen(K), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(op, core_ascii_to_hex(OP, strlen(OP), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(opc, 
            core_ascii_to_hex(OPc, strlen(OPc), buffer), 16) == 0);

    ABTS_TRUE(tc, memcmp(amf, 
            core_ascii_to_hex(AMF, strlen(AMF), buffer), 2) == 0);
}

abts_suite *testmisc(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, misc_test1, NULL);
    abts_run_test(suite, misc_test2, NULL);

    return suite;
}
