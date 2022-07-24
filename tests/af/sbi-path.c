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

#include "sbi-path.h"

static int server_cb(ogs_sbi_request_t *request, void *data)
{
    af_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = af_event_new(AF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_request_free(request);
        af_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(int status, ogs_sbi_response_t *response, void *data)
{
    af_event_t *e = NULL;
    int rv;

    if (status != OGS_OK) {
        ogs_log_message(
                status == OGS_DONE ? OGS_LOG_DEBUG : OGS_LOG_WARN, 0,
                "client_cb() failed [%d]", status);
        return OGS_ERROR;
    }

    ogs_assert(response);

    e = af_event_new(AF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_sbi_response_free(response);
        af_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int af_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /* Add SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance, OpenAPI_nf_type_AF);

    /* Build NF service information. It will be transmitted to NRF. */
    service = ogs_sbi_nf_service_build_default(nf_instance,
            (char*)OGS_SBI_SERVICE_NAME_NAF_EVENTEXPOSURE);
    ogs_assert(service);
    ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
            (char*)OGS_SBI_API_V1_0_0, NULL);

    /* Initialize NRF NF Instance */
    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        if (NF_INSTANCE_IS_NRF(nf_instance)) {
            ogs_sbi_client_t *client = NULL;

            /* Client callback is only used when NF sends to NRF */
            client = nf_instance->client;
            ogs_assert(client);
            client->cb = client_cb;

            /* NFRegister is sent and the response is received
             * by the above client callback. */
            af_nf_fsm_init(nf_instance);
        }
    }

    /* Timer expiration handler of client wait timer */
    ogs_sbi_self()->client_wait_expire = af_timer_sbi_client_wait_expire;

    /* NF register state in NF state machine */
    ogs_sbi_self()->nf_state_registered =
        (ogs_fsm_handler_t)af_nf_state_registered;

    if (ogs_sbi_server_start_all(server_cb) != OGS_OK)
        return OGS_ERROR;


    return OGS_OK;
}

void af_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool af_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = af_nnrf_nfm_build_register();
    ogs_expect_or_return_val(request, false);
    return ogs_sbi_client_send_request(
            client, client->cb, request, nf_instance);
}

bool af_sbi_send_request(
        ogs_sbi_object_t *sbi_object,
        OpenAPI_nf_type_e target_nf_type,
        void *data)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    nf_instance = OGS_SBI_NF_INSTANCE(sbi_object, target_nf_type);
    if (!nf_instance) {
        ogs_error("(NF discover) No [%s]",
                OpenAPI_nf_type_ToString(target_nf_type));
        return false;
    }

    return ogs_sbi_send_request(nf_instance, client_cb, data);
}

void af_sbi_discover_and_send(
        OpenAPI_nf_type_e target_nf_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data),
        af_sess_t *sess, void *data)
{
    ogs_sbi_xact_t *xact = NULL;
    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    ogs_assert(ogs_sbi_self()->nf_instance);
    requester_nf_type = ogs_sbi_self()->nf_instance->nf_type;
    ogs_assert(requester_nf_type);

    ogs_assert(target_nf_type);

    ogs_assert(sess);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(
            &sess->sbi, target_nf_type, discovery_option,
            (ogs_sbi_build_f)build, sess, data);
    if (!xact) {
        ogs_error("af_sbi_discover_and_send() failed");
        return;
    }

    if (ogs_sbi_discover_and_send(
            &sess->sbi,
            target_nf_type, requester_nf_type, discovery_option,
            client_cb, xact) != true) {
        ogs_error("af_sbi_discover_and_send() failed");
        return;
    }
}

void af_sbi_send_to_pcf(
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data))
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(sess);
    ogs_assert(build);
    client = sess->pcf.client;
    ogs_assert(client);

    request = (*build)(sess, data);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, client_cb, request, sess);
}
