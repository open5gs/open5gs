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

#ifndef SGSAP_BUILD_H
#define SGSAP_BUILD_H

#include "mme-context.h"

#include "sgsap-types.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_pkbuf_t *sgsap_build_location_update_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_tmsi_reallocation_complete(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_detach_indication(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_mo_csfb_indication(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_paging_reject(
    ogs_nas_mobile_identity_imsi_t *nas_mobile_identity_imsi,
    int nas_mobile_identity_imsi_len, uint8_t sgs_cause);
ogs_pkbuf_t *sgsap_build_service_request(mme_ue_t *mme_ue, uint8_t emm_mode);
ogs_pkbuf_t *sgsap_build_reset_ack(mme_vlr_t *vlr);
ogs_pkbuf_t *sgsap_build_uplink_unidata(mme_ue_t *mme_ue,
        ogs_nas_eps_message_container_t *nas_message_container);
ogs_pkbuf_t *sgsap_build_ue_unreachable(mme_ue_t *mme_ue, uint8_t sgs_cause);

#ifdef __cplusplus
}
#endif

#endif /* SGSAP_BUILD_H */
