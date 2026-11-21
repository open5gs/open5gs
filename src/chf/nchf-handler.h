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

#ifndef CHF_NCHF_HANDLER_H
#define CHF_NCHF_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Nchf_ConvergedCharging service handlers (TS 32.291) */
bool chf_nchf_convergedcharging_handle_create(
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message);
bool chf_nchf_convergedcharging_handle_update(
        chf_sess_t *sess,
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message);
bool chf_nchf_convergedcharging_handle_release(
        chf_sess_t *sess,
        ogs_sbi_stream_t *stream,
        ogs_sbi_request_t *request, ogs_sbi_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* CHF_NCHF_HANDLER_H */
