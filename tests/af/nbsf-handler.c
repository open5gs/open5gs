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

#include "sbi-path.h"
#include "nbsf-handler.h"

void af_nbsf_management_handle_pcf_binding(
        af_sess_t *sess, ogs_sbi_message_t *recvmsg)
{
    int i, rv;

    OpenAPI_pcf_binding_t *PcfBinding = NULL;
    OpenAPI_list_t *PcfIpEndPointList = NULL;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(sess);
    ogs_assert(recvmsg);

    PcfBinding = recvmsg->PcfBinding;
    ogs_assert(PcfBinding);

    if (!PcfBinding->pcf_fqdn &&
        !PcfBinding->pcf_ip_end_points) {
        ogs_error("No PCF address information [%p:%p]",
                    PcfBinding->pcf_fqdn, PcfBinding->pcf_ip_end_points);
        return;
    }

    if (PcfBinding->pcf_fqdn) {
        if (sess->pcf.fqdn)
            ogs_free(sess->pcf.fqdn);
        sess->pcf.fqdn = ogs_strdup(PcfBinding->pcf_fqdn);
        ogs_assert(sess->pcf.fqdn);
    }

    PcfIpEndPointList = PcfBinding->pcf_ip_end_points;

    for (i = 0; i < sess->pcf.num_of_ip; i++) {
        if (sess->pcf.ip[i].addr)
            ogs_freeaddrinfo(sess->pcf.ip[i].addr);
        if (sess->pcf.ip[i].addr6)
            ogs_freeaddrinfo(sess->pcf.ip[i].addr6);
    }
    sess->pcf.num_of_ip = 0;

    OpenAPI_list_for_each(PcfIpEndPointList, node) {
        OpenAPI_ip_end_point_t *IpEndPoint = node->data;
        ogs_sockaddr_t *addr = NULL, *addr6 = NULL;
        int port = 0;

        if (!IpEndPoint) continue;

        if (sess->pcf.num_of_ip < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
            if (!IpEndPoint->is_port)
                port = ogs_sbi_client_default_port();
            else
                port = IpEndPoint->port;

            if (IpEndPoint->ipv4_address) {
                rv = ogs_getaddrinfo(&addr, AF_UNSPEC,
                        IpEndPoint->ipv4_address, port, 0);
                if (rv != OGS_OK) continue;
            }
            if (IpEndPoint->ipv6_address) {
                rv = ogs_getaddrinfo(&addr6, AF_UNSPEC,
                        IpEndPoint->ipv6_address, port, 0);
                if (rv != OGS_OK) continue;
            }

            if (addr || addr6) {
                sess->pcf.ip[sess->pcf.num_of_ip].is_port = IpEndPoint->is_port;
                sess->pcf.ip[sess->pcf.num_of_ip].port = port;
                sess->pcf.ip[sess->pcf.num_of_ip].addr = addr;
                sess->pcf.ip[sess->pcf.num_of_ip].addr6 = addr6;
                sess->pcf.num_of_ip++;
            }
        }
    }

    if (PcfBinding->supi) {
        if (sess->supi)
            ogs_free(sess->supi);
        sess->supi = ogs_strdup(PcfBinding->supi);
        ogs_assert(sess->supi);
    }
    if (PcfBinding->gpsi) {
        if (sess->gpsi)
            ogs_free(sess->gpsi);
        sess->gpsi = ogs_strdup(PcfBinding->gpsi);
        ogs_assert(sess->gpsi);
    }

    af_sess_associate_pcf_client(sess);
}
