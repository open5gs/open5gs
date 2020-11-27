/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef UDM_NUDM_HANDLER_H
#define UDM_NUDM_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

bool udm_nudm_ueau_handle_get(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);
bool udm_nudm_ueau_handle_result_confirmation_inform(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

bool udm_nudm_uecm_handle_registration(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *message);

bool udm_nudm_sdm_handle_subscription_provisioned(
    udm_ue_t *udm_ue, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

#ifdef __cplusplus
}
#endif

#endif /* UDM_NUDM_HANDLER_H */
