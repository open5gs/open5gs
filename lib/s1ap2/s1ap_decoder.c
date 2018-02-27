#define TRACE_MODULE _s1ap_decoder

#include "s1ap_message.h"

int s1ap_decode_pdu(s1ap_message_t *message, pkbuf_t *pkb)
{
    s1ap_message_t *pdu_p = message;
    asn_dec_rval_t dec_ret = {0};

    d_assert(message, return -1,);
    d_assert(pkb, return -1,);
    d_assert(pkb->payload, return -1,);

    memset((void *)pdu_p, 0, sizeof(s1ap_message_t));
    dec_ret = aper_decode(NULL, &asn_DEF_S1AP_S1AP_PDU, (void **)&pdu_p, 
            pkb->payload, pkb->len, 0, 0);

    if (dec_ret.code != RC_OK) 
    {
        d_error("Failed to decode PDU");
        return -1;
    }

    return 0;
}
