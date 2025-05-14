/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "gsmue-message.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gsm_log_domain

static int encode_pdu_session_establishment_request(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_request_t
        *pdu_session_establishment_request = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_establishment_request =
        &message->gsm.pdu_session_establishment_request;

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT) {
        size = ogs_nas_5gs_encode_pdu_session_type(pkbuf, &pdu_session_establishment_request->pdu_session_type);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT) {
        size = ogs_nas_5gs_encode_ssc_mode(pkbuf, &pdu_session_establishment_request->ssc_mode);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(pkbuf, &pdu_session_establishment_request->maximum_number_of_supported_packet_filters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_sm_pdu_dn_request_container(pkbuf, &pdu_session_establishment_request->sm_pdu_dn_request_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_request->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_capability(pkbuf, &pdu_session_establishment_request->gsm_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_establishment_accept(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_accept_t
        *pdu_session_establishment_accept = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_establishment_accept =
        &message->gsm.pdu_session_establishment_accept;

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer(pkbuf, &pdu_session_establishment_accept->rq_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_establishment_accept->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_accept->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_establishment_accept->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_establishment_accept->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_accept->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_atsss_container(pkbuf, &pdu_session_establishment_accept->atsss_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_establishment_reject(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_establishment_reject_t
        *pdu_session_establishment_reject = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_establishment_reject =
        &message->gsm.pdu_session_establishment_reject;

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_establishment_reject->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENT) {
        pdu_session_establishment_reject->allowed_ssc_mode.type = (OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_TYPE >> 4);

        size = ogs_nas_5gs_encode_allowed_ssc_mode(pkbuf, &pdu_session_establishment_reject->allowed_ssc_mode);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_establishment_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_establishment_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_modification_request(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_request_t
        *pdu_session_modification_request = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_modification_request =
        &message->gsm.pdu_session_modification_request;

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_maximum_number_of_supported_packet_filters(pkbuf, &pdu_session_modification_request->maximum_number_of_supported_packet_filters);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_integrity_protection_maximum_data_rate(pkbuf, &pdu_session_modification_request->integrity_protection_maximum_data_rate);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_rules(pkbuf, &pdu_session_modification_request->requested_qos_rules);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_qos_flow_descriptions(pkbuf, &pdu_session_modification_request->requested_qos_flow_descriptions);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_request->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_capability(pkbuf, &pdu_session_modification_request->gsm_capability);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_request->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_modification_request->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_modification_reject(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_reject_t
        *pdu_session_modification_reject = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_modification_reject =
        &message->gsm.pdu_session_modification_reject;

    if (pdu_session_modification_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_modification_command(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_command_t
        *pdu_session_modification_command = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_modification_command =
        &message->gsm.pdu_session_modification_command;

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_gprs_timer(pkbuf, &pdu_session_modification_command->rq_timer_value);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_command->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_modification_command->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_mapped_eps_bearer_contexts(pkbuf, &pdu_session_modification_command->mapped_eps_bearer_contexts);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_modification_command->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_atsss_container(pkbuf, &pdu_session_modification_command->atsss_container);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_modification_complete(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_complete_t
        *pdu_session_modification_complete = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_modification_complete =
        &message->gsm.pdu_session_modification_complete;


    if (pdu_session_modification_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_complete->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_modification_command_reject(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_modification_command_reject_t
        *pdu_session_modification_command_reject = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_modification_command_reject =
        &message->gsm.pdu_session_modification_command_reject;


    if (pdu_session_modification_command_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_modification_command_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_release_request(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_request_t
        *pdu_session_release_request = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_release_request = &message->gsm.pdu_session_release_request;

    if (pdu_session_release_request->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_request->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_request->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_request->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_release_reject(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_reject_t
        *pdu_session_release_reject = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_release_reject =
        &message->gsm.pdu_session_release_reject;

    if (pdu_session_release_reject->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_reject->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_release_command(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_command_t
        *pdu_session_release_command = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_release_command =
        &message->gsm.pdu_session_release_command;

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_eap_message(pkbuf, &pdu_session_release_command->eap_message);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_command->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_command->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int encode_pdu_session_release_complete(
        ogs_pkbuf_t *pkbuf, ogs_nas_5gs_message_t *message)
{
    ogs_nas_5gs_pdu_session_release_complete_t
        *pdu_session_release_complete = NULL;
    int encoded = 0;
    int size = 0;

    ogs_assert(message);
    pdu_session_release_complete = &message->gsm.pdu_session_release_complete;

    if (pdu_session_release_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_extended_protocol_configuration_options(pkbuf, &pdu_session_release_complete->extended_protocol_configuration_options);
        ogs_assert(size >= 0);
        encoded += size;
    }

    if (pdu_session_release_complete->presencemask & OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_PRESENT) {
        size = ogs_nas_5gs_encode_optional_type(pkbuf, OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_TYPE);
        ogs_assert(size >= 0);
        encoded += size;

        size = ogs_nas_5gs_encode_5gsm_cause(pkbuf, &pdu_session_release_complete->gsm_cause);
        ogs_assert(size >= 0);
        encoded += size;
    }

    return encoded;
}

static int decode_pdu_session_establishment_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_request_t *pdu_session_establishment_request = &message->gsm.pdu_session_establishment_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_pdu_session_type(&pdu_session_establishment_request->pdu_session_type, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_pdu_session_type() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_PDU_SESSION_TYPE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_ssc_mode(&pdu_session_establishment_request->ssc_mode, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_ssc_mode() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SSC_MODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE:
            size = ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(&pdu_session_establishment_request->maximum_number_of_supported_packet_filters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_sm_pdu_dn_request_container(&pdu_session_establishment_request->sm_pdu_dn_request_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_sm_pdu_dn_request_container() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_SM_PDU_DN_REQUEST_CONTAINER_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_5gsm_capability(&pdu_session_establishment_request->gsm_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_capability() failed");
               return size;
            }

            pdu_session_establishment_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST_5GSM_CAPABILITY_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_establishment_accept(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_accept_t *pdu_session_establishment_accept = &message->gsm.pdu_session_establishment_accept;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_ACCEPT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer(&pdu_session_establishment_accept->rq_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_RQ_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_establishment_accept->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_accept->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_establishment_accept->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_establishment_accept->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_atsss_container(&pdu_session_establishment_accept->atsss_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_atsss_container() failed");
               return size;
            }

            pdu_session_establishment_accept->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_ATSSS_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_establishment_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_establishment_reject_t *pdu_session_establishment_reject = &message->gsm.pdu_session_establishment_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_ESTABLISHMENT_REJECT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_establishment_reject->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_TYPE:
            decoded--;
            ogs_assert(ogs_pkbuf_push(pkbuf, 1));
            size = ogs_nas_5gs_decode_allowed_ssc_mode(&pdu_session_establishment_reject->allowed_ssc_mode, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_allowed_ssc_mode() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_ALLOWED_SSC_MODE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_establishment_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_establishment_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_modification_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_request_t *pdu_session_modification_request = &message->gsm.pdu_session_modification_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_TYPE:
            size = ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters(&pdu_session_modification_request->maximum_number_of_supported_packet_filters, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_maximum_number_of_supported_packet_filters() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAXIMUM_NUMBER_OF_SUPPORTED_PACKET_FILTERS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_TYPE:
            size = ogs_nas_5gs_decode_integrity_protection_maximum_data_rate(&pdu_session_modification_request->integrity_protection_maximum_data_rate, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_integrity_protection_maximum_data_rate() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_INTEGRITY_PROTECTION_MAXIMUM_DATA_RATE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_TYPE:
            size = ogs_nas_5gs_decode_qos_rules(&pdu_session_modification_request->requested_qos_rules, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_rules() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_RULES_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_TYPE:
            size = ogs_nas_5gs_decode_qos_flow_descriptions(&pdu_session_modification_request->requested_qos_flow_descriptions, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_qos_flow_descriptions() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_REQUESTED_QOS_FLOW_DESCRIPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_request->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_TYPE:
            size = ogs_nas_5gs_decode_5gsm_capability(&pdu_session_modification_request->gsm_capability, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_capability() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_5GSM_CAPABILITY_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_modification_request->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_modification_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_modification_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_reject_t *pdu_session_modification_reject = &message->gsm.pdu_session_modification_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_REJECT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_modification_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_command_t *pdu_session_modification_command = &message->gsm.pdu_session_modification_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMMAND\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_TYPE:
            size = ogs_nas_5gs_decode_gprs_timer(&pdu_session_modification_command->rq_timer_value, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_gprs_timer() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_RQ_TIMER_VALUE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_command->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_modification_command->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_TYPE:
            size = ogs_nas_5gs_decode_mapped_eps_bearer_contexts(&pdu_session_modification_command->mapped_eps_bearer_contexts, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_mapped_eps_bearer_contexts() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_MAPPED_EPS_BEARER_CONTEXTS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_TYPE:
            size = ogs_nas_5gs_decode_atsss_container(&pdu_session_modification_command->atsss_container, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_atsss_container() failed");
               return size;
            }

            pdu_session_modification_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_ATSSS_CONTAINER_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_modification_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_complete_t *pdu_session_modification_complete = &message->gsm.pdu_session_modification_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_complete->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_modification_command_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_modification_command_reject_t *pdu_session_modification_command_reject = &message->gsm.pdu_session_modification_command_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_MODIFICATION_COMMAND_REJECT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_modification_command_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_modification_command_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_release_request(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_request_t *pdu_session_release_request = &message->gsm.pdu_session_release_request;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_REQUEST\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_request->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_request->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_release_request->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_release_reject(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_reject_t *pdu_session_release_reject = &message->gsm.pdu_session_release_reject;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_REJECT\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_reject->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_reject->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_release_command(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_command_t *pdu_session_release_command = &message->gsm.pdu_session_release_command;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_COMMAND\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_TYPE:
            size = ogs_nas_5gs_decode_eap_message(&pdu_session_release_command->eap_message, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_eap_message() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EAP_MESSAGE_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_command->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_command->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

static int decode_pdu_session_release_complete(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    ogs_nas_5gs_pdu_session_release_complete_t *pdu_session_release_complete = &message->gsm.pdu_session_release_complete;
    int decoded = 0;
    int size = 0;

    ogs_trace("[NAS] Decode PDU_SESSION_RELEASE_COMPLETE\n");

    while (pkbuf->len > 0) {
        uint8_t *buffer = pkbuf->data;
        uint8_t type = (*buffer) >= 0x80 ? ((*buffer) & 0xf0) : (*buffer);

        size = sizeof(uint8_t);
        if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
           ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
           return OGS_ERROR;
        }
        decoded += size;

        switch(type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_TYPE:
            size = ogs_nas_5gs_decode_extended_protocol_configuration_options(&pdu_session_release_complete->extended_protocol_configuration_options, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_extended_protocol_configuration_options() failed");
               return size;
            }

            pdu_session_release_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
            decoded += size;
            break;
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_TYPE:
            size = ogs_nas_5gs_decode_5gsm_cause(&pdu_session_release_complete->gsm_cause, pkbuf);
            if (size < 0) {
               ogs_error("ogs_nas_5gs_decode_5gsm_cause() failed");
               return size;
            }

            pdu_session_release_complete->presencemask |= OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE_5GSM_CAUSE_PRESENT;
            decoded += size;
            break;
        default:
            ogs_error("Unknown type(0x%x) or not implemented\n", type);
            break;
        }
    }

    return decoded;
}

ogs_pkbuf_t *gsmue_encode_n1_sm_info(ogs_nas_5gs_message_t *message)
{
    ogs_pkbuf_t *pkbuf = NULL;
    int size = 0;
    int encoded = 0;

    ogs_assert(message);

    /* The Packet Buffer(ogs_pkbuf_t) for NAS message MUST make a HEADROOM.
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (!pkbuf) {
        ogs_error("ogs_pkbuf_alloc() failed");
        return NULL;
    }

    ogs_pkbuf_reserve(pkbuf, OGS_NAS_HEADROOM);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN-OGS_NAS_HEADROOM);

    /* Message type */
    size = sizeof(uint8_t);
    ogs_assert(ogs_pkbuf_pull(pkbuf, size));
    memcpy(pkbuf->data - size, &message->gsm.h.message_type, size);
    encoded += size;

    switch(message->gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
        size = encode_pdu_session_establishment_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        size = encode_pdu_session_establishment_accept(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT:
        size = encode_pdu_session_establishment_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        size = encode_pdu_session_modification_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT:
        size = encode_pdu_session_modification_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        size = encode_pdu_session_modification_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
        size = encode_pdu_session_modification_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT:
        size = encode_pdu_session_modification_command_reject(
                pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
        size = encode_pdu_session_release_request(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT:
        size = encode_pdu_session_release_reject(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
        size = encode_pdu_session_release_command(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
        size = encode_pdu_session_release_complete(pkbuf, message);
        ogs_assert(size >= 0);
        encoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented",
                message->gsm.h.message_type);
        ogs_pkbuf_free(pkbuf);
        return NULL;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, encoded));
    pkbuf->len = encoded;

    return pkbuf;
}

int gsmue_decode_n1_sm_info(ogs_nas_5gs_message_t *message, ogs_pkbuf_t *pkbuf)
{
    int size = 0;
    int decoded = 0;

    ogs_assert(message);
    ogs_assert(pkbuf);
    ogs_assert(pkbuf->data);

    size = sizeof(uint8_t);
    if (ogs_pkbuf_pull(pkbuf, size) == NULL) {
       ogs_error("ogs_pkbuf_pull() failed [size:%d]", (int)size);
       return OGS_ERROR;
    }

    memset(message, 0, sizeof(ogs_nas_5gs_message_t));
    memcpy(&message->gsm.h.message_type, pkbuf->data - size, size);
    decoded += size;

    switch(message->gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
        size = decode_pdu_session_establishment_request(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_establishment_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        size = decode_pdu_session_establishment_accept(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_establishment_accept() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT:
        size = decode_pdu_session_establishment_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_establishment_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        size = decode_pdu_session_modification_request(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_modification_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT:
        size = decode_pdu_session_modification_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_modification_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        size = decode_pdu_session_modification_command(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_modification_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMPLETE:
        size = decode_pdu_session_modification_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_modification_complete() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_REJECT:
        size = decode_pdu_session_modification_command_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_modification_command_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
        size = decode_pdu_session_release_request(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_release_request() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT:
        size = decode_pdu_session_release_reject(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_release_reject() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND:
        size = decode_pdu_session_release_command(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_release_command() failed");
           return size;
        }

        decoded += size;
        break;
    case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
        size = decode_pdu_session_release_complete(message, pkbuf);
        if (size < 0) {
           ogs_error("decode_pdu_session_release_complete() failed");
           return size;
        }

        decoded += size;
        break;
    default:
        ogs_error("Unknown message type (0x%x) or not implemented",
                message->gsm.h.message_type);
        break;
    }

    ogs_assert(ogs_pkbuf_push(pkbuf, decoded));

    return OGS_OK;
}

ogs_pkbuf_t *gsmue_build_pdu_session_establishment_accept(smf_sess_t *sess)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_accept_t *
        pdu_session_establishment_accept =
            &message.gsm.pdu_session_establishment_accept;

    ogs_nas_5gsm_cause_t *gsm_cause = NULL;
    ogs_nas_extended_protocol_configuration_options_t
        *extended_protocol_configuration_options = NULL;

    uint8_t *epco_buf = NULL;
    int16_t epco_len;

    gsm_cause = &pdu_session_establishment_accept->gsm_cause;
    ogs_assert(gsm_cause);
    extended_protocol_configuration_options =
        &pdu_session_establishment_accept->
            extended_protocol_configuration_options;
    ogs_assert(extended_protocol_configuration_options);

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT;

    /* GSM cause */
    if (sess->ue_session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV4) {
            pdu_session_establishment_accept->presencemask |=
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            *gsm_cause = OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED;
        } else if (sess->session.session_type == OGS_PDU_SESSION_TYPE_IPV6) {
            pdu_session_establishment_accept->presencemask |=
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            *gsm_cause = OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED;
        }
    }

    /* Extended protocol configuration options */
    if (sess->nas.ue_epco.buffer && sess->nas.ue_epco.length) {
        epco_buf = ogs_calloc(OGS_MAX_EPCO_LEN, sizeof(uint8_t));
        ogs_assert(epco_buf);
        epco_len = smf_pco_build(epco_buf,
                sess->nas.ue_epco.buffer, sess->nas.ue_epco.length);
        ogs_assert(epco_len > 0);
        pdu_session_establishment_accept->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        extended_protocol_configuration_options->buffer = epco_buf;
        extended_protocol_configuration_options->length = epco_len;
    }

    pkbuf = gsmue_encode_n1_sm_info(&message);
    ogs_assert(pkbuf);

cleanup:
    if (epco_buf)
        ogs_free(epco_buf);

    return pkbuf;
}

ogs_pkbuf_t *gsmue_build_pdu_session_modification_command(smf_sess_t *sess)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t message;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND;

    pkbuf = gsmue_encode_n1_sm_info(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}

ogs_pkbuf_t *gsmue_build_pdu_session_release_command(smf_sess_t *sess)
{
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t message;

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND;

    pkbuf = gsmue_encode_n1_sm_info(&message);
    ogs_assert(pkbuf);

    return pkbuf;
}
