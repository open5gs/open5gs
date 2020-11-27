/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

static int server_cb(ogs_sbi_request_t *request, void *data)
{
    smf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = smf_event_new(SMF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        smf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    smf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = smf_event_new(SMF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        smf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int smf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_server_start_all(server_cb);

    /*
     * The connection between NF and NRF is a little special.
     *
     * NF and NRF share nf_instance. I get the NRF EndPoint(client) information
     * the configuration file via lib/sbi/context.c.
     * And, the NFService information will be transmitted to NRF.
     *
     * ogs_sbi_self()->nf_instance_id means NF's InstanceId.
     */
    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        ogs_sbi_nf_service_t *service = NULL;
        ogs_sbi_client_t *client = NULL;

        /* Build NF instance information. It will be transmitted to NRF. */
        ogs_sbi_nf_instance_build_default(nf_instance, smf_self()->nf_type);

        /* Build NF service information. It will be transmitted to NRF. */
        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);

        /* Client callback is only used when NF sends to NRF */
        client = nf_instance->client;
        ogs_assert(client);
        client->cb = client_cb;

        /* NFRegister is sent and the response is received
         * by the above client callback. */
        smf_nf_fsm_init(nf_instance);
    }

    return OGS_OK;
}

void smf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

void smf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_sbi_send(nf_instance, client_cb, xact);
}

void smf_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        smf_sess_t *sess, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(smf_sess_t *sess, void *data))
{
    ogs_sbi_xact_t *xact = NULL;
    smf_ue_t *smf_ue = NULL;

    ogs_assert(target_nf_type);

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    ogs_assert(stream);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(target_nf_type, &sess->sbi, data,
            (ogs_sbi_build_f)build, smf_timer_sbi_client_wait_expire);
    ogs_assert(xact);

    xact->assoc_stream = stream;

    if (ogs_sbi_discover_and_send(xact,
            (ogs_fsm_handler_t)smf_nf_state_registered, client_cb) != true) {

        ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", smf_ue->supi);
    }
}

void smf_sbi_send_response(ogs_sbi_stream_t *stream, int status)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_sbi_server_send_response(stream, response);
}

void smf_sbi_send_sm_context_create_error(
        ogs_sbi_stream_t *stream,
        int status, const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_create_error_t SmContextCreateError;
    OpenAPI_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsg;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));
    problem.status = status;
    problem.title = (char*)title;
    problem.detail = (char*)detail;

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

    ogs_sbi_server_send_response(stream, response);

    if (n1smbuf)
        ogs_pkbuf_free(n1smbuf);
}

void smf_sbi_send_sm_context_updated_data(smf_sess_t *sess,
        ogs_sbi_stream_t *stream, OpenAPI_up_cnx_state_e up_cnx_state)
{
    int status;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_updated_data_t SmContextUpdatedData;

    ogs_assert(sess);
    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));
    
    if (up_cnx_state) {
        memset(&SmContextUpdatedData, 0, sizeof(SmContextUpdatedData));
        SmContextUpdatedData.up_cnx_state = up_cnx_state;

        sendmsg.SmContextUpdatedData = &SmContextUpdatedData;
        status = OGS_SBI_HTTP_STATUS_OK;
    } else {
        status = OGS_SBI_HTTP_STATUS_NO_CONTENT;
    }

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_sbi_server_send_response(stream, response);
}

void smf_sbi_send_sm_context_updated_data_in_session_deletion(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int i;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_updated_data_t SmContextUpdatedData;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_ref_to_binary_data_t n2SmInfo;

    ogs_assert(sess);
    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    memset(&SmContextUpdatedData, 0, sizeof(SmContextUpdatedData));

    sendmsg.num_of_part = 0;

    n1SmMsg.content_id = (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
    SmContextUpdatedData.n1_sm_msg = &n1SmMsg;

    sendmsg.part[sendmsg.num_of_part].pkbuf =
        gsm_build_pdu_session_release_command(
                sess, OGS_5GSM_CAUSE_REGULAR_DEACTIVATION);
    if (sendmsg.part[sendmsg.num_of_part].pkbuf) {
        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_5GNAS_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_5GNAS_TYPE;
        sendmsg.num_of_part++;
    }

    SmContextUpdatedData.n2_sm_info_type =
        OpenAPI_n2_sm_info_type_PDU_RES_REL_CMD;
    SmContextUpdatedData.n2_sm_info = &n2SmInfo;

    memset(&n2SmInfo, 0, sizeof(n2SmInfo));
    n2SmInfo.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;

    sendmsg.part[sendmsg.num_of_part].pkbuf =
        ngap_build_pdu_session_resource_release_command_transfer(
                NGAP_Cause_PR_nas, NGAP_CauseNas_normal_release);
    if (sendmsg.part[sendmsg.num_of_part].pkbuf) {
        sendmsg.part[sendmsg.num_of_part].content_id =
            (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
        sendmsg.part[sendmsg.num_of_part].content_type =
            (char *)OGS_SBI_CONTENT_NGAP_TYPE;
        sendmsg.num_of_part++;
    }

    sendmsg.SmContextUpdatedData = &SmContextUpdatedData;

    response = ogs_sbi_build_response(&sendmsg, OGS_SBI_HTTP_STATUS_OK);
    ogs_assert(response);
    ogs_sbi_server_send_response(stream, response);

    for (i = 0; i < sendmsg.num_of_part; i++)
        ogs_pkbuf_free(sendmsg.part[i].pkbuf);
}

void smf_sbi_send_sm_context_update_error(
        ogs_sbi_stream_t *stream,
        int status, const char *title, const char *detail,
        ogs_pkbuf_t *n1smbuf, ogs_pkbuf_t *n2smbuf)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_sm_context_update_error_t SmContextUpdateError;
    OpenAPI_problem_details_t problem;
    OpenAPI_ref_to_binary_data_t n1SmMsg;
    OpenAPI_ref_to_binary_data_t n2SmInfo;

    ogs_assert(stream);

    memset(&problem, 0, sizeof(problem));
    problem.status = status;
    problem.title = (char*)title;
    problem.detail = (char*)detail;

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

    response = ogs_sbi_build_response(&sendmsg, problem.status);
    ogs_assert(response);

    ogs_sbi_server_send_response(stream, response);

    if (n1smbuf)
        ogs_pkbuf_free(n1smbuf);

    if (n2smbuf)
        ogs_pkbuf_free(n2smbuf);
}

static int client_notify_cb(ogs_sbi_response_t *response, void *data)
{
    int rv;

    ogs_sbi_message_t message;

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

void smf_sbi_send_sm_context_status_notify(smf_sess_t *sess)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->namf.client;
    ogs_assert(client);
    client->cb = client_cb;

    request = smf_namf_callback_build_sm_context_status(sess, NULL);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}
