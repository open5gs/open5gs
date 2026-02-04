/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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
#include "context.h"
#include "nnrf-handler.h"
#include "namf-build.h"
#include "namf-handler.h"
#include "nnrf-build.h"

int lmf_sbi_open(void)
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

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(OGS_SBI_SERVICE_NAME_NLMF_LOC)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance, OGS_SBI_SERVICE_NAME_NLMF_LOC);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, OGS_SBI_API_V1, OGS_SBI_API_V1_0_0, NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_AMF);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void lmf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

int lmf_amf_sbi_discover_and_send(
        ogs_sbi_service_type_e service_type,
        ogs_sbi_discovery_option_t *discovery_option,
        ogs_sbi_request_t *(*build)(lmf_location_request_t *location_request, void *data),
        lmf_location_request_t *location_request, void *data)
{
    int rv;
    ogs_sbi_xact_t *xact = NULL;

    ogs_assert(service_type);
    ogs_assert(location_request);
    ogs_assert(build);

    xact = ogs_sbi_xact_add(
            location_request->id, &location_request->sbi, 
            service_type, discovery_option,
            (ogs_sbi_build_f)build, location_request, data);
    if (!xact) {
        ogs_error("ogs_sbi_xact_add() failed");
        return OGS_ERROR;
    }

    location_request->xact = xact;

    rv = ogs_sbi_discover_and_send(xact);
    if (rv != OGS_OK) {
        ogs_error("ogs_sbi_discover_and_send() failed");
        ogs_sbi_xact_remove(xact);
        return rv;
    }

    return OGS_OK;
}

