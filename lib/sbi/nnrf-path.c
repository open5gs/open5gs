/*
 * Copyright (C) 2022 by Sukchan Lee <acetcom@gmail.com>
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

    rc = ogs_sbi_send_notification_request(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL, request, nf_instance);
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

    rc = ogs_sbi_send_notification_request(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL, request, nf_instance);
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

    rc = ogs_sbi_send_notification_request(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL, request, nf_instance);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}

bool ogs_nnrf_nfm_send_nf_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data)
{
    bool rc;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(subscription_data);

    request = ogs_nnrf_nfm_build_status_subscribe(subscription_data);
    if (!request) {
        ogs_error("No Request");
        return false;
    }

    rc = ogs_sbi_send_notification_request(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL, request, subscription_data);
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

    rc = ogs_sbi_send_notification_request(
            OGS_SBI_SERVICE_TYPE_NNRF_NFM, NULL, request, subscription_data);
    ogs_expect(rc == true);

    ogs_sbi_request_free(request);

    return rc;
}
