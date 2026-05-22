/*
 * Copyright (C) 2022-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-sbi.h"

bool ogs_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);

    request = ogs_nnrf_nfm_build_register();
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_update(ogs_sbi_nf_instance_t *nf_instance)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);

    request = ogs_nnrf_nfm_build_update();
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_de_register(ogs_sbi_nf_instance_t *nf_instance)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);

    request = ogs_nnrf_nfm_build_de_register();
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_status_subscribe(
        OpenAPI_nf_type_e req_nf_type, char *req_nf_instance_id,
        OpenAPI_nf_type_e subscr_cond_nf_type,
        char *subscr_cond_service_name)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_subscription_data_t *subscription_data = NULL;

    /* Issue #2630 : The format of subscrCond is invalid. Must be 'oneOf'. */
    ogs_assert(!subscr_cond_nf_type || !subscr_cond_service_name);

    subscription_data = ogs_sbi_subscription_data_add();
    if (!subscription_data) {
        ogs_error("ogs_sbi_subscription_data_add() failed");
        return false;
    }

    subscription_data->req_nf_type = req_nf_type;
    if (req_nf_instance_id) {
        subscription_data->req_nf_instance_id = ogs_strdup(req_nf_instance_id);
        ogs_assert(subscription_data->req_nf_instance_id);
    }

    if (subscr_cond_nf_type)
        subscription_data->subscr_cond.nf_type = subscr_cond_nf_type;
    else if (subscr_cond_service_name) {
        subscription_data->subscr_cond.service_name =
            ogs_strdup(subscr_cond_service_name);
        ogs_assert(subscription_data->subscr_cond.service_name);
    } else {
        ogs_fatal("SubscrCond must be 'oneOf'.");
        ogs_assert_if_reached();
    }

    request = ogs_nnrf_nfm_build_status_subscribe(subscription_data);
    if (!request) {
        ogs_error("No Request");
        ogs_sbi_subscription_data_remove(subscription_data);
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, subscription_data);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    if (rc != true)
        ogs_sbi_subscription_data_remove(subscription_data);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_status_subscribe_renew(
        ogs_sbi_subscription_data_t *subscription_data)
{
    char *req_nf_instance_id = NULL;
    char *subscr_cond_service_name = NULL;
    OpenAPI_nf_type_e req_nf_type;
    OpenAPI_nf_type_e subscr_cond_nf_type;
    bool rc;

    ogs_assert(subscription_data);

    /*
     * The old subscription still occupies subscription_data_pool here.
     * Keep local copies of the fields needed to rebuild the request,
     * remove the old subscription first so the pool has a free slot,
     * then resubscribe.  Without this, a full pool would make renewal
     * fail forever even though the renewal itself does not change the
     * net occupancy.
     */
    req_nf_type         = subscription_data->req_nf_type;
    subscr_cond_nf_type = subscription_data->subscr_cond.nf_type;

    if (subscription_data->req_nf_instance_id) {
        req_nf_instance_id =
            ogs_strdup(subscription_data->req_nf_instance_id);
        ogs_assert(req_nf_instance_id);
    }

    if (subscription_data->subscr_cond.service_name) {
        subscr_cond_service_name =
            ogs_strdup(subscription_data->subscr_cond.service_name);
        ogs_assert(subscr_cond_service_name);
    }

    ogs_sbi_subscription_data_remove(subscription_data);
    /* subscription_data is invalid past this point */

    rc = ogs_nnrf_nfm_send_nf_status_subscribe(
            req_nf_type, req_nf_instance_id,
            subscr_cond_nf_type, subscr_cond_service_name);
    if (rc != true)
        ogs_error("NF status subscription renewal failed");

    ogs_free(req_nf_instance_id);
    ogs_free(subscr_cond_service_name);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_status_update(
        ogs_sbi_subscription_data_t *subscription_data)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription_data);

    request = ogs_nnrf_nfm_build_status_update(subscription_data);
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, subscription_data);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_status_unsubscribe(
        ogs_sbi_subscription_data_t *subscription_data)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription_data);

    request = ogs_nnrf_nfm_build_status_unsubscribe(subscription_data);
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, subscription_data);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_list_retrieve(void)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    request = ogs_nnrf_nfm_build_nflist_retrieve();
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, ogs_sbi_self()->nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_profile_get(char *nf_instance_id)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    request = ogs_nnrf_nfm_build_profile_retrieve(nf_instance_id);
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_request_to_nrf(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL,
            ogs_sbi_client_handler, request, ogs_sbi_self()->nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}
