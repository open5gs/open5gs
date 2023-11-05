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

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
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
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
    ogs_assert(build);

    if (target_nf_type == OpenAPI_nf_type_UDM &&
        ogs_sbi_plmn_id_in_vplmn(&sess->home_plmn_id) == true) {
        int i;

        if (!discovery_option) {
            discovery_option = ogs_sbi_discovery_option_new();
            ogs_assert(discovery_option);
        }

        ogs_sbi_discovery_option_add_target_plmn_list(
                discovery_option, &sess->home_plmn_id);

        ogs_assert(ogs_app()->num_of_serving_plmn_id);
        for (i = 0; i < ogs_app()->num_of_serving_plmn_id; i++) {
            ogs_sbi_discovery_option_add_requester_plmn_list(
                    discovery_option, &ogs_app()->serving_plmn_id[i]);
        }
    }

    xact = ogs_sbi_xact_add(
            &sess->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, sess, data);
    if (!xact) {
        ogs_error("smf_sbi_discover_and_send() failed");
        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot discover", smf_ue->supi));
        return OGS_ERROR;
    }

    xact->state = state;
    xact->assoc_stream = stream;

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);

        if (stream)
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot discover", smf_ue->supi));
        return r;
    }

    return OGS_OK;
}

void smf_namf_comm_send_n1_n2_message_transfer(
        smf_sess_t *sess, smf_n1_n2_message_transfer_param_t *param)
{
    smf_ue_t *smf_ue = NULL;
    ogs_sbi_xact_t *xact = NULL;
    ogs_sbi_discovery_option_t *discovery_option = NULL;
    int r;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_assert(param);
    ogs_assert(param->state);
    ogs_assert(param->n1smbuf || param->n2smbuf);

    discovery_option = ogs_sbi_discovery_option_new();
    ogs_assert(discovery_option);
    ogs_sbi_discovery_option_set_target_nf_instance_id(
            discovery_option, sess->serving_nf_id);

    xact = ogs_sbi_xact_add(
            &sess->sbi, OGS_SBI_SERVICE_TYPE_NAMF_COMM, discovery_option,
            (ogs_sbi_build_f)smf_namf_comm_build_n1_n2_message_transfer,
            sess, param);
    if (!xact) {
        ogs_error("smf_namf_comm_send_n1_n2_message_transfer() failed");
        return;
    }

    xact->state = param->state;

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("smf_namf_comm_send_n1_n2_message_transfer() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(r != OGS_ERROR);
    }
}

void smf_namf_comm_send_n1_n2_pdu_establishment_reject(
        smf_sess_t *sess)
{
    smf_n1_n2_message_transfer_param_t param;

    memset(&param, 0, sizeof(param));
    param.state = SMF_NETWORK_REQUESTED_PDU_SESSION_RELEASE;
    param.n1smbuf = gsm_build_pdu_session_establishment_reject(sess,
        OGS_5GSM_CAUSE_NETWORK_FAILURE);
    ogs_assert(param.n1smbuf);

    smf_namf_comm_send_n1_n2_message_transfer(sess, &param);
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
