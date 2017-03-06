#define TRACE_MODULE _mme_kdf

#include "core_sha2_hmac.h"

#define FC_VALUE 0x15

void mme_kdf_nas(c_uint8_t algorithm_type_distinguishers,
    c_uint8_t algorithm_identity, c_uint8_t *kasme, c_uint8_t *knas)
{
    c_uint8_t s[7];
    c_uint8_t out[32];

    s[0] = FC_VALUE;

    s[1] = algorithm_type_distinguishers;
    s[2] = 0x00;
    s[3] = 0x01;

    s[1] = algorithm_identity;
    s[2] = 0x00;
    s[3] = 0x01;

    hmac_sha256(kasme, 32, s, 7, out, 32);
    memcpy(knas, out+16, 16);
}
