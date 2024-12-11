/*
 * The MIT License
 *
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
 * This file had been created by nas-message.py script v0.2.0
 * Please do not modify this file but regenerate it via script.
 * Created on: 2024-12-11 21:13:08.097860 by acetcom
 * from 24501-h90.docx
 ******************************************************************************/

#include "ogs-nas-5gs.h"

int ogs_nas_5gs_encode_optional_type(ogs_pkbuf_t *pkbuf, uint8_t type)
{
    int size = sizeof(uint8_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &type, size);

    return size;
}
/* 9.11.2.1 Additional information
 * O TLV 3-n */
int ogs_nas_5gs_decode_additional_information(ogs_nas_additional_information_t *additional_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_additional_information_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_additional_information_t *)pkbuf->data;

    additional_information->length = source->length;
    size = additional_information->length + sizeof(additional_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*additional_information) < size) return -1;
    memcpy(additional_information, pkbuf->data - size, size);

    ogs_trace("  ADDITIONAL_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_additional_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_t *additional_information)
{
    int size = additional_information->length + sizeof(additional_information->length);
    ogs_nas_additional_information_t target;

    memcpy(&target, additional_information, sizeof(ogs_nas_additional_information_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ADDITIONAL_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.10 Service-level-AA container
 * O TLV-E 6-n */
int ogs_nas_5gs_decode_service_level_aa_container(ogs_nas_service_level_aa_container_t *service_level_aa_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_service_level_aa_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_service_level_aa_container_t *)pkbuf->data;

    service_level_aa_container->length = be16toh(source->length);
    size = service_level_aa_container->length + sizeof(service_level_aa_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    service_level_aa_container->buffer = pkbuf->data - size + sizeof(service_level_aa_container->length);

    ogs_trace("  SERVICE_LEVEL_AA_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)service_level_aa_container->buffer, service_level_aa_container->length);

    return size;
}

int ogs_nas_5gs_encode_service_level_aa_container(ogs_pkbuf_t *pkbuf, ogs_nas_service_level_aa_container_t *service_level_aa_container)
{
    int size = 0;
    int target;

    ogs_assert(service_level_aa_container);
    ogs_assert(service_level_aa_container->buffer);

    size = sizeof(service_level_aa_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(service_level_aa_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = service_level_aa_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, service_level_aa_container->buffer, size);

    ogs_trace("  SERVICE_LEVEL_AA_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return service_level_aa_container->length + sizeof(service_level_aa_container->length);
}

/* 9.11.2.1A Access type
 * M V 1/2 */
int ogs_nas_5gs_decode_access_type(ogs_nas_access_type_t *access_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_access_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(access_type, pkbuf->data - size, size);

    ogs_trace("  ACCESS_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_access_type(ogs_pkbuf_t *pkbuf, ogs_nas_access_type_t *access_type)
{
    int size = sizeof(ogs_nas_access_type_t);
    ogs_nas_access_type_t target;

    memcpy(&target, access_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ACCESS_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.1B DNN
 * O TLV 3-102 */
int ogs_nas_5gs_decode_dnn(ogs_nas_dnn_t *dnn, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_dnn_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_dnn_t *)pkbuf->data;

    dnn->length = source->length;
    size = dnn->length + sizeof(dnn->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*dnn) < size) return -1;
    memcpy(dnn, pkbuf->data - size, size);

    {
        char data_network_name[OGS_MAX_DNN_LEN+1];
        dnn->length = ogs_fqdn_parse(data_network_name, dnn->value, ogs_min(dnn->length, OGS_MAX_DNN_LEN));
        if (dnn->length > 0) {
            ogs_cpystrn(dnn->value, data_network_name, ogs_min(dnn->length, OGS_MAX_DNN_LEN)+1);
        } else {
            ogs_error("UE not APN setting");
        }
    }

    ogs_trace("  DNN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_dnn(ogs_pkbuf_t *pkbuf, ogs_nas_dnn_t *dnn)
{
    int size = dnn->length + sizeof(dnn->length);
    ogs_nas_dnn_t target;

    memcpy(&target, dnn, sizeof(ogs_nas_dnn_t));
    target.length = ogs_fqdn_build(target.value, dnn->value, dnn->length);
    size = target.length + sizeof(target.length);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DNN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.2 EAP message
 * O TLV-E 7-1503 */
int ogs_nas_5gs_decode_eap_message(ogs_nas_eap_message_t *eap_message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_eap_message_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_eap_message_t *)pkbuf->data;

    eap_message->length = be16toh(source->length);
    size = eap_message->length + sizeof(eap_message->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    eap_message->buffer = pkbuf->data - size + sizeof(eap_message->length);

    ogs_trace("  EAP_MESSAGE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)eap_message->buffer, eap_message->length);

    return size;
}

int ogs_nas_5gs_encode_eap_message(ogs_pkbuf_t *pkbuf, ogs_nas_eap_message_t *eap_message)
{
    int size = 0;
    int target;

    ogs_assert(eap_message);
    ogs_assert(eap_message->buffer);

    size = sizeof(eap_message->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(eap_message->length);
    memcpy(pkbuf->data - size, &target, size);

    size = eap_message->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, eap_message->buffer, size);

    ogs_trace("  EAP_MESSAGE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return eap_message->length + sizeof(eap_message->length);
}

/* 9.11.2.3 GPRS timer
 * O TV 2 */
int ogs_nas_5gs_decode_gprs_timer(ogs_nas_gprs_timer_t *gprs_timer, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_gprs_timer_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(gprs_timer, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_gprs_timer(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_t *gprs_timer)
{
    int size = sizeof(ogs_nas_gprs_timer_t);
    ogs_nas_gprs_timer_t target;

    memcpy(&target, gprs_timer, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.4 GPRS timer 2
 * O TLV 3 */
int ogs_nas_5gs_decode_gprs_timer_2(ogs_nas_gprs_timer_2_t *gprs_timer_2, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_gprs_timer_2_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_gprs_timer_2_t *)pkbuf->data;

    gprs_timer_2->length = source->length;
    size = gprs_timer_2->length + sizeof(gprs_timer_2->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gprs_timer_2) < size) return -1;
    memcpy(gprs_timer_2, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_gprs_timer_2(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_2_t *gprs_timer_2)
{
    int size = gprs_timer_2->length + sizeof(gprs_timer_2->length);
    ogs_nas_gprs_timer_2_t target;

    memcpy(&target, gprs_timer_2, sizeof(ogs_nas_gprs_timer_2_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.5 GPRS timer 3
 * O TLV 3 */
int ogs_nas_5gs_decode_gprs_timer_3(ogs_nas_gprs_timer_3_t *gprs_timer_3, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_gprs_timer_3_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_gprs_timer_3_t *)pkbuf->data;

    gprs_timer_3->length = source->length;
    size = gprs_timer_3->length + sizeof(gprs_timer_3->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gprs_timer_3) < size) return -1;
    memcpy(gprs_timer_3, pkbuf->data - size, size);

    ogs_trace("  GPRS_TIMER_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_gprs_timer_3(ogs_pkbuf_t *pkbuf, ogs_nas_gprs_timer_3_t *gprs_timer_3)
{
    int size = gprs_timer_3->length + sizeof(gprs_timer_3->length);
    ogs_nas_gprs_timer_3_t target;

    memcpy(&target, gprs_timer_3, sizeof(ogs_nas_gprs_timer_3_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  GPRS_TIMER_3 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.2.8 S-NSSAI
 * O TLV 3-10 */
int ogs_nas_5gs_decode_s_nssai(ogs_nas_s_nssai_t *s_nssai, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_s_nssai_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_s_nssai_t *)pkbuf->data;

    s_nssai->length = source->length;
    size = s_nssai->length + sizeof(s_nssai->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*s_nssai) < size) return -1;
    memcpy(s_nssai, pkbuf->data - size, size);

    ogs_trace("  S_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_s_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_s_nssai_t *s_nssai)
{
    int size = s_nssai->length + sizeof(s_nssai->length);
    ogs_nas_s_nssai_t target;

    memcpy(&target, s_nssai, sizeof(ogs_nas_s_nssai_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  S_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.1 5GMM capability
 * O TLV 3-15 */
int ogs_nas_5gs_decode_5gmm_capability(ogs_nas_5gmm_capability_t *gmm_capability, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gmm_capability_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gmm_capability_t *)pkbuf->data;

    gmm_capability->length = source->length;
    size = gmm_capability->length + sizeof(gmm_capability->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gmm_capability) < size) return -1;
    memcpy(gmm_capability, pkbuf->data - size, size);

    ogs_trace("  5GMM_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gmm_capability(ogs_pkbuf_t *pkbuf, ogs_nas_5gmm_capability_t *gmm_capability)
{
    int size = gmm_capability->length + sizeof(gmm_capability->length);
    ogs_nas_5gmm_capability_t target;

    memcpy(&target, gmm_capability, sizeof(ogs_nas_5gmm_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GMM_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.10 ABBA
 * M LV 3-n */
int ogs_nas_5gs_decode_abba(ogs_nas_abba_t *abba, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_abba_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_abba_t *)pkbuf->data;

    abba->length = source->length;
    size = abba->length + sizeof(abba->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*abba) < size) return -1;
    memcpy(abba, pkbuf->data - size, size);

    ogs_trace("  ABBA - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_abba(ogs_pkbuf_t *pkbuf, ogs_nas_abba_t *abba)
{
    int size = abba->length + sizeof(abba->length);
    ogs_nas_abba_t target;

    memcpy(&target, abba, sizeof(ogs_nas_abba_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ABBA - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.12 Additional 5G security information
 * O TLV 3 */
int ogs_nas_5gs_decode_additional_5g_security_information(ogs_nas_additional_5g_security_information_t *additional_security_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_additional_5g_security_information_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_additional_5g_security_information_t *)pkbuf->data;

    additional_security_information->length = source->length;
    size = additional_security_information->length + sizeof(additional_security_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*additional_security_information) < size) return -1;
    memcpy(additional_security_information, pkbuf->data - size, size);

    ogs_trace("  ADDITIONAL_5G_SECURITY_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_additional_5g_security_information(ogs_pkbuf_t *pkbuf, ogs_nas_additional_5g_security_information_t *additional_security_information)
{
    int size = additional_security_information->length + sizeof(additional_security_information->length);
    ogs_nas_additional_5g_security_information_t target;

    memcpy(&target, additional_security_information, sizeof(ogs_nas_additional_5g_security_information_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ADDITIONAL_5G_SECURITY_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.12A Additional information requested
 * O TLV 3 */
int ogs_nas_5gs_decode_additional_information_requested(ogs_nas_additional_information_requested_t *additional_information_requested, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_additional_information_requested_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_additional_information_requested_t *)pkbuf->data;

    additional_information_requested->length = source->length;
    size = additional_information_requested->length + sizeof(additional_information_requested->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*additional_information_requested) < size) return -1;
    memcpy(additional_information_requested, pkbuf->data - size, size);

    ogs_trace("  ADDITIONAL_INFORMATION_REQUESTED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_additional_information_requested(ogs_pkbuf_t *pkbuf, ogs_nas_additional_information_requested_t *additional_information_requested)
{
    int size = additional_information_requested->length + sizeof(additional_information_requested->length);
    ogs_nas_additional_information_requested_t target;

    memcpy(&target, additional_information_requested, sizeof(ogs_nas_additional_information_requested_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ADDITIONAL_INFORMATION_REQUESTED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.13 Allowed PDU session status
 * O TLV 4-34 */
int ogs_nas_5gs_decode_allowed_pdu_session_status(ogs_nas_allowed_pdu_session_status_t *allowed_pdu_session_status, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_allowed_pdu_session_status_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_allowed_pdu_session_status_t *)pkbuf->data;

    allowed_pdu_session_status->length = source->length;
    size = allowed_pdu_session_status->length + sizeof(allowed_pdu_session_status->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*allowed_pdu_session_status) < size) return -1;
    memcpy(allowed_pdu_session_status, pkbuf->data - size, size);

    allowed_pdu_session_status->psi = be16toh(allowed_pdu_session_status->psi);

    ogs_trace("  ALLOWED_PDU_SESSION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_allowed_pdu_session_status(ogs_pkbuf_t *pkbuf, ogs_nas_allowed_pdu_session_status_t *allowed_pdu_session_status)
{
    int size = allowed_pdu_session_status->length + sizeof(allowed_pdu_session_status->length);
    ogs_nas_allowed_pdu_session_status_t target;

    memcpy(&target, allowed_pdu_session_status, sizeof(ogs_nas_allowed_pdu_session_status_t));
    target.psi = htobe16(allowed_pdu_session_status->psi);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ALLOWED_PDU_SESSION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.14 Authentication failure parameter
 * O TLV 16 */
int ogs_nas_5gs_decode_authentication_failure_parameter(ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_authentication_failure_parameter_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_authentication_failure_parameter_t *)pkbuf->data;

    authentication_failure_parameter->length = source->length;
    size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*authentication_failure_parameter) < size) return -1;
    memcpy(authentication_failure_parameter, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_FAILURE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_authentication_failure_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_failure_parameter_t *authentication_failure_parameter)
{
    int size = authentication_failure_parameter->length + sizeof(authentication_failure_parameter->length);
    ogs_nas_authentication_failure_parameter_t target;

    memcpy(&target, authentication_failure_parameter, sizeof(ogs_nas_authentication_failure_parameter_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_FAILURE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.15 Authentication parameter AUTN
 * O TLV 18 */
int ogs_nas_5gs_decode_authentication_parameter_autn(ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_authentication_parameter_autn_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_authentication_parameter_autn_t *)pkbuf->data;

    authentication_parameter_autn->length = source->length;
    size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*authentication_parameter_autn) < size) return -1;
    memcpy(authentication_parameter_autn, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_AUTN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_authentication_parameter_autn(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_autn_t *authentication_parameter_autn)
{
    int size = authentication_parameter_autn->length + sizeof(authentication_parameter_autn->length);
    ogs_nas_authentication_parameter_autn_t target;

    memcpy(&target, authentication_parameter_autn, sizeof(ogs_nas_authentication_parameter_autn_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_AUTN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.16 Authentication parameter RAND
 * O TV 17 */
int ogs_nas_5gs_decode_authentication_parameter_rand(ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_authentication_parameter_rand_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(authentication_parameter_rand, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_RAND - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_authentication_parameter_rand(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_parameter_rand_t *authentication_parameter_rand)
{
    int size = sizeof(ogs_nas_authentication_parameter_rand_t);
    ogs_nas_authentication_parameter_rand_t target;

    memcpy(&target, authentication_parameter_rand, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_PARAMETER_RAND - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.17 Authentication response parameter
 * O TLV 18 */
int ogs_nas_5gs_decode_authentication_response_parameter(ogs_nas_authentication_response_parameter_t *authentication_response_parameter, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_authentication_response_parameter_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_authentication_response_parameter_t *)pkbuf->data;

    authentication_response_parameter->length = source->length;
    size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*authentication_response_parameter) < size) return -1;
    memcpy(authentication_response_parameter, pkbuf->data - size, size);

    ogs_trace("  AUTHENTICATION_RESPONSE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_authentication_response_parameter(ogs_pkbuf_t *pkbuf, ogs_nas_authentication_response_parameter_t *authentication_response_parameter)
{
    int size = authentication_response_parameter->length + sizeof(authentication_response_parameter->length);
    ogs_nas_authentication_response_parameter_t target;

    memcpy(&target, authentication_response_parameter, sizeof(ogs_nas_authentication_response_parameter_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  AUTHENTICATION_RESPONSE_PARAMETER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.18 Configuration update indication
 * O TV 1 */
int ogs_nas_5gs_decode_configuration_update_indication(ogs_nas_configuration_update_indication_t *configuration_update_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_configuration_update_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(configuration_update_indication, pkbuf->data - size, size);

    ogs_trace("  CONFIGURATION_UPDATE_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_configuration_update_indication(ogs_pkbuf_t *pkbuf, ogs_nas_configuration_update_indication_t *configuration_update_indication)
{
    int size = sizeof(ogs_nas_configuration_update_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, configuration_update_indication, size);

    ogs_trace("  CONFIGURATION_UPDATE_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.18A CAG information list
 * O TLV-E 3-n */
int ogs_nas_5gs_decode_cag_information_list(ogs_nas_cag_information_list_t *cag_information_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_cag_information_list_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_cag_information_list_t *)pkbuf->data;

    cag_information_list->length = be16toh(source->length);
    size = cag_information_list->length + sizeof(cag_information_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    cag_information_list->buffer = pkbuf->data - size + sizeof(cag_information_list->length);

    ogs_trace("  CAG_INFORMATION_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)cag_information_list->buffer, cag_information_list->length);

    return size;
}

int ogs_nas_5gs_encode_cag_information_list(ogs_pkbuf_t *pkbuf, ogs_nas_cag_information_list_t *cag_information_list)
{
    int size = 0;
    int target;

    ogs_assert(cag_information_list);
    ogs_assert(cag_information_list->buffer);

    size = sizeof(cag_information_list->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(cag_information_list->length);
    memcpy(pkbuf->data - size, &target, size);

    size = cag_information_list->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, cag_information_list->buffer, size);

    ogs_trace("  CAG_INFORMATION_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return cag_information_list->length + sizeof(cag_information_list->length);
}

/* 9.11.3.18C Ciphering key data
 * O TLV-E 34-n */
int ogs_nas_5gs_decode_ciphering_key_data(ogs_nas_ciphering_key_data_t *ciphering_key_data, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ciphering_key_data_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ciphering_key_data_t *)pkbuf->data;

    ciphering_key_data->length = be16toh(source->length);
    size = ciphering_key_data->length + sizeof(ciphering_key_data->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    ciphering_key_data->buffer = pkbuf->data - size + sizeof(ciphering_key_data->length);

    ogs_trace("  CIPHERING_KEY_DATA - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)ciphering_key_data->buffer, ciphering_key_data->length);

    return size;
}

int ogs_nas_5gs_encode_ciphering_key_data(ogs_pkbuf_t *pkbuf, ogs_nas_ciphering_key_data_t *ciphering_key_data)
{
    int size = 0;
    int target;

    ogs_assert(ciphering_key_data);
    ogs_assert(ciphering_key_data->buffer);

    size = sizeof(ciphering_key_data->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(ciphering_key_data->length);
    memcpy(pkbuf->data - size, &target, size);

    size = ciphering_key_data->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ciphering_key_data->buffer, size);

    ogs_trace("  CIPHERING_KEY_DATA - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return ciphering_key_data->length + sizeof(ciphering_key_data->length);
}

/* 9.11.3.19 Daylight saving time
 * O TLV 3 */
int ogs_nas_5gs_decode_daylight_saving_time(ogs_nas_daylight_saving_time_t *daylight_saving_time, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_daylight_saving_time_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_daylight_saving_time_t *)pkbuf->data;

    daylight_saving_time->length = source->length;
    size = daylight_saving_time->length + sizeof(daylight_saving_time->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*daylight_saving_time) < size) return -1;
    memcpy(daylight_saving_time, pkbuf->data - size, size);

    ogs_trace("  DAYLIGHT_SAVING_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_daylight_saving_time(ogs_pkbuf_t *pkbuf, ogs_nas_daylight_saving_time_t *daylight_saving_time)
{
    int size = daylight_saving_time->length + sizeof(daylight_saving_time->length);
    ogs_nas_daylight_saving_time_t target;

    memcpy(&target, daylight_saving_time, sizeof(ogs_nas_daylight_saving_time_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DAYLIGHT_SAVING_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.2 5GMM cause
 * M V 1 */
int ogs_nas_5gs_decode_5gmm_cause(ogs_nas_5gmm_cause_t *gmm_cause, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_5gmm_cause_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(gmm_cause, pkbuf->data - size, size);

    ogs_trace("  5GMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gmm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_5gmm_cause_t *gmm_cause)
{
    int size = sizeof(ogs_nas_5gmm_cause_t);
    ogs_nas_5gmm_cause_t target;

    memcpy(&target, gmm_cause, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GMM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.20 De-registration type
 * M V 1/2 */
int ogs_nas_5gs_decode_de_registration_type(ogs_nas_de_registration_type_t *de_registration_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_de_registration_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(de_registration_type, pkbuf->data - size, size);

    ogs_trace("  DE_REGISTRATION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_de_registration_type(ogs_pkbuf_t *pkbuf, ogs_nas_de_registration_type_t *de_registration_type)
{
    int size = sizeof(ogs_nas_de_registration_type_t);
    ogs_nas_de_registration_type_t target;

    memcpy(&target, de_registration_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DE_REGISTRATION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.23 Emergency number list
 * O TLV 5-50 */
int ogs_nas_5gs_decode_emergency_number_list(ogs_nas_emergency_number_list_t *emergency_number_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_emergency_number_list_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_emergency_number_list_t *)pkbuf->data;

    emergency_number_list->length = source->length;
    size = emergency_number_list->length + sizeof(emergency_number_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*emergency_number_list) < size) return -1;
    memcpy(emergency_number_list, pkbuf->data - size, size);

    ogs_trace("  EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_emergency_number_list_t *emergency_number_list)
{
    int size = emergency_number_list->length + sizeof(emergency_number_list->length);
    ogs_nas_emergency_number_list_t target;

    memcpy(&target, emergency_number_list, sizeof(ogs_nas_emergency_number_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.23A EPS bearer context status
 * O TLV 4 */
int ogs_nas_5gs_decode_eps_bearer_context_status(ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_eps_bearer_context_status_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_eps_bearer_context_status_t *)pkbuf->data;

    eps_bearer_context_status->length = source->length;
    size = eps_bearer_context_status->length + sizeof(eps_bearer_context_status->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*eps_bearer_context_status) < size) return -1;
    memcpy(eps_bearer_context_status, pkbuf->data - size, size);

    ogs_trace("  EPS_BEARER_CONTEXT_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_eps_bearer_context_status(ogs_pkbuf_t *pkbuf, ogs_nas_eps_bearer_context_status_t *eps_bearer_context_status)
{
    int size = eps_bearer_context_status->length + sizeof(eps_bearer_context_status->length);
    ogs_nas_eps_bearer_context_status_t target;

    memcpy(&target, eps_bearer_context_status, sizeof(ogs_nas_eps_bearer_context_status_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_BEARER_CONTEXT_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.24 EPS NAS message container
 * O TLV-E 4-n */
int ogs_nas_5gs_decode_eps_nas_message_container(ogs_nas_eps_nas_message_container_t *eps_nas_message_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_eps_nas_message_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_eps_nas_message_container_t *)pkbuf->data;

    eps_nas_message_container->length = be16toh(source->length);
    size = eps_nas_message_container->length + sizeof(eps_nas_message_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    eps_nas_message_container->buffer = pkbuf->data - size + sizeof(eps_nas_message_container->length);

    ogs_trace("  EPS_NAS_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)eps_nas_message_container->buffer, eps_nas_message_container->length);

    return size;
}

int ogs_nas_5gs_encode_eps_nas_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_eps_nas_message_container_t *eps_nas_message_container)
{
    int size = 0;
    int target;

    ogs_assert(eps_nas_message_container);
    ogs_assert(eps_nas_message_container->buffer);

    size = sizeof(eps_nas_message_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(eps_nas_message_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = eps_nas_message_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, eps_nas_message_container->buffer, size);

    ogs_trace("  EPS_NAS_MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return eps_nas_message_container->length + sizeof(eps_nas_message_container->length);
}

/* 9.11.3.25 EPS NAS security algorithms
 * O TV 2 */
int ogs_nas_5gs_decode_eps_nas_security_algorithms(ogs_nas_eps_nas_security_algorithms_t *eps_nas_security_algorithms, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_eps_nas_security_algorithms_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(eps_nas_security_algorithms, pkbuf->data - size, size);

    ogs_trace("  EPS_NAS_SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_eps_nas_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_eps_nas_security_algorithms_t *eps_nas_security_algorithms)
{
    int size = sizeof(ogs_nas_eps_nas_security_algorithms_t);
    ogs_nas_eps_nas_security_algorithms_t target;

    memcpy(&target, eps_nas_security_algorithms, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EPS_NAS_SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.26 Extended emergency number list
 * O TLV-E 7-65538 */
int ogs_nas_5gs_decode_extended_emergency_number_list(ogs_nas_extended_emergency_number_list_t *extended_emergency_number_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_extended_emergency_number_list_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_extended_emergency_number_list_t *)pkbuf->data;

    extended_emergency_number_list->length = be16toh(source->length);
    size = extended_emergency_number_list->length + sizeof(extended_emergency_number_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    extended_emergency_number_list->buffer = pkbuf->data - size + sizeof(extended_emergency_number_list->length);

    ogs_trace("  EXTENDED_EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)extended_emergency_number_list->buffer, extended_emergency_number_list->length);

    return size;
}

int ogs_nas_5gs_encode_extended_emergency_number_list(ogs_pkbuf_t *pkbuf, ogs_nas_extended_emergency_number_list_t *extended_emergency_number_list)
{
    int size = 0;
    int target;

    ogs_assert(extended_emergency_number_list);
    ogs_assert(extended_emergency_number_list->buffer);

    size = sizeof(extended_emergency_number_list->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(extended_emergency_number_list->length);
    memcpy(pkbuf->data - size, &target, size);

    size = extended_emergency_number_list->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, extended_emergency_number_list->buffer, size);

    ogs_trace("  EXTENDED_EMERGENCY_NUMBER_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return extended_emergency_number_list->length + sizeof(extended_emergency_number_list->length);
}

/* 9.11.3.26A Extended DRX parameters
 * O TLV 3-4 */
int ogs_nas_5gs_decode_extended_drx_parameters(ogs_nas_extended_drx_parameters_t *extended_drx_parameters, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_extended_drx_parameters_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_extended_drx_parameters_t *)pkbuf->data;

    extended_drx_parameters->length = source->length;
    size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*extended_drx_parameters) < size) return -1;
    memcpy(extended_drx_parameters, pkbuf->data - size, size);

    ogs_trace("  EXTENDED_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_extended_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_extended_drx_parameters_t *extended_drx_parameters)
{
    int size = extended_drx_parameters->length + sizeof(extended_drx_parameters->length);
    ogs_nas_extended_drx_parameters_t target;

    memcpy(&target, extended_drx_parameters, sizeof(ogs_nas_extended_drx_parameters_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EXTENDED_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.28 IMEISV request
 * O TV 1 */
int ogs_nas_5gs_decode_imeisv_request(ogs_nas_imeisv_request_t *imeisv_request, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_imeisv_request_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(imeisv_request, pkbuf->data - size, size);

    ogs_trace("  IMEISV_REQUEST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_imeisv_request(ogs_pkbuf_t *pkbuf, ogs_nas_imeisv_request_t *imeisv_request)
{
    int size = sizeof(ogs_nas_imeisv_request_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, imeisv_request, size);

    ogs_trace("  IMEISV_REQUEST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.29 LADN indication
 * O TLV-E 3-811 */
int ogs_nas_5gs_decode_ladn_indication(ogs_nas_ladn_indication_t *ladn_indication, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ladn_indication_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ladn_indication_t *)pkbuf->data;

    ladn_indication->length = be16toh(source->length);
    size = ladn_indication->length + sizeof(ladn_indication->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    ladn_indication->buffer = pkbuf->data - size + sizeof(ladn_indication->length);

    ogs_trace("  LADN_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)ladn_indication->buffer, ladn_indication->length);

    return size;
}

int ogs_nas_5gs_encode_ladn_indication(ogs_pkbuf_t *pkbuf, ogs_nas_ladn_indication_t *ladn_indication)
{
    int size = 0;
    int target;

    ogs_assert(ladn_indication);
    ogs_assert(ladn_indication->buffer);

    size = sizeof(ladn_indication->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(ladn_indication->length);
    memcpy(pkbuf->data - size, &target, size);

    size = ladn_indication->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ladn_indication->buffer, size);

    ogs_trace("  LADN_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return ladn_indication->length + sizeof(ladn_indication->length);
}

/* 9.11.3.2A 5GS DRX parameters
 * O TLV 3 */
int ogs_nas_5gs_decode_5gs_drx_parameters(ogs_nas_5gs_drx_parameters_t *drx_parameters, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_drx_parameters_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_drx_parameters_t *)pkbuf->data;

    drx_parameters->length = source->length;
    size = drx_parameters->length + sizeof(drx_parameters->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*drx_parameters) < size) return -1;
    memcpy(drx_parameters, pkbuf->data - size, size);

    ogs_trace("  5GS_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_drx_parameters_t *drx_parameters)
{
    int size = drx_parameters->length + sizeof(drx_parameters->length);
    ogs_nas_5gs_drx_parameters_t target;

    memcpy(&target, drx_parameters, sizeof(ogs_nas_5gs_drx_parameters_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.3 5GS identity type
 * M V 1/2 */
int ogs_nas_5gs_decode_5gs_identity_type(ogs_nas_5gs_identity_type_t *identity_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_5gs_identity_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(identity_type, pkbuf->data - size, size);

    ogs_trace("  5GS_IDENTITY_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_identity_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_identity_type_t *identity_type)
{
    int size = sizeof(ogs_nas_5gs_identity_type_t);
    ogs_nas_5gs_identity_type_t target;

    memcpy(&target, identity_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_IDENTITY_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.30 LADN information
 * O TLV-E 12-1715 */
int ogs_nas_5gs_decode_ladn_information(ogs_nas_ladn_information_t *ladn_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ladn_information_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ladn_information_t *)pkbuf->data;

    ladn_information->length = be16toh(source->length);
    size = ladn_information->length + sizeof(ladn_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    ladn_information->buffer = pkbuf->data - size + sizeof(ladn_information->length);

    ogs_trace("  LADN_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)ladn_information->buffer, ladn_information->length);

    return size;
}

int ogs_nas_5gs_encode_ladn_information(ogs_pkbuf_t *pkbuf, ogs_nas_ladn_information_t *ladn_information)
{
    int size = 0;
    int target;

    ogs_assert(ladn_information);
    ogs_assert(ladn_information->buffer);

    size = sizeof(ladn_information->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(ladn_information->length);
    memcpy(pkbuf->data - size, &target, size);

    size = ladn_information->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ladn_information->buffer, size);

    ogs_trace("  LADN_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return ladn_information->length + sizeof(ladn_information->length);
}

/* 9.11.3.31 MICO indication
 * O TV 1 */
int ogs_nas_5gs_decode_mico_indication(ogs_nas_mico_indication_t *mico_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_mico_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(mico_indication, pkbuf->data - size, size);

    ogs_trace("  MICO_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_mico_indication(ogs_pkbuf_t *pkbuf, ogs_nas_mico_indication_t *mico_indication)
{
    int size = sizeof(ogs_nas_mico_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, mico_indication, size);

    ogs_trace("  MICO_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.31A MA PDU session information
 * O TV 1 */
int ogs_nas_5gs_decode_ma_pdu_session_information(ogs_nas_ma_pdu_session_information_t *ma_pdu_session_information, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_ma_pdu_session_information_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(ma_pdu_session_information, pkbuf->data - size, size);

    ogs_trace("  MA_PDU_SESSION_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ma_pdu_session_information(ogs_pkbuf_t *pkbuf, ogs_nas_ma_pdu_session_information_t *ma_pdu_session_information)
{
    int size = sizeof(ogs_nas_ma_pdu_session_information_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ma_pdu_session_information, size);

    ogs_trace("  MA_PDU_SESSION_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.31B Mapped NSSAI
 * O TLV 3-42 */
int ogs_nas_5gs_decode_mapped_nssai(ogs_nas_mapped_nssai_t *mapped_nssai, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_mapped_nssai_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_mapped_nssai_t *)pkbuf->data;

    mapped_nssai->length = source->length;
    size = mapped_nssai->length + sizeof(mapped_nssai->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*mapped_nssai) < size) return -1;
    memcpy(mapped_nssai, pkbuf->data - size, size);

    ogs_trace("  MAPPED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_mapped_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_mapped_nssai_t *mapped_nssai)
{
    int size = mapped_nssai->length + sizeof(mapped_nssai->length);
    ogs_nas_mapped_nssai_t target;

    memcpy(&target, mapped_nssai, sizeof(ogs_nas_mapped_nssai_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MAPPED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.31C Mobile station classmark 2
 * O TLV 5 */
int ogs_nas_5gs_decode_mobile_station_classmark_2(ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_mobile_station_classmark_2_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_mobile_station_classmark_2_t *)pkbuf->data;

    mobile_station_classmark_2->length = source->length;
    size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*mobile_station_classmark_2) < size) return -1;
    memcpy(mobile_station_classmark_2, pkbuf->data - size, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_mobile_station_classmark_2(ogs_pkbuf_t *pkbuf, ogs_nas_mobile_station_classmark_2_t *mobile_station_classmark_2)
{
    int size = mobile_station_classmark_2->length + sizeof(mobile_station_classmark_2->length);
    ogs_nas_mobile_station_classmark_2_t target;

    memcpy(&target, mobile_station_classmark_2, sizeof(ogs_nas_mobile_station_classmark_2_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MOBILE_STATION_CLASSMARK_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.32 key set identifier
 * O TV 1 */
int ogs_nas_5gs_decode_key_set_identifier(ogs_nas_key_set_identifier_t *key_set_identifier, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_key_set_identifier_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(key_set_identifier, pkbuf->data - size, size);

    ogs_trace("  KEY_SET_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_key_set_identifier(ogs_pkbuf_t *pkbuf, ogs_nas_key_set_identifier_t *key_set_identifier)
{
    int size = sizeof(ogs_nas_key_set_identifier_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, key_set_identifier, size);

    ogs_trace("  KEY_SET_IDENTIFIER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.33 message container
 * O TLV-E 4-n */
int ogs_nas_5gs_decode_message_container(ogs_nas_message_container_t *message_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_message_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_message_container_t *)pkbuf->data;

    message_container->length = be16toh(source->length);
    size = message_container->length + sizeof(message_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    message_container->buffer = pkbuf->data - size + sizeof(message_container->length);

    ogs_trace("  MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)message_container->buffer, message_container->length);

    return size;
}

int ogs_nas_5gs_encode_message_container(ogs_pkbuf_t *pkbuf, ogs_nas_message_container_t *message_container)
{
    int size = 0;
    int target;

    ogs_assert(message_container);
    ogs_assert(message_container->buffer);

    size = sizeof(message_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(message_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = message_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, message_container->buffer, size);

    ogs_trace("  MESSAGE_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return message_container->length + sizeof(message_container->length);
}

/* 9.11.3.34 security algorithms
 * M V 1 */
int ogs_nas_5gs_decode_security_algorithms(ogs_nas_security_algorithms_t *security_algorithms, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_security_algorithms_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(security_algorithms, pkbuf->data - size, size);

    ogs_trace("  SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_security_algorithms(ogs_pkbuf_t *pkbuf, ogs_nas_security_algorithms_t *security_algorithms)
{
    int size = sizeof(ogs_nas_security_algorithms_t);
    ogs_nas_security_algorithms_t target;

    memcpy(&target, security_algorithms, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SECURITY_ALGORITHMS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.35 Network name
 * O TLV 3-n */
int ogs_nas_5gs_decode_network_name(ogs_nas_network_name_t *network_name, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_network_name_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_network_name_t *)pkbuf->data;

    network_name->length = source->length;
    size = network_name->length + sizeof(network_name->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*network_name) < size) return -1;
    memcpy(network_name, pkbuf->data - size, size);

    ogs_trace("  NETWORK_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_network_name(ogs_pkbuf_t *pkbuf, ogs_nas_network_name_t *network_name)
{
    int size = network_name->length + sizeof(network_name->length);
    ogs_nas_network_name_t target;

    memcpy(&target, network_name, sizeof(ogs_nas_network_name_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NETWORK_NAME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.36 Network slicing indication
 * O TV 1 */
int ogs_nas_5gs_decode_network_slicing_indication(ogs_nas_network_slicing_indication_t *network_slicing_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_network_slicing_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(network_slicing_indication, pkbuf->data - size, size);

    ogs_trace("  NETWORK_SLICING_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_network_slicing_indication(ogs_pkbuf_t *pkbuf, ogs_nas_network_slicing_indication_t *network_slicing_indication)
{
    int size = sizeof(ogs_nas_network_slicing_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, network_slicing_indication, size);

    ogs_trace("  NETWORK_SLICING_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.36A Non-3GPP NW provided policies
 * O TV 1 */
int ogs_nas_5gs_decode_non_3gpp_nw_provided_policies(ogs_nas_non_3gpp_nw_provided_policies_t *non_3gpp_nw_provided_policies, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_non_3gpp_nw_provided_policies_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(non_3gpp_nw_provided_policies, pkbuf->data - size, size);

    ogs_trace("  NON_3GPP_NW_PROVIDED_POLICIES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_non_3gpp_nw_provided_policies(ogs_pkbuf_t *pkbuf, ogs_nas_non_3gpp_nw_provided_policies_t *non_3gpp_nw_provided_policies)
{
    int size = sizeof(ogs_nas_non_3gpp_nw_provided_policies_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, non_3gpp_nw_provided_policies, size);

    ogs_trace("  NON_3GPP_NW_PROVIDED_POLICIES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.37 NSSAI
 * O TLV 4-74 */
int ogs_nas_5gs_decode_nssai(ogs_nas_nssai_t *nssai, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_nssai_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_nssai_t *)pkbuf->data;

    nssai->length = source->length;
    size = nssai->length + sizeof(nssai->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*nssai) < size) return -1;
    memcpy(nssai, pkbuf->data - size, size);

    ogs_trace("  NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_nssai_t *nssai)
{
    int size = nssai->length + sizeof(nssai->length);
    ogs_nas_nssai_t target;

    memcpy(&target, nssai, sizeof(ogs_nas_nssai_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.37A NSSAI inclusion mode
 * O TV 1 */
int ogs_nas_5gs_decode_nssai_inclusion_mode(ogs_nas_nssai_inclusion_mode_t *nssai_inclusion_mode, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_nssai_inclusion_mode_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(nssai_inclusion_mode, pkbuf->data - size, size);

    ogs_trace("  NSSAI_INCLUSION_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_nssai_inclusion_mode(ogs_pkbuf_t *pkbuf, ogs_nas_nssai_inclusion_mode_t *nssai_inclusion_mode)
{
    int size = sizeof(ogs_nas_nssai_inclusion_mode_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, nssai_inclusion_mode, size);

    ogs_trace("  NSSAI_INCLUSION_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.38 Operator-defined access category definitions
 * O TLV-E 3-8323 */
int ogs_nas_5gs_decode_operator_defined_access_category_definitions(ogs_nas_operator_defined_access_category_definitions_t *operator_defined_access_category_definitions, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_operator_defined_access_category_definitions_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_operator_defined_access_category_definitions_t *)pkbuf->data;

    operator_defined_access_category_definitions->length = be16toh(source->length);
    size = operator_defined_access_category_definitions->length + sizeof(operator_defined_access_category_definitions->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    operator_defined_access_category_definitions->buffer = pkbuf->data - size + sizeof(operator_defined_access_category_definitions->length);

    ogs_trace("  OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)operator_defined_access_category_definitions->buffer, operator_defined_access_category_definitions->length);

    return size;
}

int ogs_nas_5gs_encode_operator_defined_access_category_definitions(ogs_pkbuf_t *pkbuf, ogs_nas_operator_defined_access_category_definitions_t *operator_defined_access_category_definitions)
{
    int size = 0;
    int target;

    ogs_assert(operator_defined_access_category_definitions);
    ogs_assert(operator_defined_access_category_definitions->buffer);

    size = sizeof(operator_defined_access_category_definitions->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(operator_defined_access_category_definitions->length);
    memcpy(pkbuf->data - size, &target, size);

    size = operator_defined_access_category_definitions->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, operator_defined_access_category_definitions->buffer, size);

    ogs_trace("  OPERATOR_DEFINED_ACCESS_CATEGORY_DEFINITIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return operator_defined_access_category_definitions->length + sizeof(operator_defined_access_category_definitions->length);
}

/* 9.11.3.39 Payload container
 * O TLV-E 4-65538 */
int ogs_nas_5gs_decode_payload_container(ogs_nas_payload_container_t *payload_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_payload_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_payload_container_t *)pkbuf->data;

    payload_container->length = be16toh(source->length);
    size = payload_container->length + sizeof(payload_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    payload_container->buffer = pkbuf->data - size + sizeof(payload_container->length);

    ogs_trace("  PAYLOAD_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)payload_container->buffer, payload_container->length);

    return size;
}

int ogs_nas_5gs_encode_payload_container(ogs_pkbuf_t *pkbuf, ogs_nas_payload_container_t *payload_container)
{
    int size = 0;
    int target;

    ogs_assert(payload_container);
    ogs_assert(payload_container->buffer);

    size = sizeof(payload_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(payload_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = payload_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, payload_container->buffer, size);

    ogs_trace("  PAYLOAD_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return payload_container->length + sizeof(payload_container->length);
}

/* 9.11.3.4 5GS mobile identity
 * M LV-E 6-n */
int ogs_nas_5gs_decode_5gs_mobile_identity(ogs_nas_5gs_mobile_identity_t *mobile_identity, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_mobile_identity_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_mobile_identity_t *)pkbuf->data;

    mobile_identity->length = be16toh(source->length);
    size = mobile_identity->length + sizeof(mobile_identity->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    mobile_identity->buffer = pkbuf->data - size + sizeof(mobile_identity->length);

    ogs_trace("  5GS_MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)mobile_identity->buffer, mobile_identity->length);

    return size;
}

int ogs_nas_5gs_encode_5gs_mobile_identity(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_mobile_identity_t *mobile_identity)
{
    int size = 0;
    int target;

    ogs_assert(mobile_identity);
    ogs_assert(mobile_identity->buffer);

    size = sizeof(mobile_identity->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(mobile_identity->length);
    memcpy(pkbuf->data - size, &target, size);

    size = mobile_identity->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, mobile_identity->buffer, size);

    ogs_trace("  5GS_MOBILE_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return mobile_identity->length + sizeof(mobile_identity->length);
}

/* 9.11.3.40 Payload container type
 * O TV 1 */
int ogs_nas_5gs_decode_payload_container_type(ogs_nas_payload_container_type_t *payload_container_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_payload_container_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(payload_container_type, pkbuf->data - size, size);

    ogs_trace("  PAYLOAD_CONTAINER_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_payload_container_type(ogs_pkbuf_t *pkbuf, ogs_nas_payload_container_type_t *payload_container_type)
{
    int size = sizeof(ogs_nas_payload_container_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, payload_container_type, size);

    ogs_trace("  PAYLOAD_CONTAINER_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.41 PDU session identity 2
 * C TV 2 */
int ogs_nas_5gs_decode_pdu_session_identity_2(ogs_nas_pdu_session_identity_2_t *pdu_session_identity_2, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_pdu_session_identity_2_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(pdu_session_identity_2, pkbuf->data - size, size);

    ogs_trace("  PDU_SESSION_IDENTITY_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_identity_2(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_identity_2_t *pdu_session_identity_2)
{
    int size = sizeof(ogs_nas_pdu_session_identity_2_t);
    ogs_nas_pdu_session_identity_2_t target;

    memcpy(&target, pdu_session_identity_2, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDU_SESSION_IDENTITY_2 - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.42 PDU session reactivation result
 * O TLV 4-34 */
int ogs_nas_5gs_decode_pdu_session_reactivation_result(ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_pdu_session_reactivation_result_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_pdu_session_reactivation_result_t *)pkbuf->data;

    pdu_session_reactivation_result->length = source->length;
    size = pdu_session_reactivation_result->length + sizeof(pdu_session_reactivation_result->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*pdu_session_reactivation_result) < size) return -1;
    memcpy(pdu_session_reactivation_result, pkbuf->data - size, size);

    pdu_session_reactivation_result->psi = be16toh(pdu_session_reactivation_result->psi);

    ogs_trace("  PDU_SESSION_REACTIVATION_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_reactivation_result(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_reactivation_result_t *pdu_session_reactivation_result)
{
    int size = pdu_session_reactivation_result->length + sizeof(pdu_session_reactivation_result->length);
    ogs_nas_pdu_session_reactivation_result_t target;

    memcpy(&target, pdu_session_reactivation_result, sizeof(ogs_nas_pdu_session_reactivation_result_t));
    target.psi = htobe16(pdu_session_reactivation_result->psi);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDU_SESSION_REACTIVATION_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.43 PDU session reactivation result error cause
 * O TLV-E 5-515 */
int ogs_nas_5gs_decode_pdu_session_reactivation_result_error_cause(ogs_nas_pdu_session_reactivation_result_error_cause_t *pdu_session_reactivation_result_error_cause, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_pdu_session_reactivation_result_error_cause_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_pdu_session_reactivation_result_error_cause_t *)pkbuf->data;

    pdu_session_reactivation_result_error_cause->length = be16toh(source->length);
    size = pdu_session_reactivation_result_error_cause->length + sizeof(pdu_session_reactivation_result_error_cause->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    pdu_session_reactivation_result_error_cause->buffer = pkbuf->data - size + sizeof(pdu_session_reactivation_result_error_cause->length);

    ogs_trace("  PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)pdu_session_reactivation_result_error_cause->buffer, pdu_session_reactivation_result_error_cause->length);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_reactivation_result_error_cause(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_reactivation_result_error_cause_t *pdu_session_reactivation_result_error_cause)
{
    int size = 0;
    int target;

    ogs_assert(pdu_session_reactivation_result_error_cause);
    ogs_assert(pdu_session_reactivation_result_error_cause->buffer);

    size = sizeof(pdu_session_reactivation_result_error_cause->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(pdu_session_reactivation_result_error_cause->length);
    memcpy(pkbuf->data - size, &target, size);

    size = pdu_session_reactivation_result_error_cause->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, pdu_session_reactivation_result_error_cause->buffer, size);

    ogs_trace("  PDU_SESSION_REACTIVATION_RESULT_ERROR_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return pdu_session_reactivation_result_error_cause->length + sizeof(pdu_session_reactivation_result_error_cause->length);
}

/* 9.11.3.44 PDU session status
 * O TLV 4-34 */
int ogs_nas_5gs_decode_pdu_session_status(ogs_nas_pdu_session_status_t *pdu_session_status, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_pdu_session_status_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_pdu_session_status_t *)pkbuf->data;

    pdu_session_status->length = source->length;
    size = pdu_session_status->length + sizeof(pdu_session_status->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*pdu_session_status) < size) return -1;
    memcpy(pdu_session_status, pkbuf->data - size, size);

    pdu_session_status->psi = be16toh(pdu_session_status->psi);

    ogs_trace("  PDU_SESSION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_status(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_status_t *pdu_session_status)
{
    int size = pdu_session_status->length + sizeof(pdu_session_status->length);
    ogs_nas_pdu_session_status_t target;

    memcpy(&target, pdu_session_status, sizeof(ogs_nas_pdu_session_status_t));
    target.psi = htobe16(pdu_session_status->psi);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDU_SESSION_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.45 PLMN list
 * O TLV 5-47 */
int ogs_nas_5gs_decode_plmn_list(ogs_nas_plmn_list_t *plmn_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_plmn_list_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_plmn_list_t *)pkbuf->data;

    plmn_list->length = source->length;
    size = plmn_list->length + sizeof(plmn_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*plmn_list) < size) return -1;
    memcpy(plmn_list, pkbuf->data - size, size);

    ogs_trace("  PLMN_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_plmn_list(ogs_pkbuf_t *pkbuf, ogs_nas_plmn_list_t *plmn_list)
{
    int size = plmn_list->length + sizeof(plmn_list->length);
    ogs_nas_plmn_list_t target;

    memcpy(&target, plmn_list, sizeof(ogs_nas_plmn_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PLMN_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.46 Rejected NSSAI
 * O TLV 4-42 */
int ogs_nas_5gs_decode_rejected_nssai(ogs_nas_rejected_nssai_t *rejected_nssai, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_rejected_nssai_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_rejected_nssai_t *)pkbuf->data;

    rejected_nssai->length = source->length;
    size = rejected_nssai->length + sizeof(rejected_nssai->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*rejected_nssai) < size) return -1;
    memcpy(rejected_nssai, pkbuf->data - size, size);

    ogs_trace("  REJECTED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_rejected_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_rejected_nssai_t *rejected_nssai)
{
    int size = rejected_nssai->length + sizeof(rejected_nssai->length);
    ogs_nas_rejected_nssai_t target;

    memcpy(&target, rejected_nssai, sizeof(ogs_nas_rejected_nssai_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  REJECTED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.46A Release assistance indication
 * O TV 1 */
int ogs_nas_5gs_decode_release_assistance_indication(ogs_nas_release_assistance_indication_t *release_assistance_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_release_assistance_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(release_assistance_indication, pkbuf->data - size, size);

    ogs_trace("  RELEASE_ASSISTANCE_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_release_assistance_indication(ogs_pkbuf_t *pkbuf, ogs_nas_release_assistance_indication_t *release_assistance_indication)
{
    int size = sizeof(ogs_nas_release_assistance_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, release_assistance_indication, size);

    ogs_trace("  RELEASE_ASSISTANCE_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.47 Request type
 * O TV 1 */
int ogs_nas_5gs_decode_request_type(ogs_nas_request_type_t *request_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_request_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(request_type, pkbuf->data - size, size);

    ogs_trace("  REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_request_type(ogs_pkbuf_t *pkbuf, ogs_nas_request_type_t *request_type)
{
    int size = sizeof(ogs_nas_request_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, request_type, size);

    ogs_trace("  REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.48 S1 UE network capability
 * O TLV 4-15 */
int ogs_nas_5gs_decode_s1_ue_network_capability(ogs_nas_s1_ue_network_capability_t *s1_ue_network_capability, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_s1_ue_network_capability_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_s1_ue_network_capability_t *)pkbuf->data;

    s1_ue_network_capability->length = source->length;
    size = s1_ue_network_capability->length + sizeof(s1_ue_network_capability->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*s1_ue_network_capability) < size) return -1;
    memcpy(s1_ue_network_capability, pkbuf->data - size, size);

    ogs_trace("  S1_UE_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_s1_ue_network_capability(ogs_pkbuf_t *pkbuf, ogs_nas_s1_ue_network_capability_t *s1_ue_network_capability)
{
    int size = s1_ue_network_capability->length + sizeof(s1_ue_network_capability->length);
    ogs_nas_s1_ue_network_capability_t target;

    memcpy(&target, s1_ue_network_capability, sizeof(ogs_nas_s1_ue_network_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  S1_UE_NETWORK_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.48A S1 UE security capability
 * O TLV 4-7 */
int ogs_nas_5gs_decode_s1_ue_security_capability(ogs_nas_s1_ue_security_capability_t *s1_ue_security_capability, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_s1_ue_security_capability_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_s1_ue_security_capability_t *)pkbuf->data;

    s1_ue_security_capability->length = source->length;
    size = s1_ue_security_capability->length + sizeof(s1_ue_security_capability->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*s1_ue_security_capability) < size) return -1;
    memcpy(s1_ue_security_capability, pkbuf->data - size, size);

    ogs_trace("  S1_UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_s1_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_s1_ue_security_capability_t *s1_ue_security_capability)
{
    int size = s1_ue_security_capability->length + sizeof(s1_ue_security_capability->length);
    ogs_nas_s1_ue_security_capability_t target;

    memcpy(&target, s1_ue_security_capability, sizeof(ogs_nas_s1_ue_security_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  S1_UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.49 Service area list
 * O TLV 6-114 */
int ogs_nas_5gs_decode_service_area_list(ogs_nas_service_area_list_t *service_area_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_service_area_list_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_service_area_list_t *)pkbuf->data;

    service_area_list->length = source->length;
    size = service_area_list->length + sizeof(service_area_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*service_area_list) < size) return -1;
    memcpy(service_area_list, pkbuf->data - size, size);

    ogs_trace("  SERVICE_AREA_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_service_area_list(ogs_pkbuf_t *pkbuf, ogs_nas_service_area_list_t *service_area_list)
{
    int size = service_area_list->length + sizeof(service_area_list->length);
    ogs_nas_service_area_list_t target;

    memcpy(&target, service_area_list, sizeof(ogs_nas_service_area_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SERVICE_AREA_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.5 5GS network feature support
 * O TLV 3-5 */
int ogs_nas_5gs_decode_5gs_network_feature_support(ogs_nas_5gs_network_feature_support_t *network_feature_support, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_network_feature_support_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_network_feature_support_t *)pkbuf->data;

    network_feature_support->length = source->length;
    size = network_feature_support->length + sizeof(network_feature_support->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*network_feature_support) < size) return -1;
    memcpy(network_feature_support, pkbuf->data - size, size);

    ogs_trace("  5GS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_network_feature_support_t *network_feature_support)
{
    int size = network_feature_support->length + sizeof(network_feature_support->length);
    ogs_nas_5gs_network_feature_support_t target;

    memcpy(&target, network_feature_support, sizeof(ogs_nas_5gs_network_feature_support_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.50A SMS indication
 * O TV 1 */
int ogs_nas_5gs_decode_sms_indication(ogs_nas_sms_indication_t *sms_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_sms_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(sms_indication, pkbuf->data - size, size);

    ogs_trace("  SMS_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_sms_indication(ogs_pkbuf_t *pkbuf, ogs_nas_sms_indication_t *sms_indication)
{
    int size = sizeof(ogs_nas_sms_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, sms_indication, size);

    ogs_trace("  SMS_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.51 SOR transparent container
 * O TLV-E 20-n */
int ogs_nas_5gs_decode_sor_transparent_container(ogs_nas_sor_transparent_container_t *sor_transparent_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_sor_transparent_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_sor_transparent_container_t *)pkbuf->data;

    sor_transparent_container->length = be16toh(source->length);
    size = sor_transparent_container->length + sizeof(sor_transparent_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    sor_transparent_container->buffer = pkbuf->data - size + sizeof(sor_transparent_container->length);

    ogs_trace("  SOR_TRANSPARENT_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)sor_transparent_container->buffer, sor_transparent_container->length);

    return size;
}

int ogs_nas_5gs_encode_sor_transparent_container(ogs_pkbuf_t *pkbuf, ogs_nas_sor_transparent_container_t *sor_transparent_container)
{
    int size = 0;
    int target;

    ogs_assert(sor_transparent_container);
    ogs_assert(sor_transparent_container->buffer);

    size = sizeof(sor_transparent_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(sor_transparent_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = sor_transparent_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, sor_transparent_container->buffer, size);

    ogs_trace("  SOR_TRANSPARENT_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return sor_transparent_container->length + sizeof(sor_transparent_container->length);
}

/* 9.11.3.51A Supported codec list
 * O TLV 5-n */
int ogs_nas_5gs_decode_supported_codec_list(ogs_nas_supported_codec_list_t *supported_codec_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_supported_codec_list_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_supported_codec_list_t *)pkbuf->data;

    supported_codec_list->length = source->length;
    size = supported_codec_list->length + sizeof(supported_codec_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*supported_codec_list) < size) return -1;
    memcpy(supported_codec_list, pkbuf->data - size, size);

    ogs_trace("  SUPPORTED_CODEC_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_supported_codec_list(ogs_pkbuf_t *pkbuf, ogs_nas_supported_codec_list_t *supported_codec_list)
{
    int size = supported_codec_list->length + sizeof(supported_codec_list->length);
    ogs_nas_supported_codec_list_t target;

    memcpy(&target, supported_codec_list, sizeof(ogs_nas_supported_codec_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SUPPORTED_CODEC_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.52 Time zone
 * O TV 2 */
int ogs_nas_5gs_decode_time_zone(ogs_nas_time_zone_t *time_zone, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_time_zone_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(time_zone, pkbuf->data - size, size);

    ogs_trace("  TIME_ZONE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_time_zone(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_t *time_zone)
{
    int size = sizeof(ogs_nas_time_zone_t);
    ogs_nas_time_zone_t target;

    memcpy(&target, time_zone, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TIME_ZONE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.53 Time zone and time
 * O TV 8 */
int ogs_nas_5gs_decode_time_zone_and_time(ogs_nas_time_zone_and_time_t *time_zone_and_time, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_time_zone_and_time_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(time_zone_and_time, pkbuf->data - size, size);

    ogs_trace("  TIME_ZONE_AND_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_time_zone_and_time(ogs_pkbuf_t *pkbuf, ogs_nas_time_zone_and_time_t *time_zone_and_time)
{
    int size = sizeof(ogs_nas_time_zone_and_time_t);
    ogs_nas_time_zone_and_time_t target;

    memcpy(&target, time_zone_and_time, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TIME_ZONE_AND_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.54 UE security capability
 * O TLV 4-10 */
int ogs_nas_5gs_decode_ue_security_capability(ogs_nas_ue_security_capability_t *ue_security_capability, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_security_capability_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_security_capability_t *)pkbuf->data;

    ue_security_capability->length = source->length;
    size = ue_security_capability->length + sizeof(ue_security_capability->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_security_capability) < size) return -1;
    memcpy(ue_security_capability, pkbuf->data - size, size);

    ogs_trace("  UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_security_capability(ogs_pkbuf_t *pkbuf, ogs_nas_ue_security_capability_t *ue_security_capability)
{
    int size = ue_security_capability->length + sizeof(ue_security_capability->length);
    ogs_nas_ue_security_capability_t target;

    memcpy(&target, ue_security_capability, sizeof(ogs_nas_ue_security_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_SECURITY_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.55 UE usage setting
 * O TLV 3 */
int ogs_nas_5gs_decode_ue_usage_setting(ogs_nas_ue_usage_setting_t *ue_usage_setting, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_usage_setting_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_usage_setting_t *)pkbuf->data;

    ue_usage_setting->length = source->length;
    size = ue_usage_setting->length + sizeof(ue_usage_setting->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_usage_setting) < size) return -1;
    memcpy(ue_usage_setting, pkbuf->data - size, size);

    ogs_trace("  UE_USAGE_SETTING - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_usage_setting(ogs_pkbuf_t *pkbuf, ogs_nas_ue_usage_setting_t *ue_usage_setting)
{
    int size = ue_usage_setting->length + sizeof(ue_usage_setting->length);
    ogs_nas_ue_usage_setting_t target;

    memcpy(&target, ue_usage_setting, sizeof(ogs_nas_ue_usage_setting_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_USAGE_SETTING - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.56 UE status
 * O TLV 3 */
int ogs_nas_5gs_decode_ue_status(ogs_nas_ue_status_t *ue_status, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_status_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_status_t *)pkbuf->data;

    ue_status->length = source->length;
    size = ue_status->length + sizeof(ue_status->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_status) < size) return -1;
    memcpy(ue_status, pkbuf->data - size, size);

    ogs_trace("  UE_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_status(ogs_pkbuf_t *pkbuf, ogs_nas_ue_status_t *ue_status)
{
    int size = ue_status->length + sizeof(ue_status->length);
    ogs_nas_ue_status_t target;

    memcpy(&target, ue_status, sizeof(ogs_nas_ue_status_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.57 Uplink data status
 * O TLV 4-34 */
int ogs_nas_5gs_decode_uplink_data_status(ogs_nas_uplink_data_status_t *uplink_data_status, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_uplink_data_status_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_uplink_data_status_t *)pkbuf->data;

    uplink_data_status->length = source->length;
    size = uplink_data_status->length + sizeof(uplink_data_status->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*uplink_data_status) < size) return -1;
    memcpy(uplink_data_status, pkbuf->data - size, size);

    uplink_data_status->psi = be16toh(uplink_data_status->psi);

    ogs_trace("  UPLINK_DATA_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_uplink_data_status(ogs_pkbuf_t *pkbuf, ogs_nas_uplink_data_status_t *uplink_data_status)
{
    int size = uplink_data_status->length + sizeof(uplink_data_status->length);
    ogs_nas_uplink_data_status_t target;

    memcpy(&target, uplink_data_status, sizeof(ogs_nas_uplink_data_status_t));
    target.psi = htobe16(uplink_data_status->psi);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UPLINK_DATA_STATUS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.6 5GS registration result
 * M LV 2 */
int ogs_nas_5gs_decode_5gs_registration_result(ogs_nas_5gs_registration_result_t *registration_result, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_registration_result_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_registration_result_t *)pkbuf->data;

    registration_result->length = source->length;
    size = registration_result->length + sizeof(registration_result->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*registration_result) < size) return -1;
    memcpy(registration_result, pkbuf->data - size, size);

    ogs_trace("  5GS_REGISTRATION_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_registration_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_registration_result_t *registration_result)
{
    int size = registration_result->length + sizeof(registration_result->length);
    ogs_nas_5gs_registration_result_t target;

    memcpy(&target, registration_result, sizeof(ogs_nas_5gs_registration_result_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_REGISTRATION_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.68 UE radio capability ID
 * O TLV 3-n */
int ogs_nas_5gs_decode_ue_radio_capability_id(ogs_nas_ue_radio_capability_id_t *ue_radio_capability_id, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_radio_capability_id_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_radio_capability_id_t *)pkbuf->data;

    ue_radio_capability_id->length = source->length;
    size = ue_radio_capability_id->length + sizeof(ue_radio_capability_id->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_radio_capability_id) < size) return -1;
    memcpy(ue_radio_capability_id, pkbuf->data - size, size);

    ogs_trace("  UE_RADIO_CAPABILITY_ID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_radio_capability_id(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_id_t *ue_radio_capability_id)
{
    int size = ue_radio_capability_id->length + sizeof(ue_radio_capability_id->length);
    ogs_nas_ue_radio_capability_id_t target;

    memcpy(&target, ue_radio_capability_id, sizeof(ogs_nas_ue_radio_capability_id_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_RADIO_CAPABILITY_ID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.69 UE radio capability ID deletion indication
 * O TV 1 */
int ogs_nas_5gs_decode_ue_radio_capability_id_deletion_indication(ogs_nas_ue_radio_capability_id_deletion_indication_t *ue_radio_capability_id_deletion_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_ue_radio_capability_id_deletion_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(ue_radio_capability_id_deletion_indication, pkbuf->data - size, size);

    ogs_trace("  UE_RADIO_CAPABILITY_ID_DELETION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_radio_capability_id_deletion_indication(ogs_pkbuf_t *pkbuf, ogs_nas_ue_radio_capability_id_deletion_indication_t *ue_radio_capability_id_deletion_indication)
{
    int size = sizeof(ogs_nas_ue_radio_capability_id_deletion_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ue_radio_capability_id_deletion_indication, size);

    ogs_trace("  UE_RADIO_CAPABILITY_ID_DELETION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.7 5GS registration type
 * M V 1/2 */
int ogs_nas_5gs_decode_5gs_registration_type(ogs_nas_5gs_registration_type_t *registration_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_5gs_registration_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(registration_type, pkbuf->data - size, size);

    ogs_trace("  5GS_REGISTRATION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_registration_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_registration_type_t *registration_type)
{
    int size = sizeof(ogs_nas_5gs_registration_type_t);
    ogs_nas_5gs_registration_type_t target;

    memcpy(&target, registration_type, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_REGISTRATION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.70 Truncated 5G-S-TMSI configuration
 * O TLV 3 */
int ogs_nas_5gs_decode_truncated_5g_s_tmsi_configuration(ogs_nas_truncated_5g_s_tmsi_configuration_t *truncated_s_tmsi_configuration, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_truncated_5g_s_tmsi_configuration_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_truncated_5g_s_tmsi_configuration_t *)pkbuf->data;

    truncated_s_tmsi_configuration->length = source->length;
    size = truncated_s_tmsi_configuration->length + sizeof(truncated_s_tmsi_configuration->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*truncated_s_tmsi_configuration) < size) return -1;
    memcpy(truncated_s_tmsi_configuration, pkbuf->data - size, size);

    ogs_trace("  TRUNCATED_5G_S_TMSI_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_truncated_5g_s_tmsi_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_truncated_5g_s_tmsi_configuration_t *truncated_s_tmsi_configuration)
{
    int size = truncated_s_tmsi_configuration->length + sizeof(truncated_s_tmsi_configuration->length);
    ogs_nas_truncated_5g_s_tmsi_configuration_t target;

    memcpy(&target, truncated_s_tmsi_configuration, sizeof(ogs_nas_truncated_5g_s_tmsi_configuration_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  TRUNCATED_5G_S_TMSI_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.71 WUS assistance information
 * O TLV 3-n */
int ogs_nas_5gs_decode_wus_assistance_information(ogs_nas_wus_assistance_information_t *wus_assistance_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_wus_assistance_information_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_wus_assistance_information_t *)pkbuf->data;

    wus_assistance_information->length = source->length;
    size = wus_assistance_information->length + sizeof(wus_assistance_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*wus_assistance_information) < size) return -1;
    memcpy(wus_assistance_information, pkbuf->data - size, size);

    ogs_trace("  WUS_ASSISTANCE_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_wus_assistance_information(ogs_pkbuf_t *pkbuf, ogs_nas_wus_assistance_information_t *wus_assistance_information)
{
    int size = wus_assistance_information->length + sizeof(wus_assistance_information->length);
    ogs_nas_wus_assistance_information_t target;

    memcpy(&target, wus_assistance_information, sizeof(ogs_nas_wus_assistance_information_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  WUS_ASSISTANCE_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.72 N5GC indication
 * O TV 1 */
int ogs_nas_5gs_decode_n5gc_indication(ogs_nas_n5gc_indication_t *n5gc_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_n5gc_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(n5gc_indication, pkbuf->data - size, size);

    ogs_trace("  N5GC_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_n5gc_indication(ogs_pkbuf_t *pkbuf, ogs_nas_n5gc_indication_t *n5gc_indication)
{
    int size = sizeof(ogs_nas_n5gc_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, n5gc_indication, size);

    ogs_trace("  N5GC_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.73 NB-N1 mode DRX parameters
 * O TLV 3 */
int ogs_nas_5gs_decode_nb_n1_mode_drx_parameters(ogs_nas_nb_n1_mode_drx_parameters_t *nb_n1_mode_drx_parameters, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_nb_n1_mode_drx_parameters_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_nb_n1_mode_drx_parameters_t *)pkbuf->data;

    nb_n1_mode_drx_parameters->length = source->length;
    size = nb_n1_mode_drx_parameters->length + sizeof(nb_n1_mode_drx_parameters->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*nb_n1_mode_drx_parameters) < size) return -1;
    memcpy(nb_n1_mode_drx_parameters, pkbuf->data - size, size);

    ogs_trace("  NB_N1_MODE_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_nb_n1_mode_drx_parameters(ogs_pkbuf_t *pkbuf, ogs_nas_nb_n1_mode_drx_parameters_t *nb_n1_mode_drx_parameters)
{
    int size = nb_n1_mode_drx_parameters->length + sizeof(nb_n1_mode_drx_parameters->length);
    ogs_nas_nb_n1_mode_drx_parameters_t target;

    memcpy(&target, nb_n1_mode_drx_parameters, sizeof(ogs_nas_nb_n1_mode_drx_parameters_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NB_N1_MODE_DRX_PARAMETERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.74 Additional configuration indication
 * O TV 1 */
int ogs_nas_5gs_decode_additional_configuration_indication(ogs_nas_additional_configuration_indication_t *additional_configuration_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_additional_configuration_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(additional_configuration_indication, pkbuf->data - size, size);

    ogs_trace("  ADDITIONAL_CONFIGURATION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_additional_configuration_indication(ogs_pkbuf_t *pkbuf, ogs_nas_additional_configuration_indication_t *additional_configuration_indication)
{
    int size = sizeof(ogs_nas_additional_configuration_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, additional_configuration_indication, size);

    ogs_trace("  ADDITIONAL_CONFIGURATION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.75 Extended rejected NSSAI
 * O TLV 5-90 */
int ogs_nas_5gs_decode_extended_rejected_nssai(ogs_nas_extended_rejected_nssai_t *extended_rejected_nssai, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_extended_rejected_nssai_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_extended_rejected_nssai_t *)pkbuf->data;

    extended_rejected_nssai->length = source->length;
    size = extended_rejected_nssai->length + sizeof(extended_rejected_nssai->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*extended_rejected_nssai) < size) return -1;
    memcpy(extended_rejected_nssai, pkbuf->data - size, size);

    ogs_trace("  EXTENDED_REJECTED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_extended_rejected_nssai(ogs_pkbuf_t *pkbuf, ogs_nas_extended_rejected_nssai_t *extended_rejected_nssai)
{
    int size = extended_rejected_nssai->length + sizeof(extended_rejected_nssai->length);
    ogs_nas_extended_rejected_nssai_t target;

    memcpy(&target, extended_rejected_nssai, sizeof(ogs_nas_extended_rejected_nssai_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  EXTENDED_REJECTED_NSSAI - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.76 UE request type
 * O TLV 3 */
int ogs_nas_5gs_decode_ue_request_type(ogs_nas_ue_request_type_t *ue_request_type, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_request_type_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_request_type_t *)pkbuf->data;

    ue_request_type->length = source->length;
    size = ue_request_type->length + sizeof(ue_request_type->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_request_type) < size) return -1;
    memcpy(ue_request_type, pkbuf->data - size, size);

    ogs_trace("  UE_REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_request_type(ogs_pkbuf_t *pkbuf, ogs_nas_ue_request_type_t *ue_request_type)
{
    int size = ue_request_type->length + sizeof(ue_request_type->length);
    ogs_nas_ue_request_type_t target;

    memcpy(&target, ue_request_type, sizeof(ogs_nas_ue_request_type_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_REQUEST_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.77 Paging restriction
 * O TLV 3-35 */
int ogs_nas_5gs_decode_paging_restriction(ogs_nas_paging_restriction_t *paging_restriction, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_paging_restriction_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_paging_restriction_t *)pkbuf->data;

    paging_restriction->length = source->length;
    size = paging_restriction->length + sizeof(paging_restriction->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*paging_restriction) < size) return -1;
    memcpy(paging_restriction, pkbuf->data - size, size);

    ogs_trace("  PAGING_RESTRICTION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_paging_restriction(ogs_pkbuf_t *pkbuf, ogs_nas_paging_restriction_t *paging_restriction)
{
    int size = paging_restriction->length + sizeof(paging_restriction->length);
    ogs_nas_paging_restriction_t target;

    memcpy(&target, paging_restriction, sizeof(ogs_nas_paging_restriction_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PAGING_RESTRICTION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.79 NID
 * O TLV 8 */
int ogs_nas_5gs_decode_nid(ogs_nas_nid_t *nid, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_nid_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_nid_t *)pkbuf->data;

    nid->length = source->length;
    size = nid->length + sizeof(nid->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*nid) < size) return -1;
    memcpy(nid, pkbuf->data - size, size);

    ogs_trace("  NID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_nid(ogs_pkbuf_t *pkbuf, ogs_nas_nid_t *nid)
{
    int size = nid->length + sizeof(nid->length);
    ogs_nas_nid_t target;

    memcpy(&target, nid, sizeof(ogs_nas_nid_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  NID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.8 5GS tracking area identity
 * O TV 7 */
int ogs_nas_5gs_decode_5gs_tracking_area_identity(ogs_nas_5gs_tracking_area_identity_t *tracking_area_identity, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_5gs_tracking_area_identity_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(tracking_area_identity, pkbuf->data - size, size);

    tracking_area_identity->tac = ogs_be24toh(tracking_area_identity->tac);

    ogs_trace("  5GS_TRACKING_AREA_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_tracking_area_identity(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_tracking_area_identity_t *tracking_area_identity)
{
    int size = sizeof(ogs_nas_5gs_tracking_area_identity_t);
    ogs_nas_5gs_tracking_area_identity_t target;

    memcpy(&target, tracking_area_identity, size);
    target.tac = ogs_htobe24(tracking_area_identity->tac);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_TRACKING_AREA_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.80 PEIPS assistance information
 * O TLV 3-n */
int ogs_nas_5gs_decode_peips_assistance_information(ogs_nas_peips_assistance_information_t *peips_assistance_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_peips_assistance_information_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_peips_assistance_information_t *)pkbuf->data;

    peips_assistance_information->length = source->length;
    size = peips_assistance_information->length + sizeof(peips_assistance_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*peips_assistance_information) < size) return -1;
    memcpy(peips_assistance_information, pkbuf->data - size, size);

    ogs_trace("  PEIPS_ASSISTANCE_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_peips_assistance_information(ogs_pkbuf_t *pkbuf, ogs_nas_peips_assistance_information_t *peips_assistance_information)
{
    int size = peips_assistance_information->length + sizeof(peips_assistance_information->length);
    ogs_nas_peips_assistance_information_t target;

    memcpy(&target, peips_assistance_information, sizeof(ogs_nas_peips_assistance_information_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PEIPS_ASSISTANCE_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.81 5GS additional request result
 * O TLV 3 */
int ogs_nas_5gs_decode_5gs_additional_request_result(ogs_nas_5gs_additional_request_result_t *additional_request_result, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_additional_request_result_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_additional_request_result_t *)pkbuf->data;

    additional_request_result->length = source->length;
    size = additional_request_result->length + sizeof(additional_request_result->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*additional_request_result) < size) return -1;
    memcpy(additional_request_result, pkbuf->data - size, size);

    ogs_trace("  5GS_ADDITIONAL_REQUEST_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_additional_request_result(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_additional_request_result_t *additional_request_result)
{
    int size = additional_request_result->length + sizeof(additional_request_result->length);
    ogs_nas_5gs_additional_request_result_t target;

    memcpy(&target, additional_request_result, sizeof(ogs_nas_5gs_additional_request_result_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_ADDITIONAL_REQUEST_RESULT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.82 NSSRG information
 * O TLV-E 7-4099 */
int ogs_nas_5gs_decode_nssrg_information(ogs_nas_nssrg_information_t *nssrg_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_nssrg_information_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_nssrg_information_t *)pkbuf->data;

    nssrg_information->length = be16toh(source->length);
    size = nssrg_information->length + sizeof(nssrg_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    nssrg_information->buffer = pkbuf->data - size + sizeof(nssrg_information->length);

    ogs_trace("  NSSRG_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)nssrg_information->buffer, nssrg_information->length);

    return size;
}

int ogs_nas_5gs_encode_nssrg_information(ogs_pkbuf_t *pkbuf, ogs_nas_nssrg_information_t *nssrg_information)
{
    int size = 0;
    int target;

    ogs_assert(nssrg_information);
    ogs_assert(nssrg_information->buffer);

    size = sizeof(nssrg_information->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(nssrg_information->length);
    memcpy(pkbuf->data - size, &target, size);

    size = nssrg_information->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, nssrg_information->buffer, size);

    ogs_trace("  NSSRG_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return nssrg_information->length + sizeof(nssrg_information->length);
}

/* 9.11.3.83 List of PLMNs to be used in disaster condition
 * O TLV 2-n */
int ogs_nas_5gs_decode_list_of_plmns_to_be_used_in_disaster_condition(ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t *list_of_plmns_to_be_used_in_disaster_condition, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t *)pkbuf->data;

    list_of_plmns_to_be_used_in_disaster_condition->length = source->length;
    size = list_of_plmns_to_be_used_in_disaster_condition->length + sizeof(list_of_plmns_to_be_used_in_disaster_condition->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*list_of_plmns_to_be_used_in_disaster_condition) < size) return -1;
    memcpy(list_of_plmns_to_be_used_in_disaster_condition, pkbuf->data - size, size);

    ogs_trace("  LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_list_of_plmns_to_be_used_in_disaster_condition(ogs_pkbuf_t *pkbuf, ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t *list_of_plmns_to_be_used_in_disaster_condition)
{
    int size = list_of_plmns_to_be_used_in_disaster_condition->length + sizeof(list_of_plmns_to_be_used_in_disaster_condition->length);
    ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t target;

    memcpy(&target, list_of_plmns_to_be_used_in_disaster_condition, sizeof(ogs_nas_list_of_plmns_to_be_used_in_disaster_condition_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  LIST_OF_PLMNS_TO_BE_USED_IN_DISASTER_CONDITION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.84 Registration wait range
 * O TLV 4 */
int ogs_nas_5gs_decode_registration_wait_range(ogs_nas_registration_wait_range_t *registration_wait_range, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_registration_wait_range_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_registration_wait_range_t *)pkbuf->data;

    registration_wait_range->length = source->length;
    size = registration_wait_range->length + sizeof(registration_wait_range->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*registration_wait_range) < size) return -1;
    memcpy(registration_wait_range, pkbuf->data - size, size);

    ogs_trace("  REGISTRATION_WAIT_RANGE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_registration_wait_range(ogs_pkbuf_t *pkbuf, ogs_nas_registration_wait_range_t *registration_wait_range)
{
    int size = registration_wait_range->length + sizeof(registration_wait_range->length);
    ogs_nas_registration_wait_range_t target;

    memcpy(&target, registration_wait_range, sizeof(ogs_nas_registration_wait_range_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  REGISTRATION_WAIT_RANGE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.85 PLMN identity
 * O TLV 5 */
int ogs_nas_5gs_decode_plmn_identity(ogs_nas_plmn_identity_t *plmn_identity, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_plmn_identity_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_plmn_identity_t *)pkbuf->data;

    plmn_identity->length = source->length;
    size = plmn_identity->length + sizeof(plmn_identity->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*plmn_identity) < size) return -1;
    memcpy(plmn_identity, pkbuf->data - size, size);

    ogs_trace("  PLMN_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_plmn_identity(ogs_pkbuf_t *pkbuf, ogs_nas_plmn_identity_t *plmn_identity)
{
    int size = plmn_identity->length + sizeof(plmn_identity->length);
    ogs_nas_plmn_identity_t target;

    memcpy(&target, plmn_identity, sizeof(ogs_nas_plmn_identity_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PLMN_IDENTITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.86 Extended CAG information list
 * O TLV-E 3-n */
int ogs_nas_5gs_decode_extended_cag_information_list(ogs_nas_extended_cag_information_list_t *extended_cag_information_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_extended_cag_information_list_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_extended_cag_information_list_t *)pkbuf->data;

    extended_cag_information_list->length = be16toh(source->length);
    size = extended_cag_information_list->length + sizeof(extended_cag_information_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    extended_cag_information_list->buffer = pkbuf->data - size + sizeof(extended_cag_information_list->length);

    ogs_trace("  EXTENDED_CAG_INFORMATION_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)extended_cag_information_list->buffer, extended_cag_information_list->length);

    return size;
}

int ogs_nas_5gs_encode_extended_cag_information_list(ogs_pkbuf_t *pkbuf, ogs_nas_extended_cag_information_list_t *extended_cag_information_list)
{
    int size = 0;
    int target;

    ogs_assert(extended_cag_information_list);
    ogs_assert(extended_cag_information_list->buffer);

    size = sizeof(extended_cag_information_list->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(extended_cag_information_list->length);
    memcpy(pkbuf->data - size, &target, size);

    size = extended_cag_information_list->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, extended_cag_information_list->buffer, size);

    ogs_trace("  EXTENDED_CAG_INFORMATION_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return extended_cag_information_list->length + sizeof(extended_cag_information_list->length);
}

/* 9.11.3.87 NSAG information
 * O TLV-E 9-3143 */
int ogs_nas_5gs_decode_nsag_information(ogs_nas_nsag_information_t *nsag_information, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_nsag_information_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_nsag_information_t *)pkbuf->data;

    nsag_information->length = be16toh(source->length);
    size = nsag_information->length + sizeof(nsag_information->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    nsag_information->buffer = pkbuf->data - size + sizeof(nsag_information->length);

    ogs_trace("  NSAG_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)nsag_information->buffer, nsag_information->length);

    return size;
}

int ogs_nas_5gs_encode_nsag_information(ogs_pkbuf_t *pkbuf, ogs_nas_nsag_information_t *nsag_information)
{
    int size = 0;
    int target;

    ogs_assert(nsag_information);
    ogs_assert(nsag_information->buffer);

    size = sizeof(nsag_information->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(nsag_information->length);
    memcpy(pkbuf->data - size, &target, size);

    size = nsag_information->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, nsag_information->buffer, size);

    ogs_trace("  NSAG_INFORMATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return nsag_information->length + sizeof(nsag_information->length);
}

/* 9.11.3.9 5GS tracking area identity list
 * O TLV 9-114 */
int ogs_nas_5gs_decode_5gs_tracking_area_identity_list(ogs_nas_5gs_tracking_area_identity_list_t *tracking_area_identity_list, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_tracking_area_identity_list_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_tracking_area_identity_list_t *)pkbuf->data;

    tracking_area_identity_list->length = source->length;
    size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*tracking_area_identity_list) < size) return -1;
    memcpy(tracking_area_identity_list, pkbuf->data - size, size);

    ogs_trace("  5GS_TRACKING_AREA_IDENTITY_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_tracking_area_identity_list(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_tracking_area_identity_list_t *tracking_area_identity_list)
{
    int size = tracking_area_identity_list->length + sizeof(tracking_area_identity_list->length);
    ogs_nas_5gs_tracking_area_identity_list_t target;

    memcpy(&target, tracking_area_identity_list, sizeof(ogs_nas_5gs_tracking_area_identity_list_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_TRACKING_AREA_IDENTITY_LIST - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.91 Priority indicator
 * O TV 1 */
int ogs_nas_5gs_decode_priority_indicator(ogs_nas_priority_indicator_t *priority_indicator, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_priority_indicator_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(priority_indicator, pkbuf->data - size, size);

    ogs_trace("  PRIORITY_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_priority_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_priority_indicator_t *priority_indicator)
{
    int size = sizeof(ogs_nas_priority_indicator_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, priority_indicator, size);

    ogs_trace("  PRIORITY_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.3.9A 5GS update type
 * O TLV 3 */
int ogs_nas_5gs_decode_5gs_update_type(ogs_nas_5gs_update_type_t *update_type, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gs_update_type_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gs_update_type_t *)pkbuf->data;

    update_type->length = source->length;
    size = update_type->length + sizeof(update_type->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*update_type) < size) return -1;
    memcpy(update_type, pkbuf->data - size, size);

    ogs_trace("  5GS_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gs_update_type(ogs_pkbuf_t *pkbuf, ogs_nas_5gs_update_type_t *update_type)
{
    int size = update_type->length + sizeof(update_type->length);
    ogs_nas_5gs_update_type_t target;

    memcpy(&target, update_type, sizeof(ogs_nas_5gs_update_type_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GS_UPDATE_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.1 5GSM capability
 * O TLV 3-15 */
int ogs_nas_5gs_decode_5gsm_capability(ogs_nas_5gsm_capability_t *gsm_capability, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gsm_capability_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gsm_capability_t *)pkbuf->data;

    gsm_capability->length = source->length;
    size = gsm_capability->length + sizeof(gsm_capability->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gsm_capability) < size) return -1;
    memcpy(gsm_capability, pkbuf->data - size, size);

    ogs_trace("  5GSM_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gsm_capability(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_capability_t *gsm_capability)
{
    int size = gsm_capability->length + sizeof(gsm_capability->length);
    ogs_nas_5gsm_capability_t target;

    memcpy(&target, gsm_capability, sizeof(ogs_nas_5gsm_capability_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GSM_CAPABILITY - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.10 PDU address
 * O TLV 11 */
int ogs_nas_5gs_decode_pdu_address(ogs_nas_pdu_address_t *pdu_address, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_pdu_address_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_pdu_address_t *)pkbuf->data;

    pdu_address->length = source->length;
    size = pdu_address->length + sizeof(pdu_address->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*pdu_address) < size) return -1;
    memcpy(pdu_address, pkbuf->data - size, size);

    ogs_trace("  PDU_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_address(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_address_t *pdu_address)
{
    int size = pdu_address->length + sizeof(pdu_address->length);
    ogs_nas_pdu_address_t target;

    memcpy(&target, pdu_address, sizeof(ogs_nas_pdu_address_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDU_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.11 PDU session type
 * O TV 1 */
int ogs_nas_5gs_decode_pdu_session_type(ogs_nas_pdu_session_type_t *pdu_session_type, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_pdu_session_type_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(pdu_session_type, pkbuf->data - size, size);

    ogs_trace("  PDU_SESSION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_type(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_type_t *pdu_session_type)
{
    int size = sizeof(ogs_nas_pdu_session_type_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, pdu_session_type, size);

    ogs_trace("  PDU_SESSION_TYPE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.12 QoS flow descriptions
 * O TLV-E 6-65538 */
int ogs_nas_5gs_decode_qos_flow_descriptions(ogs_nas_qos_flow_descriptions_t *qos_flow_descriptions, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_qos_flow_descriptions_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_qos_flow_descriptions_t *)pkbuf->data;

    qos_flow_descriptions->length = be16toh(source->length);
    size = qos_flow_descriptions->length + sizeof(qos_flow_descriptions->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    qos_flow_descriptions->buffer = pkbuf->data - size + sizeof(qos_flow_descriptions->length);

    ogs_trace("  QOS_FLOW_DESCRIPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)qos_flow_descriptions->buffer, qos_flow_descriptions->length);

    return size;
}

int ogs_nas_5gs_encode_qos_flow_descriptions(ogs_pkbuf_t *pkbuf, ogs_nas_qos_flow_descriptions_t *qos_flow_descriptions)
{
    int size = 0;
    int target;

    ogs_assert(qos_flow_descriptions);
    ogs_assert(qos_flow_descriptions->buffer);

    size = sizeof(qos_flow_descriptions->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(qos_flow_descriptions->length);
    memcpy(pkbuf->data - size, &target, size);

    size = qos_flow_descriptions->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, qos_flow_descriptions->buffer, size);

    ogs_trace("  QOS_FLOW_DESCRIPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return qos_flow_descriptions->length + sizeof(qos_flow_descriptions->length);
}

/* 9.11.4.13 QoS rules
 * M LV-E 6-65538 */
int ogs_nas_5gs_decode_qos_rules(ogs_nas_qos_rules_t *qos_rules, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_qos_rules_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_qos_rules_t *)pkbuf->data;

    qos_rules->length = be16toh(source->length);
    size = qos_rules->length + sizeof(qos_rules->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    qos_rules->buffer = pkbuf->data - size + sizeof(qos_rules->length);

    ogs_trace("  QOS_RULES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)qos_rules->buffer, qos_rules->length);

    return size;
}

int ogs_nas_5gs_encode_qos_rules(ogs_pkbuf_t *pkbuf, ogs_nas_qos_rules_t *qos_rules)
{
    int size = 0;
    int target;

    ogs_assert(qos_rules);
    ogs_assert(qos_rules->buffer);

    size = sizeof(qos_rules->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(qos_rules->length);
    memcpy(pkbuf->data - size, &target, size);

    size = qos_rules->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, qos_rules->buffer, size);

    ogs_trace("  QOS_RULES - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return qos_rules->length + sizeof(qos_rules->length);
}

/* 9.11.4.14 Session-AMBR
 * M LV 7 */
int ogs_nas_5gs_decode_session_ambr(ogs_nas_session_ambr_t *session_ambr, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_session_ambr_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_session_ambr_t *)pkbuf->data;

    session_ambr->length = source->length;
    size = session_ambr->length + sizeof(session_ambr->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*session_ambr) < size) return -1;
    memcpy(session_ambr, pkbuf->data - size, size);

    session_ambr->downlink.value = be16toh(source->downlink.value);
    session_ambr->uplink.value = be16toh(source->uplink.value);

    ogs_trace("  SESSION_AMBR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_session_ambr(ogs_pkbuf_t *pkbuf, ogs_nas_session_ambr_t *session_ambr)
{
    int size = session_ambr->length + sizeof(session_ambr->length);
    ogs_nas_session_ambr_t target;

    memcpy(&target, session_ambr, sizeof(ogs_nas_session_ambr_t));
    target.downlink.value = htobe16(session_ambr->downlink.value);
    target.uplink.value = htobe16(session_ambr->uplink.value);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SESSION_AMBR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.15 SM PDU DN request container
 * O TLV 3-255 */
int ogs_nas_5gs_decode_sm_pdu_dn_request_container(ogs_nas_sm_pdu_dn_request_container_t *sm_pdu_dn_request_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_sm_pdu_dn_request_container_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_sm_pdu_dn_request_container_t *)pkbuf->data;

    sm_pdu_dn_request_container->length = source->length;
    size = sm_pdu_dn_request_container->length + sizeof(sm_pdu_dn_request_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*sm_pdu_dn_request_container) < size) return -1;
    memcpy(sm_pdu_dn_request_container, pkbuf->data - size, size);

    ogs_trace("  SM_PDU_DN_REQUEST_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_sm_pdu_dn_request_container(ogs_pkbuf_t *pkbuf, ogs_nas_sm_pdu_dn_request_container_t *sm_pdu_dn_request_container)
{
    int size = sm_pdu_dn_request_container->length + sizeof(sm_pdu_dn_request_container->length);
    ogs_nas_sm_pdu_dn_request_container_t target;

    memcpy(&target, sm_pdu_dn_request_container, sizeof(ogs_nas_sm_pdu_dn_request_container_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SM_PDU_DN_REQUEST_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.16 SSC mode
 * O TV 1 */
int ogs_nas_5gs_decode_ssc_mode(ogs_nas_ssc_mode_t *ssc_mode, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_ssc_mode_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(ssc_mode, pkbuf->data - size, size);

    ogs_trace("  SSC_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ssc_mode(ogs_pkbuf_t *pkbuf, ogs_nas_ssc_mode_t *ssc_mode)
{
    int size = sizeof(ogs_nas_ssc_mode_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, ssc_mode, size);

    ogs_trace("  SSC_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.17 Re-attempt indicator
 * O TLV 3 */
int ogs_nas_5gs_decode_re_attempt_indicator(ogs_nas_re_attempt_indicator_t *re_attempt_indicator, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_re_attempt_indicator_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_re_attempt_indicator_t *)pkbuf->data;

    re_attempt_indicator->length = source->length;
    size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*re_attempt_indicator) < size) return -1;
    memcpy(re_attempt_indicator, pkbuf->data - size, size);

    ogs_trace("  RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_re_attempt_indicator_t *re_attempt_indicator)
{
    int size = re_attempt_indicator->length + sizeof(re_attempt_indicator->length);
    ogs_nas_re_attempt_indicator_t target;

    memcpy(&target, re_attempt_indicator, sizeof(ogs_nas_re_attempt_indicator_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.18 5GSM network feature support
 * O TLV 3-15 */
int ogs_nas_5gs_decode_5gsm_network_feature_support(ogs_nas_5gsm_network_feature_support_t *gsm_network_feature_support, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gsm_network_feature_support_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gsm_network_feature_support_t *)pkbuf->data;

    gsm_network_feature_support->length = source->length;
    size = gsm_network_feature_support->length + sizeof(gsm_network_feature_support->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gsm_network_feature_support) < size) return -1;
    memcpy(gsm_network_feature_support, pkbuf->data - size, size);

    ogs_trace("  5GSM_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gsm_network_feature_support(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_network_feature_support_t *gsm_network_feature_support)
{
    int size = gsm_network_feature_support->length + sizeof(gsm_network_feature_support->length);
    ogs_nas_5gsm_network_feature_support_t target;

    memcpy(&target, gsm_network_feature_support, sizeof(ogs_nas_5gsm_network_feature_support_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GSM_NETWORK_FEATURE_SUPPORT - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.2 5GSM cause
 * O TV 2 */
int ogs_nas_5gs_decode_5gsm_cause(ogs_nas_5gsm_cause_t *gsm_cause, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_5gsm_cause_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(gsm_cause, pkbuf->data - size, size);

    ogs_trace("  5GSM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gsm_cause(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_cause_t *gsm_cause)
{
    int size = sizeof(ogs_nas_5gsm_cause_t);
    ogs_nas_5gsm_cause_t target;

    memcpy(&target, gsm_cause, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GSM_CAUSE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.20 Serving PLMN rate control
 * O TLV 4 */
int ogs_nas_5gs_decode_serving_plmn_rate_control(ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_serving_plmn_rate_control_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_serving_plmn_rate_control_t *)pkbuf->data;

    serving_plmn_rate_control->length = source->length;
    size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*serving_plmn_rate_control) < size) return -1;
    memcpy(serving_plmn_rate_control, pkbuf->data - size, size);

    ogs_trace("  SERVING_PLMN_RATE_CONTROL - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_serving_plmn_rate_control(ogs_pkbuf_t *pkbuf, ogs_nas_serving_plmn_rate_control_t *serving_plmn_rate_control)
{
    int size = serving_plmn_rate_control->length + sizeof(serving_plmn_rate_control->length);
    ogs_nas_serving_plmn_rate_control_t target;

    memcpy(&target, serving_plmn_rate_control, sizeof(ogs_nas_serving_plmn_rate_control_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  SERVING_PLMN_RATE_CONTROL - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.21 5GSM congestion re-attempt indicator
 * O TLV 3 */
int ogs_nas_5gs_decode_5gsm_congestion_re_attempt_indicator(ogs_nas_5gsm_congestion_re_attempt_indicator_t *gsm_congestion_re_attempt_indicator, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_5gsm_congestion_re_attempt_indicator_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_5gsm_congestion_re_attempt_indicator_t *)pkbuf->data;

    gsm_congestion_re_attempt_indicator->length = source->length;
    size = gsm_congestion_re_attempt_indicator->length + sizeof(gsm_congestion_re_attempt_indicator->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*gsm_congestion_re_attempt_indicator) < size) return -1;
    memcpy(gsm_congestion_re_attempt_indicator, pkbuf->data - size, size);

    ogs_trace("  5GSM_CONGESTION_RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_5gsm_congestion_re_attempt_indicator(ogs_pkbuf_t *pkbuf, ogs_nas_5gsm_congestion_re_attempt_indicator_t *gsm_congestion_re_attempt_indicator)
{
    int size = gsm_congestion_re_attempt_indicator->length + sizeof(gsm_congestion_re_attempt_indicator->length);
    ogs_nas_5gsm_congestion_re_attempt_indicator_t target;

    memcpy(&target, gsm_congestion_re_attempt_indicator, sizeof(ogs_nas_5gsm_congestion_re_attempt_indicator_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  5GSM_CONGESTION_RE_ATTEMPT_INDICATOR - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.22 ATSSS container
 * O TLV-E 3-65538 */
int ogs_nas_5gs_decode_atsss_container(ogs_nas_atsss_container_t *atsss_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_atsss_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_atsss_container_t *)pkbuf->data;

    atsss_container->length = be16toh(source->length);
    size = atsss_container->length + sizeof(atsss_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    atsss_container->buffer = pkbuf->data - size + sizeof(atsss_container->length);

    ogs_trace("  ATSSS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)atsss_container->buffer, atsss_container->length);

    return size;
}

int ogs_nas_5gs_encode_atsss_container(ogs_pkbuf_t *pkbuf, ogs_nas_atsss_container_t *atsss_container)
{
    int size = 0;
    int target;

    ogs_assert(atsss_container);
    ogs_assert(atsss_container->buffer);

    size = sizeof(atsss_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(atsss_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = atsss_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, atsss_container->buffer, size);

    ogs_trace("  ATSSS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return atsss_container->length + sizeof(atsss_container->length);
}

/* 9.11.4.23 Control plane only indication
 * O TV 1 */
int ogs_nas_5gs_decode_control_plane_only_indication(ogs_nas_control_plane_only_indication_t *control_plane_only_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_control_plane_only_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(control_plane_only_indication, pkbuf->data - size, size);

    ogs_trace("  CONTROL_PLANE_ONLY_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_control_plane_only_indication(ogs_pkbuf_t *pkbuf, ogs_nas_control_plane_only_indication_t *control_plane_only_indication)
{
    int size = sizeof(ogs_nas_control_plane_only_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, control_plane_only_indication, size);

    ogs_trace("  CONTROL_PLANE_ONLY_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.24 IP header compression configuration
 * O TLV 5-257 */
int ogs_nas_5gs_decode_ip_header_compression_configuration(ogs_nas_ip_header_compression_configuration_t *ip_header_compression_configuration, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ip_header_compression_configuration_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ip_header_compression_configuration_t *)pkbuf->data;

    ip_header_compression_configuration->length = source->length;
    size = ip_header_compression_configuration->length + sizeof(ip_header_compression_configuration->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ip_header_compression_configuration) < size) return -1;
    memcpy(ip_header_compression_configuration, pkbuf->data - size, size);

    ogs_trace("  IP_HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ip_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_ip_header_compression_configuration_t *ip_header_compression_configuration)
{
    int size = ip_header_compression_configuration->length + sizeof(ip_header_compression_configuration->length);
    ogs_nas_ip_header_compression_configuration_t target;

    memcpy(&target, ip_header_compression_configuration, sizeof(ogs_nas_ip_header_compression_configuration_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  IP_HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.24 Header compression configuration
 * O TLV 5-257 */
int ogs_nas_5gs_decode_header_compression_configuration(ogs_nas_header_compression_configuration_t *header_compression_configuration, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_header_compression_configuration_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_header_compression_configuration_t *)pkbuf->data;

    header_compression_configuration->length = source->length;
    size = header_compression_configuration->length + sizeof(header_compression_configuration->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*header_compression_configuration) < size) return -1;
    memcpy(header_compression_configuration, pkbuf->data - size, size);

    header_compression_configuration->max_cid = be16toh(header_compression_configuration->max_cid);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_header_compression_configuration_t *header_compression_configuration)
{
    int size = header_compression_configuration->length + sizeof(header_compression_configuration->length);
    ogs_nas_header_compression_configuration_t target;

    memcpy(&target, header_compression_configuration, sizeof(ogs_nas_header_compression_configuration_t));
    target.max_cid = htobe16(header_compression_configuration->max_cid);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.25 DS-TT Ethernet port MAC address
 * O TLV 8 */
int ogs_nas_5gs_decode_ds_tt_ethernet_port_mac_address(ogs_nas_ds_tt_ethernet_port_mac_address_t *ds_tt_ethernet_port_mac_address, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ds_tt_ethernet_port_mac_address_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ds_tt_ethernet_port_mac_address_t *)pkbuf->data;

    ds_tt_ethernet_port_mac_address->length = source->length;
    size = ds_tt_ethernet_port_mac_address->length + sizeof(ds_tt_ethernet_port_mac_address->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ds_tt_ethernet_port_mac_address) < size) return -1;
    memcpy(ds_tt_ethernet_port_mac_address, pkbuf->data - size, size);

    ogs_trace("  DS_TT_ETHERNET_PORT_MAC_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ds_tt_ethernet_port_mac_address(ogs_pkbuf_t *pkbuf, ogs_nas_ds_tt_ethernet_port_mac_address_t *ds_tt_ethernet_port_mac_address)
{
    int size = ds_tt_ethernet_port_mac_address->length + sizeof(ds_tt_ethernet_port_mac_address->length);
    ogs_nas_ds_tt_ethernet_port_mac_address_t target;

    memcpy(&target, ds_tt_ethernet_port_mac_address, sizeof(ogs_nas_ds_tt_ethernet_port_mac_address_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  DS_TT_ETHERNET_PORT_MAC_ADDRESS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.26 UE-DS-TT residence time
 * O TLV 10 */
int ogs_nas_5gs_decode_ue_ds_tt_residence_time(ogs_nas_ue_ds_tt_residence_time_t *ue_ds_tt_residence_time, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ue_ds_tt_residence_time_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ue_ds_tt_residence_time_t *)pkbuf->data;

    ue_ds_tt_residence_time->length = source->length;
    size = ue_ds_tt_residence_time->length + sizeof(ue_ds_tt_residence_time->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ue_ds_tt_residence_time) < size) return -1;
    memcpy(ue_ds_tt_residence_time, pkbuf->data - size, size);

    ogs_trace("  UE_DS_TT_RESIDENCE_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ue_ds_tt_residence_time(ogs_pkbuf_t *pkbuf, ogs_nas_ue_ds_tt_residence_time_t *ue_ds_tt_residence_time)
{
    int size = ue_ds_tt_residence_time->length + sizeof(ue_ds_tt_residence_time->length);
    ogs_nas_ue_ds_tt_residence_time_t target;

    memcpy(&target, ue_ds_tt_residence_time, sizeof(ogs_nas_ue_ds_tt_residence_time_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  UE_DS_TT_RESIDENCE_TIME - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.27 Port management information container
 * O TLV-E 8-65538 */
int ogs_nas_5gs_decode_port_management_information_container(ogs_nas_port_management_information_container_t *port_management_information_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_port_management_information_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_port_management_information_container_t *)pkbuf->data;

    port_management_information_container->length = be16toh(source->length);
    size = port_management_information_container->length + sizeof(port_management_information_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    port_management_information_container->buffer = pkbuf->data - size + sizeof(port_management_information_container->length);

    ogs_trace("  PORT_MANAGEMENT_INFORMATION_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)port_management_information_container->buffer, port_management_information_container->length);

    return size;
}

int ogs_nas_5gs_encode_port_management_information_container(ogs_pkbuf_t *pkbuf, ogs_nas_port_management_information_container_t *port_management_information_container)
{
    int size = 0;
    int target;

    ogs_assert(port_management_information_container);
    ogs_assert(port_management_information_container->buffer);

    size = sizeof(port_management_information_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(port_management_information_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = port_management_information_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, port_management_information_container->buffer, size);

    ogs_trace("  PORT_MANAGEMENT_INFORMATION_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return port_management_information_container->length + sizeof(port_management_information_container->length);
}

/* 9.11.4.28 Ethernet header compression configuration
 * O TLV 3 */
int ogs_nas_5gs_decode_ethernet_header_compression_configuration(ogs_nas_ethernet_header_compression_configuration_t *ethernet_header_compression_configuration, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_ethernet_header_compression_configuration_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_ethernet_header_compression_configuration_t *)pkbuf->data;

    ethernet_header_compression_configuration->length = source->length;
    size = ethernet_header_compression_configuration->length + sizeof(ethernet_header_compression_configuration->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*ethernet_header_compression_configuration) < size) return -1;
    memcpy(ethernet_header_compression_configuration, pkbuf->data - size, size);

    ogs_trace("  ETHERNET_HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_ethernet_header_compression_configuration(ogs_pkbuf_t *pkbuf, ogs_nas_ethernet_header_compression_configuration_t *ethernet_header_compression_configuration)
{
    int size = ethernet_header_compression_configuration->length + sizeof(ethernet_header_compression_configuration->length);
    ogs_nas_ethernet_header_compression_configuration_t target;

    memcpy(&target, ethernet_header_compression_configuration, sizeof(ogs_nas_ethernet_header_compression_configuration_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  ETHERNET_HEADER_COMPRESSION_CONFIGURATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.3 Always-on PDU session indication
 * O TV 1 */
int ogs_nas_5gs_decode_always_on_pdu_session_indication(ogs_nas_always_on_pdu_session_indication_t *always_on_pdu_session_indication, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_always_on_pdu_session_indication_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(always_on_pdu_session_indication, pkbuf->data - size, size);

    ogs_trace("  ALWAYS_ON_PDU_SESSION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_always_on_pdu_session_indication(ogs_pkbuf_t *pkbuf, ogs_nas_always_on_pdu_session_indication_t *always_on_pdu_session_indication)
{
    int size = sizeof(ogs_nas_always_on_pdu_session_indication_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, always_on_pdu_session_indication, size);

    ogs_trace("  ALWAYS_ON_PDU_SESSION_INDICATION - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.30 Requested MBS container
 * O TLV-E 8-65538 */
int ogs_nas_5gs_decode_requested_mbs_container(ogs_nas_requested_mbs_container_t *requested_mbs_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_requested_mbs_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_requested_mbs_container_t *)pkbuf->data;

    requested_mbs_container->length = be16toh(source->length);
    size = requested_mbs_container->length + sizeof(requested_mbs_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    requested_mbs_container->buffer = pkbuf->data - size + sizeof(requested_mbs_container->length);

    ogs_trace("  REQUESTED_MBS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)requested_mbs_container->buffer, requested_mbs_container->length);

    return size;
}

int ogs_nas_5gs_encode_requested_mbs_container(ogs_pkbuf_t *pkbuf, ogs_nas_requested_mbs_container_t *requested_mbs_container)
{
    int size = 0;
    int target;

    ogs_assert(requested_mbs_container);
    ogs_assert(requested_mbs_container->buffer);

    size = sizeof(requested_mbs_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(requested_mbs_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = requested_mbs_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, requested_mbs_container->buffer, size);

    ogs_trace("  REQUESTED_MBS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return requested_mbs_container->length + sizeof(requested_mbs_container->length);
}

/* 9.11.4.31 Received MBS container
 * O TLV-E 9-65538 */
int ogs_nas_5gs_decode_received_mbs_container(ogs_nas_received_mbs_container_t *received_mbs_container, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_received_mbs_container_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_received_mbs_container_t *)pkbuf->data;

    received_mbs_container->length = be16toh(source->length);
    size = received_mbs_container->length + sizeof(received_mbs_container->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    received_mbs_container->buffer = pkbuf->data - size + sizeof(received_mbs_container->length);

    ogs_trace("  RECEIVED_MBS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)received_mbs_container->buffer, received_mbs_container->length);

    return size;
}

int ogs_nas_5gs_encode_received_mbs_container(ogs_pkbuf_t *pkbuf, ogs_nas_received_mbs_container_t *received_mbs_container)
{
    int size = 0;
    int target;

    ogs_assert(received_mbs_container);
    ogs_assert(received_mbs_container->buffer);

    size = sizeof(received_mbs_container->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(received_mbs_container->length);
    memcpy(pkbuf->data - size, &target, size);

    size = received_mbs_container->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, received_mbs_container->buffer, size);

    ogs_trace("  RECEIVED_MBS_CONTAINER - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return received_mbs_container->length + sizeof(received_mbs_container->length);
}

/* 9.11.4.32 PDU session pair ID
 * O TLV 3 */
int ogs_nas_5gs_decode_pdu_session_pair_id(ogs_nas_pdu_session_pair_id_t *pdu_session_pair_id, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_pdu_session_pair_id_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_pdu_session_pair_id_t *)pkbuf->data;

    pdu_session_pair_id->length = source->length;
    size = pdu_session_pair_id->length + sizeof(pdu_session_pair_id->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*pdu_session_pair_id) < size) return -1;
    memcpy(pdu_session_pair_id, pkbuf->data - size, size);

    ogs_trace("  PDU_SESSION_PAIR_ID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_pdu_session_pair_id(ogs_pkbuf_t *pkbuf, ogs_nas_pdu_session_pair_id_t *pdu_session_pair_id)
{
    int size = pdu_session_pair_id->length + sizeof(pdu_session_pair_id->length);
    ogs_nas_pdu_session_pair_id_t target;

    memcpy(&target, pdu_session_pair_id, sizeof(ogs_nas_pdu_session_pair_id_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  PDU_SESSION_PAIR_ID - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.33 RSN
 * O TLV 3 */
int ogs_nas_5gs_decode_rsn(ogs_nas_rsn_t *rsn, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_rsn_t *source = NULL;

    if (pkbuf->len < 1) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_rsn_t *)pkbuf->data;

    rsn->length = source->length;
    size = rsn->length + sizeof(rsn->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    if (sizeof(*rsn) < size) return -1;
    memcpy(rsn, pkbuf->data - size, size);

    ogs_trace("  RSN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_rsn(ogs_pkbuf_t *pkbuf, ogs_nas_rsn_t *rsn)
{
    int size = rsn->length + sizeof(rsn->length);
    ogs_nas_rsn_t target;

    memcpy(&target, rsn, sizeof(ogs_nas_rsn_t));
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  RSN - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.4 Always-on PDU session requested
 * O TV 1 */
int ogs_nas_5gs_decode_always_on_pdu_session_requested(ogs_nas_always_on_pdu_session_requested_t *always_on_pdu_session_requested, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_always_on_pdu_session_requested_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(always_on_pdu_session_requested, pkbuf->data - size, size);

    ogs_trace("  ALWAYS_ON_PDU_SESSION_REQUESTED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_always_on_pdu_session_requested(ogs_pkbuf_t *pkbuf, ogs_nas_always_on_pdu_session_requested_t *always_on_pdu_session_requested)
{
    int size = sizeof(ogs_nas_always_on_pdu_session_requested_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, always_on_pdu_session_requested, size);

    ogs_trace("  ALWAYS_ON_PDU_SESSION_REQUESTED - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.5 Allowed SSC mode
 * O TV 1 */
int ogs_nas_5gs_decode_allowed_ssc_mode(ogs_nas_allowed_ssc_mode_t *allowed_ssc_mode, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_allowed_ssc_mode_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(allowed_ssc_mode, pkbuf->data - size, size);

    ogs_trace("  ALLOWED_SSC_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_allowed_ssc_mode(ogs_pkbuf_t *pkbuf, ogs_nas_allowed_ssc_mode_t *allowed_ssc_mode)
{
    int size = sizeof(ogs_nas_allowed_ssc_mode_t);

    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, allowed_ssc_mode, size);

    ogs_trace("  ALLOWED_SSC_MODE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.6 Extended protocol configuration options
 * O TLV-E 4-65538 */
int ogs_nas_5gs_decode_extended_protocol_configuration_options(ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_extended_protocol_configuration_options_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_extended_protocol_configuration_options_t *)pkbuf->data;

    extended_protocol_configuration_options->length = be16toh(source->length);
    size = extended_protocol_configuration_options->length + sizeof(extended_protocol_configuration_options->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    extended_protocol_configuration_options->buffer = pkbuf->data - size + sizeof(extended_protocol_configuration_options->length);

    ogs_trace("  EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)extended_protocol_configuration_options->buffer, extended_protocol_configuration_options->length);

    return size;
}

int ogs_nas_5gs_encode_extended_protocol_configuration_options(ogs_pkbuf_t *pkbuf, ogs_nas_extended_protocol_configuration_options_t *extended_protocol_configuration_options)
{
    int size = 0;
    int target;

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

/* 9.11.4.7 Integrity protection maximum data rate
 * M V 2 */
int ogs_nas_5gs_decode_integrity_protection_maximum_data_rate(ogs_nas_integrity_protection_maximum_data_rate_t *integrity_protection_maximum_data_rate, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_integrity_protection_maximum_data_rate_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(integrity_protection_maximum_data_rate, pkbuf->data - size, size);

    ogs_trace("  INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_integrity_protection_maximum_data_rate(ogs_pkbuf_t *pkbuf, ogs_nas_integrity_protection_maximum_data_rate_t *integrity_protection_maximum_data_rate)
{
    int size = sizeof(ogs_nas_integrity_protection_maximum_data_rate_t);
    ogs_nas_integrity_protection_maximum_data_rate_t target;

    memcpy(&target, integrity_protection_maximum_data_rate, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

/* 9.11.4.8 Mapped EPS bearer contexts
 * O TLV-E 7-65538 */
int ogs_nas_5gs_decode_mapped_eps_bearer_contexts(ogs_nas_mapped_eps_bearer_contexts_t *mapped_eps_bearer_contexts, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    ogs_nas_mapped_eps_bearer_contexts_t *source = NULL;

    if (pkbuf->len < 2) {
       ogs_error("Not enough pkbuf [len:%d]", pkbuf->len);
       return -1;
    }

    source = (ogs_nas_mapped_eps_bearer_contexts_t *)pkbuf->data;

    mapped_eps_bearer_contexts->length = be16toh(source->length);
    size = mapped_eps_bearer_contexts->length + sizeof(mapped_eps_bearer_contexts->length);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    mapped_eps_bearer_contexts->buffer = pkbuf->data - size + sizeof(mapped_eps_bearer_contexts->length);

    ogs_trace("  MAPPED_EPS_BEARER_CONTEXTS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, (void*)mapped_eps_bearer_contexts->buffer, mapped_eps_bearer_contexts->length);

    return size;
}

int ogs_nas_5gs_encode_mapped_eps_bearer_contexts(ogs_pkbuf_t *pkbuf, ogs_nas_mapped_eps_bearer_contexts_t *mapped_eps_bearer_contexts)
{
    int size = 0;
    int target;

    ogs_assert(mapped_eps_bearer_contexts);
    ogs_assert(mapped_eps_bearer_contexts->buffer);

    size = sizeof(mapped_eps_bearer_contexts->length);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    target = htobe16(mapped_eps_bearer_contexts->length);
    memcpy(pkbuf->data - size, &target, size);

    size = mapped_eps_bearer_contexts->length;
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, mapped_eps_bearer_contexts->buffer, size);

    ogs_trace("  MAPPED_EPS_BEARER_CONTEXTS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return mapped_eps_bearer_contexts->length + sizeof(mapped_eps_bearer_contexts->length);
}

/* 9.11.4.9 Maximum number of supported packet filters
 * O TV 3 */
int ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(ogs_nas_maximum_number_of_supported_packet_filters_t *maximum_number_of_supported_packet_filters, ogs_pkbuf_t *pkbuf)
{
    int size = sizeof(ogs_nas_maximum_number_of_supported_packet_filters_t);

    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return -1;
    }

    memcpy(maximum_number_of_supported_packet_filters, pkbuf->data - size, size);

    ogs_trace("  MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

int ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(ogs_pkbuf_t *pkbuf, ogs_nas_maximum_number_of_supported_packet_filters_t *maximum_number_of_supported_packet_filters)
{
    int size = sizeof(ogs_nas_maximum_number_of_supported_packet_filters_t);
    ogs_nas_maximum_number_of_supported_packet_filters_t target;

    memcpy(&target, maximum_number_of_supported_packet_filters, size);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &target, size);

    ogs_trace("  MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS - ");
    ogs_log_hexdump(OGS_LOG_TRACE, pkbuf->data - size, size);

    return size;
}

