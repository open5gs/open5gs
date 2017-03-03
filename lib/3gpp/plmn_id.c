#define TRACE_MODULE _plmn_id

#include "plmn_id.h"

#define PLMN_ID_DIGIT1(x) (((x) / 100) % 10)
#define PLMN_ID_DIGIT2(x) (((x) / 10) % 10)
#define PLMN_ID_DIGIT3(x) ((x) % 10)

void *plmn_id_to_buffer(plmn_id_t *plmn_id, c_uint8_t *buf)
{
    buf[0] = (PLMN_ID_DIGIT2(plmn_id->mcc) << 4) | PLMN_ID_DIGIT1(plmn_id->mcc);

    if (plmn_id->mnc_len == 2)
        buf[1] = (0xf << 4);
    else
        buf[1] = (PLMN_ID_DIGIT1(plmn_id->mnc) << 4);
    buf[1] |= PLMN_ID_DIGIT3(plmn_id->mcc);

    buf[2] = (PLMN_ID_DIGIT3(plmn_id->mnc) << 4) | PLMN_ID_DIGIT2(plmn_id->mnc);

    return buf;
}
