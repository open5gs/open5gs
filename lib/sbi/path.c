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

static ogs_sbi_nf_instance_t *find_or_discover_nf_instance(
        ogs_sbi_object_t *sbi_object, bool *nrf, bool *nf)
{
    ogs_assert(sbi_object);
    ogs_assert(sbi_object->nf_type);
    ogs_assert(sbi_object->nf_state_registered);
    ogs_assert(sbi_object->client_wait.duration);
    ogs_assert(nrf);
    ogs_assert(nf);

    if (!OGS_SBI_NF_INSTANCE_GET(sbi_object->nf_types, OpenAPI_nf_type_NRF))
        *nrf = ogs_sbi_nf_types_associate(sbi_object->nf_types,
                OpenAPI_nf_type_NRF, sbi_object->nf_state_registered);
    if (!OGS_SBI_NF_INSTANCE_GET(sbi_object->nf_types,
                sbi_object->nf_type))
        *nf = ogs_sbi_nf_types_associate(sbi_object->nf_types,
                sbi_object->nf_type, sbi_object->nf_state_registered);

    if (*nrf == false && *nf == false) {
        ogs_error("Cannot discover [%s]",
                OpenAPI_nf_type_ToString(sbi_object->nf_type));
        return NULL;
    }

    if (*nf == false) {
        ogs_warn("[%s] Try to discover",
                OpenAPI_nf_type_ToString(sbi_object->nf_type));
        ogs_timer_start(sbi_object->client_wait.timer,
                sbi_object->client_wait.duration);

        ogs_nnrf_disc_send_nf_discover(
            sbi_object->nf_types[OpenAPI_nf_type_NRF].nf_instance,
            sbi_object->nf_type, sbi_object);

        return NULL;
    }

    return sbi_object->nf_types[sbi_object->nf_type].nf_instance;
}

void ogs_sbi_send(
        ogs_sbi_object_t *sbi_object, ogs_sbi_nf_instance_t *nf_instance)
{
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sbi_object);
    request = sbi_object->request;
    ogs_assert(request);
    ogs_assert(sbi_object->client_wait.duration);

    ogs_assert(nf_instance);

    ogs_timer_start(sbi_object->client_wait.timer,
            sbi_object->client_wait.duration);

    ogs_sbi_client_send_request_to_nf_instance(
            nf_instance, sbi_object->request, sbi_object);
}

bool ogs_sbi_discover_and_send(
        OpenAPI_nf_type_e nf_type, ogs_sbi_object_t *sbi_object, void *data,
        ogs_sbi_build_f build)
{
    bool nrf = true;
    bool nf = true;

    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(sbi_object);
    ogs_assert(nf_type);
    ogs_assert(build);

    sbi_object->running = true;

    sbi_object->nf_type = nf_type;
    if (sbi_object->request)
        ogs_sbi_request_free(sbi_object->request);
    sbi_object->request = (*build)(sbi_object, data);

    nf_instance = find_or_discover_nf_instance(sbi_object, &nrf, &nf);
    if (nrf == false && nf == false) return false;
    if (!nf_instance) return true;

    ogs_sbi_send(sbi_object, nf_instance);

    return true;
}

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
    ogs_sbi_request_free(request);
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
    ogs_sbi_request_free(request);
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
    ogs_sbi_request_free(request);
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
    ogs_sbi_request_free(request);
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
    ogs_sbi_request_free(request);
}

void ogs_nnrf_disc_send_nf_discover(ogs_sbi_nf_instance_t *nf_instance,
        OpenAPI_nf_type_e target_nf_type, void *data)
{
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(nf_instance);
    ogs_assert(nf_instance->nf_type);
    client = nf_instance->client;
    ogs_assert(client);

    request = ogs_nnrf_disc_build_discover(
            target_nf_type, nf_instance->nf_type);
    ogs_assert(request);
    ogs_sbi_client_send_request(client, request, data);
    ogs_sbi_request_free(request);
}
