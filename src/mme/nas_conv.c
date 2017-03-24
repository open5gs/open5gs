#define TRACE_MODULE _nasconv

#include "core_debug.h"

#include "nas_conv.h"

void nas_imsi_bcd_to_buffer(
    nas_mobile_identity_imsi_t *bcd, c_uint8_t bcd_len, 
    c_uint8_t *buf, c_uint8_t *buf_len)
{
    buf[0] = '0' + bcd->digit1;
    buf[1] = '0' + bcd->digit2;
    buf[2] = '0' + bcd->digit3;
    buf[3] = '0' + bcd->digit4;
    buf[4] = '0' + bcd->digit5;
    buf[5] = '0' + bcd->digit6;
    buf[6] = '0' + bcd->digit7;
    buf[7] = '0' + bcd->digit8;
    buf[8] = '0' + bcd->digit9;
    buf[9] = '0' + bcd->digit10;
    buf[10] = '0' + bcd->digit11;
    buf[11] = '0' + bcd->digit12;
    buf[12] = '0' + bcd->digit13;
    buf[13] = '0' + bcd->digit14;
    buf[14] = '0' + bcd->digit15;

    *buf_len = bcd_len * 2 - 1;
    if (!bcd->odd_even) /* if bcd length is even */
    {
        if (buf[*buf_len] != 0xf)
            d_warn("Spec warning : buf[%d] = 0x%x", *buf_len, buf[*buf_len]);
        (*buf_len)--; 
    }

    buf[*buf_len] = 0;
}
