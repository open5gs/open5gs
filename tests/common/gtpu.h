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

#ifndef TEST_COMMON_GTPU_H
#define TEST_COMMON_GTPU_H

#ifdef __cplusplus
extern "C" {
#endif

#define testgnb_gtpu_read(x) test_gtpu_read(x)
#define testgnb_gtpu_close(x) test_gtpu_close(x)

ogs_socknode_t *test_gtpu_server(int index, int family);
ogs_pkbuf_t *test_gtpu_read(ogs_socknode_t *node);
void test_gtpu_close(ogs_socknode_t *node);

void testgtpu_recv(test_ue_t *test_ue, ogs_pkbuf_t *pkbuf);

int test_gtpu_send(
        ogs_socknode_t *node, test_bearer_t *bearer,
        ogs_gtp2_header_desc_t *header_desc, ogs_pkbuf_t *pkbuf);
int test_gtpu_send_ping(
        ogs_socknode_t *node, test_bearer_t *bearer, const char *dst_ip);
int test_gtpu_send_slacc_rs(ogs_socknode_t *node, test_bearer_t *bearer);
int test_gtpu_send_slacc_rs_with_unspecified_source_address(
        ogs_socknode_t *node, test_bearer_t *bearer);
int test_gtpu_send_error_indication(
        ogs_socknode_t *node, test_bearer_t *bearer);
int test_gtpu_send_indirect_data_forwarding(
        ogs_socknode_t *node, test_bearer_t *bearer, ogs_pkbuf_t *pkbuf);

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_GTPU_H */
