#define TRACE_MODULE _mme_kdf

#include "core_sha2_hmac.h"

void mme_kdf_nas(c_uint8_t algorithm_type_distinguishers,
    c_uint8_t algorithm_identity, c_uint8_t *kasme, c_uint8_t *knas)
{
    c_uint8_t s[7];
    c_uint8_t out[32];

    s[0] = 0x15; /* FC Value */

    s[1] = algorithm_type_distinguishers;
    s[2] = 0x00;
    s[3] = 0x01;

    s[4] = algorithm_identity;
    s[5] = 0x00;
    s[6] = 0x01;

    hmac_sha256(kasme, 32, s, 7, out, 32);
    memcpy(knas, out+16, 16);
}

void mme_kdf_enb(c_uint8_t *kasme, c_uint32_t ul_count, c_uint8_t *kenb)
{
    c_uint8_t s[7];

    s[0] = 0x11; /* FC Value */

    ul_count = htonl(ul_count);
    memcpy(s+1, &ul_count, 4);

    s[5] = 0x00;
    s[6] = 0x04;

    hmac_sha256(kasme, 32, s, 7, kenb, 32);
}

void mme_kdf_nh(c_uint8_t *kasme, c_uint8_t *sync_input, c_uint8_t *kenb)
{
    c_uint8_t s[35];

    s[0] = 0x12; /* FC Value */

    memcpy(s+1, sync_input, 32);

    s[33] = 0x00;
    s[34] = 0x20;

    hmac_sha256(kasme, 32, s, 35, kenb, 32);
}
