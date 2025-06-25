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

#ifndef NGAP_PATH_H
#define NGAP_PATH_H

#include "ogs-core.h"
#include "context.h"
#include "sbc-message.h"

#define NGAP_NON_UE_SIGNALLING 0

static inline ogs_pkbuf_t *ngap_build_write_replace_warning_request(sbc_pws_data_t *sbc_pws) { return NULL; }
static inline ogs_pkbuf_t *ngap_build_kill_request(sbc_pws_data_t *sbc_pws) { return NULL; }

#ifdef __cplusplus
extern "C" {
#endif

/* PWS-IWS NGAP-AMF Message Functions */
int pwsiws_ngap_send_write_replace_warning_request(sbc_pws_data_t *sbc_pws);
int pwsiws_ngap_send_kill_request(sbc_pws_data_t *sbc_pws);
int pwsiws_ngap_send_to_amf(ogs_pkbuf_t *buf);

#ifdef __cplusplus
}
#endif

#endif /* NGAP_PATH_H */ 