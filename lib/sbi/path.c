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

#include "ogs-sbi.h"

void ogs_nnrf_nfm_send_nf_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_register(nf_instance);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, nf_instance);
}

void ogs_nnrf_nfm_send_nf_update(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_update(nf_instance);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, nf_instance);
}

void ogs_nnrf_nfm_send_nf_de_register(ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_de_register(nf_instance);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, nf_instance);
}

void ogs_nnrf_nfm_send_nf_status_subscribe(ogs_sbi_client_t *client,
        OpenAPI_nf_type_e nf_type, char *nf_instance_id)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_subscription_t *subscription = NULL;

    ogs_assert(client);

    subscription = ogs_sbi_subscription_add();
    ogs_assert(subscription);

    OGS_SETUP_SBI_CLIENT(subscription, client);
    subscription->nf_type = nf_type;
    if (nf_instance_id)
        subscription->nf_instance_id = ogs_strdup(nf_instance_id);

    request = ogs_nnrf_nfm_build_status_subscribe(subscription);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, subscription);
}

void ogs_nnrf_nfm_send_nf_status_unsubscribe(
        ogs_sbi_subscription_t *subscription)
{
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_client_t *client = NULL;

    ogs_assert(subscription);
    client = subscription->client;
    ogs_assert(client);

    request = ogs_nnrf_nfm_build_status_unsubscribe(subscription);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, subscription);
}

void ogs_nnrf_disc_send_nf_discover(ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type, OpenAPI_nf_type_e requester_nf_type,
        void *data)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_disc_build_discover(target_nf_type, requester_nf_type);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, data);
}
