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
    bsf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = bsf_event_new(BSF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        bsf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    bsf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = bsf_event_new(BSF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        bsf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int bsf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    if (ogs_sbi_server_start_all(server_cb) != OGS_OK)
        return OGS_ERROR;

    /* Add SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance, OpenAPI_nf_type_BSF);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_PCF);

    /* Build NF service information. It will be transmitted to NRF. */
    service = ogs_sbi_nf_service_build_default(nf_instance,
            (char*)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT);
    ogs_assert(service);
    ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
            (char*)OGS_SBI_API_V1_0_0, NULL);
    ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_PCF);

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
            bsf_nf_fsm_init(nf_instance);
        }
    }

    return OGS_OK;
}

void bsf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

bool bsf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = bsf_nnrf_nfm_build_register();
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client->cb, request, nf_instance);
}

bool bsf_sbi_send(ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    return ogs_sbi_send(nf_instance, client_cb, xact);
}

bool bsf_sbi_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        bsf_sess_t *sess, ogs_sbi_stream_t *stream, void *data,
        ogs_sbi_request_t *(*build)(bsf_sess_t *sess, void *data))
{
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(target_nf_type);

    ogs_assert(sess);
    ogs_assert(stream);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(target_nf_type, &sess->sbi,
            (ogs_sbi_build_f)build, sess, data,
            bsf_timer_sbi_client_wait_expire);
    if (!xact) {
        ogs_error("bsf_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", sess->dnn));
        return false;
    }

    xact->assoc_stream = stream;

    if (ogs_sbi_discover_and_send(xact,
            (ogs_fsm_handler_t)bsf_nf_state_registered, client_cb) != true) {
        ogs_error("bsf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", sess->dnn));
        return false;
    }

    return true;
}

void bsf_sbi_send_response(ogs_sbi_stream_t *stream, int status)
{
    ogs_sbi_message_t sendmsg;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(stream);

    memset(&sendmsg, 0, sizeof(sendmsg));

    response = ogs_sbi_build_response(&sendmsg, status);
    ogs_assert(response);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));
}
