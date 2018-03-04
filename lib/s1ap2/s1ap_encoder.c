#define TRACE_MODULE _s1ap_encoder

#include "3gpp_types.h"

#include "s1ap_message.h"

int s1ap_encode_pdu(pkbuf_t **pkb, s1ap_message_t *message_p)
{
    int encoded = -1;

#if 0
    d_assert (message_p, return -1, "Null param");

    *pkb = pkbuf_alloc(0, MAX_SDU_LEN);
    d_assert(*pkb, return -1, "Null Param");

    switch (message_p->direction) 
    {
        case S1AP_PDU_PR_initiatingMessage:
            encoded = s1ap_encode_initiating_message(message_p, *pkb);
            break;

        case S1AP_PDU_PR_successfulOutcome:
            encoded = s1ap_encode_successfull_outcome(message_p, *pkb);
            break;

        case S1AP_PDU_PR_unsuccessfulOutcome:
            encoded = s1ap_encode_unsuccessfull_outcome(message_p, *pkb);
            break;

        default:
            d_warn("Unknown message outcome (%d) or not implemented", 
                    (int)message_p->direction);
            pkbuf_free(*pkb);
            return -1;
    }

    if (encoded < 0)
    {
        pkbuf_free(*pkb);
        return -1;
    }

    (*pkb)->len = (encoded >> 3);
#endif

    return encoded;
}

#if 0
static inline int s1ap_encode_initiating_message(
    s1ap_message_t *message_p, pkbuf_t *pkbuf)
{
    int ret = -1;
    switch (message_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            ret = s1ap_encode_s1setup_request(message_p, pkbuf);
            break;
        default:
            d_warn("Unknown procedure ID (%d) for initiating message_p\n", 
                    (int)message_p->procedureCode);
            break;
    }

    return ret;
}
#endif
