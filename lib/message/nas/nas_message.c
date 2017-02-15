#define TRACE_MODULE _nasmsg

#include "core_debug.h"
#include "nas_message.h"

status_t nas_decode_attach_request(nas_message_t *message, pkbuf_t *pkbuf)
{
    nas_attach_request_t *attach_request = &message->emm.attach_request;
    status_t rv;

    rv = nas_decode_attach_info(&attach_request->attach_info, pkbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "decode failed");

    rv = nas_decode_eps_mobile_identity(
            &attach_request->eps_mobile_identity, pkbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "decode failed");

    rv = nas_decode_ue_network_capability(
            &attach_request->ue_network_capability, pkbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "decode failed");

    return CORE_OK;
}

status_t nas_decode_pdu(nas_message_t *message, pkbuf_t *pkbuf)
{
    status_t rv = CORE_ERROR;
    nas_security_header_t *security_header = pkbuf->payload;
    nas_header_t *header = NULL;

    if (security_header->security_header_type != 
            NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE)
    {
        pkbuf_header(pkbuf, -(c_int16_t)(sizeof(nas_security_header_t)));
    }
    header = pkbuf->payload;
    pkbuf_header(pkbuf, -(c_int16_t)(sizeof(nas_header_t)));

    memset(message, 0, sizeof(nas_message_t));
    switch(header->message_type)
    {
        case NAS_ATTACH_REQUEST:
            rv = nas_decode_attach_request(message, pkbuf);
            break;
        case NAS_ATTACH_ACCEPT:
        case NAS_ATTACH_COMPLETE:
        case NAS_ATTACH_REJECT:
        case NAS_DETACH_REQUEST:
        case NAS_DETACH_ACCEPT:
        case NAS_TRACKING_AREA_UPDATE_REQUEST:
        case NAS_TRACKING_AREA_UPDATE_ACCEPT:
        case NAS_TRACKING_AREA_UPDATE_COMPLETE:
        case NAS_TRACKING_AREA_UPDATE_REJECT:
        case NAS_EXTENDED_SERVICE_REQUEST:
        case NAS_SERVICE_REJECT:
        case NAS_GUTI_REALLOCATION_COMMAND:
        case NAS_GUTI_REALLOCATION_COMPLETE:
        case NAS_AUTHENTICATION_REQUEST:
        case NAS_AUTHENTICATION_RESPONSE:
        case NAS_AUTHENTICATION_REJECT:
        case NAS_AUTHENTICATION_FAILURE:
        case NAS_IDENTITY_REQUEST:
        case NAS_IDENTITY_RESPONSE:
        case NAS_SECURITY_MODE_COMMAND:
        case NAS_SECURITY_MODE_COMPLETE:
        case NAS_SECURITY_MODE_REJECT:
        case NAS_EMM_STATUS:
        case NAS_EMM_INFORMATION:
        case NAS_DOWNLINK_NAS_TRANSPORT:
        case NAS_UPLINK_NAS_TRANSPORT:
        case NAS_CS_SERVICE_NOTIFICATION:
        case NAS_DOWNLINK_GENERIC_NAS_TRANSPORT:
        case NAS_UPLINK_GENERIC_NAS_TRANSPORT:

        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST:
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
        case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REJECT:
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST:
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
        case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REJECT:
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST:
        case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
        case NAS_MODIFY_EPS_BEARER_CONTEXT_REJECT:
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST:
        case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
        case NAS_PDN_CONNECTIVITY_REQUEST:
        case NAS_PDN_CONNECTIVITY_REJECT:
        case NAS_PDN_DISCONNECT_REQUEST:
        case NAS_PDN_DISCONNECT_REJECT:
        case NAS_BEARER_RESOURCE_ALLOCATION_REQUEST:
        case NAS_BEARER_RESOURCE_ALLOCATION_REJECT:
        case NAS_BEARER_RESOURCE_MODIFICATION_REQUEST:
        case NAS_BEARER_RESOURCE_MODIFICATION_REJECT:
        case NAS_ESM_INFORMATION_REQUEST:
        case NAS_ESM_INFORMATION_RESPONSE:
        case NAS_ESM_STATUS:
            break;
        default:
            d_error("Unknown message type (%d) or not implemented", 
                    header->message_type);
            break;
    }
    

    return rv;
}
