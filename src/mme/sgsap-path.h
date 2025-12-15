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

#ifndef SGSAP_PATH_H
#define SGSAP_PATH_H

#include "mme-context.h"
#include "mme-event.h"

#include "sgsap-build.h"

#ifdef __cplusplus
extern "C" {
#endif

#define sgsap_event_push mme_sctp_event_push

int sgsap_open(void);
void sgsap_close(void);

ogs_sock_t *sgsap_client(mme_vlr_t *vlr);

int sgsap_send(ogs_sock_t *sock, ogs_pkbuf_t *pkbuf, uint16_t stream_no);

int sgsap_send_to_vlr_with_sid(
        mme_vlr_t *vlr, ogs_pkbuf_t *pkbuf, uint16_t stream_no);
int sgsap_send_to_vlr(mme_ue_t *mme_ue, ogs_pkbuf_t *pkbuf);

int sgsap_send_location_update_request(mme_ue_t *mme_ue);
int sgsap_send_tmsi_reallocation_complete(mme_ue_t *mme_ue);
int sgsap_send_detach_indication(mme_ue_t *mme_ue);
int sgsap_send_mo_csfb_indication(mme_ue_t *mme_ue);
int sgsap_send_paging_reject(mme_ue_t *mme_ue, uint8_t sgs_cause);
int sgsap_send_service_request(mme_ue_t *mme_ue, uint8_t emm_mode);
int sgsap_send_reset_ack(mme_vlr_t *vlr);
int sgsap_send_uplink_unitdata(mme_ue_t *mme_ue,
        ogs_nas_eps_message_container_t *nas_message_container);
int sgsap_send_ue_unreachable(mme_ue_t *mme_ue, uint8_t sgs_cause);

#ifdef __cplusplus
}
#endif

#endif /* SGSAP_PATH_H */
