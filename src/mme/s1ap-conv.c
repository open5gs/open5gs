#include "base/types.h"
#include "s1ap-conv.h"

void s1ap_uint8_to_OCTET_STRING(uint8_t uint8, OCTET_STRING_t *octet_string)
{
    octet_string->size = 1;
    octet_string->buf = ogs_calloc(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint8;
}

void s1ap_uint16_to_OCTET_STRING(uint16_t uint16, OCTET_STRING_t *octet_string)
{
    octet_string->size = 2;
    octet_string->buf = ogs_calloc(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint16 >> 8;
    octet_string->buf[1] = uint16;
}

void s1ap_uint32_to_OCTET_STRING(uint32_t uint32, OCTET_STRING_t *octet_string)
{
    octet_string->size = 4;
    octet_string->buf = ogs_calloc(octet_string->size, sizeof(uint8_t));

    octet_string->buf[0] = uint32 >> 24;
    octet_string->buf[1] = uint32 >> 16;
    octet_string->buf[2] = uint32 >> 8;
    octet_string->buf[3] = uint32;
}

void s1ap_buffer_to_OCTET_STRING(
        void *buf, int size, S1AP_TBCD_STRING_t *tbcd_string)
{
    tbcd_string->size = size;
    tbcd_string->buf = ogs_calloc(tbcd_string->size, sizeof(uint8_t));

    memcpy(tbcd_string->buf, buf, size);
}

void s1ap_uint32_to_ENB_ID(
    S1AP_ENB_ID_PR present, uint32_t enb_id, S1AP_ENB_ID_t *eNB_ID)
{
    ogs_assert(eNB_ID);

    eNB_ID->present = present;
    if (present == S1AP_ENB_ID_PR_macroENB_ID)
    {
        BIT_STRING_t *bit_string = &eNB_ID->choice.macroENB_ID;
        ogs_assert(bit_string);

        bit_string->size = 3;
        bit_string->buf = ogs_calloc(bit_string->size, sizeof(uint8_t));

        bit_string->buf[0] = enb_id >> 12;
        bit_string->buf[1] = enb_id >> 4;
        bit_string->buf[2] = (enb_id & 0xf) << 4;

        bit_string->bits_unused = 4;
    } 
    else if (present == S1AP_ENB_ID_PR_homeENB_ID)
    {
        BIT_STRING_t *bit_string = &eNB_ID->choice.homeENB_ID;
        ogs_assert(bit_string);

        bit_string->size = 4;
        bit_string->buf = ogs_calloc(bit_string->size, sizeof(uint8_t));

        bit_string->buf[0] = enb_id >> 20;
        bit_string->buf[1] = enb_id >> 12;
        bit_string->buf[2] = enb_id >> 4;
        bit_string->buf[3] = (enb_id & 0xf) << 4;

        bit_string->bits_unused = 4;
    }
    else
    {
        ogs_assert_if_reached();
    }

}

void s1ap_ENB_ID_to_uint32(S1AP_ENB_ID_t *eNB_ID, uint32_t *uint32)
{
    ogs_assert(uint32);
    ogs_assert(eNB_ID);

    if (eNB_ID->present == S1AP_ENB_ID_PR_homeENB_ID)
    {
        uint8_t *buf = eNB_ID->choice.homeENB_ID.buf;
        ogs_assert(buf);
        *uint32 = (buf[0] << 20) + (buf[1] << 12) + (buf[2] << 4) +
            ((buf[3] & 0xf0) >> 4);

    } 
    else if (eNB_ID->present == S1AP_ENB_ID_PR_macroENB_ID)
    {
        uint8_t *buf = eNB_ID->choice.macroENB_ID.buf;
        ogs_assert(buf);
        *uint32 = (buf[0] << 12) + (buf[1] << 4) + ((buf[2] & 0xf0) >> 4);
    }
    else
    {
        ogs_assert_if_reached();
    }
}

int s1ap_BIT_STRING_to_ip(BIT_STRING_t *bit_string, ip_t *ip)
{
    char buf[OGS_ADDRSTRLEN], buf2[OGS_ADDRSTRLEN];

    ogs_assert(bit_string);
    ogs_assert(ip);

    if (bit_string->size == IPV4V6_LEN)
    {
        ip->ipv4 = 1;
        ip->ipv6 = 1;
        memcpy(&ip->both.addr, bit_string->buf, IPV4_LEN);
        memcpy(&ip->both.addr6, bit_string->buf+IPV4_LEN, IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            INET_NTOP(&ip->both.addr, buf), INET6_NTOP(&ip->both.addr6, buf2));
    }
    else if (bit_string->size == IPV4_LEN)
    {
        ip->ipv4 = 1;
        memcpy(&ip->addr, bit_string->buf, IPV4_LEN);
        ogs_debug("    IPv4[%s]", INET_NTOP(&ip->addr, buf));
    }
    else if (bit_string->size == IPV6_LEN)
    {
        ip->ipv6 = 1;
        memcpy(&ip->addr6, bit_string->buf, IPV6_LEN);
        ogs_debug("    IPv6[%s]", INET_NTOP(&ip->addr6, buf));
    }
    else
        ogs_assert_if_reached();

    ip->len =  bit_string->size;

    return OGS_OK;
}
int s1ap_ip_to_BIT_STRING(ip_t *ip, BIT_STRING_t *bit_string)
{
    char buf[OGS_ADDRSTRLEN], buf2[OGS_ADDRSTRLEN];

    ogs_assert(ip);
    ogs_assert(bit_string);

    if (ip->ipv4 && ip->ipv6)
    {
        bit_string->size = IPV4V6_LEN;
        bit_string->buf = ogs_calloc(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->both.addr, IPV4_LEN);
        memcpy(bit_string->buf+IPV4_LEN, &ip->both.addr6, IPV6_LEN);
        ogs_debug("    IPv4[%s] IPv6[%s]",
            INET_NTOP(&ip->both.addr, buf), INET6_NTOP(&ip->both.addr6, buf2));
    }
    else if (ip->ipv4)
    {
        bit_string->size = IPV4_LEN;
        bit_string->buf = ogs_calloc(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr, IPV4_LEN);
        ogs_debug("    IPv4[%s]", INET_NTOP(&ip->addr, buf));
    }
    else if (ip->ipv6)
    {
        bit_string->size = IPV6_LEN;
        bit_string->buf = ogs_calloc(bit_string->size, sizeof(uint8_t));
        memcpy(bit_string->buf, &ip->addr6, IPV6_LEN);
        ogs_debug("    IPv6[%s]", INET_NTOP(&ip->addr6, buf));
    }
    else
        ogs_assert_if_reached();

    return OGS_OK;
}

int s1ap_copy_ie(const asn_TYPE_descriptor_t *td, void *src, void *dst)
{
    asn_enc_rval_t enc_ret = {0};
    asn_dec_rval_t dec_ret = {0};
    uint8_t buffer[MAX_SDU_LEN];

    ogs_assert(td);
    ogs_assert(src);
    ogs_assert(dst);

    enc_ret = aper_encode_to_buffer(td, NULL, src, buffer, MAX_SDU_LEN);
    if (enc_ret.encoded < 0)
    {
        ogs_error("aper_encode_to_buffer() failed[%d]", (int)enc_ret.encoded);
        return OGS_ERROR;
    }

    dec_ret = aper_decode(NULL, td, (void **)&dst,
            buffer, (enc_ret.encoded >> 3), 0, 0);

    if (dec_ret.code != RC_OK) 
    {
        ogs_error("aper_decode() failed[%d]", dec_ret.code);
        return OGS_ERROR;
    }

    return OGS_OK;
}
