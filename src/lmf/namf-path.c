/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "namf-path.h"
#include "namf-build.h"
#include "sbi-path.h"

int lmf_amf_send_nrppa_measurement_request(lmf_location_request_t *location_request)
{
    int rv;
    ogs_sbi_discovery_option_t *discovery_option = NULL;

    ogs_assert(location_request);

    ogs_info("[%s] Sending NRPPa measurement request to AMF",
            location_request->supi ? location_request->supi : "Unknown");

    /* Build discovery option */
    discovery_option = ogs_sbi_discovery_option_new();
    if (!discovery_option) {
        ogs_error("[%s] ogs_sbi_discovery_option_new() failed",
                location_request->supi ? location_request->supi : "Unknown");
        return OGS_ERROR;
    }

    /* If AMF ID is provided and looks like a UUID, verify it exists before using it */
    /* NF Instance ID must be a UUID v4 format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx */
    if (location_request->amf_id) {
        /* Simple UUID format check: should have 4 hyphens and be ~36 chars */
        const char *amf_id = location_request->amf_id;
        int hyphen_count = 0;
        int i;
        bool looks_like_uuid = false;
        ogs_sbi_nf_instance_t *nf_instance = NULL;
        
        for (i = 0; amf_id[i] != '\0'; i++) {
            if (amf_id[i] == '-')
                hyphen_count++;
        }
        
        /* UUID format: 8-4-4-4-12 = 36 chars with 4 hyphens */
        if (i == 36 && hyphen_count == 4) {
            looks_like_uuid = true;
        }
        
        if (looks_like_uuid) {
            /* Verify that this AMF instance actually exists and is registered */
            nf_instance = ogs_sbi_nf_instance_find(location_request->amf_id);
            if (nf_instance && nf_instance->nf_type == OpenAPI_nf_type_AMF) {
                ogs_sbi_discovery_option_set_target_nf_instance_id(
                        discovery_option, location_request->amf_id);
                ogs_info("[%s] Using AMF ID (UUID) for discovery: %s",
                        location_request->supi ? location_request->supi : "Unknown",
                        location_request->amf_id);
            } else {
                ogs_warn("[%s] AMF ID '%s' not found in registered instances, "
                        "performing general AMF discovery",
                        location_request->supi ? location_request->supi : "Unknown",
                        location_request->amf_id);
            }
        } else {
            ogs_warn("[%s] AMF ID '%s' is not a valid UUID format, "
                    "performing general AMF discovery",
                    location_request->supi ? location_request->supi : "Unknown",
                    location_request->amf_id);
        }
    }
    
    if (!discovery_option->target_nf_instance_id) {
        ogs_info("[%s] Discovering AMF via NRF (general discovery)",
                location_request->supi ? location_request->supi : "Unknown");
    }

    /* Send NRPPa measurement request to AMF */
    rv = lmf_amf_sbi_discover_and_send(
            OGS_SBI_SERVICE_TYPE_NAMF_COMM, discovery_option,
            (ogs_sbi_request_t *(*)(lmf_location_request_t *, void *))lmf_namf_build_nrppa_measurement_request,
            location_request, NULL);

    if (rv != OGS_OK) {
        ogs_error("[%s] lmf_amf_sbi_discover_and_send() failed: %d",
                location_request->supi ? location_request->supi : "Unknown", rv);
        /* discovery_option is freed by ogs_sbi_xact_remove() in sbi-path.c */
    } else {
        ogs_info("[%s] NRPPa measurement request sent to AMF, waiting for response",
                location_request->supi ? location_request->supi : "Unknown");
        /* discovery_option is freed by ogs_sbi_xact_remove() when transaction completes */
    }

    return rv;
}

