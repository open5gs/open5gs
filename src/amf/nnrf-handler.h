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

#ifndef AMF_NNRF_HANDLER_H
#define AMF_NNRF_HANDLER_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

void amf_nnrf_handle_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_message_t *recvmsg);
void amf_nnrf_handle_nf_status_subscribe(
        ogs_sbi_subscription_t *subscription, ogs_sbi_message_t *recvmsg);

bool amf_nnrf_handle_nf_status_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

void amf_nnrf_handle_nf_discover(
        ogs_sbi_xact_t *xact, ogs_sbi_message_t *recvmsg);
void amf_nnrf_handle_nf_discover_search_result(
        ogs_sbi_object_t *sbi_object, OpenAPI_search_result_t *SearchResult);

#ifdef __cplusplus
}
#endif

#endif /* AMF_NNRF_HANDLER_H */
