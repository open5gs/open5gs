#define TRACE_MODULE _s1conv

#include "core_debug.h"
#include "s1ap_conv.h"

CORE_DECLARE(void) s1ap_conv_uint8_to_octet_string(
        c_uint8_t uint8, OCTET_STRING_t *octet_string)
{
    octet_string->size = 1;
    octet_string->buf = core_calloc(octet_string->size, sizeof(c_uint8_t));

    octet_string->buf[0] = uint8;
}

CORE_DECLARE(void) s1ap_conv_uint16_to_octet_string(
        c_uint16_t uint16, OCTET_STRING_t *octet_string)
{
    octet_string->size = 2;
    octet_string->buf = core_calloc(octet_string->size, sizeof(c_uint8_t));

    octet_string->buf[0] = uint16 >> 8;
    octet_string->buf[1] = uint16;
}

CORE_DECLARE(void) s1ap_conv_uint32_to_octet_string(
        c_uint32_t uint32, OCTET_STRING_t *octet_string)
{
    octet_string->size = 4;
    octet_string->buf = core_calloc(octet_string->size, sizeof(c_uint8_t));

    octet_string->buf[0] = uint32 >> 24;
    octet_string->buf[0] = uint32 >> 16;
    octet_string->buf[0] = uint32 >> 8;
    octet_string->buf[0] = uint32;
}

#define S1AP_CONV_DECIMAL100(x) (((x) / 100) % 10)
#define S1AP_CONV_DECIMAL10(x) (((x) / 10) % 10)
#define S1AP_CONV_DECIMAL1(x) ((x) % 10)

CORE_DECLARE(void) s1ap_conv_plmn_id_to_tbcd_string(
        plmn_id_t *plmn_id, S1ap_TBCD_STRING_t *tbcd_string)

{
    tbcd_string->size = 3;
    tbcd_string->buf = core_calloc(tbcd_string->size, sizeof(c_uint8_t));

    tbcd_string->buf[0] = (S1AP_CONV_DECIMAL10(plmn_id->mcc) << 4) |
                          S1AP_CONV_DECIMAL100(plmn_id->mcc);

    if (plmn_id->mnc_len == 2)
        tbcd_string->buf[1] = (0xf << 4);
    else
        tbcd_string->buf[1] = (S1AP_CONV_DECIMAL100(plmn_id->mnc) << 4);
    tbcd_string->buf[1] |= S1AP_CONV_DECIMAL1(plmn_id->mcc);

    tbcd_string->buf[2] = (S1AP_CONV_DECIMAL1(plmn_id->mnc) << 4) |
                          S1AP_CONV_DECIMAL10(plmn_id->mnc);
}

CORE_DECLARE(void) s1ap_conv_macro_enb_id_to_bit_string(
    c_uint32_t enb_id, BIT_STRING_t *bit_string)
{
    bit_string->size = 3;
    bit_string->buf = core_calloc(bit_string->size, sizeof(c_uint8_t));

    bit_string->buf[0] = enb_id >> 12;
    bit_string->buf[1] = enb_id >> 4;
    bit_string->buf[2] = (enb_id & 0xf) << 4;

    bit_string->bits_unused = 4;
}

CORE_DECLARE(void) s1ap_conv_home_enb_id_to_bit_string(
    c_uint32_t enb_id, BIT_STRING_t *bit_string)
{
    bit_string->size = 4;
    bit_string->buf = core_calloc(bit_string->size, sizeof(c_uint8_t));

    bit_string->buf[0] = enb_id >> 20;
    bit_string->buf[1] = enb_id >> 12;
    bit_string->buf[2] = enb_id >> 4;
    bit_string->buf[3] = (enb_id & 0xf) << 4;

    bit_string->bits_unused = 4;
}

CORE_DECLARE(status_t) s1ap_conv_uint32_from_enb_id(
        c_uint32_t *uint32, S1ap_ENB_ID_t *eNB_ID)
{
    d_assert(uint32, return CORE_ERROR, "Null param");
    d_assert(eNB_ID, return CORE_ERROR, "Null param");

    if (eNB_ID->present == S1ap_ENB_ID_PR_homeENB_ID)
    {
        c_uint8_t *buf = eNB_ID->choice.homeENB_ID.buf;
        d_assert(buf, return CORE_ERROR, "Null param");
        *uint32 = (buf[0] << 20) + (buf[1] << 12) + (buf[2] << 4) +
            ((buf[3] & 0xf0) >> 4);

    } 
    else if (eNB_ID->present == S1ap_ENB_ID_PR_macroENB_ID)
    {
        c_uint8_t *buf = eNB_ID->choice.macroENB_ID.buf;
        d_assert(buf, return CORE_ERROR, "Null param");
        *uint32 = (buf[0] << 12) + (buf[1] << 4) + ((buf[2] & 0xf0) >> 4);
    }
    else
    {
        return CORE_ERROR;
    }

    return CORE_OK;
}
