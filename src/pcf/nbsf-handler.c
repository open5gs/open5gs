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

#include "sbi-path.h"

#include "nbsf-handler.h"

bool pcf_nbsf_management_handle_register(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int i, rv, status = 0;
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_sbi_message_t message;

    ogs_session_data_t session_data;

    ogs_session_t *session = NULL;

    OpenAPI_pcf_binding_t *PcfBinding = NULL;

    OpenAPI_sm_policy_decision_t SmPolicyDecision;

    OpenAPI_lnode_t *node = NULL;

    OpenAPI_list_t *SessRuleList = NULL;
    OpenAPI_map_t *SessRuleMap = NULL;
    OpenAPI_session_rule_t *SessionRule = NULL;

    OpenAPI_ambr_t AuthSessAmbr;
    OpenAPI_authorized_default_qos_t AuthDefQos;

    OpenAPI_list_t *PccRuleList = NULL;
    OpenAPI_map_t *PccRuleMap = NULL;
    OpenAPI_pcc_rule_t *PccRule = NULL;

    OpenAPI_list_t *QosDecisionList = NULL;
    OpenAPI_map_t *QosDecisionMap = NULL;
    OpenAPI_qos_data_t *QosData = NULL;

    OpenAPI_list_t *PolicyCtrlReqTriggers = NULL;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    memset(&session_data, 0, sizeof(ogs_session_data_t));

    ogs_assert(pcf_ue->supi);
    ogs_assert(sess->dnn);

    if (!recvmsg->http.location) {
        strerror = ogs_msprintf("[%s:%d] No http.location",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    if (!recvmsg->PcfBinding) {
        strerror = ogs_msprintf("[%s:%d] No PcfBinding",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    PcfBinding = recvmsg->PcfBinding;
    if (PcfBinding->supp_feat) {
        uint64_t supported_features =
            ogs_uint64_from_string(PcfBinding->supp_feat);
        sess->management_features &= supported_features;
    }

    memset(&header, 0, sizeof(header));
    header.uri = recvmsg->http.location;

    rv = ogs_sbi_parse_header(&message, &header);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s:%d] Cannot parse http.location [%s]",
                pcf_ue->supi, sess->psi, recvmsg->http.location);
        goto cleanup;
    }

    if (!message.h.resource.component[1]) {
        strerror = ogs_msprintf("[%s:%d] No Binding ID [%s]",
                pcf_ue->supi, sess->psi, recvmsg->http.location);

        ogs_sbi_header_free(&header);
        goto cleanup;
    }

    if (sess->binding_id)
        ogs_free(sess->binding_id);
    sess->binding_id = ogs_strdup(message.h.resource.component[1]);
    ogs_assert(sess->binding_id);

    ogs_sbi_header_free(&header);

    rv = ogs_dbi_session_data(
            pcf_ue->supi, &sess->s_nssai, sess->dnn, &session_data);
    if (rv != OGS_OK) {
        strerror = ogs_msprintf("[%s:%d] Cannot find SUPI in DB",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
        goto cleanup;
    }

    session = &session_data.session;

    if (!session->qos.index) {
        strerror = ogs_msprintf("[%s:%d] No 5QI", pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }
    if (!session->qos.arp.priority_level) {
        strerror = ogs_msprintf("[%s:%d] No Priority Level",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    if (!session->ambr.uplink && !session->ambr.downlink) {
        strerror = ogs_msprintf("[%s:%d] No Session-AMBR",
                pcf_ue->supi, sess->psi);
        status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
        goto cleanup;
    }

    memset(&SmPolicyDecision, 0, sizeof(SmPolicyDecision));

    PolicyCtrlReqTriggers = OpenAPI_list_create();
    ogs_assert(PolicyCtrlReqTriggers);

    /**************************************************************
     * Session Rule
     *************************************************************/
    SessRuleList = OpenAPI_list_create();
    ogs_assert(SessRuleList);

    SessionRule = ogs_calloc(1, sizeof(*SessionRule));
    ogs_assert(SessionRule);

    /* Only 1 SessionRule is used */
    SessionRule->sess_rule_id = (char *)"1";

    if (OGS_SBI_FEATURES_IS_SET(sess->smpolicycontrol_features,
                OGS_SBI_NPCF_SMPOLICYCONTROL_DN_AUTHORIZATION)) {
        if (sess->subscribed_sess_ambr) {
            ogs_bitrate_t subscribed_sess_ambr;

            subscribed_sess_ambr.uplink = ogs_sbi_bitrate_from_string(
                    sess->subscribed_sess_ambr->uplink);
            subscribed_sess_ambr.downlink = ogs_sbi_bitrate_from_string(
                    sess->subscribed_sess_ambr->downlink);
            if (((subscribed_sess_ambr.uplink / 1024) !=
                 (session->ambr.uplink / 1024)) ||
                ((subscribed_sess_ambr.downlink / 1024) !=
                 (session->ambr.downlink / 1024))) {

                OpenAPI_list_add(PolicyCtrlReqTriggers,
                    (void *)OpenAPI_policy_control_request_trigger_SE_AMBR_CH);
            }

            memset(&AuthSessAmbr, 0, sizeof(AuthSessAmbr));
            AuthSessAmbr.uplink = ogs_sbi_bitrate_to_string(
                    session->ambr.uplink, OGS_SBI_BITRATE_KBPS);
            AuthSessAmbr.downlink = ogs_sbi_bitrate_to_string(
                    session->ambr.downlink, OGS_SBI_BITRATE_KBPS);
            SessionRule->auth_sess_ambr = &AuthSessAmbr;
        }
    }

    if (sess->subscribed_default_qos) {
        bool triggered = false;

        memset(&AuthDefQos, 0, sizeof(AuthDefQos));
        AuthDefQos.arp = ogs_calloc(1, sizeof(OpenAPI_arp_t));
        ogs_assert(AuthDefQos.arp);

        AuthDefQos.is__5qi = true;
        AuthDefQos._5qi = session->qos.index;
        AuthDefQos.is_priority_level = true;
        AuthDefQos.priority_level = session->qos.arp.priority_level;

        if (session->qos.arp.pre_emption_capability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            AuthDefQos.arp->preempt_cap =
                OpenAPI_preemption_capability_MAY_PREEMPT;
        else if (session->qos.arp.pre_emption_capability ==
                OGS_5GC_PRE_EMPTION_DISABLED)
            AuthDefQos.arp->preempt_cap =
                OpenAPI_preemption_capability_NOT_PREEMPT;
        ogs_assert(AuthDefQos.arp->preempt_cap);

        if (session->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_ENABLED)
            AuthDefQos.arp->preempt_vuln =
                OpenAPI_preemption_vulnerability_PREEMPTABLE;
        else if (session->qos.arp.pre_emption_vulnerability ==
                OGS_5GC_PRE_EMPTION_DISABLED)
            AuthDefQos.arp->preempt_vuln =
                OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
        ogs_assert(AuthDefQos.arp->preempt_vuln);
        AuthDefQos.arp->priority_level = session->qos.arp.priority_level;

        SessionRule->auth_def_qos = &AuthDefQos;

        if (sess->subscribed_default_qos->_5qi != AuthDefQos._5qi)
            triggered = true;
        if (sess->subscribed_default_qos->priority_level !=
                AuthDefQos.priority_level)
            triggered = true;
        if (sess->subscribed_default_qos->arp) {
            if (sess->subscribed_default_qos->arp->priority_level !=
                    AuthDefQos.arp->priority_level)
                triggered = true;
            if (sess->subscribed_default_qos->arp->preempt_cap !=
                    AuthDefQos.arp->preempt_cap)
                triggered = true;
            if (sess->subscribed_default_qos->arp->preempt_vuln !=
                    AuthDefQos.arp->preempt_vuln)
                triggered = true;

        }

        if (triggered)
            OpenAPI_list_add(PolicyCtrlReqTriggers,
                (void *)OpenAPI_policy_control_request_trigger_DEF_QOS_CH);

    }

    SessRuleMap = OpenAPI_map_create(
            SessionRule->sess_rule_id, SessionRule);
    ogs_assert(SessRuleMap);

    OpenAPI_list_add(SessRuleList, SessRuleMap);

    if (SessRuleList->count)
        SmPolicyDecision.sess_rules = SessRuleList;

    /**************************************************************
     * PCC Rule & QoS Decision
     *************************************************************/
    PccRuleList = OpenAPI_list_create();
    ogs_assert(PccRuleList);

    QosDecisionList = OpenAPI_list_create();
    ogs_assert(QosDecisionList);

    for (i = 0; i < session_data.num_of_pcc_rule; i++) {
        ogs_pcc_rule_t *pcc_rule = &session_data.pcc_rule[i];

        ogs_assert(pcc_rule);

        if (!pcc_rule->num_of_flow) {
            /* No Flow */
            continue;
        }

        PccRule = ogs_sbi_build_pcc_rule(pcc_rule, 1);
        ogs_assert(PccRule->pcc_rule_id);

        PccRuleMap = OpenAPI_map_create(PccRule->pcc_rule_id, PccRule);
        ogs_assert(PccRuleMap);

        OpenAPI_list_add(PccRuleList, PccRuleMap);

        QosData = ogs_sbi_build_qos_data(pcc_rule);
        ogs_assert(QosData);
        ogs_assert(QosData->qos_id);

        QosDecisionMap = OpenAPI_map_create(QosData->qos_id, QosData);
        ogs_assert(QosDecisionMap);

        OpenAPI_list_add(QosDecisionList, QosDecisionMap);
    }

    if (PccRuleList->count)
        SmPolicyDecision.pcc_rules = PccRuleList;

    if (QosDecisionList->count)
        SmPolicyDecision.qos_decs = QosDecisionList;

    /* Policy Control Request Triggers */
    if (PolicyCtrlReqTriggers->count)
        SmPolicyDecision.policy_ctrl_req_triggers = PolicyCtrlReqTriggers;

    /* Supported Features */
    if (sess->smpolicycontrol_features) {
        SmPolicyDecision.supp_feat =
            ogs_uint64_to_string(sess->smpolicycontrol_features);
        ogs_assert(SmPolicyDecision.supp_feat);
    }

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_POLICIES;
    header.resource.component[1] = sess->sm_policy_id;

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.SmPolicyDecision = &SmPolicyDecision;
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);

    response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_free(sendmsg.http.location);

    OpenAPI_list_for_each(SessRuleList, node) {
        SessRuleMap = node->data;
        if (SessRuleMap) {
            SessionRule = SessRuleMap->value;
            if (SessionRule) {
                if (SessionRule->auth_sess_ambr) {
                    if (SessionRule->auth_sess_ambr->uplink)
                        ogs_free(SessionRule->auth_sess_ambr->uplink);
                    if (SessionRule->auth_sess_ambr->downlink)
                        ogs_free(SessionRule->auth_sess_ambr->downlink);
                }
                if (SessionRule->auth_def_qos) {
                    ogs_free(SessionRule->auth_def_qos->arp);

                }
                ogs_free(SessionRule);
            }
            ogs_free(SessRuleMap);
        }
    }
    OpenAPI_list_free(SessRuleList);

    OpenAPI_list_for_each(PccRuleList, node) {
        PccRuleMap = node->data;
        if (PccRuleMap) {
            PccRule = PccRuleMap->value;
            if (PccRule)
                ogs_sbi_free_pcc_rule(PccRule);
            ogs_free(PccRuleMap);
        }
    }
    OpenAPI_list_free(PccRuleList);

    OpenAPI_list_for_each(QosDecisionList, node) {
        QosDecisionMap = node->data;
        if (QosDecisionMap) {
            QosData = QosDecisionMap->value;
            if (QosData)
                ogs_sbi_free_qos_data(QosData);
            ogs_free(QosDecisionMap);
        }
    }
    OpenAPI_list_free(QosDecisionList);

    OpenAPI_list_free(PolicyCtrlReqTriggers);

    if (SmPolicyDecision.supp_feat)
        ogs_free(SmPolicyDecision.supp_feat);

    ogs_session_data_free(&session_data);

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL));
    ogs_free(strerror);

    ogs_session_data_free(&session_data);

    return false;
}

bool pcf_nbsf_management_handle_de_register(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(sess);
    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(
            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    return true;
}
