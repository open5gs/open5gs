/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

int nssf_sbi_open(void)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *service = NULL;

    /* Initialize SELF NF instance */
    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_assert(nf_instance);
    ogs_sbi_nf_fsm_init(nf_instance);

    /* Build NF instance information. It will be transmitted to NRF. */
    ogs_sbi_nf_instance_build_default(nf_instance);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SCP);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_AMF);
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_NSSF);

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(
                OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NNSSF_NSSELECTION);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V2, OGS_SBI_API_V2_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_NSSF);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* Setup Subscription-Data */
    ogs_sbi_subscription_spec_add(OpenAPI_nf_type_SEPP, NULL);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void nssf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool nssf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}

int nssf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(nssf_home_t *home, void *data),
        nssf_home_t *home, ogs_sbi_stream_t *stream, void *data)
{
    ogs_sbi_xact_t *xact = NULL;
    int r;

    ogs_assert(service_type);
    ogs_assert(home);
    ogs_assert(stream);
    ogs_assert(build);

    ogs_assert(home->id >= OGS_MIN_POOL_ID && home->id <= OGS_MAX_POOL_ID);

    xact = ogs_sbi_xact_add(
            home->id, &home->sbi, service_type, discovery_option,
            (ogs_sbi_build_f)build, home, data);
    if (!xact) {
        ogs_error("nssf_sbi_discover_and_send() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL, NULL));
        return OGS_ERROR;
    }

    xact->assoc_stream_id = ogs_sbi_id_from_stream(stream);
    ogs_assert(xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
            xact->assoc_stream_id <= OGS_MAX_POOL_ID);

    r = ogs_sbi_discover_and_send(xact);
    if (r != OGS_OK) {
        ogs_error("nssf_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                "Cannot discover", NULL, NULL));
        return r;
    }

    return OGS_OK;
}
