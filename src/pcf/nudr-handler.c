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

#include "nudr-handler.h"

bool pcf_nudr_dr_handle_query_am_data(
    pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv, status = 0;
    char *strerror = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_subscription_data_t subscription_data;

    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    memset(&subscription_data, 0, sizeof(ogs_subscription_data_t));

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
        OpenAPI_policy_association_t PolicyAssociation;
        OpenAPI_ambr_t UeAmbr;
        OpenAPI_list_t *TriggerList = NULL;

        if (!recvmsg->AmPolicyData) {
            strerror = ogs_msprintf("[%s] No AmPolicyData", pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        if (!pcf_ue->policy_association_request) {
            strerror = ogs_msprintf("[%s] No PolicyAssociationRequest",
                    pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        rv = ogs_dbi_subscription_data(pcf_ue->supi, &subscription_data);
        if (rv != OGS_OK) {
            strerror = ogs_msprintf("[%s] Cannot find SUPI in DB",
                    pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            goto cleanup;
        }

        if (!subscription_data.ambr.uplink &&
                !subscription_data.ambr.downlink) {
            ogs_error("[%s] No UE-AMBR", pcf_ue->supi);
            status = OGS_SBI_HTTP_STATUS_NOT_FOUND;
            goto cleanup;
        }

        memset(&PolicyAssociation, 0, sizeof(PolicyAssociation));
        PolicyAssociation.request = pcf_ue->policy_association_request;
        PolicyAssociation.supp_feat =
            ogs_uint64_to_string(pcf_ue->am_policy_control_features);
        ogs_assert(PolicyAssociation.supp_feat);

        TriggerList = OpenAPI_list_create();
        ogs_assert(TriggerList);

        memset(&UeAmbr, 0, sizeof(UeAmbr));
        if (OGS_SBI_FEATURES_IS_SET(pcf_ue->am_policy_control_features,
                    OGS_SBI_NPCF_AM_POLICY_CONTROL_UE_AMBR_AUTHORIZATION)) {
            if (pcf_ue->subscribed_ue_ambr) {
                ogs_bitrate_t subscribed_ue_ambr;

                subscribed_ue_ambr.uplink = ogs_sbi_bitrate_from_string(
                        pcf_ue->subscribed_ue_ambr->uplink);
                subscribed_ue_ambr.downlink = ogs_sbi_bitrate_from_string(
                        pcf_ue->subscribed_ue_ambr->downlink);

                if (((subscribed_ue_ambr.uplink / 1024) !=
                     (subscription_data.ambr.uplink / 1024)) ||
                    ((subscribed_ue_ambr.downlink / 1024) !=
                     (subscription_data.ambr.downlink / 1024))) {

                    OpenAPI_list_add(TriggerList,
                            (void *)OpenAPI_request_trigger_UE_AMBR_CH);
                }

                UeAmbr.uplink = ogs_sbi_bitrate_to_string(
                        subscription_data.ambr.uplink, OGS_SBI_BITRATE_KBPS);
                UeAmbr.downlink = ogs_sbi_bitrate_to_string(
                        subscription_data.ambr.downlink, OGS_SBI_BITRATE_KBPS);
                PolicyAssociation.ue_ambr = &UeAmbr;
            }
        }

        if (TriggerList->count)
            PolicyAssociation.triggers = TriggerList;

        memset(&header, 0, sizeof(header));
        header.service.name =
            (char *)OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL;
        header.api.version = (char *)OGS_SBI_API_V1;
        header.resource.component[0] =
            (char *)OGS_SBI_RESOURCE_NAME_POLICIES;
        header.resource.component[1] = pcf_ue->association_id;

        memset(&sendmsg, 0, sizeof(sendmsg));
        sendmsg.PolicyAssociation = &PolicyAssociation;
        sendmsg.http.location = ogs_sbi_server_uri(server, &header);

        response = ogs_sbi_build_response(
                &sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
        ogs_assert(response);
        ogs_sbi_server_send_response(stream, response);

        ogs_free(sendmsg.http.location);

        ogs_free(PolicyAssociation.supp_feat);

        OpenAPI_list_free(TriggerList);

        if (UeAmbr.uplink)
            ogs_free(UeAmbr.uplink);
        if (UeAmbr.downlink)
            ogs_free(UeAmbr.downlink);

        ogs_subscription_data_free(&subscription_data);

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s] Invalid resource name [%s]", 
                        pcf_ue->supi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL);
    ogs_free(strerror);

    ogs_subscription_data_free(&subscription_data);

    return false;
}

bool pcf_nudr_dr_handle_query_sm_data(
    pcf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int i, j, rv, status = 0;
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_session_data_t session_data;

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    ogs_assert(recvmsg);

    memset(&session_data, 0, sizeof(ogs_session_data_t));

    SWITCH(recvmsg->h.resource.component[3])
    CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
        ogs_session_t *session = NULL;

        OpenAPI_sm_policy_decision_t SmPolicyDecision;

        OpenAPI_lnode_t *node = NULL, *node2 = NULL;

        OpenAPI_list_t *SessRuleList = NULL;
        OpenAPI_map_t *SessRuleMap = NULL;
        OpenAPI_session_rule_t *SessionRule = NULL;

        OpenAPI_ambr_t AuthSessAmbr;
        OpenAPI_authorized_default_qos_t AuthDefQos;

        OpenAPI_list_t *PccRuleList = NULL;
        OpenAPI_map_t *PccRuleMap = NULL;
        OpenAPI_pcc_rule_t *PccRule = NULL;
        OpenAPI_flow_information_t *FlowInformation = NULL;

        OpenAPI_list_t *QosDecisionList = NULL;
        OpenAPI_map_t *QosDecisionMap = NULL;
        OpenAPI_qos_data_t *QosData = NULL;

        OpenAPI_list_t *PolicyCtrlReqTriggers = NULL;

        if (!recvmsg->SmPolicyData) {
            strerror = ogs_msprintf("[%s:%d] No SmPolicyData",
                    pcf_ue->supi, sess->psi);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        }

        ogs_assert(pcf_ue->supi);
        ogs_assert(sess->dnn);

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

            AuthDefQos._5qi = session->qos.index;
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
            OpenAPI_list_t *FlowInformationList = NULL;
            ogs_pcc_rule_t *pcc_rule = &session_data.pcc_rule[i];

            ogs_assert(pcc_rule);

            PccRule = ogs_calloc(1, sizeof(*PccRule));
            ogs_assert(PccRule);
            QosData = ogs_calloc(1, sizeof(*QosData));
            ogs_assert(QosData);

            /*
             * At this point, only 1 QosData is used for PccRule.
             * Therefore, QoS ID uses the same value as PCC Rule ID.
             */
            PccRule->pcc_rule_id = pcc_rule->id;
            QosData->qos_id = pcc_rule->id;

            PccRule->ref_qos_data = OpenAPI_list_create();
            ogs_assert(PccRule->ref_qos_data);

            OpenAPI_list_add(PccRule->ref_qos_data, QosData->qos_id);

            PccRule->precedence = pcc_rule->precedence;

            FlowInformationList = OpenAPI_list_create();
            ogs_assert(FlowInformationList);

            for (j = 0; j < pcc_rule->num_of_flow; j++) {
                ogs_flow_t *flow = &pcc_rule->flow[j];
                ogs_assert(flow);

                FlowInformation = ogs_calloc(1, sizeof(*FlowInformation));
                ogs_assert(FlowInformation);

                if (flow->direction == OGS_FLOW_UPLINK_ONLY)
                    FlowInformation->flow_direction =
                        OpenAPI_flow_direction_UPLINK;
                else if (flow->direction == OGS_FLOW_DOWNLINK_ONLY)
                    FlowInformation->flow_direction =
                        OpenAPI_flow_direction_DOWNLINK;
                else {
                    ogs_fatal("Unsupported direction [%d]", flow->direction);
                    ogs_assert_if_reached();
                }

                ogs_assert(flow->description);
                FlowInformation->flow_description = flow->description;

                OpenAPI_list_add(FlowInformationList, FlowInformation);
            }

            if (FlowInformationList->count)
                PccRule->flow_infos = FlowInformationList;
            else
                OpenAPI_list_free(FlowInformationList);

            PccRuleMap = OpenAPI_map_create(PccRule->pcc_rule_id, PccRule);
            ogs_assert(PccRuleMap);

            OpenAPI_list_add(PccRuleList, PccRuleMap);

            QosData->_5qi = pcc_rule->qos.index;
            QosData->priority_level = pcc_rule->qos.arp.priority_level;

            QosData->arp = ogs_calloc(1, sizeof(OpenAPI_arp_t));
            ogs_assert(QosData->arp);

            if (pcc_rule->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                QosData->arp->preempt_cap =
                    OpenAPI_preemption_capability_MAY_PREEMPT;
            else if (pcc_rule->qos.arp.pre_emption_capability ==
                    OGS_5GC_PRE_EMPTION_DISABLED)
                QosData->arp->preempt_cap =
                    OpenAPI_preemption_capability_NOT_PREEMPT;
            ogs_assert(pcc_rule->qos.arp.pre_emption_capability);

            if (pcc_rule->qos.arp.pre_emption_vulnerability ==
                    OGS_5GC_PRE_EMPTION_ENABLED)
                QosData->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_PREEMPTABLE;
            else if (pcc_rule->qos.arp.pre_emption_vulnerability ==
                    OGS_5GC_PRE_EMPTION_DISABLED)
                QosData->arp->preempt_vuln =
                    OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
            ogs_assert(pcc_rule->qos.arp.pre_emption_vulnerability);
            QosData->arp->priority_level = pcc_rule->qos.arp.priority_level;

            if (pcc_rule->qos.mbr.uplink)
                QosData->maxbr_ul = ogs_sbi_bitrate_to_string(
                        pcc_rule->qos.mbr.uplink, OGS_SBI_BITRATE_KBPS);
            if (pcc_rule->qos.mbr.downlink)
                QosData->maxbr_dl = ogs_sbi_bitrate_to_string(
                        pcc_rule->qos.mbr.downlink, OGS_SBI_BITRATE_KBPS);

            if (pcc_rule->qos.gbr.uplink)
                QosData->gbr_ul = ogs_sbi_bitrate_to_string(
                        pcc_rule->qos.gbr.uplink, OGS_SBI_BITRATE_KBPS);
            if (pcc_rule->qos.gbr.downlink)
                QosData->gbr_dl = ogs_sbi_bitrate_to_string(
                        pcc_rule->qos.gbr.downlink, OGS_SBI_BITRATE_KBPS);

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
        ogs_sbi_server_send_response(stream, response);

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
                if (PccRule) {
                    if (PccRule->ref_qos_data)
                        OpenAPI_list_free(PccRule->ref_qos_data);
                    if (PccRule->flow_infos) {
                        OpenAPI_list_for_each(PccRule->flow_infos, node2) {
                            FlowInformation = node2->data;
                            if (FlowInformation) ogs_free(FlowInformation);
                        }
                        OpenAPI_list_free(PccRule->flow_infos);
                    }
                    ogs_free(PccRule);
                }
                ogs_free(PccRuleMap);
            }
        }
        OpenAPI_list_free(PccRuleList);

        OpenAPI_list_for_each(QosDecisionList, node) {
            QosDecisionMap = node->data;
            if (QosDecisionMap) {
                QosData = QosDecisionMap->value;
                if (QosData) {
                    if (QosData->arp) ogs_free(QosData->arp);
                    if (QosData->maxbr_ul) ogs_free(QosData->maxbr_ul);
                    if (QosData->maxbr_dl) ogs_free(QosData->maxbr_dl);
                    if (QosData->gbr_ul) ogs_free(QosData->gbr_ul);
                    if (QosData->gbr_dl) ogs_free(QosData->gbr_dl);

                    ogs_free(QosData);
                }
                ogs_free(QosDecisionMap);
            }
        }
        OpenAPI_list_free(QosDecisionList);

        OpenAPI_list_free(PolicyCtrlReqTriggers);

        if (SmPolicyDecision.supp_feat)
            ogs_free(SmPolicyDecision.supp_feat);

        ogs_session_data_free(&session_data);

        return true;

    DEFAULT
        strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]", 
                    pcf_ue->supi, sess->psi, recvmsg->h.resource.component[3]);
    END

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL);
    ogs_free(strerror);

    ogs_session_data_free(&session_data);

    return false;
}
