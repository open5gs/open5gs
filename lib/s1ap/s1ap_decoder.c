#define TRACE_MODULE _s1ap_decoder

#include "s1ap_message.h"

status_t s1ap_decode_pdu(s1ap_message_t *message, pkbuf_t *pkbuf)
{
    asn_dec_rval_t dec_ret = {0};

    d_assert(message, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);
    d_assert(pkbuf->payload, return CORE_ERROR,);

    memset((void *)message, 0, sizeof(s1ap_message_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1AP_S1AP_PDU, (void **)&message, 
            pkbuf->payload, pkbuf->len, 0, 0);

    if (dec_ret.code != RC_OK) 
    {
        d_error("Failed to decode S1AP-PDU[%d]", dec_ret.code);
        return CORE_ERROR;
    }

    if (g_trace_mask && TRACE_MODULE >= 25)
        asn_fprint(stdout, &asn_DEF_S1AP_S1AP_PDU, message);

    return CORE_OK;
}
