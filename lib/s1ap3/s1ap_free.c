#define TRACE_MODULE _s1ap_free

#include "s1ap_message.h"

status_t s1ap_free_pdu(s1ap_message_t *message)
{
    d_assert(message, return CORE_ERROR,);

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1AP_S1AP_PDU, message);

    return CORE_OK;
}
