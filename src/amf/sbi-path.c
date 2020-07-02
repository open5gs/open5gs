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
#include "nas-path.h"
#include "nausf-build.h"
#include "nudm-build.h"

static int server_cb(ogs_sbi_server_t *server,
        ogs_sbi_session_t *session, ogs_sbi_request_t *request)
{
    amf_event_t *e = NULL;
    int rv;

    ogs_assert(session);
    ogs_assert(request);

    e = amf_event_new(AMF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.server = server;
    e->sbi.session = session;
    e->sbi.request = request;

    rv = ogs_queue_push(amf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    amf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = amf_event_new(AMF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(amf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int amf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_server_start_all(server_cb);

    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        ogs_sbi_nf_service_t *service = NULL;

        ogs_sbi_nf_instance_build_default(nf_instance, amf_self()->nf_type);

        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NAMF_COMM);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);

        amf_nf_fsm_init(nf_instance);
        amf_sbi_setup_client_callback(nf_instance);
    }

    return OGS_OK;
}

void amf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

void amf_sbi_setup_client_callback(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_nf_service_t *nf_service = NULL;
    ogs_assert(nf_instance);

    client = nf_instance->client;
    ogs_assert(client);

    client->cb = client_cb;

    ogs_list_for_each(&nf_instance->nf_service_list, nf_service) {
        client = nf_service->client;
        if (client)
            client->cb = client_cb;
    }
}

void amf_ue_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, amf_ue_t *amf_ue, void *data,
        ogs_sbi_request_t *(*build)(amf_ue_t *amf_ue, void *data))
{
    ogs_assert(nf_type);
    ogs_assert(amf_ue);

    if (amf_ue->sbi.running == true) {
        ogs_error("amf_ue_sbi_discover_and_send() is running");
        return;
    }

    amf_ue->sbi.nf_state_registered = amf_nf_state_registered;
    amf_ue->sbi.client_wait.duration =
        amf_timer_cfg(AMF_TIMER_SBI_CLIENT_WAIT)->duration;

    if (ogs_sbi_discover_and_send(
            nf_type, &amf_ue->sbi, data, (ogs_sbi_build_f)build) != true) {
        nas_5gs_send_gmm_reject_from_sbi(amf_ue,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
    }
}

void amf_sess_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, amf_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(amf_sess_t *sess, void *data))
{
    ogs_assert(nf_type);
    ogs_assert(sess);
    ogs_assert(build);

    if (sess->sbi.running == true) {
        ogs_error("amf_sess_sbi_discover_and_send() is running");
        return;
    }

    sess->sbi.nf_state_registered = amf_nf_state_registered;
    sess->sbi.client_wait.duration =
        amf_timer_cfg(AMF_TIMER_SBI_CLIENT_WAIT)->duration;

    if (ogs_sbi_discover_and_send(
            nf_type, &sess->sbi, data, (ogs_sbi_build_f)build) != true) {
        nas_5gs_send_back_5gsm_message_from_sbi(sess,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT);
    }
}

void amf_sbi_send_activating_session(amf_sess_t *sess)
{
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    ogs_assert(sess);

    /* UPDATE_UpCnxState - ACTIVATING */
    sess->ueUpCnxState = OpenAPI_up_cnx_state_ACTIVATING;

    memset(&param, 0, sizeof(param));
    param.upCnxState = sess->ueUpCnxState;
    amf_sess_sbi_discover_and_send(
            OpenAPI_nf_type_SMF, sess, &param,
            amf_nsmf_pdu_session_build_update_sm_context);
}

void amf_sbi_send_deactivate_session(
        amf_sess_t *sess, int group, int cause)
{
    amf_nsmf_pdu_session_update_sm_context_param_t param;

    ogs_assert(sess);

    /* UPDATE_UpCnxState - DEACTIVATED */
    sess->ueUpCnxState = OpenAPI_up_cnx_state_DEACTIVATED;

    memset(&param, 0, sizeof(param));
    param.upCnxState = sess->ueUpCnxState;
    param.ngApCause.group = group;
    param.ngApCause.value = cause;
    amf_sess_sbi_discover_and_send(
            OpenAPI_nf_type_SMF, sess, &param,
            amf_nsmf_pdu_session_build_update_sm_context);
}

void amf_sbi_send_deactivate_all_sessions(
        amf_ue_t *amf_ue, int group, int cause)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (SESSION_CONTEXT_IN_SMF(sess))
            amf_sbi_send_deactivate_session(sess, group, cause);
    }
}

void amf_sbi_send_release_session(amf_sess_t *sess)
{
    ogs_assert(sess);

    amf_sess_sbi_discover_and_send(
            OpenAPI_nf_type_SMF, sess, NULL,
            amf_nsmf_pdu_session_build_release_sm_context);
}

void amf_sbi_send_release_all_sessions(amf_ue_t *amf_ue)
{
    amf_sess_t *sess = NULL;

    ogs_assert(amf_ue);

    ogs_list_for_each(&amf_ue->sess_list, sess) {
        if (SESSION_CONTEXT_IN_SMF(sess))
            amf_sbi_send_release_session(sess);
    }
}
