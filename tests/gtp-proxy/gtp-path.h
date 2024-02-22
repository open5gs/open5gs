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

#ifndef TEST_NON3GPP_GTP_PATH_H
#define TEST_NON3GPP_GTP_PATH_H

#include "test-common.h"

#ifdef __cplusplus
extern "C" {
#endif

ogs_socknode_t *test_sgw_server(uint16_t port);
void test_sgw_close(ogs_socknode_t *node);

ogs_gtp_node_t* test_gtpc_proxy_node_new(void);

ogs_pkbuf_t *test_sgw_read(ogs_socknode_t *node, ogs_sockaddr_t* from);

void test_s8_recv(test_sess_t *sess, ogs_pkbuf_t *pkbuf);

int test_s8_send_create_session_request(test_sess_t *sess, bool handover_ind);
int test_s8_send_delete_session_request(test_sess_t *sess);

#ifdef __cplusplus
}
#endif

#endif /* TEST_NON3GPP_GTP_PATH_H */
