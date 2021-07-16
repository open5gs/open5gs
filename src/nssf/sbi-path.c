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
    nssf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = nssf_event_new(NSSF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        nssf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

static int client_cb(ogs_sbi_response_t *response, void *data)
{
    nssf_event_t *e = NULL;
    int rv;

    ogs_assert(response);

    e = nssf_event_new(NSSF_EVT_SBI_CLIENT);
    ogs_assert(e);
    e->sbi.response = response;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        nssf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
}

int nssf_sbi_open(void)
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
        ogs_sbi_nf_instance_build_default(nf_instance, nssf_self()->nf_type);
        ogs_sbi_nf_instance_add_allowed_nf_type(
                nf_instance, OpenAPI_nf_type_AMF);

        /* Build NF service information. It will be transmitted to NRF. */
        service = ogs_sbi_nf_service_build_default(nf_instance,
                (char*)OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(service, (char*)OGS_SBI_API_V2,
                (char*)OGS_SBI_API_V2_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);

        /* Client callback is only used when NF sends to NRF */
        client = nf_instance->client;
        ogs_assert(client);
        client->cb = client_cb;

        /* NFRegister is sent and the response is received
         * by the above client callback. */
        nssf_nf_fsm_init(nf_instance);
    }

    return OGS_OK;
}

void nssf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

bool nssf_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = nssf_nnrf_nfm_build_register(nf_instance);
    ogs_expect_or_return_val(request, false);

    return ogs_sbi_client_send_request(
            client, client->cb, request, nf_instance);
}
