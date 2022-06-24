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
    int num_of_param, rv;

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
    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];

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
    ogs_assert(ogs_list_next(qos_flow) == NULL);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT;

    selected_pdu_session_type->type = sess->session.ssc_mode;
    selected_pdu_session_type->value = sess->session.session_type;

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
    qos_rule[0].identifier = qos_flow->qfi; /* Use QFI in Open5GS */
    qos_rule[0].code = OGS_NAS_QOS_CODE_CREATE_NEW_QOS_RULE;
    qos_rule[0].DQR_bit = 1;
    qos_rule[0].num_of_packet_filter = 1;

    qos_rule[0].pf[0].direction = OGS_NAS_QOS_DIRECTION_BIDIRECTIONAL;
    qos_rule[0].pf[0].identifier = 1;
    qos_rule[0].pf[0].content.length = 1;
    qos_rule[0].pf[0].content.num_of_component = 1;
    qos_rule[0].pf[0].content.component[0].type = OGS_PACKET_FILTER_MATCH_ALL;

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

    rv = ogs_nas_build_qos_rules(authorized_qos_rules, qos_rule, 1);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    ogs_expect_or_return_val(authorized_qos_rules->length, NULL);

    /* Session-AMBR */
    session_ambr->length = 6;
    ogs_nas_bitrate_from_uint64(
            &session_ambr->downlink, sess->session.ambr.downlink);
    ogs_nas_bitrate_from_uint64(
            &session_ambr->uplink, sess->session.ambr.uplink);

    /* PDU Address */
    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_PDU_ADDRESS_PRESENT;
    pdu_address->pdn_type = sess->session.session_type;

    if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV4) {
        pdu_address->addr = sess->session.paa.addr;
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV4_LEN;
    } else if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV6) {
        memcpy(pdu_address->addr6,
                sess->session.paa.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1);
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV6_LEN;
    } else if (pdu_address->pdn_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        pdu_address->both.addr = sess->session.paa.both.addr;
        memcpy(pdu_address->both.addr6,
            sess->session.paa.both.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1);
        pdu_address->length = OGS_NAS_PDU_ADDRESS_IPV4V6_LEN;
    } else {
        ogs_error("Unexpected PDN Type %u", pdu_address->pdn_type);
        return NULL;
    }

    /* GSM cause */
    if (sess->ue_session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
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
    ogs_nas_build_s_nssai2(nas_s_nssai, &sess->s_nssai, &sess->mapped_hplmn);

    /* QoS flow descriptions */
    memset(&qos_flow_description, 0, sizeof(qos_flow_description));
    qos_flow_description[0].identifier = qos_flow->qfi;
    qos_flow_description[0].code = OGS_NAS_CREATE_NEW_QOS_FLOW_DESCRIPTION;
    qos_flow_description[0].E_bit = 1;

    num_of_param = 0;

    qos_flow_description[0].param[num_of_param].identifier =
        OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI;
    qos_flow_description[0].param[num_of_param].len =
        sizeof(qos_flow_description[0].param[num_of_param].qos_index);
    qos_flow_description[0].param[num_of_param].qos_index = qos_flow->qos.index;
    num_of_param++;

    qos_flow_description[0].num_of_parameter = num_of_param;

    pdu_session_establishment_accept->presencemask |=
        OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
    rv = ogs_nas_build_qos_flow_descriptions(
            authorized_qos_flow_descriptions, qos_flow_description, 1);
    ogs_expect_or_return_val(rv == OGS_OK, NULL);
    ogs_expect_or_return_val(authorized_qos_flow_descriptions->length, NULL);

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
    ogs_assert(sess->session.name);
    dnn->length = strlen(sess->session.name);
    ogs_cpystrn(dnn->value, sess->session.name,
            ogs_min(dnn->length, OGS_MAX_DNN_LEN) + 1);

    pkbuf = ogs_nas_5gs_plain_encode(&message);
    ogs_assert(pkbuf);

    ogs_free(authorized_qos_rules->buffer);
    ogs_free(authorized_qos_flow_descriptions->buffer);

    return pkbuf;
}

ogs_pkbuf_t *gsm_build_pdu_session_establishment_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_establishment_reject_t *
        pdu_session_establishment_reject =
            &message.gsm.pdu_session_establishment_reject;

    ogs_assert(sess);
    ogs_assert(gsm_cause);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REJECT;

    pdu_session_establishment_reject->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}

static void encode_qos_rule_packet_filter(
        ogs_nas_qos_rule_t *qos_rule, smf_bearer_t *qos_flow)
{
    int i;
    smf_pf_t *pf = NULL;

    ogs_assert(qos_rule);
    ogs_assert(qos_flow);

    if (qos_rule->code == OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS) {

        for (i = 0; i < qos_flow->num_of_pf_to_delete; i++) {
            qos_rule->pf[i].identifier = qos_flow->pf_to_delete[i];
        }
        qos_rule->num_of_packet_filter = qos_flow->num_of_pf_to_delete;

    } else {

        i = 0;
        ogs_list_for_each_entry(&qos_flow->pf_to_add_list, pf, to_add_node) {
            ogs_assert(i < OGS_MAX_NUM_OF_FLOW_IN_NAS);
            qos_rule->pf[i].direction = pf->direction;
            qos_rule->pf[i].identifier = pf->identifier;

            ogs_pf_content_from_ipfw_rule(
                    pf->direction, &qos_rule->pf[i].content, &pf->ipfw_rule,
                    ogs_app()->parameter.no_ipv4v6_local_addr_in_packet_filter);
            i++;
        }
        qos_rule->num_of_packet_filter = i;

    }
}

ogs_pkbuf_t *gsm_build_pdu_session_modification_command(
        smf_sess_t *sess, uint8_t pti,
        uint8_t qos_rule_code, uint8_t qos_flow_description_code)
{
    ogs_pkbuf_t *pkbuf = NULL;
    smf_bearer_t *qos_flow = NULL;
    ogs_pfcp_pdr_t *dl_pdr = NULL;
    int num_of_param, rv, i;

    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_modification_command_t
        *pdu_session_modification_command =
            &message.gsm.pdu_session_modification_command;

    ogs_nas_qos_rules_t *authorized_qos_rules = NULL;
    ogs_nas_qos_flow_descriptions_t *authorized_qos_flow_descriptions = NULL;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];
    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];

    authorized_qos_rules = &pdu_session_modification_command->
        authorized_qos_rules;
    ogs_assert(authorized_qos_rules);
    authorized_qos_flow_descriptions =
        &pdu_session_modification_command->authorized_qos_flow_descriptions;
    ogs_assert(authorized_qos_flow_descriptions);

    ogs_assert(sess);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND;

    /* QoS rule */
    if (qos_rule_code) {

        i = 0;
        memset(qos_rule, 0, sizeof(qos_rule));

        ogs_list_for_each_entry(
                &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {
            ogs_assert(i < OGS_MAX_NUM_OF_BEARER);

            dl_pdr = qos_flow->dl_pdr;
            ogs_assert(dl_pdr);

            qos_rule[i].identifier = qos_flow->qfi; /* Use QFI in Open5GS */
            qos_rule[i].code = qos_rule_code;

            if (qos_rule_code != OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE &&
                qos_rule_code != OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS)
                encode_qos_rule_packet_filter(&qos_rule[i], qos_flow);

            if (qos_rule_code != OGS_NAS_QOS_CODE_DELETE_EXISTING_QOS_RULE &&
                qos_rule_code != OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_AND_DELETE_PACKET_FILTERS &&
                qos_rule_code != OGS_NAS_QOS_CODE_MODIFY_EXISTING_QOS_RULE_WITHOUT_MODIFYING_PACKET_FILTERS) {
                ogs_assert(dl_pdr->precedence > 0 && dl_pdr->precedence < 255);
                /* Use PCC Rule Precedence */
                qos_rule[i].precedence = dl_pdr->precedence;

                qos_rule[i].flow.segregation = 0;
                qos_rule[i].flow.identifier = qos_flow->qfi;
            }

            i++;
        }

        rv = ogs_nas_build_qos_rules(authorized_qos_rules, qos_rule, i);
        ogs_expect_or_return_val(rv == OGS_OK, NULL);
        ogs_expect_or_return_val(authorized_qos_rules->length, NULL);

        pdu_session_modification_command->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT;
    }

    /* QoS flow descriptions */
    if (qos_flow_description_code) {

        i = 0;
        memset(&qos_flow_description, 0, sizeof(qos_flow_description));

        ogs_list_for_each_entry(
                &sess->qos_flow_to_modify_list, qos_flow, to_modify_node) {
            ogs_assert(i < OGS_MAX_NUM_OF_BEARER);

            qos_flow_description[i].identifier = qos_flow->qfi;
            qos_flow_description[i].code = qos_flow_description_code;

            num_of_param = 0;

            if (qos_flow_description_code !=
                    OGS_NAS_DELETE_NEW_QOS_FLOW_DESCRIPTION) {
                qos_flow_description[i].E_bit = 1;

                qos_flow_description[i].param[num_of_param].identifier =
                    OGS_NAX_QOS_FLOW_PARAMETER_ID_5QI;
                qos_flow_description[i].param[num_of_param].len =
                    sizeof(qos_flow_description[i].param[num_of_param].
                            qos_index);
                qos_flow_description[i].param[num_of_param].qos_index =
                    qos_flow->qos.index;
                num_of_param++;

                if (qos_flow->qos.gbr.uplink) {
                    qos_flow_description[i].param[num_of_param].identifier =
                        OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_UPLINK;
                    qos_flow_description[i].param[num_of_param].len =
                        sizeof(qos_flow_description[i].param[num_of_param].br);
                    ogs_nas_bitrate_from_uint64(
                            &qos_flow_description[i].param[num_of_param].br,
                            qos_flow->qos.gbr.uplink);
                    num_of_param++;
                }
                if (qos_flow->qos.gbr.downlink) {
                    qos_flow_description[i].param[num_of_param].identifier =
                        OGS_NAX_QOS_FLOW_PARAMETER_ID_GFBR_DOWNLINK;
                    qos_flow_description[i].param[num_of_param].len =
                        sizeof(qos_flow_description[i].param[num_of_param].br);
                    ogs_nas_bitrate_from_uint64(
                            &qos_flow_description[i].param[num_of_param].br,
                            qos_flow->qos.gbr.downlink);
                    num_of_param++;
                }
                if (qos_flow->qos.mbr.uplink) {
                    qos_flow_description[i].param[num_of_param].identifier =
                        OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_UPLINK;
                    qos_flow_description[i].param[num_of_param].len =
                        sizeof(qos_flow_description[i].param[num_of_param].br);
                    ogs_nas_bitrate_from_uint64(
                            &qos_flow_description[i].param[num_of_param].br,
                            qos_flow->qos.mbr.uplink);
                    num_of_param++;
                }
                if (qos_flow->qos.mbr.downlink) {
                    qos_flow_description[i].param[num_of_param].identifier =
                        OGS_NAX_QOS_FLOW_PARAMETER_ID_MFBR_DOWNLINK;
                    qos_flow_description[i].param[num_of_param].len =
                        sizeof(qos_flow_description[i].param[num_of_param].br);
                    ogs_nas_bitrate_from_uint64(
                            &qos_flow_description[i].param[num_of_param].br,
                            qos_flow->qos.mbr.downlink);
                    num_of_param++;
                }
            }

            qos_flow_description[i].num_of_parameter = num_of_param;

            i++;
        }

        rv = ogs_nas_build_qos_flow_descriptions(
                authorized_qos_flow_descriptions, qos_flow_description, i);
        ogs_expect_or_return_val(rv == OGS_OK, NULL);
        ogs_expect_or_return_val(
                authorized_qos_flow_descriptions->length, NULL);

        pdu_session_modification_command->presencemask |=
            OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT;
    }

    pkbuf = ogs_nas_5gs_plain_encode(&message);
    ogs_assert(pkbuf);

    if (pdu_session_modification_command->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_RULES_PRESENT) {
        ogs_free(authorized_qos_rules->buffer);
    }
    if (pdu_session_modification_command->presencemask &
        OGS_NAS_5GS_PDU_SESSION_MODIFICATION_COMMAND_AUTHORIZED_QOS_FLOW_DESCRIPTIONS_PRESENT) {
        ogs_free(authorized_qos_flow_descriptions->buffer);
    }

    return pkbuf;
}

ogs_pkbuf_t *gsm_build_pdu_session_modification_reject(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_modification_reject_t *
        pdu_session_modification_reject =
            &message.gsm.pdu_session_modification_reject;

    ogs_assert(sess);
    ogs_assert(gsm_cause);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_MODIFICATION_REJECT;

    pdu_session_modification_reject->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}

ogs_pkbuf_t *gsm_build_pdu_session_release_command(
        smf_sess_t *sess, ogs_nas_5gsm_cause_t gsm_cause)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_pdu_session_release_command_t *pdu_session_release_command =
        &message.gsm.pdu_session_release_command;

    ogs_assert(sess);
    ogs_assert(gsm_cause);

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

    ogs_assert(sess);
    ogs_assert(gsm_cause);

    memset(&message, 0, sizeof(message));
    message.gsm.h.extended_protocol_discriminator =
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GSM;
    message.gsm.h.pdu_session_identity = sess->psi;
    message.gsm.h.procedure_transaction_identity = sess->pti;
    message.gsm.h.message_type = OGS_NAS_5GS_PDU_SESSION_RELEASE_REJECT;

    pdu_session_release_reject->gsm_cause = gsm_cause;

    return ogs_nas_5gs_plain_encode(&message);
}
