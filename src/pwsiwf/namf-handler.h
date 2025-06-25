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

#ifndef PWSIWS_NONUEN2_HANDLER_H
#define PWSIWS_NONUEN2_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "context.h"

bool pwsiws_nonuen2_comm_handle_nonuen2_message_transfer(
        pwsiws_warning_t *warning, int state, ogs_sbi_message_t *recvmsg);

bool pwsiws_nonuen2_comm_handle_nonuen2_message_transfer_failure_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWS_NONUEN2_HANDLER_H */