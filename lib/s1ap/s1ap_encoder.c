#define TRACE_MODULE _s1ap_encoder

#include "3gpp_types.h"

#include "s1ap_message.h"

status_t s1ap_encode_pdu(pkbuf_t **pkbuf, s1ap_message_t *message)
{
    asn_enc_rval_t enc_ret = {0};

    d_assert(message, return CORE_ERROR,);

    if (g_trace_mask && TRACE_MODULE >= 25)
        asn_fprint(stdout, &asn_DEF_S1AP_S1AP_PDU, message);

    *pkbuf = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(*pkbuf, return CORE_ERROR,);

    enc_ret = aper_encode_to_buffer(&asn_DEF_S1AP_S1AP_PDU, NULL,
                    message, (*pkbuf)->payload, MAX_SDU_LEN);
    if (enc_ret.encoded < 0)
    {
        d_error("Failed to decode S1AP-PDU[%d]", enc_ret.encoded);
        pkbuf_free(*pkbuf);
        return CORE_ERROR;
    }

    (*pkbuf)->len = (enc_ret.encoded >> 3);

    return CORE_OK;
}
