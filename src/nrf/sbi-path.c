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
    nrf_event_t *e = NULL;
    int rv;

    ogs_assert(request);
    ogs_assert(data);

    e = nrf_event_new(NRF_EVT_SBI_SERVER);
    ogs_assert(e);

    e->sbi.request = request;
    e->sbi.data = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        nrf_event_free(e);
        return OGS_ERROR;
    }

    return OGS_OK;
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
        ogs_warn("Subscription notification failed [%d]", message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}

int nrf_sbi_open(void)
{
    ogs_sbi_server_start_all(server_cb);

    return OGS_OK;
}

void nrf_sbi_close(void)
{
    ogs_sbi_server_stop_all();
}

void nrf_nnrf_nfm_send_nf_status_notify(ogs_sbi_subscription_t *subscription,
        OpenAPI_notification_event_type_e event,
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(subscription);
    client = subscription->client;
    ogs_assert(client);

    request = nrf_nnrf_nfm_build_nf_status_notify(
            client, subscription, event, nf_instance);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, client_notify_cb, request, NULL);
}

void nrf_nnrf_nfm_send_nf_status_notify_all(
        OpenAPI_notification_event_type_e event,
        ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_subscription_t *subscription = NULL;

    ogs_assert(nf_instance);

    ogs_list_for_each(&ogs_sbi_self()->subscription_list, subscription) {

        if (strcmp(subscription->nf_instance_id, nf_instance->id) == 0)
            continue;

        nrf_nnrf_nfm_send_nf_status_notify(subscription, event, nf_instance);
    }
}
