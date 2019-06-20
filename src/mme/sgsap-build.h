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

#include "asn1c/s1ap-message.h"
#include "mme-context.h"

#include "sbc-message.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SGSAP_PAGING_REQUEST                1
#define SGSAP_SERVICE_REQUEST               6
#define SGSAP_LOCATION_UPDATE_REQUEST       9
#define SGSAP_LOCATION_UPDATE_ACCEPT        10
#define SGSAP_TMSI_REALLOCATION_COMPLETE    12
#define SGSAP_MO_CSFB_INDICIATION           24

ogs_pkbuf_t *sgsap_build_location_update_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_tmsi_reallocation_complete(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_service_request(mme_ue_t *mme_ue);
ogs_pkbuf_t *sgsap_build_mo_csfb_indication(mme_ue_t *mme_ue);

#ifdef __cplusplus
}
#endif

#endif /* SGSAP_BUILD_H */
