/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

int pcf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    bool smpolicycontrol_enabled = false;
    bool policyauthorization_enabled = false;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SCP);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_AMF);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SMF);

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
    }

    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);

        smpolicycontrol_enabled = true;
    }

    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);

        policyauthorization_enabled = true;
    }

    if ((smpolicycontrol_enabled == true &&
            policyauthorization_enabled == false) ||
        (smpolicycontrol_enabled == false &&
            policyauthorization_enabled == true)) {
        ogs_fatal("CHECK CONFIGURATION:");
        ogs_fatal("   %s - %s",
            OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL,
            smpolicycontrol_enabled ? "enabled" : "disabled");
        ogs_fatal("   %s - %s",
            OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION,
            policyauthorization_enabled ? "enabled" : "disabled");
        ogs_fatal("Only one of %s and %s cannot be enabled.",
            OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL,
            OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION);
        ogs_fatal("They can be enabled or disabled together.");

        return OGS_ERROR;
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NUDR_DR);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void pcf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool pcf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

static int pcf_sbi_discover_and_send(
        ogs_pool_id_t sbi_object_id,
        ogs_sbi_object_t *sbi_object,
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_build_f build,
        void *context, ogs_sbi_stream_t *stream, void *data)
{
    ogs_sbi_xact_t *xact = NULL;
    int r;

    ogs_assert(service_type);
    ogs_assert(sbi_object);
    ogs_assert(stream);
    ogs_assert(build);

    ogs_assert(sbi_object_id >= OGS_MIN_POOL_ID &&
            sbi_object_id <= OGS_MAX_POOL_ID);

    xact = ogs_sbi_xact_add(
            sbi_object_id, sbi_object, service_type, discovery_option,
            build, context, data);
    if (!xact) {
        ogs_error("ogs_sbi_xact_add() failed");
        return OGS_ERROR;
    }

    if (stream) {
        xact->assoc_stream_id = ogs_sbi_id_from_stream(stream);
        ogs_assert(xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                xact->assoc_stream_id <= OGS_MAX_POOL_ID);
    }

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("ogs_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        return r;
    }

    return OGS_OK;
}

int pcf_ue_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(pcf_ue_t *pcf_ue, void *data),
        pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, void *data)
{
    int r;

    r = pcf_sbi_discover_and_send(
            pcf_ue->id, &pcf_ue->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, pcf_ue, stream, data);
    if (r != OGS_OK) {
        ogs_error("pcf_ue_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", pcf_ue->supi, NULL));
        return r;
    }

    return OGS_OK;
}

int pcf_sess_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(pcf_sess_t *sess, void *data),
        pcf_sess_t *sess, ogs_sbi_stream_t *stream, void *data)
{
    int r;

    r = pcf_sbi_discover_and_send(
            sess->id, &sess->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, sess, stream, data);
    if (r != OGS_OK) {
        ogs_error("pcf_sess_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL, NULL));
        return r;
    }

    return OGS_OK;
}

static int client_notify_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    ogs_sbi_message_t message;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "client_notify_cb() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_error("cannot parse HTTP response");
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return OGS_ERROR;
    }

    if (message.res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT)
        ogs_error("SmContextStatusNotification failed [%d]",
                message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

static int client_delete_notify_cb(
        int status, ogs_sbi_response_t *response, void *data)
{
    pcf_app_t *app_session = data;

    ogs_assert(app_session);
    client_notify_cb(status, response, data);

    pcf_app_remove(app_session);

    return OGS_OK;
}

bool pcf_sbi_send_am_policy_control_notify(pcf_ue_t *pcf_ue)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(pcf_ue);
    client = pcf_ue->namf.client;
    ogs_assert(client);

    request = pcf_namf_callback_build_am_policy_control(pcf_ue, NULL);
    if (!request) {
        ogs_error("pcf_namf_callback_build_am_policy_control() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_smpolicycontrol_create_response(
        pcf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int i, rv, status = 0;
    char *strerror = NULL;
    pcf_ue_t *pcf_ue = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    ogs_session_data_t session_data;

    ogs_session_t *session = NULL;

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
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);
    ogs_assert(stream);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    memset(&session_data, 0, sizeof(ogs_session_data_t));

    ogs_assert(pcf_ue->supi);
    ogs_assert(sess->dnn);

    rv = pcf_db_qos_data(
            pcf_ue->supi,
            sess->home.presence == true ? &sess->home.plmn_id : NULL,
            &sess->s_nssai, sess->dnn, &session_data);
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
            if (((subscribed_sess_ambr.uplink / 1000) !=
                 (session->ambr.uplink / 1000)) ||
                ((subscribed_sess_ambr.downlink / 1000) !=
                 (session->ambr.downlink / 1000))) {

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
        ogs_assert(pcc_rule->id);

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
    header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_SM_POLICIES;
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

    pcf_metrics_inst_by_slice_add(&pcf_ue->guami.plmn_id,
            &sess->s_nssai, PCF_METR_CTR_PA_POLICYSMASSOSUCC, 1);

    OGS_SESSION_DATA_FREE(&session_data);

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true == ogs_sbi_server_send_error(
                stream, status, NULL, strerror, NULL, NULL));
    ogs_free(strerror);

    OGS_SESSION_DATA_FREE(&session_data);

    return false;
}

bool pcf_sbi_send_smpolicycontrol_update_notify(
        pcf_sess_t *sess, OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    if (!request) {
        ogs_error("pcf_nsmf_callback_build_smpolicycontrol_update() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_smpolicycontrol_delete_notify(
        pcf_sess_t *sess, pcf_app_t *app_session,
        OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    if (!request) {
        ogs_error("pcf_nsmf_callback_build_smpolicycontrol_update() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_delete_notify_cb, request, app_session);
    if (rc == false) {
        ogs_error("ogs_sbi_send_request_to_client() failed");
        pcf_app_remove(app_session);
    }

    ogs_sbi_request_free(request);

    return rc;
}

bool pcf_sbi_send_policyauthorization_terminate_notify(pcf_app_t *app)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(app);
    client = app->naf.client;
    ogs_assert(client);

    request = pcf_naf_callback_build_policyauthorization_terminate(app, NULL);
    if (!request) {
        ogs_error("pcf_naf_callback_build_policyauthorization_terminate() "
                "failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}
