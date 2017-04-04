#define TRACE_MODULE _hss_kdf

#include "core_sha2_hmac.h"

#define FC_VALUE 0x10

void hss_kdf_kasme(const c_uint8_t *ck, const c_uint8_t *ik, 
        const c_uint8_t plmn_id[3], const c_uint8_t *sqn,  const c_uint8_t *ak,
        c_uint8_t *kasme)
{
    c_uint8_t s[14];
    c_uint8_t k[32];
    int i;

    memcpy(&k[0], ck, 16);
    memcpy(&k[16], ik, 16);

    s[0] = FC_VALUE;
    memcpy(&s[1], plmn_id, 3);
    s[4] = 0x00;
    s[5] = 0x03;

    for (i = 0; i < 6; i++)
        s[6+i] = sqn[i] ^ ak[i];
    s[12] = 0x00;
    s[13] = 0x06;

    hmac_sha256(k, 32, s, 14, kasme, 32);
}
