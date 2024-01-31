/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SMSF_NAMF_BUILD_H
#define SMSF_NAMF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct smsf_n1_n2_message_transfer_param_s {
    ogs_pkbuf_t *n1smbuf;
    bool n1n2_failure_txf_notif_uri;
    bool skip_ind;
} smsf_n1_n2_message_transfer_param_t;

ogs_sbi_request_t *smsf_namf_comm_build_n1_n2_message_transfer(
        smsf_ue_t *smsf_ue, smsf_n1_n2_message_transfer_param_t *param);

#ifdef __cplusplus
}
#endif

#endif /* SMSF_NAMF_BUILD_H */
