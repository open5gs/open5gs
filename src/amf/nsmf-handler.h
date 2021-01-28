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

#ifndef AMF_NSMF_HANDLER_H
#define AMF_NSMF_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "context.h"

int amf_nsmf_pdusession_handle_create_sm_context(
        amf_sess_t *sess, ogs_sbi_message_t *message);
int amf_nsmf_pdusession_handle_update_sm_context(
        amf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg);
int amf_nsmf_pdusession_handle_release_sm_context(amf_sess_t *sess, int state);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NSMF_HANDLER_H */
