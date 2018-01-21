#define TRACE_MODULE _hss_kdf

#include "core_debug.h"
#include "core_sha2_hmac.h"
#include "3gpp_types.h"

#include "hss_auc.h"
#include "milenage.h"

#define FC_VALUE 0x10

void hss_auc_kasme(const c_uint8_t *ck, const c_uint8_t *ik, 
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

void hss_auc_sqn(
    const c_uint8_t *opc, const c_uint8_t *k, const c_uint8_t *auts,
    c_uint8_t *sqn_ms, c_uint8_t *mac_s)
{
    int i;
    c_uint8_t ak[HSS_AK_LEN];
    c_uint8_t amf[2] = { 0, 0 };
    const c_uint8_t *rand = auts;
    const c_uint8_t *conc_sqn_ms = auts + RAND_LEN;

    milenage_f2345(opc, k, rand, NULL, NULL, NULL, NULL, ak);
    for (i = 0; i < HSS_SQN_LEN; i++)
        sqn_ms[i] = ak[i] ^ conc_sqn_ms[i];
    milenage_f1(opc, k, auts, sqn_ms, amf, NULL, mac_s);
}
