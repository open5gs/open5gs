/*
 * The MIT License
 *
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*******************************************************************************
 * This file had been created by nas-message.py script v0.1.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2020-04-24 08:50:31.629657 by acetcom
 * from 24301-d80.docx
 ******************************************************************************/

#include "ogs-nas.h"

int ogs_nas_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type)
{
    uint16_t size = sizeof(uint8_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &type, size);

    return size;
}
/* 9.9.2.0 Additional information
 * O TLV 3-n */
int ogs_nas_decode_additional_information(ogs_nas_additional_information_t *additional_information, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_additional_information_t *source = (ogs_nas_additional_information_t *)pkbuf->data;

    additional_information->length = source->length;
    size = additional_information->length + sizeof(additional_information->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(additional_information, pkbuf->data - size, size);

    ogs_trace("  ADDITIONAL_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_additional_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_t *additional_information)
{
    uint16_t size = additional_information->length + sizeof(additional_information->length);
    ogs_nas_additional_information_t target;

    memcpy(&target, additional_information, sizeof(ogs_nas_additional_information_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ADDITIONAL_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.0A Device properties
 * O TV 1 */
int ogs_nas_decode_device_properties(ogs_nas_device_properties_t *device_properties, ogs_pkbuf_t *pkbuf)
{
    memcpy(device_properties, pkbuf->data - 1, 1);

    ogs_trace("  DEVICE_PROPERTIES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_device_properties(ogs_pkbuf_t *pkbuf, ogs_nas_device_properties_t *device_properties)
{
    uint16_t size = sizeof(ogs_nas_device_properties_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, device_properties, size);

    ogs_trace("  DEVICE_PROPERTIES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.1 EPS bearer context status
 * O TLV 4 */
int ogs_nas_decode_eps_bearer_context_status(ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_eps_bearer_context_status_t *source = (ogs_nas_eps_bearer_context_status_t *)pkbuf->data;

    eps_bearer_context_status->length = source->length;
    size = eps_bearer_context_status->length + sizeof(eps_bearer_context_status->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_bearer_context_status, pkbuf->data - size, size);

    ogs_trace("  EPS_BEARER_CONTEXT_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_bearer_context_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status)
{
    uint16_t size = eps_bearer_context_status->length + sizeof(eps_bearer_context_status->length);
    ogs_nas_eps_bearer_context_status_t target;

    memcpy(&target, eps_bearer_context_status, sizeof(ogs_nas_eps_bearer_context_status_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_BEARER_CONTEXT_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.10 Supported Codec List
 * O TLV 5-n */
int ogs_nas_decode_supported_codec_list(ogs_nas_supported_codec_list_t *supported_codec_list, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_supported_codec_list_t *source = (ogs_nas_supported_codec_list_t *)pkbuf->data;

    supported_codec_list->length = source->length;
    size = supported_codec_list->length + sizeof(supported_codec_list->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(supported_codec_list, pkbuf->data - size, size);

    ogs_trace("  SUPPORTED_CODEC_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_supported_codec_list(ogs_pkbuf_t *pkbuf, ogs_nas_supported_codec_list_t *supported_codec_list)
{
    uint16_t size = supported_codec_list->length + sizeof(supported_codec_list->length);
    ogs_nas_supported_codec_list_t target;

    memcpy(&target, supported_codec_list, sizeof(ogs_nas_supported_codec_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SUPPORTED_CODEC_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.2 Location area identification
 * O TV 6 */
int ogs_nas_decode_location_area_identification(ogs_nas_location_area_identification_t *location_area_identification, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_location_area_identification_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(location_area_identification, pkbuf->data - size, size);

    location_area_identification->lac = be16toh(location_area_identification->lac);

    ogs_trace("  LOCATION_AREA_IDENTIFICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_location_area_identification(ogs_pkbuf_t *pkbuf, ogs_nas_location_area_identification_t *location_area_identification)
{
    uint16_t size = sizeof(ogs_nas_location_area_identification_t);
    ogs_nas_location_area_identification_t target;

    memcpy(&target, location_area_identification, size);
    target.lac = htobe16(location_area_identification->lac);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LOCATION_AREA_IDENTIFICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.3 Mobile identity
 * O TLV 7-10 */
int ogs_nas_decode_mobile_identity(ogs_nas_mobile_identity_t *mobile_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_mobile_identity_t *source = (ogs_nas_mobile_identity_t *)pkbuf->data;

    mobile_identity->length = source->length;
    size = mobile_identity->length + sizeof(mobile_identity->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(mobile_identity, pkbuf->data - size, size);

    if (mobile_identity->tmsi.type == OGS_NAS_MOBILE_IDENTITY_TMSI)
    {
        mobile_identity->tmsi.tmsi = be32toh(mobile_identity->tmsi.tmsi);
    }

    ogs_trace("  MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_identity_t *mobile_identity)
{
    uint16_t size = mobile_identity->length + sizeof(mobile_identity->length);
    ogs_nas_mobile_identity_t target;

    memcpy(&target, mobile_identity, sizeof(ogs_nas_mobile_identity_t));
    if (mobile_identity->tmsi.type == OGS_NAS_MOBILE_IDENTITY_TMSI)
    {
        target.tmsi.tmsi = htobe32(mobile_identity->tmsi.tmsi);
        target.tmsi.spare = 0xf;
    }

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.4 Mobile station classmark 2
 * O TLV 5 */
int ogs_nas_decode_mobile_station_classmark_2(ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_mobile_station_classmark_2_t *source = (ogs_nas_mobile_station_classmark_2_t *)pkbuf->data;

    mobile_station_classmark_2->length = source->length;
    size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(mobile_station_classmark_2, pkbuf->data - size, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_mobile_station_classmark_2(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2)
{
    uint16_t size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);
    ogs_nas_mobile_station_classmark_2_t target;

    memcpy(&target, mobile_station_classmark_2, sizeof(ogs_nas_mobile_station_classmark_2_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.5 Mobile station classmark 3
 * O TLV 2-34 */
int ogs_nas_decode_mobile_station_classmark_3(ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_mobile_station_classmark_3_t *source = (ogs_nas_mobile_station_classmark_3_t *)pkbuf->data;

    mobile_station_classmark_3->length = source->length;
    size = mobile_station_classmark_3->length + sizeof(mobile_station_classmark_3->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(mobile_station_classmark_3, pkbuf->data - size, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_mobile_station_classmark_3(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_3_t *mobile_station_classmark_3)
{
    uint16_t size = mobile_station_classmark_3->length + sizeof(mobile_station_classmark_3->length);
    ogs_nas_mobile_station_classmark_3_t target;

    memcpy(&target, mobile_station_classmark_3, sizeof(ogs_nas_mobile_station_classmark_3_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.2.8 PLMN list
 * O TLV 5-47 */
int ogs_nas_decode_plmn_list(ogs_nas_plmn_list_t *plmn_list, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_plmn_list_t *source = (ogs_nas_plmn_list_t *)pkbuf->data;

    plmn_list->length = source->length;
    size = plmn_list->length + sizeof(plmn_list->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(plmn_list, pkbuf->data - size, size);

    ogs_trace("  PLMN_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_plmn_list(ogs_pkbuf_t *pkbuf, ogs_nas_plmn_list_t *plmn_list)
{
    uint16_t size = plmn_list->length + sizeof(plmn_list->length);
    ogs_nas_plmn_list_t target;

    memcpy(&target, plmn_list, sizeof(ogs_nas_plmn_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PLMN_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.0A Additional update result
 * O TV 1 */
int ogs_nas_decode_additional_update_result(ogs_nas_additional_update_result_t *additional_update_result, ogs_pkbuf_t *pkbuf)
{
    memcpy(additional_update_result, pkbuf->data - 1, 1);

    ogs_trace("  ADDITIONAL_UPDATE_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_additional_update_result(ogs_pkbuf_t *pkbuf, ogs_nas_additional_update_result_t *additional_update_result)
{
    uint16_t size = sizeof(ogs_nas_additional_update_result_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, additional_update_result, size);

    ogs_trace("  ADDITIONAL_UPDATE_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.0B Additional update type
 * O TV 1 */
int ogs_nas_decode_additional_update_type(ogs_nas_additional_update_type_t *additional_update_type, ogs_pkbuf_t *pkbuf)
{
    memcpy(additional_update_type, pkbuf->data - 1, 1);

    ogs_trace("  ADDITIONAL_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_additional_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_additional_update_type_t *additional_update_type)
{
    uint16_t size = sizeof(ogs_nas_additional_update_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, additional_update_type, size);

    ogs_trace("  ADDITIONAL_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.1 Authentication failure parameter
 * O TLV 16 */
int ogs_nas_decode_authentication_failure_parameter(ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_authentication_failure_parameter_t *source = (ogs_nas_authentication_failure_parameter_t *)pkbuf->data;

    authentication_failure_parameter->length = source->length;
    size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(authentication_failure_parameter, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_FAILURE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_authentication_failure_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter)
{
    uint16_t size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);
    ogs_nas_authentication_failure_parameter_t target;

    memcpy(&target, authentication_failure_parameter, sizeof(ogs_nas_authentication_failure_parameter_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_FAILURE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.10 EPS attach result
 * M V 1/2 */
int ogs_nas_decode_eps_attach_result(ogs_nas_eps_attach_result_t *eps_attach_result, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_eps_attach_result_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_attach_result, pkbuf->data - size, size);

    ogs_trace("  EPS_ATTACH_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_attach_result(ogs_pkbuf_t *pkbuf, ogs_nas_eps_attach_result_t *eps_attach_result)
{
    uint16_t size = sizeof(ogs_nas_eps_attach_result_t);
    ogs_nas_eps_attach_result_t target;

    memcpy(&target, eps_attach_result, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_ATTACH_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.11 EPS attach type
 * M V 1/2 */
int ogs_nas_decode_eps_attach_type(ogs_nas_eps_attach_type_t *eps_attach_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_eps_attach_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_attach_type, pkbuf->data - size, size);

    ogs_trace("  EPS_ATTACH_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_attach_type(ogs_pkbuf_t *pkbuf, ogs_nas_eps_attach_type_t *eps_attach_type)
{
    uint16_t size = sizeof(ogs_nas_eps_attach_type_t);
    ogs_nas_eps_attach_type_t target;

    memcpy(&target, eps_attach_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_ATTACH_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.12 EPS mobile identity
 * M LV 5-12 */
int ogs_nas_decode_eps_mobile_identity(ogs_nas_eps_mobile_identity_t *eps_mobile_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_eps_mobile_identity_t *source = (ogs_nas_eps_mobile_identity_t *)pkbuf->data;

    eps_mobile_identity->length = source->length;
    size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_mobile_identity, pkbuf->data - size, size);

    if (eps_mobile_identity->guti.type == OGS_NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        eps_mobile_identity->guti.mme_gid = be16toh(eps_mobile_identity->guti.mme_gid);
        eps_mobile_identity->guti.m_tmsi = be32toh(eps_mobile_identity->guti.m_tmsi);
    }

    ogs_trace("  EPS_MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_eps_mobile_identity_t *eps_mobile_identity)
{
    uint16_t size = eps_mobile_identity->length + sizeof(eps_mobile_identity->length);
    ogs_nas_eps_mobile_identity_t target;

    memcpy(&target, eps_mobile_identity, sizeof(ogs_nas_eps_mobile_identity_t));
    if (target.guti.type == OGS_NAS_EPS_MOBILE_IDENTITY_GUTI)
    {
        target.guti.spare = 0xf;
        target.guti.mme_gid = htobe16(eps_mobile_identity->guti.mme_gid);
        target.guti.m_tmsi = htobe32(eps_mobile_identity->guti.m_tmsi);
    }

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.12A EPS network feature support
 * O TLV 3 */
int ogs_nas_decode_eps_network_feature_support(ogs_nas_eps_network_feature_support_t *eps_network_feature_support, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_eps_network_feature_support_t *source = (ogs_nas_eps_network_feature_support_t *)pkbuf->data;

    eps_network_feature_support->length = source->length;
    size = eps_network_feature_support->length + sizeof(eps_network_feature_support->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_network_feature_support, pkbuf->data - size, size);

    ogs_trace("  EPS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_eps_network_feature_support_t *eps_network_feature_support)
{
    uint16_t size = eps_network_feature_support->length + sizeof(eps_network_feature_support->length);
    ogs_nas_eps_network_feature_support_t target;

    memcpy(&target, eps_network_feature_support, sizeof(ogs_nas_eps_network_feature_support_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.13 EPS update result
 * M V 1/2 */
int ogs_nas_decode_eps_update_result(ogs_nas_eps_update_result_t *eps_update_result, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_eps_update_result_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_update_result, pkbuf->data - size, size);

    ogs_trace("  EPS_UPDATE_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_update_result(ogs_pkbuf_t *pkbuf, ogs_nas_eps_update_result_t *eps_update_result)
{
    uint16_t size = sizeof(ogs_nas_eps_update_result_t);
    ogs_nas_eps_update_result_t target;

    memcpy(&target, eps_update_result, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_UPDATE_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.14 EPS update type
 * M V 1/2 */
int ogs_nas_decode_eps_update_type(ogs_nas_eps_update_type_t *eps_update_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_eps_update_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_update_type, pkbuf->data - size, size);

    ogs_trace("  EPS_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_eps_update_type_t *eps_update_type)
{
    uint16_t size = sizeof(ogs_nas_eps_update_type_t);
    ogs_nas_eps_update_type_t target;

    memcpy(&target, eps_update_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.15 ESM message container
 * M LV-E 5-n */
int ogs_nas_decode_esm_message_container(ogs_nas_esm_message_container_t *esm_message_container, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_esm_message_container_t *source = (ogs_nas_esm_message_container_t *)pkbuf->data;

    esm_message_container->length = be16toh(source->length);
    size = esm_message_container->length + sizeof(esm_message_container->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    esm_message_container->buffer = pkbuf->data - size + sizeof(esm_message_container->length);

    ogs_trace("  ESM_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)esm_message_container->buffer, esm_message_container->length);

    return size;
}

int ogs_nas_encode_esm_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_esm_message_container_t *esm_message_container)
{
    uint16_t size = 0;
    uint16_t target;

    ogs_assert(esm_message_container);
    ogs_assert(esm_message_container->buffer);

    size = sizeof(esm_message_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(esm_message_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = esm_message_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, esm_message_container->buffer, size);

    ogs_trace("  ESM_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return esm_message_container->length + sizeof(esm_message_container->length);
}

/* 9.9.3.16 GPRS timer
 * M V 1 */
int ogs_nas_decode_gprs_timer(ogs_nas_gprs_timer_t *gprs_timer, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_gprs_timer_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(gprs_timer, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_gprs_timer(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_t *gprs_timer)
{
    uint16_t size = sizeof(ogs_nas_gprs_timer_t);
    ogs_nas_gprs_timer_t target;

    memcpy(&target, gprs_timer, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.16A GPRS timer 2
 * O TLV 3 */
int ogs_nas_decode_gprs_timer_2(ogs_nas_gprs_timer_2_t *gprs_timer_2, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_gprs_timer_2_t *source = (ogs_nas_gprs_timer_2_t *)pkbuf->data;

    gprs_timer_2->length = source->length;
    size = gprs_timer_2->length + sizeof(gprs_timer_2->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(gprs_timer_2, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_gprs_timer_2(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_2_t *gprs_timer_2)
{
    uint16_t size = gprs_timer_2->length + sizeof(gprs_timer_2->length);
    ogs_nas_gprs_timer_2_t target;

    memcpy(&target, gprs_timer_2, sizeof(ogs_nas_gprs_timer_2_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.16B GPRS timer 3
 * O TLV 3 */
int ogs_nas_decode_gprs_timer_3(ogs_nas_gprs_timer_3_t *gprs_timer_3, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_gprs_timer_3_t *source = (ogs_nas_gprs_timer_3_t *)pkbuf->data;

    gprs_timer_3->length = source->length;
    size = gprs_timer_3->length + sizeof(gprs_timer_3->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(gprs_timer_3, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_gprs_timer_3(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_3_t *gprs_timer_3)
{
    uint16_t size = gprs_timer_3->length + sizeof(gprs_timer_3->length);
    ogs_nas_gprs_timer_3_t target;

    memcpy(&target, gprs_timer_3, sizeof(ogs_nas_gprs_timer_3_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.17 Identity type 2
 * M V 1/2 */
int ogs_nas_decode_identity_type_2(ogs_nas_identity_type_2_t *identity_type_2, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_identity_type_2_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(identity_type_2, pkbuf->data - size, size);

    ogs_trace("  IDENTITY_TYPE_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_identity_type_2(ogs_pkbuf_t *pkbuf, ogs_nas_identity_type_2_t *identity_type_2)
{
    uint16_t size = sizeof(ogs_nas_identity_type_2_t);
    ogs_nas_identity_type_2_t target;

    memcpy(&target, identity_type_2, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  IDENTITY_TYPE_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.18 IMEISV request
 * O TV 1 */
int ogs_nas_decode_imeisv_request(ogs_nas_imeisv_request_t *imeisv_request, ogs_pkbuf_t *pkbuf)
{
    memcpy(imeisv_request, pkbuf->data - 1, 1);

    ogs_trace("  IMEISV_REQUEST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_imeisv_request(ogs_pkbuf_t *pkbuf, ogs_nas_imeisv_request_t *imeisv_request)
{
    uint16_t size = sizeof(ogs_nas_imeisv_request_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, imeisv_request, size);

    ogs_trace("  IMEISV_REQUEST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.19 KSI and sequence number
 * M V 1 */
int ogs_nas_decode_ksi_and_sequence_number(ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_ksi_and_sequence_number_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(ksi_and_sequence_number, pkbuf->data - size, size);

    ogs_trace("  KSI_AND_SEQUENCE_NUMBER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_ksi_and_sequence_number(ogs_pkbuf_t *pkbuf, ogs_nas_ksi_and_sequence_number_t *ksi_and_sequence_number)
{
    uint16_t size = sizeof(ogs_nas_ksi_and_sequence_number_t);
    ogs_nas_ksi_and_sequence_number_t target;

    memcpy(&target, ksi_and_sequence_number, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  KSI_AND_SEQUENCE_NUMBER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.2 Authentication parameter AUTN
 * M LV 17 */
int ogs_nas_decode_authentication_parameter_autn(ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_authentication_parameter_autn_t *source = (ogs_nas_authentication_parameter_autn_t *)pkbuf->data;

    authentication_parameter_autn->length = source->length;
    size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(authentication_parameter_autn, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_AUTN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_authentication_parameter_autn(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn)
{
    uint16_t size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);
    ogs_nas_authentication_parameter_autn_t target;

    memcpy(&target, authentication_parameter_autn, sizeof(ogs_nas_authentication_parameter_autn_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_AUTN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.20 MS network capability
 * O TLV 4-10 */
int ogs_nas_decode_ms_network_capability(ogs_nas_ms_network_capability_t *ms_network_capability, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_ms_network_capability_t *source = (ogs_nas_ms_network_capability_t *)pkbuf->data;

    ms_network_capability->length = source->length;
    size = ms_network_capability->length + sizeof(ms_network_capability->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(ms_network_capability, pkbuf->data - size, size);

    ogs_trace("  MS_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_ms_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ms_network_capability_t *ms_network_capability)
{
    uint16_t size = ms_network_capability->length + sizeof(ms_network_capability->length);
    ogs_nas_ms_network_capability_t target;

    memcpy(&target, ms_network_capability, sizeof(ogs_nas_ms_network_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MS_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.20A MS network feature support
 * O TV 1 */
int ogs_nas_decode_ms_network_feature_support(ogs_nas_ms_network_feature_support_t *ms_network_feature_support, ogs_pkbuf_t *pkbuf)
{
    memcpy(ms_network_feature_support, pkbuf->data - 1, 1);

    ogs_trace("  MS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_ms_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_ms_network_feature_support_t *ms_network_feature_support)
{
    uint16_t size = sizeof(ogs_nas_ms_network_feature_support_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ms_network_feature_support, size);

    ogs_trace("  MS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.21 key set identifier
 * O TV 1 */
int ogs_nas_decode_key_set_identifier(ogs_nas_key_set_identifier_t *key_set_identifier, ogs_pkbuf_t *pkbuf)
{
    memcpy(key_set_identifier, pkbuf->data - 1, 1);

    ogs_trace("  KEY_SET_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_key_set_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_key_set_identifier_t *key_set_identifier)
{
    uint16_t size = sizeof(ogs_nas_key_set_identifier_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, key_set_identifier, size);

    ogs_trace("  KEY_SET_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.22 message container
 * M LV 3-252 */
int ogs_nas_decode_message_container(ogs_nas_message_container_t *message_container, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_message_container_t *source = (ogs_nas_message_container_t *)pkbuf->data;

    message_container->length = source->length;
    size = message_container->length + sizeof(message_container->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(message_container, pkbuf->data - size, size);

    ogs_trace("  MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_message_container_t *message_container)
{
    uint16_t size = message_container->length + sizeof(message_container->length);
    ogs_nas_message_container_t target;

    memcpy(&target, message_container, sizeof(ogs_nas_message_container_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.23 security algorithms
 * M V 1 */
int ogs_nas_decode_security_algorithms(ogs_nas_security_algorithms_t *security_algorithms, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_security_algorithms_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(security_algorithms, pkbuf->data - size, size);

    ogs_trace("  SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_security_algorithms_t *security_algorithms)
{
    uint16_t size = sizeof(ogs_nas_security_algorithms_t);
    ogs_nas_security_algorithms_t target;

    memcpy(&target, security_algorithms, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.24 Network name
 * O TLV 3-n */
int ogs_nas_decode_network_name(ogs_nas_network_name_t *network_name, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_network_name_t *source = (ogs_nas_network_name_t *)pkbuf->data;

    network_name->length = source->length;
    size = network_name->length + sizeof(network_name->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(network_name, pkbuf->data - size, size);

    ogs_trace("  NETWORK_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_network_name(ogs_pkbuf_t *pkbuf, ogs_nas_network_name_t *network_name)
{
    uint16_t size = network_name->length + sizeof(network_name->length);
    ogs_nas_network_name_t target;

    memcpy(&target, network_name, sizeof(ogs_nas_network_name_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NETWORK_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.24A Network resource identifier container
 * O TLV 4 */
int ogs_nas_decode_network_resource_identifier_container(ogs_nas_network_resource_identifier_container_t *network_resource_identifier_container, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_network_resource_identifier_container_t *source = (ogs_nas_network_resource_identifier_container_t *)pkbuf->data;

    network_resource_identifier_container->length = source->length;
    size = network_resource_identifier_container->length + sizeof(network_resource_identifier_container->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(network_resource_identifier_container, pkbuf->data - size, size);

    ogs_trace("  NETWORK_RESOURCE_IDENTIFIER_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_network_resource_identifier_container(ogs_pkbuf_t *pkbuf, ogs_nas_network_resource_identifier_container_t *network_resource_identifier_container)
{
    uint16_t size = network_resource_identifier_container->length + sizeof(network_resource_identifier_container->length);
    ogs_nas_network_resource_identifier_container_t target;

    memcpy(&target, network_resource_identifier_container, sizeof(ogs_nas_network_resource_identifier_container_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NETWORK_RESOURCE_IDENTIFIER_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.25 Nonce
 * O TV 5 */
int ogs_nas_decode_nonce(ogs_nas_nonce_t *nonce, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_nonce_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(nonce, pkbuf->data - size, size);

    *nonce = be32toh(*nonce);

    ogs_trace("  NONCE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_nonce(ogs_pkbuf_t *pkbuf, ogs_nas_nonce_t *nonce)
{
    uint16_t size = sizeof(ogs_nas_nonce_t);
    ogs_nas_nonce_t target;

    memcpy(&target, nonce, size);
    target = htobe32(*nonce);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NONCE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.25A Paging identity
 * M V 1 */
int ogs_nas_decode_paging_identity(ogs_nas_paging_identity_t *paging_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_paging_identity_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(paging_identity, pkbuf->data - size, size);

    ogs_trace("  PAGING_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_paging_identity(ogs_pkbuf_t *pkbuf, ogs_nas_paging_identity_t *paging_identity)
{
    uint16_t size = sizeof(ogs_nas_paging_identity_t);
    ogs_nas_paging_identity_t target;

    memcpy(&target, paging_identity, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PAGING_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.26 P-TMSI signature
 * O TV 4 */
int ogs_nas_decode_p_tmsi_signature(ogs_nas_p_tmsi_signature_t *p_tmsi_signature, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 3;

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(p_tmsi_signature, pkbuf->data - size, size);

    *p_tmsi_signature = htobe32(*p_tmsi_signature);

    ogs_trace("  P_TMSI_SIGNATURE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_p_tmsi_signature(ogs_pkbuf_t *pkbuf, ogs_nas_p_tmsi_signature_t *p_tmsi_signature)
{
    uint16_t size = 3;
    ogs_nas_p_tmsi_signature_t target;

    memcpy(&target, p_tmsi_signature, size);
    *p_tmsi_signature = be32toh(*p_tmsi_signature);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  P_TMSI_SIGNATURE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.26A Extended EMM cause
 * O TV 1 */
int ogs_nas_decode_extended_emm_cause(ogs_nas_extended_emm_cause_t *extended_emm_cause, ogs_pkbuf_t *pkbuf)
{
    memcpy(extended_emm_cause, pkbuf->data - 1, 1);

    ogs_trace("  EXTENDED_EMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_extended_emm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_extended_emm_cause_t *extended_emm_cause)
{
    uint16_t size = sizeof(ogs_nas_extended_emm_cause_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, extended_emm_cause, size);

    ogs_trace("  EXTENDED_EMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.27 Service type
 * M V 1/2 */
int ogs_nas_decode_service_type(ogs_nas_service_type_t *service_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_service_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(service_type, pkbuf->data - size, size);

    ogs_trace("  SERVICE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_service_type(ogs_pkbuf_t *pkbuf, ogs_nas_service_type_t *service_type)
{
    uint16_t size = sizeof(ogs_nas_service_type_t);
    ogs_nas_service_type_t target;

    memcpy(&target, service_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SERVICE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.28 Short MAC
 * M V 2 */
int ogs_nas_decode_short_mac(ogs_nas_short_mac_t *short_mac, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_short_mac_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(short_mac, pkbuf->data - size, size);

    *short_mac = be16toh(*short_mac);

    ogs_trace("  SHORT_MAC - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_short_mac(ogs_pkbuf_t *pkbuf, ogs_nas_short_mac_t *short_mac)
{
    uint16_t size = sizeof(ogs_nas_short_mac_t);
    ogs_nas_short_mac_t target;

    memcpy(&target, short_mac, size);
    target = htobe16(*short_mac);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SHORT_MAC - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.29 Time zone
 * O TV 2 */
int ogs_nas_decode_time_zone(ogs_nas_time_zone_t *time_zone, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_time_zone_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(time_zone, pkbuf->data - size, size);

    ogs_trace("  TIME_ZONE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_time_zone(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_t *time_zone)
{
    uint16_t size = sizeof(ogs_nas_time_zone_t);
    ogs_nas_time_zone_t target;

    memcpy(&target, time_zone, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TIME_ZONE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.3 Authentication parameter RAND
 * M V 16 */
int ogs_nas_decode_authentication_parameter_rand(ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_authentication_parameter_rand_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(authentication_parameter_rand, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_RAND - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_authentication_parameter_rand(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand)
{
    uint16_t size = sizeof(ogs_nas_authentication_parameter_rand_t);
    ogs_nas_authentication_parameter_rand_t target;

    memcpy(&target, authentication_parameter_rand, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_RAND - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.30 Time zone and time
 * O TV 8 */
int ogs_nas_decode_time_zone_and_time(ogs_nas_time_zone_and_time_t *time_zone_and_time, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_time_zone_and_time_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(time_zone_and_time, pkbuf->data - size, size);

    ogs_trace("  TIME_ZONE_AND_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_time_zone_and_time(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_and_time_t *time_zone_and_time)
{
    uint16_t size = sizeof(ogs_nas_time_zone_and_time_t);
    ogs_nas_time_zone_and_time_t target;

    memcpy(&target, time_zone_and_time, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TIME_ZONE_AND_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.31 TMSI status
 * O TV 1 */
int ogs_nas_decode_tmsi_status(ogs_nas_tmsi_status_t *tmsi_status, ogs_pkbuf_t *pkbuf)
{
    memcpy(tmsi_status, pkbuf->data - 1, 1);

    ogs_trace("  TMSI_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_tmsi_status(ogs_pkbuf_t *pkbuf, ogs_nas_tmsi_status_t *tmsi_status)
{
    uint16_t size = sizeof(ogs_nas_tmsi_status_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, tmsi_status, size);

    ogs_trace("  TMSI_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.32 Tracking area identity
 * O TV 6 */
int ogs_nas_decode_tracking_area_identity(ogs_nas_tracking_area_identity_t *tracking_area_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_tracking_area_identity_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(tracking_area_identity, pkbuf->data - size, size);

    tracking_area_identity->tac = be16toh(tracking_area_identity->tac);

    ogs_trace("  TRACKING_AREA_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_tracking_area_identity(ogs_pkbuf_t *pkbuf, ogs_nas_tracking_area_identity_t *tracking_area_identity)
{
    uint16_t size = sizeof(ogs_nas_tracking_area_identity_t);
    ogs_nas_tracking_area_identity_t target;

    memcpy(&target, tracking_area_identity, size);
    target.tac = htobe16(tracking_area_identity->tac);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRACKING_AREA_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.33 Tracking area identity list
 * M LV 7-97 */
int ogs_nas_decode_tracking_area_identity_list(ogs_nas_tracking_area_identity_list_t *tracking_area_identity_list, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_tracking_area_identity_list_t *source = (ogs_nas_tracking_area_identity_list_t *)pkbuf->data;

    tracking_area_identity_list->length = source->length;
    size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(tracking_area_identity_list, pkbuf->data - size, size);

    ogs_trace("  TRACKING_AREA_IDENTITY_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_tracking_area_identity_list(ogs_pkbuf_t *pkbuf, ogs_nas_tracking_area_identity_list_t *tracking_area_identity_list)
{
    uint16_t size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);
    ogs_nas_tracking_area_identity_list_t target;

    memcpy(&target, tracking_area_identity_list, sizeof(ogs_nas_tracking_area_identity_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRACKING_AREA_IDENTITY_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.34 UE network capability
 * M LV 3-14 */
int ogs_nas_decode_ue_network_capability(ogs_nas_ue_network_capability_t *ue_network_capability, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_ue_network_capability_t *source = (ogs_nas_ue_network_capability_t *)pkbuf->data;

    ue_network_capability->length = source->length;
    size = ue_network_capability->length + sizeof(ue_network_capability->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(ue_network_capability, pkbuf->data - size, size);

    ogs_trace("  UE_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_ue_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_network_capability_t *ue_network_capability)
{
    uint16_t size = ue_network_capability->length + sizeof(ue_network_capability->length);
    ogs_nas_ue_network_capability_t target;

    memcpy(&target, ue_network_capability, sizeof(ogs_nas_ue_network_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.35 UE radio capability information update needed
 * O TV 1 */
int ogs_nas_decode_ue_radio_capability_information_update_needed(ogs_nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed, ogs_pkbuf_t *pkbuf)
{
    memcpy(ue_radio_capability_information_update_needed, pkbuf->data - 1, 1);

    ogs_trace("  UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_ue_radio_capability_information_update_needed(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_information_update_needed_t *ue_radio_capability_information_update_needed)
{
    uint16_t size = sizeof(ogs_nas_ue_radio_capability_information_update_needed_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ue_radio_capability_information_update_needed, size);

    ogs_trace("  UE_RADIO_CAPABILITY_INFORMATION_UPDATE_NEEDED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.36 UE security capability
 * M LV 3-6 */
int ogs_nas_decode_ue_security_capability(ogs_nas_ue_security_capability_t *ue_security_capability, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_ue_security_capability_t *source = (ogs_nas_ue_security_capability_t *)pkbuf->data;

    ue_security_capability->length = source->length;
    size = ue_security_capability->length + sizeof(ue_security_capability->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(ue_security_capability, pkbuf->data - size, size);

    ogs_trace("  UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_security_capability_t *ue_security_capability)
{
    uint16_t size = ue_security_capability->length + sizeof(ue_security_capability->length);
    ogs_nas_ue_security_capability_t target;

    memcpy(&target, ue_security_capability, sizeof(ogs_nas_ue_security_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.37 Emergency number list
 * O TLV 5-50 */
int ogs_nas_decode_emergency_number_list(ogs_nas_emergency_number_list_t *emergency_number_list, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_emergency_number_list_t *source = (ogs_nas_emergency_number_list_t *)pkbuf->data;

    emergency_number_list->length = source->length;
    size = emergency_number_list->length + sizeof(emergency_number_list->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(emergency_number_list, pkbuf->data - size, size);

    ogs_trace("  EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_emergency_number_list_t *emergency_number_list)
{
    uint16_t size = emergency_number_list->length + sizeof(emergency_number_list->length);
    ogs_nas_emergency_number_list_t target;

    memcpy(&target, emergency_number_list, sizeof(ogs_nas_emergency_number_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.38 CLI
 * O TLV 3-14 */
int ogs_nas_decode_cli(ogs_nas_cli_t *cli, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_cli_t *source = (ogs_nas_cli_t *)pkbuf->data;

    cli->length = source->length;
    size = cli->length + sizeof(cli->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(cli, pkbuf->data - size, size);

    ogs_trace("  CLI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_cli(ogs_pkbuf_t *pkbuf, ogs_nas_cli_t *cli)
{
    uint16_t size = cli->length + sizeof(cli->length);
    ogs_nas_cli_t target;

    memcpy(&target, cli, sizeof(ogs_nas_cli_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  CLI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.39 SS Code
 * O TV 2 */
int ogs_nas_decode_ss_code(ogs_nas_ss_code_t *ss_code, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_ss_code_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(ss_code, pkbuf->data - size, size);

    ogs_trace("  SS_CODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_ss_code(ogs_pkbuf_t *pkbuf, ogs_nas_ss_code_t *ss_code)
{
    uint16_t size = sizeof(ogs_nas_ss_code_t);
    ogs_nas_ss_code_t target;

    memcpy(&target, ss_code, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SS_CODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.4 Authentication response parameter
 * M LV 5-17 */
int ogs_nas_decode_authentication_response_parameter(ogs_nas_authentication_response_parameter_t *authentication_response_parameter, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_authentication_response_parameter_t *source = (ogs_nas_authentication_response_parameter_t *)pkbuf->data;

    authentication_response_parameter->length = source->length;
    size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(authentication_response_parameter, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_RESPONSE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_authentication_response_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_response_parameter_t *authentication_response_parameter)
{
    uint16_t size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);
    ogs_nas_authentication_response_parameter_t target;

    memcpy(&target, authentication_response_parameter, sizeof(ogs_nas_authentication_response_parameter_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_RESPONSE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.40 LCS indicator
 * O TV 2 */
int ogs_nas_decode_lcs_indicator(ogs_nas_lcs_indicator_t *lcs_indicator, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_lcs_indicator_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(lcs_indicator, pkbuf->data - size, size);

    ogs_trace("  LCS_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_lcs_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_lcs_indicator_t *lcs_indicator)
{
    uint16_t size = sizeof(ogs_nas_lcs_indicator_t);
    ogs_nas_lcs_indicator_t target;

    memcpy(&target, lcs_indicator, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LCS_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.41 LCS client identity
 * O TLV 3-257 */
int ogs_nas_decode_lcs_client_identity(ogs_nas_lcs_client_identity_t *lcs_client_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_lcs_client_identity_t *source = (ogs_nas_lcs_client_identity_t *)pkbuf->data;

    lcs_client_identity->length = source->length;
    size = lcs_client_identity->length + sizeof(lcs_client_identity->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(lcs_client_identity, pkbuf->data - size, size);

    ogs_trace("  LCS_CLIENT_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_lcs_client_identity(ogs_pkbuf_t *pkbuf, ogs_nas_lcs_client_identity_t *lcs_client_identity)
{
    uint16_t size = lcs_client_identity->length + sizeof(lcs_client_identity->length);
    ogs_nas_lcs_client_identity_t target;

    memcpy(&target, lcs_client_identity, sizeof(ogs_nas_lcs_client_identity_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LCS_CLIENT_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.42 Generic message container type
 * M V 1 */
int ogs_nas_decode_generic_message_container_type(ogs_nas_generic_message_container_type_t *generic_message_container_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_generic_message_container_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(generic_message_container_type, pkbuf->data - size, size);

    ogs_trace("  GENERIC_MESSAGE_CONTAINER_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_generic_message_container_type(ogs_pkbuf_t *pkbuf, ogs_nas_generic_message_container_type_t *generic_message_container_type)
{
    uint16_t size = sizeof(ogs_nas_generic_message_container_type_t);
    ogs_nas_generic_message_container_type_t target;

    memcpy(&target, generic_message_container_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GENERIC_MESSAGE_CONTAINER_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.43 Generic message container
 * M LV-E 3-n */
int ogs_nas_decode_generic_message_container(ogs_nas_generic_message_container_t *generic_message_container, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_generic_message_container_t *source = (ogs_nas_generic_message_container_t *)pkbuf->data;

    generic_message_container->length = be16toh(source->length);
    size = generic_message_container->length + sizeof(generic_message_container->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    generic_message_container->buffer = pkbuf->data - size + sizeof(generic_message_container->length);

    ogs_trace("  GENERIC_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)generic_message_container->buffer, generic_message_container->length);

    return size;
}

int ogs_nas_encode_generic_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_generic_message_container_t *generic_message_container)
{
    uint16_t size = 0;
    uint16_t target;

    ogs_assert(generic_message_container);
    ogs_assert(generic_message_container->buffer);

    size = sizeof(generic_message_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(generic_message_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = generic_message_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, generic_message_container->buffer, size);

    ogs_trace("  GENERIC_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return generic_message_container->length + sizeof(generic_message_container->length);
}

/* 9.9.3.44 Voice domain preference and UE usage setting
 * O TLV 3 */
int ogs_nas_decode_voice_domain_preference_and_ue_usage_setting(ogs_nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t *source = (ogs_nas_voice_domain_preference_and_ue_usage_setting_t *)pkbuf->data;

    voice_domain_preference_and_ue_usage_setting->length = source->length;
    size = voice_domain_preference_and_ue_usage_setting->length + sizeof(voice_domain_preference_and_ue_usage_setting->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(voice_domain_preference_and_ue_usage_setting, pkbuf->data - size, size);

    ogs_trace("  VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_voice_domain_preference_and_ue_usage_setting(ogs_pkbuf_t *pkbuf, ogs_nas_voice_domain_preference_and_ue_usage_setting_t *voice_domain_preference_and_ue_usage_setting)
{
    uint16_t size = voice_domain_preference_and_ue_usage_setting->length + sizeof(voice_domain_preference_and_ue_usage_setting->length);
    ogs_nas_voice_domain_preference_and_ue_usage_setting_t target;

    memcpy(&target, voice_domain_preference_and_ue_usage_setting, sizeof(ogs_nas_voice_domain_preference_and_ue_usage_setting_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  VOICE_DOMAIN_PREFERENCE_AND_UE_USAGE_SETTING - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.45 GUTI type
 * O TV 1 */
int ogs_nas_decode_guti_type(ogs_nas_guti_type_t *guti_type, ogs_pkbuf_t *pkbuf)
{
    memcpy(guti_type, pkbuf->data - 1, 1);

    ogs_trace("  GUTI_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_guti_type(ogs_pkbuf_t *pkbuf, ogs_nas_guti_type_t *guti_type)
{
    uint16_t size = sizeof(ogs_nas_guti_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, guti_type, size);

    ogs_trace("  GUTI_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.46 Extended DRX parameters
 * O TLV 3 */
int ogs_nas_decode_extended_drx_parameters(ogs_nas_extended_drx_parameters_t *extended_drx_parameters, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_extended_drx_parameters_t *source = (ogs_nas_extended_drx_parameters_t *)pkbuf->data;

    extended_drx_parameters->length = source->length;
    size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(extended_drx_parameters, pkbuf->data - size, size);

    ogs_trace("  EXTENDED_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_extended_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_extended_drx_parameters_t *extended_drx_parameters)
{
    uint16_t size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);
    ogs_nas_extended_drx_parameters_t target;

    memcpy(&target, extended_drx_parameters, sizeof(ogs_nas_extended_drx_parameters_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EXTENDED_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.4a Ciphering key sequence number
 * O TV 1 */
int ogs_nas_decode_ciphering_key_sequence_number(ogs_nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number, ogs_pkbuf_t *pkbuf)
{
    memcpy(ciphering_key_sequence_number, pkbuf->data - 1, 1);

    ogs_trace("  CIPHERING_KEY_SEQUENCE_NUMBER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_ciphering_key_sequence_number(ogs_pkbuf_t *pkbuf, ogs_nas_ciphering_key_sequence_number_t *ciphering_key_sequence_number)
{
    uint16_t size = sizeof(ogs_nas_ciphering_key_sequence_number_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ciphering_key_sequence_number, size);

    ogs_trace("  CIPHERING_KEY_SEQUENCE_NUMBER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.5 CSFB response
 * O TV 1 */
int ogs_nas_decode_csfb_response(ogs_nas_csfb_response_t *csfb_response, ogs_pkbuf_t *pkbuf)
{
    memcpy(csfb_response, pkbuf->data - 1, 1);

    ogs_trace("  CSFB_RESPONSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_csfb_response(ogs_pkbuf_t *pkbuf, ogs_nas_csfb_response_t *csfb_response)
{
    uint16_t size = sizeof(ogs_nas_csfb_response_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, csfb_response, size);

    ogs_trace("  CSFB_RESPONSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.6 Daylight saving time
 * O TLV 3 */
int ogs_nas_decode_daylight_saving_time(ogs_nas_daylight_saving_time_t *daylight_saving_time, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_daylight_saving_time_t *source = (ogs_nas_daylight_saving_time_t *)pkbuf->data;

    daylight_saving_time->length = source->length;
    size = daylight_saving_time->length + sizeof(daylight_saving_time->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(daylight_saving_time, pkbuf->data - size, size);

    ogs_trace("  DAYLIGHT_SAVING_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_daylight_saving_time(ogs_pkbuf_t *pkbuf, ogs_nas_daylight_saving_time_t *daylight_saving_time)
{
    uint16_t size = daylight_saving_time->length + sizeof(daylight_saving_time->length);
    ogs_nas_daylight_saving_time_t target;

    memcpy(&target, daylight_saving_time, sizeof(ogs_nas_daylight_saving_time_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DAYLIGHT_SAVING_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.7 Detach type
 * M V 1/2 */
int ogs_nas_decode_detach_type(ogs_nas_detach_type_t *detach_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_detach_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(detach_type, pkbuf->data - size, size);

    ogs_trace("  DETACH_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_detach_type(ogs_pkbuf_t *pkbuf, ogs_nas_detach_type_t *detach_type)
{
    uint16_t size = sizeof(ogs_nas_detach_type_t);
    ogs_nas_detach_type_t target;

    memcpy(&target, detach_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DETACH_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.8 DRX parameter
 * O TV 3 */
int ogs_nas_decode_drx_parameter(ogs_nas_drx_parameter_t *drx_parameter, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_drx_parameter_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(drx_parameter, pkbuf->data - size, size);

    ogs_trace("  DRX_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_drx_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_drx_parameter_t *drx_parameter)
{
    uint16_t size = sizeof(ogs_nas_drx_parameter_t);
    ogs_nas_drx_parameter_t target;

    memcpy(&target, drx_parameter, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DRX_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.3.9 EMM cause
 * O TV 2 */
int ogs_nas_decode_emm_cause(ogs_nas_emm_cause_t *emm_cause, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_emm_cause_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(emm_cause, pkbuf->data - size, size);

    ogs_trace("  EMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_emm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_emm_cause_t *emm_cause)
{
    uint16_t size = sizeof(ogs_nas_emm_cause_t);
    ogs_nas_emm_cause_t target;

    memcpy(&target, emm_cause, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.1 Access point name
 * M LV 2-101 */
int ogs_nas_decode_access_point_name(ogs_nas_access_point_name_t *access_point_name, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_access_point_name_t *source = (ogs_nas_access_point_name_t *)pkbuf->data;

    access_point_name->length = source->length;
    size = access_point_name->length + sizeof(access_point_name->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(access_point_name, pkbuf->data - size, size);

    {
        char apn[OGS_MAX_APN_LEN];
        access_point_name->length  = ogs_fqdn_parse(apn, access_point_name->apn, access_point_name->length);
        ogs_cpystrn(access_point_name->apn, apn, ogs_min(access_point_name->length, OGS_MAX_APN_LEN) + 1);
    }

    ogs_trace("  ACCESS_POINT_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_access_point_name(ogs_pkbuf_t *pkbuf, ogs_nas_access_point_name_t *access_point_name)
{
    uint16_t size = access_point_name->length + sizeof(access_point_name->length);
    ogs_nas_access_point_name_t target;

    memcpy(&target, access_point_name, sizeof(ogs_nas_access_point_name_t));
    target.length = ogs_fqdn_build(target.apn, access_point_name->apn, access_point_name->length);
    size = target.length + sizeof(target.length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ACCESS_POINT_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.11 Protocol configuration options
 * O TLV 3-253 */
int ogs_nas_decode_protocol_configuration_options(ogs_nas_protocol_configuration_options_t *protocol_configuration_options, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_protocol_configuration_options_t *source = (ogs_nas_protocol_configuration_options_t *)pkbuf->data;

    protocol_configuration_options->length = source->length;
    size = protocol_configuration_options->length + sizeof(protocol_configuration_options->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(protocol_configuration_options, pkbuf->data - size, size);

    ogs_trace("  PROTOCOL_CONFIGURATION_OPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_protocol_configuration_options_t *protocol_configuration_options)
{
    uint16_t size = protocol_configuration_options->length + sizeof(protocol_configuration_options->length);
    ogs_nas_protocol_configuration_options_t target;

    memcpy(&target, protocol_configuration_options, sizeof(ogs_nas_protocol_configuration_options_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PROTOCOL_CONFIGURATION_OPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.12 Quality of service
 * O TLV 14-22 */
int ogs_nas_decode_quality_of_service(ogs_nas_quality_of_service_t *quality_of_service, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_quality_of_service_t *source = (ogs_nas_quality_of_service_t *)pkbuf->data;

    quality_of_service->length = source->length;
    size = quality_of_service->length + sizeof(quality_of_service->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(quality_of_service, pkbuf->data - size, size);

    ogs_trace("  QUALITY_OF_SERVICE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_quality_of_service(ogs_pkbuf_t *pkbuf, ogs_nas_quality_of_service_t *quality_of_service)
{
    uint16_t size = quality_of_service->length + sizeof(quality_of_service->length);
    ogs_nas_quality_of_service_t target;

    memcpy(&target, quality_of_service, sizeof(ogs_nas_quality_of_service_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  QUALITY_OF_SERVICE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.13 Radio priority
 * O TV 1 */
int ogs_nas_decode_radio_priority(ogs_nas_radio_priority_t *radio_priority, ogs_pkbuf_t *pkbuf)
{
    memcpy(radio_priority, pkbuf->data - 1, 1);

    ogs_trace("  RADIO_PRIORITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_radio_priority(ogs_pkbuf_t *pkbuf, ogs_nas_radio_priority_t *radio_priority)
{
    uint16_t size = sizeof(ogs_nas_radio_priority_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, radio_priority, size);

    ogs_trace("  RADIO_PRIORITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.13A Re-attempt indicator
 * O TLV 3 */
int ogs_nas_decode_re_attempt_indicator(ogs_nas_re_attempt_indicator_t *re_attempt_indicator, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_re_attempt_indicator_t *source = (ogs_nas_re_attempt_indicator_t *)pkbuf->data;

    re_attempt_indicator->length = source->length;
    size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(re_attempt_indicator, pkbuf->data - size, size);

    ogs_trace("  RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_re_attempt_indicator_t *re_attempt_indicator)
{
    uint16_t size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);
    ogs_nas_re_attempt_indicator_t target;

    memcpy(&target, re_attempt_indicator, sizeof(ogs_nas_re_attempt_indicator_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.14 Request type
 * M V 1/2 */
int ogs_nas_decode_request_type(ogs_nas_request_type_t *request_type, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_request_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(request_type, pkbuf->data - size, size);

    ogs_trace("  REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_request_type(ogs_pkbuf_t *pkbuf, ogs_nas_request_type_t *request_type)
{
    uint16_t size = sizeof(ogs_nas_request_type_t);
    ogs_nas_request_type_t target;

    memcpy(&target, request_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.15 Traffic flow aggregate description
 * M LV 2-256 */
int ogs_nas_decode_traffic_flow_aggregate_description(ogs_nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_traffic_flow_aggregate_description_t *source = (ogs_nas_traffic_flow_aggregate_description_t *)pkbuf->data;

    traffic_flow_aggregate_description->length = source->length;
    size = traffic_flow_aggregate_description->length + sizeof(traffic_flow_aggregate_description->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(traffic_flow_aggregate_description, pkbuf->data - size, size);

    ogs_trace("  TRAFFIC_FLOW_AGGREGATE_DESCRIPTION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_traffic_flow_aggregate_description(ogs_pkbuf_t *pkbuf, ogs_nas_traffic_flow_aggregate_description_t *traffic_flow_aggregate_description)
{
    uint16_t size = traffic_flow_aggregate_description->length + sizeof(traffic_flow_aggregate_description->length);
    ogs_nas_traffic_flow_aggregate_description_t target;

    memcpy(&target, traffic_flow_aggregate_description, sizeof(ogs_nas_traffic_flow_aggregate_description_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRAFFIC_FLOW_AGGREGATE_DESCRIPTION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.16 Traffic flow template
 * M LV 2-256 */
int ogs_nas_decode_traffic_flow_template(ogs_nas_traffic_flow_template_t *traffic_flow_template, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_traffic_flow_template_t *source = (ogs_nas_traffic_flow_template_t *)pkbuf->data;

    traffic_flow_template->length = source->length;
    size = traffic_flow_template->length + sizeof(traffic_flow_template->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(traffic_flow_template, pkbuf->data - size, size);

    ogs_trace("  TRAFFIC_FLOW_TEMPLATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_traffic_flow_template(ogs_pkbuf_t *pkbuf, ogs_nas_traffic_flow_template_t *traffic_flow_template)
{
    uint16_t size = traffic_flow_template->length + sizeof(traffic_flow_template->length);
    ogs_nas_traffic_flow_template_t target;

    memcpy(&target, traffic_flow_template, sizeof(ogs_nas_traffic_flow_template_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRAFFIC_FLOW_TEMPLATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.17 Transaction identifier
 * O TLV 3-4 */
int ogs_nas_decode_transaction_identifier(ogs_nas_transaction_identifier_t *transaction_identifier, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_transaction_identifier_t *source = (ogs_nas_transaction_identifier_t *)pkbuf->data;

    transaction_identifier->length = source->length;
    size = transaction_identifier->length + sizeof(transaction_identifier->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(transaction_identifier, pkbuf->data - size, size);

    ogs_trace("  TRANSACTION_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_transaction_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_transaction_identifier_t *transaction_identifier)
{
    uint16_t size = transaction_identifier->length + sizeof(transaction_identifier->length);
    ogs_nas_transaction_identifier_t target;

    memcpy(&target, transaction_identifier, sizeof(ogs_nas_transaction_identifier_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRANSACTION_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.18 WLAN offload acceptability
 * O TV 1 */
int ogs_nas_decode_wlan_offload_acceptability(ogs_nas_wlan_offload_acceptability_t *wlan_offload_acceptability, ogs_pkbuf_t *pkbuf)
{
    memcpy(wlan_offload_acceptability, pkbuf->data - 1, 1);

    ogs_trace("  WLAN_OFFLOAD_ACCEPTABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_wlan_offload_acceptability(ogs_pkbuf_t *pkbuf, ogs_nas_wlan_offload_acceptability_t *wlan_offload_acceptability)
{
    uint16_t size = sizeof(ogs_nas_wlan_offload_acceptability_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, wlan_offload_acceptability, size);

    ogs_trace("  WLAN_OFFLOAD_ACCEPTABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.19 NBIFOM container
 * O TLV 3-257 */
int ogs_nas_decode_nbifom_container(ogs_nas_nbifom_container_t *nbifom_container, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_nbifom_container_t *source = (ogs_nas_nbifom_container_t *)pkbuf->data;

    nbifom_container->length = source->length;
    size = nbifom_container->length + sizeof(nbifom_container->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(nbifom_container, pkbuf->data - size, size);

    ogs_trace("  NBIFOM_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_nbifom_container(ogs_pkbuf_t *pkbuf, ogs_nas_nbifom_container_t *nbifom_container)
{
    uint16_t size = nbifom_container->length + sizeof(nbifom_container->length);
    ogs_nas_nbifom_container_t target;

    memcpy(&target, nbifom_container, sizeof(ogs_nas_nbifom_container_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NBIFOM_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.2 APN aggregate maximum bit rate
 * O TLV 4-8 */
int ogs_nas_decode_apn_aggregate_maximum_bit_rate(ogs_nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_apn_aggregate_maximum_bit_rate_t *source = (ogs_nas_apn_aggregate_maximum_bit_rate_t *)pkbuf->data;

    apn_aggregate_maximum_bit_rate->length = source->length;
    size = apn_aggregate_maximum_bit_rate->length + sizeof(apn_aggregate_maximum_bit_rate->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(apn_aggregate_maximum_bit_rate, pkbuf->data - size, size);

    ogs_trace("  APN_AGGREGATE_MAXIMUM_BIT_RATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_apn_aggregate_maximum_bit_rate(ogs_pkbuf_t *pkbuf, ogs_nas_apn_aggregate_maximum_bit_rate_t *apn_aggregate_maximum_bit_rate)
{
    uint16_t size = apn_aggregate_maximum_bit_rate->length + sizeof(apn_aggregate_maximum_bit_rate->length);
    ogs_nas_apn_aggregate_maximum_bit_rate_t target;

    memcpy(&target, apn_aggregate_maximum_bit_rate, sizeof(ogs_nas_apn_aggregate_maximum_bit_rate_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  APN_AGGREGATE_MAXIMUM_BIT_RATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.22 Header compression configuration
 * O TLV 5-257 */
int ogs_nas_decode_header_compression_configuration(ogs_nas_header_compression_configuration_t *header_compression_configuration, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_header_compression_configuration_t *source = (ogs_nas_header_compression_configuration_t *)pkbuf->data;

    header_compression_configuration->length = source->length;
    size = header_compression_configuration->length + sizeof(header_compression_configuration->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(header_compression_configuration, pkbuf->data - size, size);

    header_compression_configuration->max_cid = be16toh(header_compression_configuration->max_cid);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_t *header_compression_configuration)
{
    uint16_t size = header_compression_configuration->length + sizeof(header_compression_configuration->length);
    ogs_nas_header_compression_configuration_t target;

    memcpy(&target, header_compression_configuration, sizeof(ogs_nas_header_compression_configuration_t));
    target.max_cid = htobe16(header_compression_configuration->max_cid);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.23 Control plane only indication
 * O TV 1 */
int ogs_nas_decode_control_plane_only_indication(ogs_nas_control_plane_only_indication_t *control_plane_only_indication, ogs_pkbuf_t *pkbuf)
{
    memcpy(control_plane_only_indication, pkbuf->data - 1, 1);

    ogs_trace("  CONTROL_PLANE_ONLY_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_control_plane_only_indication(ogs_pkbuf_t *pkbuf, ogs_nas_control_plane_only_indication_t *control_plane_only_indication)
{
    uint16_t size = sizeof(ogs_nas_control_plane_only_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, control_plane_only_indication, size);

    ogs_trace("  CONTROL_PLANE_ONLY_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.26 Extended protocol configuration options
 * O TLV-E 4-65538 */
int ogs_nas_decode_extended_protocol_configuration_options(ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_extended_protocol_configuration_options_t *source = (ogs_nas_extended_protocol_configuration_options_t *)pkbuf->data;

    extended_protocol_configuration_options->length = be16toh(source->length);
    size = extended_protocol_configuration_options->length + sizeof(extended_protocol_configuration_options->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    extended_protocol_configuration_options->buffer = pkbuf->data - size + sizeof(extended_protocol_configuration_options->length);

    ogs_trace("  EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)extended_protocol_configuration_options->buffer, extended_protocol_configuration_options->length);

    return size;
}

int ogs_nas_encode_extended_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options)
{
    uint16_t size = 0;
    uint16_t target;

    ogs_assert(extended_protocol_configuration_options);
    ogs_assert(extended_protocol_configuration_options->buffer);

    size = sizeof(extended_protocol_configuration_options->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(extended_protocol_configuration_options->length);
    memcpy(pkbuf->data - size, &target, size);

    size = extended_protocol_configuration_options->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, extended_protocol_configuration_options->buffer, size);

    ogs_trace("  EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return extended_protocol_configuration_options->length + sizeof(extended_protocol_configuration_options->length);
}

/* 9.9.4.27 Header compression configuration status
 * O TLV 4 */
int ogs_nas_decode_header_compression_configuration_status(ogs_nas_header_compression_configuration_status_t *header_compression_configuration_status, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_header_compression_configuration_status_t *source = (ogs_nas_header_compression_configuration_status_t *)pkbuf->data;

    header_compression_configuration_status->length = source->length;
    size = header_compression_configuration_status->length + sizeof(header_compression_configuration_status->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(header_compression_configuration_status, pkbuf->data - size, size);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_header_compression_configuration_status(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_status_t *header_compression_configuration_status)
{
    uint16_t size = header_compression_configuration_status->length + sizeof(header_compression_configuration_status->length);
    ogs_nas_header_compression_configuration_status_t target;

    memcpy(&target, header_compression_configuration_status, sizeof(ogs_nas_header_compression_configuration_status_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.28 Serving PLMN rate control
 * O TLV 4 */
int ogs_nas_decode_serving_plmn_rate_control(ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_serving_plmn_rate_control_t *source = (ogs_nas_serving_plmn_rate_control_t *)pkbuf->data;

    serving_plmn_rate_control->length = source->length;
    size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(serving_plmn_rate_control, pkbuf->data - size, size);

    ogs_trace("  SERVING_PLMN_RATE_CONTROL - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_serving_plmn_rate_control(ogs_pkbuf_t *pkbuf, ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control)
{
    uint16_t size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);
    ogs_nas_serving_plmn_rate_control_t target;

    memcpy(&target, serving_plmn_rate_control, sizeof(ogs_nas_serving_plmn_rate_control_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SERVING_PLMN_RATE_CONTROL - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.2A Connectivity type
 * O TV 1 */
int ogs_nas_decode_connectivity_type(ogs_nas_connectivity_type_t *connectivity_type, ogs_pkbuf_t *pkbuf)
{
    memcpy(connectivity_type, pkbuf->data - 1, 1);

    ogs_trace("  CONNECTIVITY_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_connectivity_type(ogs_pkbuf_t *pkbuf, ogs_nas_connectivity_type_t *connectivity_type)
{
    uint16_t size = sizeof(ogs_nas_connectivity_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, connectivity_type, size);

    ogs_trace("  CONNECTIVITY_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.3 EPS quality of service
 * M LV 2-14 */
int ogs_nas_decode_eps_quality_of_service(ogs_nas_eps_quality_of_service_t *eps_quality_of_service, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_eps_quality_of_service_t *source = (ogs_nas_eps_quality_of_service_t *)pkbuf->data;

    eps_quality_of_service->length = source->length;
    size = eps_quality_of_service->length + sizeof(eps_quality_of_service->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(eps_quality_of_service, pkbuf->data - size, size);

    ogs_trace("  EPS_QUALITY_OF_SERVICE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_eps_quality_of_service(ogs_pkbuf_t *pkbuf, ogs_nas_eps_quality_of_service_t *eps_quality_of_service)
{
    uint16_t size = eps_quality_of_service->length + sizeof(eps_quality_of_service->length);
    ogs_nas_eps_quality_of_service_t target;

    memcpy(&target, eps_quality_of_service, sizeof(ogs_nas_eps_quality_of_service_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_QUALITY_OF_SERVICE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.4 ESM cause
 * O TV 2 */
int ogs_nas_decode_esm_cause(ogs_nas_esm_cause_t *esm_cause, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_esm_cause_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(esm_cause, pkbuf->data - size, size);

    ogs_trace("  ESM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_esm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_esm_cause_t *esm_cause)
{
    uint16_t size = sizeof(ogs_nas_esm_cause_t);
    ogs_nas_esm_cause_t target;

    memcpy(&target, esm_cause, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ESM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.5 ESM information transfer flag
 * O TV 1 */
int ogs_nas_decode_esm_information_transfer_flag(ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag, ogs_pkbuf_t *pkbuf)
{
    memcpy(esm_information_transfer_flag, pkbuf->data - 1, 1);

    ogs_trace("  ESM_INFORMATION_TRANSFER_FLAG - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - 1, 1);

    return 0;
}

int ogs_nas_encode_esm_information_transfer_flag(ogs_pkbuf_t *pkbuf, ogs_nas_esm_information_transfer_flag_t *esm_information_transfer_flag)
{
    uint16_t size = sizeof(ogs_nas_esm_information_transfer_flag_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, esm_information_transfer_flag, size);

    ogs_trace("  ESM_INFORMATION_TRANSFER_FLAG - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.6 Linked EPS bearer identity
 * M V 1/2 */
int ogs_nas_decode_linked_eps_bearer_identity(ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_linked_eps_bearer_identity_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(linked_eps_bearer_identity, pkbuf->data - size, size);

    ogs_trace("  LINKED_EPS_BEARER_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_linked_eps_bearer_identity(ogs_pkbuf_t *pkbuf, ogs_nas_linked_eps_bearer_identity_t *linked_eps_bearer_identity)
{
    uint16_t size = sizeof(ogs_nas_linked_eps_bearer_identity_t);
    ogs_nas_linked_eps_bearer_identity_t target;

    memcpy(&target, linked_eps_bearer_identity, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LINKED_EPS_BEARER_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.7 LLC service access point identifier
 * O TV 2 */
int ogs_nas_decode_llc_service_access_point_identifier(ogs_nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = sizeof(ogs_nas_llc_service_access_point_identifier_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(llc_service_access_point_identifier, pkbuf->data - size, size);

    ogs_trace("  LLC_SERVICE_ACCESS_POINT_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_llc_service_access_point_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_llc_service_access_point_identifier_t *llc_service_access_point_identifier)
{
    uint16_t size = sizeof(ogs_nas_llc_service_access_point_identifier_t);
    ogs_nas_llc_service_access_point_identifier_t target;

    memcpy(&target, llc_service_access_point_identifier, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LLC_SERVICE_ACCESS_POINT_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.8 Packet flow Identifier
 * O TLV 3 */
int ogs_nas_decode_packet_flow_identifier(ogs_nas_packet_flow_identifier_t *packet_flow_identifier, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_packet_flow_identifier_t *source = (ogs_nas_packet_flow_identifier_t *)pkbuf->data;

    packet_flow_identifier->length = source->length;
    size = packet_flow_identifier->length + sizeof(packet_flow_identifier->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(packet_flow_identifier, pkbuf->data - size, size);

    ogs_trace("  PACKET_FLOW_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_packet_flow_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_packet_flow_identifier_t *packet_flow_identifier)
{
    uint16_t size = packet_flow_identifier->length + sizeof(packet_flow_identifier->length);
    ogs_nas_packet_flow_identifier_t target;

    memcpy(&target, packet_flow_identifier, sizeof(ogs_nas_packet_flow_identifier_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PACKET_FLOW_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.9.4.9 PDN address
 * M LV 6-14 */
int ogs_nas_decode_pdn_address(ogs_nas_pdn_address_t *pdn_address, ogs_pkbuf_t *pkbuf)
{
    uint16_t size = 0;
    ogs_nas_pdn_address_t *source = (ogs_nas_pdn_address_t *)pkbuf->data;

    pdn_address->length = source->length;
    size = pdn_address->length + sizeof(pdn_address->length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pdn_address, pkbuf->data - size, size);

    ogs_trace("  PDN_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_encode_pdn_address(ogs_pkbuf_t *pkbuf, ogs_nas_pdn_address_t *pdn_address)
{
    uint16_t size = pdn_address->length + sizeof(pdn_address->length);
    ogs_nas_pdn_address_t target;

    memcpy(&target, pdn_address, sizeof(ogs_nas_pdn_address_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDN_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

