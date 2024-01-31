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

#ifndef SMSF_NUDM_HANDLER_H
#define SMSF_NUDM_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "context.h"

int smsf_nudm_sdm_handle_provisioned_data(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);
int smsf_nudm_sdm_handle_subscription(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);
bool smsf_nudm_uecm_handle_smsf_registration(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);
int smsf_nudm_uecm_handle_smsf_registration_delete(
    smsf_ue_t *smsf_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

#ifdef __cplusplus
}
#endif

#endif /* SMSF_NUDM_HANDLER_H */
