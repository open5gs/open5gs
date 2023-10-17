/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef OGS_NNRF_HANDLER_H
#define OGS_NNRF_HANDLER_H

#ifdef __cplusplus
extern "C" {
#endif

void ogs_nnrf_nfm_handle_nf_register(
        ogs_sbi_nf_instance_t *nf_instance, ogs_sbi_message_t *recvmsg);
void ogs_nnrf_nfm_handle_nf_profile(
        ogs_sbi_nf_instance_t *nf_instance, OpenAPI_nf_profile_t *NFProfile);

void ogs_nnrf_nfm_handle_nf_status_subscribe(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg);
void ogs_nnrf_nfm_handle_nf_status_update(
        ogs_sbi_subscription_data_t *subscription_data,
        ogs_sbi_message_t *recvmsg);
bool ogs_nnrf_nfm_handle_nf_status_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg);

void ogs_nnrf_disc_handle_nf_discover_search_result(
        OpenAPI_search_result_t *SearchResult);

#ifdef __cplusplus
}
#endif

#endif /* OGS_NNRF_HANDLER_H */
