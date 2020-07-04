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

static int server_cb(ogs_sbi_server_t *server,
        ogs_sbi_session_t *session, ogs_sbi_request_t *request)
{
    ausf_event_t *e = NULL;
    int rv;

    ogs_assert(session);
    ogs_assert(request);

    e = ausf_event_new(AUSF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.server = server;
    e->sbi.session = session;
    e->sbi.request = request;

    rv = ogs_queue_push(ausf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ausf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    ausf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = ausf_event_new(AUSF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ausf_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ausf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int ausf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_server_start_all(server_cb);

    ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance) {
        ogs_sbi_nf_service_t *service = NULL;

        ogs_sbi_nf_instance_build_default(nf_instance, ausf_self()->nf_type);

        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NAUSF_AUTH);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V1,
                (char*)OGS_SBI_API_V1_0_0, NULL);

        ausf_nf_fsm_init(nf_instance);
        ausf_sbi_setup_client_callback(nf_instance);
    }

    return OGS_OK;
}

void ausf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

void ausf_sbi_setup_client_callback(ogs_sbi_nf_instance_t *nf_instance)
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

void ausf_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, ausf_ue_t *ausf_ue, void *data,
        ogs_sbi_request_t *(*build)(ausf_ue_t *ausf_ue, void *data))
{
    ogs_sbi_session_t *session = NULL;

    ogs_assert(ausf_ue);
    session = ausf_ue->sbi.session;
    ogs_assert(nf_type);
    ogs_assert(build);

    ausf_ue->sbi.nf_state_registered = ausf_nf_state_registered;
    ausf_ue->sbi.client_wait.duration =
        ausf_timer_cfg(AUSF_TIMER_SBI_CLIENT_WAIT)->duration;

    if (ogs_sbi_discover_and_send(
            nf_type, &ausf_ue->sbi, data, (ogs_sbi_build_f)build) != true) {
        ogs_sbi_server_send_error(session,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", ausf_ue->suci);
    }
}
