/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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

int chf_sbi_open(void)
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
    ogs_sbi_nf_instance_add_allowed_nf_type(nf_instance, OpenAPI_nf_type_SMF);

    /* Register CHF NF info (chfInfo) with SUPI ranges if configured.
     * TS 29.510 §6.1.6.2.9 — ChfInfo / SupiRangeList */
    if (chf_self()->num_of_supi_range > 0) {
        int i;
        ogs_sbi_nf_info_t *nf_info =
            ogs_sbi_nf_info_add(&nf_instance->nf_info_list,
                                OpenAPI_nf_type_CHF);
        ogs_assert(nf_info);

        for (i = 0; i < chf_self()->num_of_supi_range; i++) {
            if (chf_self()->supi_range[i].start)
                nf_info->chf.supi_range[i].start =
                    ogs_strdup(chf_self()->supi_range[i].start);
            if (chf_self()->supi_range[i].end)
                nf_info->chf.supi_range[i].end =
                    ogs_strdup(chf_self()->supi_range[i].end);
        }
        nf_info->chf.num_of_supi_range = chf_self()->num_of_supi_range;

        ogs_info("CHF NF info registered with %d SUPI range(s)",
                 chf_self()->num_of_supi_range);
    }

    /*
     * Slice restriction, as NFProfile.allowedNssais (TS 33.518 §4.2.2.2.1).
     *
     * Not part of chfInfo: TS 29.510's ChfInfo has no S-NSSAI member, only
     * SUPI, GPSI and PLMN ranges.  allowedNssais is the NF-profile-level
     * mechanism and the NRF applies it to any NF type, so an SMF asking for
     * a CHF on a given S-NSSAI only discovers the instances that serve it.
     *
     * Leaving charging.slices empty registers no allowedNssais at all, which
     * leaves this instance eligible for every slice.  That is the default,
     * and it is what every chf.yaml written before this key existed gets.
     */
    if (chf_self()->num_of_slice > 0) {
        int i;

        for (i = 0; i < chf_self()->num_of_slice; i++)
            nf_instance->allowed_nssai[i] = chf_self()->slice[i];
        nf_instance->num_of_allowed_nssai = chf_self()->num_of_slice;

        for (i = 0; i < chf_self()->num_of_slice; i++) {
            if (chf_self()->slice[i].sd.v == OGS_S_NSSAI_NO_SD_VALUE)
                ogs_info("CHF serves slice SST:%d (no SD)",
                         chf_self()->slice[i].sst);
            else
                ogs_info("CHF serves slice SST:%d SD:%06x",
                         chf_self()->slice[i].sst,
                         chf_self()->slice[i].sd.v);
        }
        ogs_info("CHF restricted to %d slice(s) — an SMF on any other "
                 "S-NSSAI will not discover this instance",
                 chf_self()->num_of_slice);
    }

    /* Build NF service information. It will be transmitted to NRF. */
    if (ogs_sbi_nf_service_is_available(
                OpenAPI_service_name_nchf_convergedcharging)) {
        service = ogs_sbi_nf_service_build_default(
                    nf_instance,
                    OpenAPI_service_name_nchf_convergedcharging);
        ogs_assert(service);
        ogs_sbi_nf_service_add_version(
                    service, CHF_API_VERSION, "3.0.0", NULL);
        ogs_sbi_nf_service_add_allowed_nf_type(service, OpenAPI_nf_type_SMF);
    }

    /* Initialize NRF NF Instance */
    nf_instance = ogs_sbi_self()->nrf_instance;
    if (nf_instance)
        ogs_sbi_nf_fsm_init(nf_instance);

    /* No external NF subscriptions needed for Phase 1 */

    if (ogs_sbi_server_start_all(ogs_sbi_server_handler) != OGS_OK)
        return OGS_ERROR;

    return OGS_OK;
}

void chf_sbi_close(void)
{
    ogs_sbi_client_stop_all();
    ogs_sbi_server_stop_all();
}

bool chf_sbi_send_request(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_xact_t *xact)
{
    ogs_assert(nf_instance);
    ogs_assert(xact);
    return ogs_sbi_send_request_to_nf_instance(nf_instance, xact);
}
