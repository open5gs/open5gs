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

static int server_cb(ogs_sbi_request_t *request, void *data)
{
    pcf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = pcf_event_new(PCF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        pcf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    pcf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = pcf_event_new(PCF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        pcf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int pcf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    if (ogs_sbi_server_start_all(server_cb) != OGS_OK)
        return OGS_ERROR;

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
        ogs_sbi_nf_instance_build_default(nf_instance, pcf_self()->nf_type);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_AMF);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_SMF);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_NEF);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_AF);

        /* Build NF service information. It will be transmitted to NRF. */
        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_NEF);

        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_NEF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AF);

        /* Client callback is only used when NF sends to NRF */
        client = nf_instance->client;
        ogs_assert(client);
        client->cb = client_cb;

        /* NFRegister is sent and the response is received
         * by the above client callback. */
        pcf_nf_fsm_init(nf_instance);
    }

    return OGS_OK;
}

void pcf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

bool pcf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = pcf_nnrf_nfm_build_register(nf_instance);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client->cb, request, nf_instance);
}

bool pcf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    return ogs_sbi_send(nf_instance, client_cb, xact);
}

static bool pcf_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        ogs_sbi_object_t *sbi_object, ogs_sbi_stream_t *stream,
        ogs_sbi_build_f build, void *context, void *data)
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(target_nf_type);
    ogs_assert(sbi_object);
    ogs_assert(stream);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(target_nf_type, sbi_object,
                            build, context, data,
                            pcf_timer_sbi_client_wait_expire);
    ogs_expect_or_return_val(xact, false);

    xact->assoc_stream = stream;

    if (ogs_sbi_discover_and_send(xact,
            (ogs_fsm_handler_t)pcf_nf_state_registered, client_cb) != true) {
        ogs_error("ogs_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        return false;
    }

    return true;
}

bool pcf_ue_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        pcf_ue_t *pcf_ue, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(pcf_ue_t *pcf_ue, void *data))
{
    if (pcf_sbi_discover_and_send(target_nf_type, &pcf_ue->sbi, stream,
                (ogs_sbi_build_f)build, pcf_ue, data) != true) {
        ogs_error("pcf_ue_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", pcf_ue->supi));
        return false;
    }

    return true;
}

bool pcf_sess_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        pcf_sess_t *sess, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(pcf_sess_t *sess, void *data))
{
    if (pcf_sbi_discover_and_send(target_nf_type, &sess->sbi, stream,
                (ogs_sbi_build_f)build, sess, data) != true) {
        ogs_error("pcf_sess_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL));
        return false;
    }

    return true;
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

static int client_delete_notify_cb(ogs_sbi_response_t *response, void *data)
{
    pcf_app_t *app_session = data;

    ogs_assert(app_session);
    client_notify_cb(response, data);

    pcf_app_remove(app_session);

    return OGS_OK;
}

bool pcf_sbi_send_am_policy_control_notify(pcf_ue_t *pcf_ue)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(pcf_ue);
    client = pcf_ue->namf.client;
    ogs_assert(client);

    request = pcf_namf_callback_build_am_policy_control(pcf_ue, NULL);
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}

bool pcf_sbi_send_smpolicycontrol_update_notify(
        pcf_sess_t *sess, OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}

bool pcf_sbi_send_smpolicycontrol_delete_notify(
        pcf_sess_t *sess, pcf_app_t *app_session,
        OpenAPI_sm_policy_decision_t *SmPolicyDecision)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_update(
                sess, SmPolicyDecision);
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(
            client, client_delete_notify_cb, request, app_session);
}

bool pcf_sbi_send_smpolicycontrol_terminate_notify(pcf_sess_t *sess)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    client = sess->nsmf.client;
    ogs_assert(client);

    request = pcf_nsmf_callback_build_smpolicycontrol_terminate(sess, NULL);
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}

bool pcf_sbi_send_policyauthorization_terminate_notify(pcf_app_t *app)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(app);
    client = app->naf.client;
    ogs_assert(client);

    request = pcf_naf_callback_build_policyauthorization_terminate(app, NULL);
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}
