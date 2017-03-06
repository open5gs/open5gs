#define TRACE_MODULE _nasies

#include "core_debug.h"
#include "core_lib.h"
#include "nas_ies.h"

c_int32_t nas_encode_optional_type(pkbuf_t *pkbuf, c_uint8_t type)
{
    c_uint16_t size = 0;

    size = sizeof(c_uint8_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, &type, size);

    return size;
}

/* 9.9.2.0A Device properties
 * See subclause 10.5.7.8 in 3GPP TS 24.008 [13].
 * O TV 1 */
c_int32_t nas_decode_device_properties(
    nas_device_properties_t *device_properties, pkbuf_t *pkbuf)
{
    memcpy(device_properties, pkbuf->payload - 1, 1);
    return 0;
}

/* 9.9.2.2 Location area identification
 * See subclause 10.5.1.3 in 3GPP TS 24.008 [13]
 * O TV 6 */
c_int32_t nas_decode_location_area_identification(
    nas_location_area_identification_t *location_area_identification, 
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_location_area_identification_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(location_area_identification, pkbuf->payload - size, size);
    
    location_area_identification->lac = 
        ntohs(location_area_identification->lac);

    return size;
}

c_int32_t nas_encode_location_area_identification(
    pkbuf_t *pkbuf,
    nas_location_area_identification_t *location_area_identification)
{
    c_uint16_t size = 0;
    nas_location_area_identification_t target;

    d_assert(location_area_identification, return -1, "Null param");
    memcpy(&target, location_area_identification, 
            sizeof(nas_location_area_identification_t));

    size = sizeof(nas_location_area_identification_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    target.lac = htons(location_area_identification->lac);
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.2.3 Mobile identity
 * See subclause 10.5.1.4 in 3GPP TS 24.008 [13].
 * O TLV 7-10 */
c_int32_t nas_encode_mobile_identity(
    pkbuf_t *pkbuf, nas_mobile_identity_t *mobile_identity)
{
    c_uint16_t size = 0;
    nas_mobile_identity_t target;

    d_assert(mobile_identity, return -1, "Null param");
    memcpy(&target, mobile_identity, sizeof(nas_mobile_identity_t));

    size = mobile_identity->length + sizeof(mobile_identity->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    if (mobile_identity->tmsi.type_of_identity == NAS_MOBILE_IDENTITY_TMSI)
    {
        target.tmsi.tmsi = htonl(mobile_identity->tmsi.tmsi);
        target.tmsi.spare = 0xf;
    }
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

c_int32_t nas_decode_mobile_identity(
    nas_mobile_identity_t *mobile_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_identity_t *source = pkbuf->payload;

    mobile_identity->length = source->length;
    size = mobile_identity->length + sizeof(mobile_identity->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(mobile_identity, pkbuf->payload - size, size);
    
    return size;
}

/* 9.9.2.4 Mobile station classmark 2
 * See subclause 10.5.1.6 in 3GPP TS 24.008
 * O TLV 5 */
c_int32_t nas_decode_mobile_station_classmark_2(
    nas_mobile_station_classmark_2_t *mobile_station_classmark_2, 
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_station_classmark_2_t *source = pkbuf->payload;

    mobile_station_classmark_2->length = source->length;
    size = mobile_station_classmark_2->length + 
        sizeof(mobile_station_classmark_2->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(mobile_station_classmark_2, pkbuf->payload - size, size);
    
    return size;
}

/*9.9.2.5 Mobile station classmark 3
 * See subclause 10.5.1.7 in 3GPP TS 24.008 [13].
 * O TLV 2-34 */
c_int32_t nas_decode_mobile_station_classmark_3(
    nas_mobile_station_classmark_3_t *mobile_station_classmark_3, 
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_mobile_station_classmark_3_t *source = pkbuf->payload;

    mobile_station_classmark_3->length = source->length;
    size = mobile_station_classmark_3->length + 
        sizeof(mobile_station_classmark_3->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    /* TODO 
    memcpy(mobile_station_classmark_3, pkbuf->payload - size, size);
    */
    
    return size;
}

/* 9.9.2.8 PLMN list
 * See subclause 10.5.1.13 in 3GPP TS 24.008 [13].
 * O TLV 5-47 */
status_t nas_encode_plmn_list(
    pkbuf_t *pkbuf, nas_plmn_list_t *plmn_list)
{
    c_uint16_t size = 0;

    d_assert(plmn_list, return -1, "Null param");

    size = plmn_list->length + sizeof(plmn_list->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, plmn_list, size);

    return size;
}

/* 9.9.2.10 Supported codec list
 * See subclause 10.5.4.32 in 3GPP TS 24.008 [13].
 * O TLV 5-n */
c_int32_t nas_decode_supported_codec_list(
    nas_supported_codec_list_t *supported_codec_list, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_supported_codec_list_t *source = pkbuf->payload;

    supported_codec_list->length = source->length;
    size = supported_codec_list->length + 
        sizeof(supported_codec_list->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(supported_codec_list, pkbuf->payload - size, 
           c_min(size, sizeof(nas_supported_codec_list_t)));

    return size;
}

c_int32_t nas_decode_additional_update_type(
    nas_additional_update_type_t *additional_update_type, pkbuf_t *pkbuf)
{
    memcpy(additional_update_type, pkbuf->payload - 1, 1);
    return 0;
}

/* 9.9.3.0A Additional update result
 * O TV 1  */
c_int32_t nas_encode_additional_update_result(
    pkbuf_t *pkbuf, 
    nas_additional_update_result_t *additional_update_result)
{
    c_uint16_t size = 0;

    d_assert(additional_update_result, return -1, "Null param");

    size = sizeof(nas_additional_update_result_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, additional_update_result, size);

    return size;
}

/* 9.9.3.1 Authentication failure parameter
 * See subclause 10.5.3.2.2 in 3GPP TS 24.008 [13].
 * O TLV 16 */
c_int32_t nas_decode_authentication_failure_parameter(
    nas_authentication_failure_parameter_t *authentication_failure_parameter,
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_authentication_failure_parameter_t *source = pkbuf->payload;

    authentication_failure_parameter->length = source->length;
    size = authentication_failure_parameter->length + 
        sizeof(authentication_failure_parameter->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(authentication_failure_parameter, pkbuf->payload - size, size);
    
    return size;
}

/* 9.9.3.2 Authentication parameter AUTN
 * See subclause 10.5.3.1.1 in 3GPP TS 24.008 [13].
 * M LV 17 */
c_int32_t nas_encode_authentication_parameter_autn(pkbuf_t *pkbuf, 
    nas_authentication_parameter_autn_t *authentication_parameter_autn)
{
    c_uint16_t size = 0;

    d_assert(authentication_parameter_autn, return -1, "Null param");

    size = authentication_parameter_autn->length + 
        sizeof(authentication_parameter_autn->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, authentication_parameter_autn, size);

    return size;
}

/* 9.9.3.3 Authentication parameter RAND
 * See subclause 10.5.3.1 in 3GPP TS 24.008 [13].
 * M V 16 */
c_int32_t nas_encode_authentication_parameter_rand(pkbuf_t *pkbuf, 
    nas_authentication_parameter_rand_t *authentication_parameter_rand)
{
    c_uint16_t size = 0;

    d_assert(authentication_parameter_rand, return -1, "Null param");

    size = sizeof(nas_authentication_parameter_rand_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, authentication_parameter_rand, size);

    return size;
}

/* 9.9.3.4 Authentication response parameter
 * M LV 5-17 */
c_int32_t nas_decode_authentication_response_parameter(
    nas_authentication_response_parameter_t *authentication_response_parameter,
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_authentication_response_parameter_t *source = pkbuf->payload;

    authentication_response_parameter->length = source->length;
    size = authentication_response_parameter->length + 
            sizeof(authentication_response_parameter->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(authentication_response_parameter, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.8 DRX parameter
 * See subclause 10.5.5.6 in 3GPP TS 24.008
 * O TV 3 */
c_int32_t nas_decode_drx_parameter(
    nas_drx_parameter_t *drx_parameter, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_drx_parameter_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(drx_parameter, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.9 EMM cause
 * O TV 2 */
c_int32_t nas_encode_emm_cause(pkbuf_t *pkbuf, nas_emm_cause_t *emm_cause)
{
    c_uint16_t size = 0;

    d_assert(emm_cause, return -1, "Null param");

    size = sizeof(nas_emm_cause_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, emm_cause, size);

    return size;
}

c_int32_t nas_decode_emm_cause(nas_emm_cause_t *emm_cause, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_emm_cause_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(emm_cause, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.10 * EPS attach result
 * M V 1/2 */
c_int32_t nas_encode_attach_result(
    pkbuf_t *pkbuf, nas_attach_result_t *attach_result)
{
    c_uint16_t size = 0;

    d_assert(attach_result, return -1, "Null param");

    size = sizeof(nas_attach_result_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, attach_result, size);

    return size;
}

/* 9.9.3.11 EPS attach type
 * M V 1/2
 * 9.9.3.21 NAS key set identifier 
 * M V 1/2 */
c_int32_t nas_decode_attach_type(
    nas_attach_type_t *attach_type, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_attach_type_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(attach_type, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.12 EPS mobile identity
 * M LV 5-12 */
c_int32_t nas_decode_eps_mobile_identity(
    nas_eps_mobile_identity_t *eps_mobile_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_eps_mobile_identity_t *source = pkbuf->payload;

    eps_mobile_identity->length = source->length;
    size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(eps_mobile_identity, pkbuf->payload - size, size);
    if (eps_mobile_identity->guti.type_of_identity == 
            NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        if (eps_mobile_identity->guti.spare != 0xf)
        {
            d_warn("Spec warning : eps_mobile_identy->spare = 0x%x", 
                    eps_mobile_identity->guti.spare);
        }
        eps_mobile_identity->guti.mme_group_id = 
            ntohs(eps_mobile_identity->guti.mme_group_id);
        eps_mobile_identity->guti.m_tmsi = 
            ntohl(eps_mobile_identity->guti.m_tmsi);
    }
    
    return size;
}

status_t nas_encode_eps_mobile_identity(
    pkbuf_t *pkbuf, nas_eps_mobile_identity_t *eps_mobile_identity)
{
    c_uint16_t size = 0;
    nas_eps_mobile_identity_t target;

    d_assert(eps_mobile_identity, return -1, "Null param");
    memcpy(&target, eps_mobile_identity, sizeof(nas_eps_mobile_identity_t));

    size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    if (target.guti.type_of_identity == NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        target.guti.spare = 0xf;
        target.guti.mme_group_id = 
            htons(eps_mobile_identity->guti.mme_group_id);
        target.guti.m_tmsi = 
            htonl(eps_mobile_identity->guti.m_tmsi);
    }
    memcpy(pkbuf->payload - size, &target, size);
    
    return size;
}

/* 9.9.3.12A EPS network feature support 
 * O TLV 3 */
c_int32_t nas_encode_eps_network_feature_support(
    pkbuf_t *pkbuf, 
    nas_eps_network_feature_support_t *eps_network_feature_support)
{
    c_uint16_t size = 0;

    d_assert(eps_network_feature_support, return -1, "Null param");

    size = eps_network_feature_support->length + 
        sizeof(eps_network_feature_support->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, eps_network_feature_support, size);

    return size;
}

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
c_int32_t nas_decode_esm_message_container(
    nas_esm_message_container_t *esm_message_container, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_esm_message_container_t *source = pkbuf->payload;

    esm_message_container->length = ntohs(source->length);
    size = esm_message_container->length + 
            sizeof(esm_message_container->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    esm_message_container->buffer = pkbuf->payload - size + 
        sizeof(esm_message_container->length);

    return size;
}

c_int32_t nas_encode_esm_message_container(
    pkbuf_t *pkbuf, nas_esm_message_container_t *esm_message_container)
{
    c_uint16_t size = 0;
    c_uint16_t target;

    d_assert(esm_message_container, return -1, "Null param");
    d_assert(esm_message_container->buffer, return -1, "Null param");

    size = sizeof(esm_message_container->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    target = htons(esm_message_container->length);
    memcpy(pkbuf->payload - size, &target, size);

    size = esm_message_container->length;
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, esm_message_container->buffer, size);

    return esm_message_container->length + 
        sizeof(esm_message_container->length);
}

/* 9.9.3.16 GPRS timer
 * See subclause 10.5.7.3 in 3GPP TS 24.008 [13].
 * M V 1 or  O TV 2
 *
 * Note : Other values shall be interpreted as multiples of 1 minute 
 * in this version of the protocol.  */
c_int32_t nas_encode_gprs_timer(pkbuf_t *pkbuf, nas_gprs_timer_t *gprs_timer)
{
    c_uint16_t size = 0;

    d_assert(gprs_timer, return -1, "Null param");

    size = sizeof(nas_gprs_timer_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, gprs_timer, size);

    return size;
}

/* 9.9.3.16A GPRS timer 2
 * See subclause 10.5.7.4 in 3GPP TS 24.008 [13].
 * O TLV 3 */
c_int32_t nas_decode_gprs_timer_2(
    nas_gprs_timer_2_t *gprs_timer_2, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_gprs_timer_2_t *source = pkbuf->payload;

    gprs_timer_2->length = source->length;
    size = gprs_timer_2->length + sizeof(gprs_timer_2->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(gprs_timer_2, pkbuf->payload - size, size);

    return size;
}
CORE_DECLARE(c_int32_t) nas_encode_gprs_timer_2(
    pkbuf_t *pkbuf, nas_gprs_timer_2_t *gprs_timer_2)
{
    c_uint16_t size = 0;

    d_assert(gprs_timer_2, return -1, "Null param");

    size = gprs_timer_2->length + sizeof(gprs_timer_2->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, gprs_timer_2, size);

    return size;
}

/* 9.9.3.16B GPRS timer 3
 * See subclause 10.5.7.4a in 3GPP TS 24.008 [13].
 * O TLV 3 */
c_int32_t nas_decode_gprs_timer_3(
    nas_gprs_timer_3_t *gprs_timer_3, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_gprs_timer_3_t *source = pkbuf->payload;

    gprs_timer_3->length = source->length;
    size = gprs_timer_3->length + sizeof(gprs_timer_3->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(gprs_timer_3, pkbuf->payload - size, size);

    return size;
}
CORE_DECLARE(c_int32_t) nas_encode_gprs_timer_3(
    pkbuf_t *pkbuf, nas_gprs_timer_3_t *gprs_timer_3)
{
    c_uint16_t size = 0;

    d_assert(gprs_timer_3, return -1, "Null param");

    size = gprs_timer_3->length + sizeof(gprs_timer_3->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, gprs_timer_3, size);

    return size;
}

/* 9.9.3.18 IMEISV request
 * See subclause 10.5.5.10 in 3GPP TS 24.008 [13].
 * O TV 1 */
c_int32_t nas_encode_imeisv_request(
    pkbuf_t *pkbuf, nas_imeisv_request_t *imeisv_request)
{
    memcpy(imeisv_request, pkbuf->payload - 1, 1);
    return 0;
}

/* 9.9.3.20 MS network capability
 * See subclause 10.5.5.12 in 3GPP TS 24.008
 * O TLV 4-10 */
c_int32_t nas_decode_ms_network_capability(
    nas_ms_network_capability_t *ms_network_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ms_network_capability_t *source = pkbuf->payload;

    ms_network_capability->length = source->length;
    size = ms_network_capability->length + sizeof(ms_network_capability->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(ms_network_capability, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.20A MS network feature support 
 * See subclause 10.5.1.15 in 3GPP TS 24.008 [13].
 * O TV 1 */
c_int32_t nas_decode_ms_network_feature_support(
    nas_ms_network_feature_support_t *ms_network_feature_support, 
    pkbuf_t *pkbuf)
{
    memcpy(ms_network_feature_support, pkbuf->payload - 1, 1);
    return 0;
}

/* 9.9.3.21 NAS key set identifier
 * M V 1/2
 * 9.9.2.9 Spare half octet
 * M V 1/2 */
c_int32_t nas_encode_nas_key_set_identifier(
    pkbuf_t *pkbuf, nas_key_set_identifier_t *nas_key_set_identifier)
{
    c_uint16_t size = 0;

    d_assert(nas_key_set_identifier, return -1, "Null param");

    size = sizeof(nas_key_set_identifier_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, nas_key_set_identifier, size);

    return size;
}

/* 9.9.3.23 NAS security algorithms
 * M V 1 */
c_int32_t nas_encode_nas_security_algorithms(
    pkbuf_t *pkbuf, nas_security_algorithms_t *nas_security_algorithms)
{
    c_uint16_t size = 0;

    d_assert(nas_security_algorithms, return -1, "Null param");

    size = sizeof(nas_security_algorithms_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, nas_security_algorithms, size);

    return size;
}

/* 9.9.3.24A Network resource identifier container
 * See subclause 10.5.5.31 in 3GPP TS 24.008 [13].
 * O TLV 4 */
c_int32_t nas_decode_network_resource_identifier_container(
    nas_network_resource_identifier_container_t *network_resource_identifier_container, 
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_network_resource_identifier_container_t *source = pkbuf->payload;

    network_resource_identifier_container->length = source->length;
    size = network_resource_identifier_container->length + 
        sizeof(network_resource_identifier_container->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(network_resource_identifier_container, pkbuf->payload - size, size);
    
    return size;
}

/* 9.9.3.25 Nonce
 * O TV 5 */
c_int32_t nas_encode_nonce(pkbuf_t *pkbuf, nas_nonce_t *nonce)
{
    c_uint16_t size = 0;
    c_uint32_t target;

    d_assert(nonce, return -1, "Null param");

    size = sizeof(nas_nonce_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    target = htonl(*nonce);
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.26 P-TMSI signature 
 * See subclause 10.5.5.8 in 3GPP TS 24.008
 * O TV 4 */
c_int32_t nas_decode_p_tmsi_signature(
    nas_p_tmsi_signature_t *p_tmsi_signature, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = 3;
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(p_tmsi_signature, pkbuf->payload - size, size);

    *p_tmsi_signature = ntohl(*p_tmsi_signature);

    return size;
}

/* 9.9.3.26A Extended EMM cause 
 * O TV 1 */
c_int32_t nas_encode_extended_emm_cause(
    pkbuf_t *pkbuf, nas_extended_emm_cause_t *extended_emm_cause)
{
    c_uint16_t size = 0;

    d_assert(extended_emm_cause, return -1, "Null param");

    size = sizeof(nas_extended_emm_cause_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(pkbuf->payload - size, extended_emm_cause, size);

    return size;
}

/* 9.9.3.31 TMSI status
 * See subclause 10.5.5.4 in 3GPP TS 24.008 [13]
 * O TV 1 */
c_int32_t nas_decode_tmsi_status(
    nas_tmsi_status_t *tmsi_status, pkbuf_t *pkbuf)
{
    memcpy(tmsi_status, pkbuf->payload - 1, 1);

    return 0;
}

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
c_int32_t nas_decode_tracking_area_identity(
    nas_tracking_area_identity_t *tracking_area_identity, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;

    size = sizeof(nas_tracking_area_identity_t);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(tracking_area_identity, pkbuf->payload - size, size);
    
    tracking_area_identity->tac = ntohs(tracking_area_identity->tac);

    return size;
}

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
c_int32_t nas_encode_tracking_area_identity_list(
    pkbuf_t *pkbuf,
    nas_tracking_area_identity_list_t *tracking_area_identity_list)
{
    c_uint16_t size = 0;
    int i = 0;
    nas_tracking_area_identity_list_t target;

    d_assert(tracking_area_identity_list, return -1, "Null param");
    memcpy(&target, tracking_area_identity_list, 
            sizeof(nas_tracking_area_identity_list_t));

    size = tracking_area_identity_list->length + 
        sizeof(tracking_area_identity_list->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    if (tracking_area_identity_list->type_of_list == 
        NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_NON_CONSECUTIVE_TACS)
    {
        for (i = 0; i < tracking_area_identity_list->number_of_elements + 1&& 
                i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
        {
            target.type0.tac[i] = 
                htons(tracking_area_identity_list->type0.tac[i]);
        }
    }
    else if (tracking_area_identity_list->type_of_list == 
            NAS_TRACKING_AREA_IDENTITY_LIST_ONE_PLMN_CONSECUTIVE_TACS)
    {
        target.type1.tac = htons(tracking_area_identity_list->type1.tac);
    }
    else if (tracking_area_identity_list->type_of_list == 
            NAS_TRACKING_AREA_IDENTITY_LIST_MANY_PLMNS)
    {
        for (i = 0; i < tracking_area_identity_list->number_of_elements + 1 && 
                i < NAS_MAX_TRACKING_AREA_IDENTITY; i++)
        {
            target.type2.tai[i].tac = 
                htons(tracking_area_identity_list->type2.tai[i].tac);
        }
    }
    else
        return -1;
    memcpy(pkbuf->payload - size, &target, size);

    return size;
}

/* 9.9.3.34 UE network capability
 * M LV 3-14 */
c_int32_t nas_decode_ue_network_capability(
    nas_ue_network_capability_t *ue_network_capability, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_ue_network_capability_t *source = pkbuf->payload;

    ue_network_capability->length = source->length;
    size = ue_network_capability->length + 
            sizeof(ue_network_capability->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(ue_network_capability, pkbuf->payload - size, size);

    return size;
}

/* 9.9.3.36 UE security capability
 * M LV 3-6 */
c_int32_t nas_encode_ue_security_capability(
    pkbuf_t *pkbuf, nas_ue_security_capability_t *ue_security_capability)
{
    c_uint16_t size = 0;

    d_assert(ue_security_capability, return -1, "Null param");

    size = ue_security_capability->length + 
        sizeof(ue_security_capability->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, ue_security_capability, size);

    return size;
}

/* 9.9.3.44 Voice domain preference and UE's usage setting
 * See subclause 10.5.5.28 in 3GPP TS 24.008 [13].
 * O TLV 3 */
c_int32_t nas_decode_voice_domain_preference_and_ue_usage_setting(
    nas_voice_domain_preference_and_ue_usage_setting_t *
        voice_domain_preference_and_ue_usage_setting, 
    pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_voice_domain_preference_and_ue_usage_setting_t *source = pkbuf->payload;

    voice_domain_preference_and_ue_usage_setting->length = source->length;
    size = voice_domain_preference_and_ue_usage_setting->length + 
        sizeof(voice_domain_preference_and_ue_usage_setting->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(voice_domain_preference_and_ue_usage_setting, 
            pkbuf->payload - size, size);
    
    return size;
}

/* 9.9.3.45 GUTI type 
 * O TV 1 */
c_int32_t nas_decode_guti_type(nas_guti_type_t *guti_type, pkbuf_t *pkbuf)
{
    memcpy(guti_type, pkbuf->payload - 1, 1);
    return 0;
}

/* 9.9.3.46 Extended DRX parameters
 * See subclause 10.5.5.32 in 3GPP TS 24.008 [13].
 * O TLV 3 */
c_int32_t nas_decode_extended_drx_parameters(
    nas_extended_drx_parameters_t *extended_drx_parameters, pkbuf_t *pkbuf)
{
    c_uint16_t size = 0;
    nas_extended_drx_parameters_t *source = pkbuf->payload;

    extended_drx_parameters->length = source->length;
    size = extended_drx_parameters->length + 
        sizeof(extended_drx_parameters->length);

    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");
    memcpy(extended_drx_parameters, pkbuf->payload - size, size);
    
    return size;
}
c_int32_t nas_encode_extended_drx_parameters(
    pkbuf_t *pkbuf, nas_extended_drx_parameters_t *extended_drx_parameters)
{
    c_uint16_t size = 0;

    d_assert(extended_drx_parameters, return -1, "Null param");

    size = extended_drx_parameters->length + 
        sizeof(extended_drx_parameters->length);
    d_assert(pkbuf_header(pkbuf, -size) == CORE_OK, 
            return -1, "pkbuf_header error");

    memcpy(pkbuf->payload - size, extended_drx_parameters, size);

    return size;
}
