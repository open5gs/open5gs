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

#ifndef TEST_NON3GPP_GTP_HANDLE_H
#define TEST_NON3GPP_GTP_HANDLE_H

#include "test-common.h"

#ifdef __cplusplus
extern "C" {
#endif

void test_s2b_handle_create_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_create_session_response_t *rsp);
void test_s2b_handle_delete_session_response(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_delete_session_response_t *rsp);

void test_s2b_handle_create_bearer_request(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_create_bearer_request_t *req);
void test_s2b_handle_delete_bearer_request(
        ogs_gtp_xact_t *xact, test_sess_t *sess,
        ogs_gtp2_delete_bearer_request_t *req);

#ifdef __cplusplus
}
#endif

#endif /* TEST_NON3GPP_GTP_HANDLE_H */
