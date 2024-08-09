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

#ifndef TEST_COMMON_SCTP_H
#define TEST_COMMON_SCTP_H

#ifdef __cplusplus
extern "C" {
#endif

extern ogs_sockaddr_t ogs_test_sctp_last_addr;

ogs_socknode_t *testsctp_server(const char *ipstr, int port);
ogs_socknode_t *testsctp_client(const char *ipstr, int port);
ogs_socknode_t *tests1ap_client(int family);
ogs_socknode_t *testngap_client(int index, int family);

int testsctp_send(ogs_socknode_t *node, ogs_pkbuf_t *pkbuf,
        int ppid, uint16_t stream_no, int type);
ogs_pkbuf_t *testsctp_read(ogs_socknode_t *node, int type);

#define testenb_s1ap_read(x) testsctp_read(x, 0);
#define testenb_s1ap_send(x, y) \
    testsctp_send(x, y, OGS_SCTP_S1AP_PPID, 0, 0)
#define testenb_s1ap_close ogs_socknode_free

#define testvlr_sgsap_server(x) testsctp_server(x, OGS_SGSAP_SCTP_PORT)
#define testvlr_sgsap_read(x) testsctp_read(x, 1);
#define testvlr_sgsap_send(x, y) \
    testsctp_send(x, y, OGS_SCTP_SGSAP_PPID, 0, 1)
#define testvlr_sgsap_close ogs_socknode_free

#define testgnb_ngap_read(x) testsctp_read(x, 0);
#define testgnb_ngap_send(x, y) \
    testsctp_send(x, y, OGS_SCTP_NGAP_PPID, 0, 0)
#define testgnb_ngap_close ogs_socknode_free

#ifdef __cplusplus
}
#endif

#endif /* TEST_COMMON_SCTP_H */
