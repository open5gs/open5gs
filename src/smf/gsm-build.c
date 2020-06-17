/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "gsm-build.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __gsm_log_domain

ogs_pkbuf_t *gsm_build_pdu_session_establishment_accept(smf_sess_t *sess)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_accept_t *
        pdu_session_establishment_accept =
            &message.gsm.pdu_session_establishment_accept;

    ogs_nas_pdu_session_type_t *selected_pdu_session_type = NULL;
    ogs_nas_qos_rules_t *authorized_qos_rules = NULL;
    ogs_nas_session_ambr_t *session_ambr = NULL;
    ogs_nas_pdu_address_t *pdu_address = NULL;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];

    selected_pdu_session_type = &pdu_session_establishment_accept->
        selected_pdu_session_type;
    ogs_assert(selected_pdu_session_type);
    authorized_qos_rules = &pdu_session_establishment_accept->
        authorized_qos_rules;
    ogs_assert(authorized_qos_rules);
    session_ambr = &pdu_session_establishment_accept->session_ambr;
    ogs_assert(session_ambr);
    pdu_address = &pdu_session_establishment_accept->pdu_address;
    ogs_assert(pdu_address);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT;

    selected_pdu_session_type->type = sess->pdn.ssc_mode;
    selected_pdu_session_type->value = sess->pdn.pdn_type;

    /* Default QoS Rule */
    memset(qos_rule, 0, sizeof(qos_rule));
    qos_rule[0].identifier = 1;
    qos_rule[0].length = 6;
    qos_rule[0].code = OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE;
    qos_rule[0].DQR_bit = 1;
    qos_rule[0].num_of_packet_filter = 1;
    qos_rule[0].pf[0].direction = OGS_NAS_QOS_DIRECTION_UPLINK;
    qos_rule[0].pf[0].pf_identifier = 1;
    qos_rule[0].pf[0].length = 1;
    qos_rule[0].pf[0].num_of_component = 1;
    qos_rule[0].pf[0].component[0].type = OGS_PACKET_FILTER_MATCH_ALL;
    qos_rule[0].precedence = 1; /* lowest precedence */
    qos_rule[0].flow.segregation = 0;
    qos_rule[0].flow.identifier = 1;

    ogs_nas_build_qos_rules(authorized_qos_rules, qos_rule, 1);

    /* Session-AMBR */
    session_ambr->length = 6;
    ogs_nas_bitrate_from_uint64(
            &session_ambr->downlink, sess->pdn.ambr.downlink);
    ogs_nas_bitrate_from_uint64(
            &session_ambr->uplink, sess->pdn.ambr.uplink);

    /* PDU Address */
    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT;
    pdu_address->pdn_type = sess->pdn.paa.pdn_type;

    if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV4) {
        pdu_address->addr = sess->pdn.paa.addr;
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV4_LEN;
    } else if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV6) {
        memcpy(pdu_address->addr6,
                sess->pdn.paa.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1);
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV6_LEN;
    } else if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        pdu_address->both.addr = sess->pdn.paa.both.addr;
        memcpy(pdu_address->both.addr6,
                sess->pdn.paa.both.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1);
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV4V6_LEN;
    } else {
        ogs_error("Unexpected PDN Type %u", pdu_address->pdn_type);
        return NULL;
    }

    pkbuf = ogs_nas_5gs_plain_encode(&message);
    ogs_assert(pkbuf);

    ogs_free(authorized_qos_rules->buffer);

    return pkbuf;
}

ogs_pkbuf_t *gsm_build_pdu_session_establishment_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_reject_t *
        pdu_session_establishment_reject =
            &message.gsm.pdu_session_establishment_reject;

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT;

    pdu_session_establishment_reject->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}
