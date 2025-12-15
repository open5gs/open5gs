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

#ifndef PWSIWF_SAI_SBCAP_HANDLER_H
#define PWSIWF_SAI_SBCAP_HANDLER_H

#include "context.h"
#include "pwsiwf_sai-event.h"

#ifdef __cplusplus
extern "C" {
#endif

/* SBCAP Message Handler Functions */
void pwsiwf_sai_sbcap_handle_write_replace_warning_request(
        pwsiwf_sai_connection_t *connection, ogs_sbcap_message_t *message);

void pwsiwf_sai_sbcap_handle_stop_warning_request(
        pwsiwf_sai_connection_t *connection, ogs_sbcap_message_t *message);

void pwsiwf_sai_sbcap_handle_write_replace_warning_response(
        pwsiwf_sai_connection_t *connection, ogs_sbcap_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWF_SAI_SBCAP_HANDLER_H */ 