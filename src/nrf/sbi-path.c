/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

static int client_notify_cb(
        int status, ogs_sbi_response_t *response, void *data);

int nrf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);

    /* Build NF instance information. */
    ogs_sbi_nf_instance_build_default(nf_instance);

    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NNRF_NFM)) {
        service = ogs_sbi_nf_service_build_default(
                nf_instance, OGS_SBI_SERVICE_NAME_NNRF_NFM);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
    }

    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NNRF_DISC)) {
        service = ogs_sbi_nf_service_build_default(
                nf_instance, OGS_SBI_SERVICE_NAME_NNRF_DISC);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
    }

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void nrf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool nrf_nnrf_nfm_send_nf_status_notify(
        ogs_sbi_subscription_data_t *subscription_data,
        OpenAPI_notification_event_type_e event,
        ogs_sbi_nf_instance_t *nf_instance)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(subscription_data);
    client = subscription_data->client;
    if (!client) {
        ogs_error("No Client");
        return false;
    }

    request = nrf_nnrf_nfm_build_nf_status_notify(
                subscription_data, event, nf_instance);
    if (!request) {
        ogs_error("nrf_nnrf_nfm_build_nf_status_notify() failed");
        return false;
    }

    rc = ogs_sbi_send_request_to_client(
            client, client_notify_cb, request, NULL);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool nrf_nnrf_nfm_send_nf_status_notify_all(
        OpenAPI_notification_event_type_e event,
        ogs_sbi_nf_instance_t *nf_instance)
{
    bool rc;
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    ogs_assert(nf_instance);

    ogs_list_for_each(
            &ogs_sbi_self()->subscription_data_list, subscription_data) {

        if (subscription_data->req_nf_instance_id &&
            strcmp(subscription_data->req_nf_instance_id, nf_instance->id) == 0)
            continue;

    /* Issue #2630 : The format of subscrCond is invalid. Must be 'oneOf'. */
        if (subscription_data->subscr_cond.nf_type) {

            if (subscription_data->subscr_cond.nf_type != nf_instance->nf_type)
                continue;

        } else if (subscription_data->subscr_cond.service_name) {

            ogs_sbi_nf_service_t *nf_service =
                ogs_sbi_nf_service_find_by_name(nf_instance,
                    subscription_data->subscr_cond.service_name);
            if (nf_service == NULL) continue;

            if (subscription_data->req_nf_type &&
                ogs_sbi_nf_service_is_allowed_nf_type(
                    nf_service, subscription_data->req_nf_type) == false)
                continue;
        } else if (subscription_data->subscr_cond.nf_instance_id) {
            if (strcmp(subscription_data->subscr_cond.nf_instance_id, nf_instance->id) != 0)
                continue;
        }

        if (subscription_data->req_nf_type &&
            ogs_sbi_nf_instance_is_allowed_nf_type(
                nf_instance, subscription_data->req_nf_type) == false)
            continue;

        rc = nrf_nnrf_nfm_send_nf_status_notify(
                subscription_data, event, nf_instance);
        if (rc == false) {
            ogs_error("nrf_nnrf_nfm_send_nf_status_notify() failed");
            return rc;
        }
    }

    return true;
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
        ogs_warn("Subscription notification failed [%d]", message.res_status);

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
    return OGS_OK;
}
