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

#ifndef SGSAP_HANDLER_H
#define SGSAP_HANDLER_H

#include "sgsap-types.h"

#ifdef __cplusplus
extern "C" {
#endif

void sgsap_handle_location_update_accept(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_location_update_reject(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_detach_ack(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_paging_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_downlink_unitdata(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_reset_indication(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_release_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);
void sgsap_handle_mm_information_request(mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif

#endif /* SGSAP_HANDLER_H */
