/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "context.h"
#include "ngap-path.h"
#include "sbi-path.h"

int smf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /*
     * SMF can only run to support 4G EPC mode.
     *
     * If the SMF is only running in 4G EPC mode,
     * it should not send NFRegister/NFStatusSubscribe messages to the NRF.
     */
    if (ogs_sbi_server_first() == NULL)
        return OGS_OK;

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
    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NAMF_COMM);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NUDM_SDM);
    ogs_sbi_subscription_spec_add(
            OpenAPI_nf_type_NULL, OGS_SBI_SERVICE_NAME_NUDM_UECM);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void smf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool smf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

int smf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(smf_sess_t *sess, void *data),
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int state, void *data)
{
    int r;
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_xact_t *xact = NULL;
    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;

    ogs_assert(service_type);
    target_nf_type = ogs_sbi_service_type_to_nf_type(service_type);
    ogs_assert(target_nf_type);
    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(build);

    /*
     * Use ogs_sbi_supi_in_vplmn() instead of ogs_sbi_plmn_id_in_vplmn().
     * This is because some vendors might not use the full DNN in LBO and
     * Open5GS cannot derive the home PLMN ID without the full DNN.
     *
     * TS29.502
     * 6.1 Nsmf_PDUSession Service API
     * Table 6.1.6.2.2-1: Definition of type SmContextCreateData
     *
     * NAME: dnn
     * Data type: Dnn
     * P: C
     * Cardinality: 0..1
     *
     * This IE shall be present, except during an EPS to 5GS Idle mode mobility
     * or handover using the N26 interface.
     *
     * When present, it shall contain the requested DNN; the DNN shall
     * be the full DNN (i.e. with both the Network Identifier and
     * Operator Identifier) for a HR PDU session, and it should be
     * the full DNN in LBO and non-roaming scenarios. If the Operator Identifier
     * is absent, the serving core network operator shall be assumed.
     *
     * TS29.512
     * 5 Npcf_SMPolicyControl Service API
     * 5.6 Data Model
     * 5.6.2 Structured data types
     * Table 5.6.2.3-1: Definition of type SmPolicyContextData
     *
     * NAME: dnn
     * Data type: Dnn
     * P: M
     * Cardinality: 1
     * The DNN of the PDU session, a full DNN with both the Network Identifier
     * and Operator Identifier, or a DNN with the Network Identifier only
     */
    if (target_nf_type == OpenAPI_nf_type_UDM &&
        ogs_sbi_supi_in_vplmn(smf_ue->supi) == true) {
        int i;

        /* TODO: PCF and UDM Selection
         *
         * FROM: Ultra Cloud Core 5G Session Management Function,
         * Release 2023.04 - Configuration and Administration Guide
         * https://www.cisco.com/c/en/us/td/docs/wireless/ucc/smf/2023-04/config-and-admin/b_ucc-5g-smf-config-and-admin-guide_2023-04/m_roaming-support.html#Cisco_Reference.dita_ed2a198e-b60d-4d77-b09c-932d82169c11https://www.cisco.com/c/en/us/td/docs/wireless/ucc/smf/2023-04/config-and-admin/b_ucc-5g-smf-config-and-admin-guide_2023-04/m_roaming-support.html#Cisco_Reference.dita_ed2a198e-b60d-4d77-b09c-932d82169c11
         *
         * During roaming, the AMF selects both vPCF and hPCF and sends
         * the vPCF ID and hPCF ID to the SMF and vPCF respectively
         * during policy association. The SMF selects the PCF
         * using the received vPCF ID.
         *
         * During AMF relocation, target AMF selects a new vPCF and hPCF.
         * The SMF receives a redirection indication with PCF ID
         * from the existing PCF for the PDU session. The SMF terminates
         * the current SM Policy Control association and reselects
         * a PCF based on the received PCF ID.
         *
         * The SMF then establishes an SM Policy Control association
         * with the reselected PCF.
         *
         * For selection of PCF and UDM based on local configuration,
         * the locally configured addresses map to the VPLMN and HPLMN
         * respectively since the PCF is in VPLMN and the UDM is in HPLMN
         * for roaming with LBO case.
         *
         * For NRF-based discovery of PCF and UDM, the query criteria includes
         * VPLMN for PCF discovery and HPLMN for UDM discovery. The AMF sends
         * the UDM group ID to enable the SMF to select UDM.
         *
         * The S-NSSAI used by SMF to select PCF should be the VPLMN S-NSSAI
         * received from AMF.
         */
        if (!discovery_option) {
            discovery_option = ogs_sbi_discovery_option_new();
            ogs_assert(discovery_option);
        }

        ogs_sbi_discovery_option_add_target_plmn_list(
                discovery_option, &sess->home_plmn_id);

        ogs_assert(ogs_local_conf()->num_of_serving_plmn_id);
        for (i = 0; i < ogs_local_conf()->num_of_serving_plmn_id; i++) {
            ogs_sbi_discovery_option_add_requester_plmn_list(
                    discovery_option, &ogs_local_conf()->serving_plmn_id[i]);
        }
    }

    xact = ogs_sbi_xact_add(
            sess->id, &sess->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, sess, data);
    if (!xact) {
        ogs_error("smf_sbi_discover_and_send() failed");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot discover", smf_ue->supi, NULL));
        return OGS_ERROR;
    }

    xact->state = state;

    if (stream) {
        xact->assoc_stream_id = ogs_sbi_id_from_stream(stream);
        ogs_assert(xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                xact->assoc_stream_id <= OGS_MAX_POOL_ID);
    }

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);

        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot discover", smf_ue->supi, NULL));
        return r;
    }

    return OGS_OK;
}

ogs_sbi_xact_t *smf_namf_comm_create_n1_n2_message_xact(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        smf_n1_n2_message_transfer_param_t *param)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_xact_t *xact = NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    ogs_assert(param);
    ogs_assert(param->state);
    ogs_assert(param->n1smbuf || param->n2smbuf);

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);
    ogs_sbi_discovery_option_set_target_nf_instance_id(
            discovery_option, sess->amf_nf_id);

    xact = ogs_sbi_xact_add(
            sess->id, &sess->sbi, OGS_SBI_SERVICE_TYPE_NAMF_COMM,
            discovery_option,
            (ogs_sbi_build_f)smf_namf_comm_build_n1_n2_message_transfer,
            sess, param);
    if (!xact) {
        ogs_error("ogs_sbi_xact_add() failed");
        return NULL;
    }

    xact->state = param->state;

    if (stream) {
        xact->assoc_stream_id = ogs_sbi_id_from_stream(stream);
        ogs_assert(xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                xact->assoc_stream_id <= OGS_MAX_POOL_ID);
    }

    return xact;
}

void smf_namf_comm_send_n1_n2_message_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        smf_n1_n2_message_transfer_param_t *param)
{
    ogs_sbi_xact_t *xact = NULL;
    int r;

    ogs_assert(sess);
    ogs_assert(param);
    ogs_assert(param->state);
    ogs_assert(param->n1smbuf || param->n2smbuf);

    xact = smf_namf_comm_create_n1_n2_message_xact(sess, stream, param);
    if (!xact) {
        ogs_error("smf_namf_comm_create_n1_n2_message_xact() failed");
        return;
    }

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smf_namf_comm_send_n1_n2_message_transfer() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(r != OGS_ERROR);
    }
}

void smf_namf_comm_send_n1_n2_pdu_establishment_reject(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    smf_n1_n2_message_transfer_param_t param;

    memset(&param, 0, sizeof(param));
    param.state = SMF_UE_OR_NETWORK_REQUESTED_PDU_SESSION_RELEASE;
    param.n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
        OGS_5GSM_CAUSE_NETWORK_FAILURE);
    ogs_assert(param.n1smbuf);

    smf_namf_comm_send_n1_n2_message_transfer(sess, stream, &param);
}

void smf_sbi_send_sm_context_created_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    OpenAPI_sm_context_created_data_t SmContextCreatedData;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    memset(&SmContextCreatedData, 0, sizeof(SmContextCreatedData));

    memset(&sendmsg, 0, sizeof(sendmsg));

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_SM_CONTEXTS;
    header.resource.component[1] = sess->sm_context_ref;

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);
    ogs_assert(sendmsg.http.location);

    sendmsg.SmContextCreatedData = &SmContextCreatedData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    smf_metrics_inst_by_slice_add(&sess->serving_plmn_id, &sess->s_nssai,
            SMF_METR_CTR_SM_PDUSESSIONCREATIONSUCC, 1);

    ogs_free(sendmsg.http.location);
}

void smf_sbi_send_sm_context_create_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_create_error_t SmContextCreateError;
    OpenAPI_ext_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsg;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));
    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    if (err > OGS_SBI_APP_ERRNO_NULL && err < OGS_SBI_MAX_NUM_OF_APP_ERRNO)
        problem.cause = (char*)ogs_sbi_app_strerror(err);

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.SmContextCreateError = &SmContextCreateError;

    memset(&SmContextCreateError, 0, sizeof(SmContextCreateError));
    SmContextCreateError.error = &problem;

    if (n1smbuf) {
        SmContextCreateError.n1_sm_msg = &n1SmMsg;
        n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_type = (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[0].pkbuf = n1smbuf;
        sendmsg.num_of_part = 1;
    }

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    smf_metrics_inst_by_cause_add(problem.status,
            SMF_METR_CTR_SM_PDUSESSIONCREATIONFAIL, 1);

    if (n1smbuf)
        ogs_pkbuf_free(n1smbuf);
}

void smf_sbi_send_pdu_session_created_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int rv;
    OpenAPI_pdu_session_created_data_t PduSessionCreatedData;

    OpenAPI_tunnel_info_t hcnTunnelInfo;
    char ssc_mode[2];
    OpenAPI_ambr_t sessionAmbr;
    OpenAPI_list_t *qosFlowsSetupList = NULL;
    OpenAPI_qos_flow_setup_item_t *qosFlowSetupItem = NULL;
    OpenAPI_qos_flow_profile_t *qosFlowProfile = NULL;
    OpenAPI_arp_t *Arp = NULL;

    ogs_nas_qos_rule_t qos_rule[OGS_NAS_MAX_NUM_OF_QOS_RULE];
    ogs_nas_qos_rules_t authorized_qos_rules;

    ogs_nas_qos_flow_description_t
        qos_flow_description[OGS_NAS_MAX_NUM_OF_QOS_FLOW_DESCRIPTION];
    ogs_nas_qos_flow_descriptions_t authorized_qos_flow_descriptions;

    /*
     * TS29.502
     * 6.1.6.2.10 Type: PduSessionCreatedData
     *
     * This IE shall be present if the H-SMF/SMF has assigned
     * IPv6 interface identifier to the UE during
     * the PDU session establishment for the Home-routed Roaming scenario
     * or for a PDU session with an I-SMF.
     *
     * When present, it shall encode the UE IPv6 Interface Identifier
     * to be used by the UE for its link-local address configuration
     * with 16 hexadecimal digits.
     *
     * Pattern: "^[A-Fa-f0-9]{16}$"
     */
    char ue_ipv6_interface_id[16+1]; /* 16 + 1(string-termiante:'\0') */

    int enc_len = 0;

    OpenAPI_ref_to_binary_data_t n1SmInfoToUe;
    ogs_pkbuf_t *n1SmBufToUe = NULL;

    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    smf_bearer_t *qos_flow = NULL;

    memset(&PduSessionCreatedData, 0, sizeof(PduSessionCreatedData));
    memset(ssc_mode, 0, sizeof(ssc_mode));
    memset(&hcnTunnelInfo, 0, sizeof(hcnTunnelInfo));
    memset(&sessionAmbr, 0, sizeof(sessionAmbr));

    ogs_assert(sess->session.session_type);
    PduSessionCreatedData.pdu_session_type = sess->session.session_type;

    ogs_assert(sess->session.ssc_mode);
    ssc_mode[0] = ogs_to_hex(sess->session.ssc_mode);
    PduSessionCreatedData.ssc_mode = ssc_mode;

    if (sess->local_ul_addr)
        hcnTunnelInfo.ipv4_addr = ogs_ipstrdup(sess->local_ul_addr);
    if (sess->local_ul_addr6)
        hcnTunnelInfo.ipv6_addr = ogs_ipstrdup(sess->local_ul_addr6);
    hcnTunnelInfo.gtp_teid = ogs_uint32_to_0string(sess->local_ul_teid);
    PduSessionCreatedData.hcn_tunnel_info = &hcnTunnelInfo;

    if (sess->session.ambr.uplink)
        sessionAmbr.uplink = ogs_sbi_bitrate_to_string(
            sess->session.ambr.uplink, OGS_SBI_BITRATE_KBPS);
    if (sess->session.ambr.downlink)
        sessionAmbr.downlink = ogs_sbi_bitrate_to_string(
            sess->session.ambr.downlink, OGS_SBI_BITRATE_KBPS);

    if (sessionAmbr.uplink || sessionAmbr.downlink)
        PduSessionCreatedData.session_ambr = &sessionAmbr;

    qos_flow = smf_default_bearer_in_sess(sess);
    ogs_assert(qos_flow);
    ogs_assert(ogs_list_next(qos_flow) == NULL);

    memset(qos_rule, 0, sizeof(qos_rule));
    gsm_encode_default_qos_rule(&qos_rule[0], qos_flow);

    memset(&authorized_qos_rules, 0, sizeof(authorized_qos_rules));
    rv = ogs_nas_build_qos_rules(&authorized_qos_rules, qos_rule, 1);
    ogs_assert(rv == OGS_OK);
    ogs_assert(authorized_qos_rules.length);

    /* QoS flow descriptions */
    memset(&qos_flow_description, 0, sizeof(qos_flow_description));
    gsm_encode_default_qos_flow_description(&qos_flow_description[0], qos_flow);

    memset(&authorized_qos_flow_descriptions, 0,
            sizeof(authorized_qos_flow_descriptions));
    rv = ogs_nas_build_qos_flow_descriptions(
            &authorized_qos_flow_descriptions, qos_flow_description, 1);
    ogs_assert(rv == OGS_OK);
    ogs_assert(authorized_qos_flow_descriptions.length);

    qosFlowsSetupList = OpenAPI_list_create();
    ogs_assert(qosFlowsSetupList);

    qosFlowSetupItem = ogs_calloc(1, sizeof(*qosFlowSetupItem));
    ogs_assert(qosFlowSetupItem);

    qosFlowSetupItem->qfi = qos_flow->qfi;
    if (qos_rule[0].DQR_bit) {
        qosFlowSetupItem->is_default_qos_rule_ind = true;
        qosFlowSetupItem->default_qos_rule_ind = true;
    }

    enc_len = ogs_base64_encode_len(authorized_qos_rules.length);
    qosFlowSetupItem->qos_rules = ogs_calloc(1, enc_len);
    ogs_assert(qosFlowSetupItem->qos_rules);
    ogs_base64_encode(qosFlowSetupItem->qos_rules,
            authorized_qos_rules.buffer, authorized_qos_rules.length);

    ogs_free(authorized_qos_rules.buffer);

    enc_len = ogs_base64_encode_len(
            authorized_qos_flow_descriptions.length);
    qosFlowSetupItem->qos_flow_description = ogs_calloc(1, enc_len);
    ogs_assert(qosFlowSetupItem->qos_flow_description);
    ogs_base64_encode(qosFlowSetupItem->qos_flow_description,
            authorized_qos_flow_descriptions.buffer,
            authorized_qos_flow_descriptions.length);

    ogs_free(authorized_qos_flow_descriptions.buffer);

    Arp = ogs_calloc(1, sizeof(*Arp));
    ogs_assert(Arp);
    if (qos_flow->qos.arp.pre_emption_capability ==
            OGS_5GC_PRE_EMPTION_ENABLED)
        Arp->preempt_cap = OpenAPI_preemption_capability_MAY_PREEMPT;
    else if (qos_flow->qos.arp.pre_emption_capability ==
            OGS_5GC_PRE_EMPTION_DISABLED)
        Arp->preempt_cap = OpenAPI_preemption_capability_NOT_PREEMPT;
    else {
        ogs_error("No Arp->preempt_cap");
        goto end;
    }

    if (qos_flow->qos.arp.pre_emption_vulnerability ==
            OGS_5GC_PRE_EMPTION_ENABLED)
        Arp->preempt_vuln = OpenAPI_preemption_vulnerability_PREEMPTABLE;
    else if (qos_flow->qos.arp.pre_emption_vulnerability ==
            OGS_5GC_PRE_EMPTION_DISABLED)
        Arp->preempt_vuln = OpenAPI_preemption_vulnerability_NOT_PREEMPTABLE;
    else {
        ogs_error("No Arp->preempt_vuln");
        goto end;
    }
    Arp->priority_level = qos_flow->qos.arp.priority_level;

    qosFlowProfile = ogs_calloc(1, sizeof(*qosFlowProfile));
    ogs_assert(qosFlowProfile);
    qosFlowProfile->arp = Arp;
    qosFlowProfile->_5qi = qos_flow->qos.index;

    qosFlowSetupItem->qos_flow_profile = qosFlowProfile;

    OpenAPI_list_add(qosFlowsSetupList, qosFlowSetupItem);

    if (qosFlowsSetupList->count)
        PduSessionCreatedData.qos_flows_setup_list = qosFlowsSetupList;
    else
        OpenAPI_list_free(qosFlowsSetupList);

    PduSessionCreatedData.h_smf_instance_id =
        NF_INSTANCE_ID(ogs_sbi_self()->nf_instance);
    ogs_assert(PduSessionCreatedData.h_smf_instance_id);

    memset(ue_ipv6_interface_id, 0, sizeof(ue_ipv6_interface_id));
    if (sess->paa.session_type == OGS_PDU_SESSION_TYPE_IPV4) {
        PduSessionCreatedData.ue_ipv4_address =
            ogs_ipv4_to_string(sess->paa.addr);
    } else if (sess->paa.session_type == OGS_PDU_SESSION_TYPE_IPV6) {
        PduSessionCreatedData.ue_ipv6_prefix =
            ogs_ipv6prefix_to_string(
                    sess->paa.addr6, OGS_IPV6_DEFAULT_PREFIX_LEN);
        ogs_hex_to_ascii(
                sess->paa.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1,
                ue_ipv6_interface_id, sizeof(ue_ipv6_interface_id));
        PduSessionCreatedData.ue_ipv6_interface_id = ue_ipv6_interface_id;
    } else if (sess->paa.session_type == OGS_PDU_SESSION_TYPE_IPV4V6) {
        PduSessionCreatedData.ue_ipv4_address =
            ogs_ipv4_to_string(sess->paa.both.addr);
        PduSessionCreatedData.ue_ipv6_prefix =
            ogs_ipv6prefix_to_string(
                    sess->paa.both.addr6, OGS_IPV6_DEFAULT_PREFIX_LEN);
        ogs_hex_to_ascii(
                sess->paa.addr6+(OGS_IPV6_LEN>>1), OGS_IPV6_LEN>>1,
                ue_ipv6_interface_id, sizeof(ue_ipv6_interface_id));
        PduSessionCreatedData.ue_ipv6_interface_id = ue_ipv6_interface_id;
    } else {
        ogs_error("Invalid sess->session.session_type[%d]",
                sess->paa.session_type);
        goto end;
    }

    memset(&sendmsg, 0, sizeof(sendmsg));

    n1SmBufToUe = gsmue_build_pdu_session_establishment_accept(sess);
    ogs_assert(n1SmBufToUe);

    n1SmInfoToUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    PduSessionCreatedData.n1_sm_info_to_ue = &n1SmInfoToUe;

    sendmsg.part[sendmsg.num_of_part].pkbuf = n1SmBufToUe;
    sendmsg.part[sendmsg.num_of_part].content_id =
        (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    sendmsg.part[sendmsg.num_of_part].content_type =
        (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
    sendmsg.num_of_part++;

    memset(&header, 0, sizeof(header));
    header.service.name = (char *)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION;
    header.api.version = (char *)OGS_SBI_API_V1;
    header.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_PDU_SESSIONS;
    ogs_assert(sess->pdu_session_ref);
    header.resource.component[1] = sess->pdu_session_ref;

    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);
    sendmsg.http.location = ogs_sbi_server_uri(server, &header);
    ogs_assert(sendmsg.http.location);

    sendmsg.PduSessionCreatedData = &PduSessionCreatedData;

    response = ogs_sbi_build_response(&sendmsg,
            OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_free(sendmsg.http.location);

end:
    if (hcnTunnelInfo.ipv4_addr)
        ogs_free(hcnTunnelInfo.ipv4_addr);
    if (hcnTunnelInfo.ipv6_addr)
        ogs_free(hcnTunnelInfo.ipv6_addr);
    if (hcnTunnelInfo.gtp_teid)
        ogs_free(hcnTunnelInfo.gtp_teid);

    if (sessionAmbr.uplink)
        ogs_free(sessionAmbr.uplink);
    if (sessionAmbr.downlink)
        ogs_free(sessionAmbr.downlink);

    CLEAR_QOS_FLOWS_SETUP_LIST(PduSessionCreatedData.qos_flows_setup_list);

    if (PduSessionCreatedData.ue_ipv4_address)
        ogs_free(PduSessionCreatedData.ue_ipv4_address);
    if (PduSessionCreatedData.ue_ipv6_prefix)
        ogs_free(PduSessionCreatedData.ue_ipv6_prefix);
}

void smf_sbi_send_sm_context_updated_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        OpenAPI_up_cnx_state_e up_cnx_state,
        OpenAPI_ho_state_e ho_state,
        ogs_pkbuf_t *n1smbuf,
        OpenAPI_n2_sm_info_type_e n2type, ogs_pkbuf_t *n2smbuf)
{
    int i;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_updated_data_t SmContextUpdatedData;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_ref_to_binary_data_t n2SmInfo;

    ogs_assert(sess);
    ogs_assert(stream);

    ogs_assert(up_cnx_state != OpenAPI_up_cnx_state_NULL ||
                ho_state != OpenAPI_ho_state_NULL ||
                n1smbuf || n2smbuf);

    memset(&sendmsg, 0, sizeof(sendmsg));

    memset(&SmContextUpdatedData, 0, sizeof(SmContextUpdatedData));

    /* up_cnx_state */
    SmContextUpdatedData.up_cnx_state = up_cnx_state;

    /* ho_state */
    SmContextUpdatedData.ho_state = ho_state;

    /* n1smbuf */
    if (n1smbuf) {
        n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        SmContextUpdatedData.n1_sm_msg = &n1SmMsg;

        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[sendmsg.num_of_part].pkbuf = n1smbuf;
        sendmsg.num_of_part++;
    }

    /* n2smbuf */
    if (n2smbuf) {
        ogs_assert(n2type);
        SmContextUpdatedData.n2_sm_info_type = n2type;

        n2SmInfo.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        SmContextUpdatedData.n2_sm_info = &n2SmInfo;

        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_NGAP_TYPE;
        sendmsg.part[sendmsg.num_of_part].pkbuf = n2smbuf;
        sendmsg.num_of_part++;
    }

    sendmsg.SmContextUpdatedData = &SmContextUpdatedData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    for (i = 0; i < sendmsg.num_of_part; i++)
        ogs_pkbuf_free(sendmsg.part[i].pkbuf);
}

void smf_sbi_send_sm_context_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf,
        OpenAPI_n2_sm_info_type_e n2_sm_info_type,
        OpenAPI_up_cnx_state_e up_cnx_state)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_update_error_t SmContextUpdateError;
    OpenAPI_ext_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_ref_to_binary_data_t n2SmInfo;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));
    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    if (err > OGS_SBI_APP_ERRNO_NULL && err < OGS_SBI_MAX_NUM_OF_APP_ERRNO)
        problem.cause = (char*)ogs_sbi_app_strerror(err);

    memset(&sendmsg, 0, sizeof(sendmsg));
    sendmsg.SmContextUpdateError = &SmContextUpdateError;

    memset(&SmContextUpdateError, 0, sizeof(SmContextUpdateError));
    SmContextUpdateError.error = &problem;

    if (n1smbuf) {
        SmContextUpdateError.n1_sm_msg = &n1SmMsg;
        n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[sendmsg.num_of_part].pkbuf = n1smbuf;
        sendmsg.num_of_part++;
    }

    if (n2smbuf) {
        SmContextUpdateError.n2_sm_info = &n2SmInfo;
        n2SmInfo.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_NGAP_TYPE;
        sendmsg.part[sendmsg.num_of_part].pkbuf = n2smbuf;
        sendmsg.num_of_part++;
    }

    SmContextUpdateError.n2_sm_info_type = n2_sm_info_type;
    SmContextUpdateError.up_cnx_state = up_cnx_state;

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (n1smbuf)
        ogs_pkbuf_free(n1smbuf);

    if (n2smbuf)
        ogs_pkbuf_free(n2smbuf);
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

int smf_sbi_cleanup_session(
    smf_sess_t              *sess,
    ogs_sbi_stream_t        *stream,
    int                      state,
    smf_sbi_cleanup_mode_t   mode)
{
    smf_ue_t *smf_ue = NULL;
    int r = OGS_ERROR;

    ogs_assert(mode);

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);

    ogs_assert(state);

    switch (mode) {
    case SMF_SBI_CLEANUP_MODE_POLICY_FIRST:
        if (PCF_SM_POLICY_ASSOCIATED(sess)) {
            r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NPCF_SMPOLICYCONTROL,
                NULL,
                smf_npcf_smpolicycontrol_build_delete,
                sess, stream, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else if (UDM_SDM_SUBSCRIBED(sess)) {
            ogs_error("[%s:%d] No PolicyAssociationId. Forcibly remove SESSION",
                    smf_ue->supi, sess->psi);
            r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM,
                NULL,
                smf_nudm_sdm_build_subscription_delete,
                sess, stream, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_error("[%s:%d] No UDM Subscription. Forcibly remove SESSION",
                    smf_ue->supi, sess->psi);
            r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_UECM,
                NULL,
                smf_nudm_uecm_build_deregistration,
                sess, stream, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;

    case SMF_SBI_CLEANUP_MODE_SUBSCRIPTION_FIRST:
        if (UDM_SDM_SUBSCRIBED(sess)) {
            r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_SDM,
                NULL,
                smf_nudm_sdm_build_subscription_delete,
                sess, stream, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        } else {
            ogs_error("[%s:%d] No UDM Subscription. Forcibly remove SESSION",
                    smf_ue->supi, sess->psi);
            r = smf_sbi_discover_and_send(
                OGS_SBI_SERVICE_TYPE_NUDM_UECM,
                NULL,
                smf_nudm_uecm_build_deregistration,
                sess, stream, state, NULL);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);
        }
        break;

    case SMF_SBI_CLEANUP_MODE_CONTEXT_ONLY:
        r = smf_sbi_discover_and_send(
            OGS_SBI_SERVICE_TYPE_NUDM_UECM,
            NULL,
            smf_nudm_uecm_build_deregistration,
            sess, stream, state, NULL);
        ogs_expect(r == OGS_OK);
        ogs_assert(r != OGS_ERROR);
        break;
    }

    return r;
}

bool smf_sbi_send_sm_context_status_notify(smf_sess_t *sess)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->namf.client;
    ogs_assert(client);

    request = smf_namf_callback_build_sm_context_status(sess, NULL);
    if (!request) {
        ogs_error("smf_namf_callback_build_sm_context_status() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

void smf_sbi_send_pdu_session_create_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufToUe)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_pdu_session_create_error_t PduSessionCreateError;
    OpenAPI_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsgToUe;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&problem, 0, sizeof(problem));
    memset(&PduSessionCreateError, 0, sizeof(PduSessionCreateError));

    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    if (err > OGS_SBI_APP_ERRNO_NULL && err < OGS_SBI_MAX_NUM_OF_APP_ERRNO)
        problem.cause = (char*)ogs_sbi_app_strerror(err);

    sendmsg.PduSessionCreateError = &PduSessionCreateError;

    memset(&PduSessionCreateError, 0, sizeof(PduSessionCreateError));
    PduSessionCreateError.error = &problem;

    if (n1SmCause)
        PduSessionCreateError.n1sm_cause = ogs_msprintf("%02x", n1SmCause);

    if (n1SmBufToUe) {
        PduSessionCreateError.n1_sm_info_to_ue = &n1SmMsgToUe;
        n1SmMsgToUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_type = (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[0].pkbuf = n1SmBufToUe;
        sendmsg.num_of_part = 1;
    }

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    smf_metrics_inst_by_cause_add(problem.status,
            SMF_METR_CTR_SM_PDUSESSIONCREATIONFAIL, 1);

    if (PduSessionCreateError.n1sm_cause)
        ogs_free(PduSessionCreateError.n1sm_cause);
    if (n1SmBufToUe)
        ogs_pkbuf_free(n1SmBufToUe);
}

void smf_sbi_send_hsmf_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufToUe)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_hsmf_update_error_t HsmfUpdateError;
    OpenAPI_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsgToUe;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&problem, 0, sizeof(problem));
    memset(&HsmfUpdateError, 0, sizeof(HsmfUpdateError));

    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    if (err > OGS_SBI_APP_ERRNO_NULL && err < OGS_SBI_MAX_NUM_OF_APP_ERRNO)
        problem.cause = (char*)ogs_sbi_app_strerror(err);

    sendmsg.HsmfUpdateError = &HsmfUpdateError;

    memset(&HsmfUpdateError, 0, sizeof(HsmfUpdateError));
    HsmfUpdateError.error = &problem;

    if (n1SmCause)
        HsmfUpdateError.n1sm_cause = ogs_msprintf("%02x", n1SmCause);

    if (n1SmBufToUe) {
        HsmfUpdateError.n1_sm_info_to_ue = &n1SmMsgToUe;
        n1SmMsgToUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_type = (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[0].pkbuf = n1SmBufToUe;
        sendmsg.num_of_part = 1;
    }

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (HsmfUpdateError.n1sm_cause)
        ogs_free(HsmfUpdateError.n1sm_cause);
    if (n1SmBufToUe)
        ogs_pkbuf_free(n1SmBufToUe);
}

void smf_sbi_send_vsmf_update_error(
        ogs_sbi_stream_t *stream,
        int status, ogs_sbi_app_errno_e err, int n1SmCause,
        const char *title, const char *detail,
        ogs_pkbuf_t *n1SmBufFromUe)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_vsmf_update_error_t VsmfUpdateError;
    OpenAPI_ext_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsgFromUe;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    memset(&problem, 0, sizeof(problem));
    memset(&VsmfUpdateError, 0, sizeof(VsmfUpdateError));

    if (status) {
        problem.is_status = true;
        problem.status = status;
    }
    problem.title = (char*)title;
    problem.detail = (char*)detail;
    if (err > OGS_SBI_APP_ERRNO_NULL && err < OGS_SBI_MAX_NUM_OF_APP_ERRNO)
        problem.cause = (char*)ogs_sbi_app_strerror(err);

    sendmsg.VsmfUpdateError = &VsmfUpdateError;

    memset(&VsmfUpdateError, 0, sizeof(VsmfUpdateError));
    VsmfUpdateError.error = &problem;

    if (n1SmCause)
        VsmfUpdateError.n1sm_cause = ogs_msprintf("%02x", n1SmCause);

    if (n1SmBufFromUe) {
        VsmfUpdateError.n1_sm_info_from_ue = &n1SmMsgFromUe;
        n1SmMsgFromUe.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[0].content_type = (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.part[0].pkbuf = n1SmBufFromUe;
        sendmsg.num_of_part = 1;
    }

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    if (VsmfUpdateError.n1sm_cause)
        ogs_free(VsmfUpdateError.n1sm_cause);
    if (n1SmBufFromUe)
        ogs_pkbuf_free(n1SmBufFromUe);
}

void smf_sbi_send_released_data(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    OpenAPI_released_data_t ReleasedData;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    memset(&ReleasedData, 0, sizeof(ReleasedData));
    memset(&sendmsg, 0, sizeof(sendmsg));

    sendmsg.ReleasedData = &ReleasedData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
}

bool smf_sbi_send_status_notify(smf_sess_t *sess)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->v_smf.client;
    ogs_assert(client);

    request = smf_nsmf_pdusession_build_status(sess, NULL);
    if (!request) {
        ogs_error("smf_nsmf_pdusession_build_status() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}
