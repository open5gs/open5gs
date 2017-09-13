#define TRACE_MODULE _nas_conv

#include "core_debug.h"

#include "nas_conv.h"

void nas_imsi_to_bcd(
    nas_mobile_identity_imsi_t *imsi, c_uint8_t imsi_len, c_int8_t *bcd)
{
    int bcd_len;

    bcd[0] = '0' + imsi->digit1;
    bcd[1] = '0' + imsi->digit2;
    bcd[2] = '0' + imsi->digit3;
    bcd[3] = '0' + imsi->digit4;
    bcd[4] = '0' + imsi->digit5;
    bcd[5] = '0' + imsi->digit6;
    bcd[6] = '0' + imsi->digit7;
    bcd[7] = '0' + imsi->digit8;
    bcd[8] = '0' + imsi->digit9;
    bcd[9] = '0' + imsi->digit10;
    bcd[10] = '0' + imsi->digit11;
    bcd[11] = '0' + imsi->digit12;
    bcd[12] = '0' + imsi->digit13;
    bcd[13] = '0' + imsi->digit14;
    bcd[14] = '0' + imsi->digit15;

    bcd_len = imsi_len * 2 - 1;
    if (!imsi->odd_even) /* if bcd length is even */
    {
        if (bcd[bcd_len] != 0xf)
            d_warn("Spec warning : bcd[%d] = 0x%x", bcd_len, bcd[bcd_len]);
        (bcd_len)--; 
    }

    bcd[bcd_len] = 0;
}

void nas_imsi_to_buffer(
    nas_mobile_identity_imsi_t *imsi, c_uint8_t imsi_len, 
    c_uint8_t *buf, c_uint8_t *buf_len)
{
    buf[0] = ((('0' + imsi->digit2) << 4) & 0xf0) | 
                (('0' + imsi->digit1) & 0x0f);
    buf[1] = ((('0' + imsi->digit4) << 4) & 0xf0) | 
                (('0' + imsi->digit3) & 0x0f);
    buf[2] = ((('0' + imsi->digit6) << 4) & 0xf0) | 
                (('0' + imsi->digit5) & 0x0f);
    buf[3] = ((('0' + imsi->digit8) << 4) & 0xf0) | 
                (('0' + imsi->digit7) & 0x0f);
    buf[4] = ((('0' + imsi->digit10) << 4) & 0xf0) |
                (('0' + imsi->digit9) & 0x0f);
    buf[5] = ((('0' + imsi->digit12) << 4) & 0xf0) | 
                (('0' + imsi->digit11) & 0x0f);
    buf[6] = ((('0' + imsi->digit14) << 4) & 0xf0) | 
                (('0' + imsi->digit13) & 0x0f);
    buf[7] = ((('0' + imsi->digit15)) & 0x0f);

    *buf_len = imsi_len;
    if (!imsi->odd_even) /* if imsi length is even */
    {
        (*buf_len)--; 
        if ((buf[*buf_len] & 0xf) != 0xf)
            d_warn("Spec warning : buf[%d] = 0x%x", *buf_len, buf[*buf_len]);
    }
}
