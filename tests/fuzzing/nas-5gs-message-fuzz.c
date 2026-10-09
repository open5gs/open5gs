/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
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

#include <stdio.h>
#include <stdint.h>

#include "fuzzing.h"
#include "ogs-nas-5gs.h"

#define kMinInputLength 5
#define kMaxInputLength 1024

static void parse_gsm_ies(ogs_nas_5gs_message_t *message)
{
    ogs_nas_qos_rules_t *rules = NULL;
    ogs_nas_qos_flow_descriptions_t *descriptions = NULL;
    ogs_nas_extended_protocol_configuration_options_t *options = NULL;

    switch (message->gsm.h.message_type) {
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
        options = &message->gsm.pdu_session_establishment_request.
            extended_protocol_configuration_options;
        break;
    case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT:
        rules = &message->gsm.pdu_session_establishment_accept.
            authorized_qos_rules;
        descriptions = &message->gsm.pdu_session_establishment_accept.
            authorized_qos_flow_descriptions;
        options = &message->gsm.pdu_session_establishment_accept.
            extended_protocol_configuration_options;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REQUEST:
        rules = &message->gsm.pdu_session_modification_request.
            requested_qos_rules;
        descriptions = &message->gsm.pdu_session_modification_request.
            requested_qos_flow_descriptions;
        options = &message->gsm.pdu_session_modification_request.
            extended_protocol_configuration_options;
        break;
    case OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND:
        rules = &message->gsm.pdu_session_modification_command.
            authorized_qos_rules;
        descriptions = &message->gsm.pdu_session_modification_command.
            authorized_qos_flow_descriptions;
        options = &message->gsm.pdu_session_modification_command.
            extended_protocol_configuration_options;
        break;
    default:
        break;
    }

    if (rules && rules->length) {
        ogs_nas_qos_rule_t decoded[OGS_NAS_MAX_NUM_OF_QOS_RULE];
        ogs_nas_parse_qos_rules(decoded, rules);
    }
    if (descriptions && descriptions->length) {
        ogs_nas_qos_flow_description_t
            decoded[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];
        ogs_nas_parse_qos_flow_descriptions(decoded, descriptions);
    }
    if (options && options->length) {
        ogs_pco_t decoded;
        ogs_pco_parse(&decoded, options->buffer, options->length);
    }
}

static int rejected_nssai_is_valid(const ogs_nas_rejected_nssai_t *rejected)
{
    size_t offset = 0;

    if (rejected->length > sizeof(rejected->buffer))
        return 0;

    while (offset < rejected->length) {
        size_t entry_length = (uint8_t)rejected->buffer[offset] >> 4;

        if (entry_length != 1 && entry_length != 4)
            return 0;
        if (entry_length > rejected->length - offset - 1)
            return 0;

        offset += entry_length + 1;
    }

    return 1;
}

static void parse_gmm_ies(ogs_nas_5gs_message_t *message)
{
    ogs_nas_nssai_t *nssai = NULL;
    ogs_nas_rejected_nssai_t *rejected = NULL;

    switch (message->gmm.h.message_type) {
    case OGS_NAS_5GS_REGISTRATION_REQUEST:
        nssai = &message->gmm.registration_request.requested_nssai;
        break;
    case OGS_NAS_5GS_REGISTRATION_ACCEPT:
        nssai = &message->gmm.registration_accept.allowed_nssai;
        rejected = &message->gmm.registration_accept.rejected_nssai;
        break;
    case OGS_NAS_5GS_REGISTRATION_REJECT:
        rejected = &message->gmm.registration_reject.rejected_nssai;
        break;
    case OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND:
        nssai = &message->gmm.configuration_update_command.allowed_nssai;
        rejected = &message->gmm.configuration_update_command.rejected_nssai;
        break;
    default:
        break;
    }

    if (nssai && nssai->length) {
        ogs_nas_s_nssai_ie_t decoded[OGS_MAX_NUM_OF_SLICE];
        ogs_nas_parse_nssai(decoded, nssai);
    }
    if (rejected && rejected->length && rejected_nssai_is_valid(rejected)) {
        ogs_nas_rejected_s_nssai_t decoded[OGS_MAX_NUM_OF_SLICE];
        ogs_nas_parse_rejected_nssai(decoded, rejected);
    }
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{

    if (Size < kMinInputLength || Size > kMaxInputLength) {
        return 1;
    }

    if (!initialized) {
        initialize();
        ogs_log_install_domain(&__ogs_nas_domain, "nas", OGS_LOG_NONE);
    }

    int result;
    ogs_pkbuf_t *pkbuf;
    ogs_nas_5gs_message_t message;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (pkbuf == NULL) {
        return 1;
    }

    /* First byte selects the 5GS sublayer: even = 5GMM, odd = 5GSM. */
    uint8_t selector = Data[0];
    ogs_pkbuf_put_data(pkbuf, Data + 1, Size - 1);

    if (selector & 1) {
        result = ogs_nas_5gsm_decode(&message, pkbuf);
        if (result == OGS_OK)
            parse_gsm_ies(&message);
    } else {
        result = ogs_nas_5gmm_decode(&message, pkbuf);
        if (result == OGS_OK)
            parse_gmm_ies(&message);
    }

    ogs_pkbuf_free(pkbuf);

    return result;
}
