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
    smf_bearer_t *qos_flow = NULL;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_accept_t *
        pdu_session_establishment_accept =
            &message.gsm.pdu_session_establishment_accept;

    ogs_nas_pdu_session_type_t *selected_pdu_session_type = NULL;
    ogs_nas_qos_rules_t *authorized_qos_rules = NULL;
    ogs_nas_session_ambr_t *session_ambr = NULL;
    ogs_nas_5gsm_cause_t *gsm_cause = NULL;
    ogs_nas_pdu_address_t *pdu_address = NULL;
    ogs_nas_s_nssai_t *nas_s_nssai = NULL;
    ogs_nas_qos_flow_descriptions_t *authorized_qos_flow_descriptions = NULL;
    ogs_nas_extended_protocol_configuration_options_t
        *extended_protocol_configuration_options = NULL;
    ogs_nas_dnn_t *dnn = NULL;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];
    ogs_nas_qos_flow_description_t qos_flow_description;

    uint8_t pco_buf[OGS_MAX_PCO_LEN];
    int16_t pco_len;

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
    gsm_cause = &pdu_session_establishment_accept->gsm_cause;
    ogs_assert(gsm_cause);
    nas_s_nssai = &pdu_session_establishment_accept->s_nssai;
    ogs_assert(nas_s_nssai);
    authorized_qos_flow_descriptions =
        &pdu_session_establishment_accept->authorized_qos_flow_descriptions;
    ogs_assert(authorized_qos_flow_descriptions);
    extended_protocol_configuration_options =
        &pdu_session_establishment_accept->
            extended_protocol_configuration_options;
    ogs_assert(extended_protocol_configuration_options);
    dnn = &pdu_session_establishment_accept->dnn;
    ogs_assert(dnn);

    ogs_assert(sess);
    qos_flow = smf_default_bearer_in_sess(sess);
    ogs_assert(qos_flow);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT;

    selected_pdu_session_type->type = sess->pdn.ssc_mode;
    selected_pdu_session_type->value = sess->pdn.pdn_type;

    /*
     * TS23.501
     * 5.7.1.3 QoS Rules
     *
     * A default QoS rule is required to be sent to the UE for every PDU
     * Session establishment and it is associated with a QoS Flow. For IP type
     * PDU Session or Ethernet type PDU Session, the default QoS rule is
     * the only QoS rule of a PDU Session which may contain a Packet Filter
     * Set that allows all UL packets, and in this case, the highest
     * precedence value shall be used for the QoS rule.
     *
     * As long as the default QoS rule does not contain a Packet Filter Set or
     * contains a Packet Filter Set that allows all UL packets, Reflective QoS
     * should not be applied for the QoS Flow which the default QoS rule is
     * associated with and the RQA should not be sent for this QoS Flow.
     */
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

    /*
     * TS23.501
     * 5.7.1.9 Precedence Value
     *
     * The QoS rule precedence value and the PDR precedence value determine
     * the order in which a QoS rule or a PDR, respectively, shall be evaluated.
     * The evaluation of the QoS rules or PDRs is performed in increasing order
     * of their precedence value.
     */
    qos_rule[0].precedence = 255; /* lowest precedence */
    qos_rule[0].flow.segregation = 0;
    qos_rule[0].flow.identifier = qos_flow->qfi;

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

    /* GSM cause */
    if (sess->ue_pdu_session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV4) {
            pdu_session_establishment_accept->presencemask |=
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            *gsm_cause = OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV4_ONLY_ALLOWED;
        } else if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV6) {
            pdu_session_establishment_accept->presencemask |=
                OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_5GSM_CAUSE_PRESENT;
            *gsm_cause = OGS_5GSM_CAUSE_PDU_SESSION_TYPE_IPV6_ONLY_ALLOWED;
        }
    }

    /* S-NSSAI */
    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_S_NSSAI_PRESENT;
    ogs_nas_build_s_nssai(nas_s_nssai, &sess->s_nssai);

    /* QoS flow descriptions */
    memset(&qos_flow_description, 0, sizeof(qos_flow_description));
    qos_flow_description.identifier = qos_flow->qfi;
    qos_flow_description.code = OGS_NAS_CREATE_NEW_QOS_FLOW_DESCRIPTION;
    qos_flow_description.E = 1;
    qos_flow_description.num_of_parameter = 1;
    qos_flow_description.param[0].identifier =
        OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI;
    qos_flow_description.param[0].len = 1;
    qos_flow_description.param[0].content[0] = sess->pdn.qos.qci;

    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
    authorized_qos_flow_descriptions->length = 6;
    authorized_qos_flow_descriptions->buffer = &qos_flow_description;

    /* Extended protocol configuration options */
    if (sess->nas.ue_pco.buffer && sess->nas.ue_pco.length) {
        pco_len = smf_pco_build(pco_buf,
                sess->nas.ue_pco.buffer, sess->nas.ue_pco.length);
        ogs_assert(pco_len > 0);
        pdu_session_establishment_accept->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_EXTENDED_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        extended_protocol_configuration_options->buffer = pco_buf;
        extended_protocol_configuration_options->length = pco_len;
    }

    /* DNN */
    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_DNN_PRESENT;
    dnn->length = strlen(sess->pdn.dnn);
    ogs_cpystrn(dnn->value, sess->pdn.dnn,
            ogs_min(dnn->length, OGS_MAX_DNN_LEN) + 1);

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

ogs_pkbuf_t *gsm_build_pdu_session_release_command(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_release_command_t *pdu_session_release_command =
        &message.gsm.pdu_session_release_command;

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_COMMAND;

    pdu_session_release_command->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gsm_build_pdu_session_release_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_release_reject_t *
        pdu_session_release_reject = &message.gsm.pdu_session_release_reject;

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT;

    pdu_session_release_reject->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}
