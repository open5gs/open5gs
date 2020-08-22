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

#ifndef TEST_NGAP_HANDLE_H
#define TEST_NGAP_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif

void testngap_handle_ng_setup_response(
        test_ue_t *test_ue, ogs_ngap_message_t *message);

void testngap_handle_downlink_nas_transport(
        test_ue_t *test_ue, ogs_ngap_message_t *message);
void testngap_handle_initial_context_setup_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message);
void testngap_handle_ue_release_context_command(
        test_ue_t *test_ue, ogs_ngap_message_t *message);
void testngap_handle_pdu_session_resource_setup_request(
        test_ue_t *test_ue, ogs_ngap_message_t *message);
void testngap_handle_pdu_session_resource_release_command(
        test_ue_t *test_ue, ogs_ngap_message_t *message);

#ifdef __cplusplus
}
#endif

#endif /* TEST_NGAP_HANDLE_H */
