#define TRACE_MODULE _nasmsg

#include "core_debug.h"
#include "nas_message.h"

c_int32_t nas_encode_attach_accept(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_accept_t *attach_accept = &message->emm.attach_accept;
    c_int32_t size = 0;
    c_int32_t encoded = 0;

    size = nas_encode_attach_result(pkbuf, &attach_accept->attach_result);
    d_assert(size >= 0, return encoded, "decode failed");
    encoded += size;

    size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3412_value);
    d_assert(size >= 0, return encoded, "decode failed");
    encoded += size;

    size = nas_encode_tracking_area_identity_list(pkbuf, 
            &attach_accept->tai_list);
    d_assert(size >= 0, return encoded, "decode failed");
    encoded += size;

    size = nas_encode_esm_message_container(pkbuf, 
            &attach_accept->esm_message_container);
    d_assert(size >= 0, return encoded, "decode failed");
    encoded += size;

    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_GUTI_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, NAS_ATTACH_ACCEPT_GUTI_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_mobile_identity(pkbuf, &attach_accept->guti);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_LOCATION_AREA_IDENTIFICATION_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_location_area_identification(pkbuf, 
                &attach_accept->location_area_identification);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_MS_IDENTITY_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_MS_IDENTITY_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_mobile_identity(pkbuf, 
                &attach_accept->ms_identity);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_EMM_CAUSE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_emm_cause(pkbuf, &attach_accept->emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_T3402_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3402_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_T3423_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer(pkbuf, &attach_accept->t3423_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_EQUIVALENT_PLMNS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_plmn_list(pkbuf, &attach_accept->equivalent_plmns);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    /* 9.9.3.37 Emergency number list
     * See subclause 10.5.3.13 in 3GPP TS 24.008 [13].
     * O TLV 5-50 
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_EMERGENCY_NUMBER_LIST_PRESENT)
    {
    }
    */
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_eps_network_feature_support(pkbuf, 
                &attach_accept->eps_network_feature_support);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_PRESENT)
    {
        attach_accept->additional_update_result.type = 
            (NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = nas_encode_additional_update_result(pkbuf, 
                &attach_accept->additional_update_result);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_T3412_EXTENDED_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_3(pkbuf, 
                &attach_accept->t3412_extended_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & NAS_ATTACH_ACCEPT_T3324_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_T3324_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_accept->t3324_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }
    if (attach_accept->presencemask & 
            NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_ACCEPT_EXTENDED_DRX_PARAMETERS_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_extended_drx_parameters(pkbuf,
                &attach_accept->extended_drx_parameters);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

c_int32_t nas_encode_attach_reject(pkbuf_t *pkbuf, nas_message_t *message)
{
    nas_attach_reject_t *attach_reject = &message->emm.attach_reject;
    c_int32_t size = 0;
    c_int32_t encoded = 0;

    size = nas_encode_emm_cause(pkbuf, &attach_reject->emm_cause);
    d_assert(size >= 0, return encoded, "decode failed");
    encoded += size;

    if (attach_reject->presencemask & 
            NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_esm_message_container(pkbuf, 
                &attach_reject->esm_message_container);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_T3346_VALUE_PRESENT)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_REJECT_T3346_VALUE_PRESENT);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_reject->t3346_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_T3402_VALUE_TYPE)
    {
        size = nas_encode_optional_type(pkbuf, 
                NAS_ATTACH_REJECT_T3402_VALUE_TYPE);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;

        size = nas_encode_gprs_timer_2(pkbuf, &attach_reject->t3402_value);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    if (attach_reject->presencemask & NAS_ATTACH_REJECT_EXTENDED_EMM_CAUSE_TYPE)
    {
        attach_reject->extended_emm_cause.type = 
            (NAS_ATTACH_ACCEPT_ADDITIONAL_UPDATE_RESULT_TYPE >> 4);

        size = nas_encode_extended_emm_cause(pkbuf, 
                &attach_reject->extended_emm_cause);
        d_assert(size >= 0, return encoded, "decode failed");
        encoded += size;
    }

    return encoded;
}

status_t nas_encode_pdu(pkbuf_t **pkbuf, nas_message_t *message)
{
    status_t rv = CORE_ERROR;
    c_int32_t size = 0;
    c_int32_t encoded = 0;

    d_assert(message, return CORE_ERROR, "Null param");

    *pkbuf = pkbuf_alloc(0, NAS_SDU_SIZE);
    d_assert(*pkbuf, return -1, "Null Param");

    size = sizeof(nas_header_t);
    rv = pkbuf_header(*pkbuf, -size);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");
    memcpy((*pkbuf)->payload - size, &message->h, size);
    encoded += size;

    switch(message->h.message_type)
    {
        case NAS_ATTACH_REQUEST:
            d_error("Not implemented", message->h.message_type);
            pkbuf_free((*pkbuf));
            return CORE_ERROR;
        case NAS_ATTACH_ACCEPT:
            size = nas_encode_attach_accept(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
            break;
        case NAS_ATTACH_COMPLETE:
            d_error("Not implemented", message->h.message_type);
            pkbuf_free((*pkbuf));
            return CORE_ERROR;
        case NAS_ATTACH_REJECT:
            size = nas_encode_attach_reject(*pkbuf, message);
            d_assert(size >= 0, return CORE_ERROR, "decode error");
            encoded += size;
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
        default:
            d_error("Unknown message type (%d) or not implemented", 
                    message->h.message_type);
            pkbuf_free((*pkbuf));
            return CORE_ERROR;
    }

    rv = pkbuf_header(*pkbuf, encoded);
    d_assert(rv == CORE_OK, return CORE_ERROR, "pkbuf_header error");

    (*pkbuf)->len = encoded;

    return CORE_OK;
}
