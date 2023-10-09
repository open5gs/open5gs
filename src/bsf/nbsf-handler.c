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

bool bsf_nbsf_management_handle_pcf_binding(
        bsf_sess_t *sess, ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int status = 0;
    char *strerror = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_message_t sendmsg;
    ogs_sbi_header_t header;
    ogs_sbi_response_t *response = NULL;

    OpenAPI_pcf_binding_t *RecvPcfBinding = NULL;
    OpenAPI_pcf_binding_t SendPcfBinding;
    OpenAPI_snssai_t Snssai;

    ogs_assert(stream);
    ogs_assert(recvmsg);
    server = ogs_sbi_server_from_stream(stream);
    ogs_assert(server);

    if (recvmsg->h.resource.component[1]) {
        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            memset(&sendmsg, 0, sizeof(sendmsg));

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            ogs_free(sendmsg.http.location);

            bsf_sess_remove(sess);
            break;

        CASE(OGS_SBI_HTTP_METHOD_PATCH)
            break;

        DEFAULT
            strerror = ogs_msprintf("Invalid HTTP method [%s]",
                    recvmsg->h.method);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        END
    } else {
        OpenAPI_list_t *PcfIpEndPointList = NULL;
        OpenAPI_lnode_t *node = NULL;
        int i;

        SWITCH(recvmsg->h.method)
        CASE(OGS_SBI_HTTP_METHOD_POST)

            RecvPcfBinding = recvmsg->PcfBinding;
            ogs_assert(RecvPcfBinding);

            if (!RecvPcfBinding->snssai) {
                strerror = ogs_msprintf("No S-NSSAI");
                status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                goto cleanup;
            }

            if (!RecvPcfBinding->dnn) {
                strerror = ogs_msprintf("No DNN");
                status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                goto cleanup;
            }

            if (!RecvPcfBinding->pcf_fqdn &&
                !RecvPcfBinding->pcf_ip_end_points) {
                strerror = ogs_msprintf("No PCF address information [%p:%p]",
                            RecvPcfBinding->pcf_fqdn,
                            RecvPcfBinding->pcf_ip_end_points);
                status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
                goto cleanup;
            }

            if (RecvPcfBinding->pcf_fqdn) {
                if (sess->pcf_fqdn)
                    ogs_free(sess->pcf_fqdn);
                sess->pcf_fqdn = ogs_strdup(RecvPcfBinding->pcf_fqdn);
                ogs_assert(sess->pcf_fqdn);
            }


            if (RecvPcfBinding->ipv4_frame_route_list) {
                OpenAPI_lnode_t *node = NULL;

                OpenAPI_clear_and_free_string_list(sess->ipv4_frame_route_list);
                sess->ipv4_frame_route_list = OpenAPI_list_create();
                OpenAPI_list_for_each(RecvPcfBinding->ipv4_frame_route_list, node) {
                    if (!node->data)
                        continue;
                    OpenAPI_list_add(sess->ipv4_frame_route_list, ogs_strdup(node->data));
                }
            }

            if (RecvPcfBinding->ipv6_frame_route_list) {
                OpenAPI_lnode_t *node = NULL;

                OpenAPI_clear_and_free_string_list(sess->ipv6_frame_route_list);
                sess->ipv6_frame_route_list = OpenAPI_list_create();
                OpenAPI_list_for_each(RecvPcfBinding->ipv6_frame_route_list, node) {
                    if (!node->data)
                        continue;
                    OpenAPI_list_add(sess->ipv6_frame_route_list, ogs_strdup(node->data));
                }
            }

            sess->s_nssai.sst = RecvPcfBinding->snssai->sst;
            sess->s_nssai.sd =
                ogs_s_nssai_sd_from_string(RecvPcfBinding->snssai->sd);

            sess->dnn = ogs_strdup(RecvPcfBinding->dnn);
            ogs_assert(sess->dnn);

            PcfIpEndPointList = RecvPcfBinding->pcf_ip_end_points;

            if (PcfIpEndPointList) {
                for (i = 0; i < sess->num_of_pcf_ip; i++) {
                    if (sess->pcf_ip[i].addr)
                        ogs_free(sess->pcf_ip[i].addr);
                    if (sess->pcf_ip[i].addr6)
                        ogs_free(sess->pcf_ip[i].addr6);
                }
                sess->num_of_pcf_ip = 0;

                OpenAPI_list_for_each(PcfIpEndPointList, node) {
                    OpenAPI_ip_end_point_t *IpEndPoint = node->data;

                    if (!IpEndPoint) continue;

                    if (sess->num_of_pcf_ip < OGS_SBI_MAX_NUM_OF_IP_ADDRESS) {
                        if (IpEndPoint->ipv4_address ||
                            IpEndPoint->ipv6_address) {
                            if (IpEndPoint->ipv4_address) {
                                sess->pcf_ip[sess->num_of_pcf_ip].addr =
                                    ogs_strdup(IpEndPoint->ipv4_address);
                            }
                            if (IpEndPoint->ipv6_address) {
                                sess->pcf_ip[sess->num_of_pcf_ip].addr6 =
                                    ogs_strdup(IpEndPoint->ipv6_address);
                            }
                            if (IpEndPoint->is_port) {
                                sess->pcf_ip[sess->num_of_pcf_ip].is_port =
                                    IpEndPoint->is_port;
                                sess->pcf_ip[sess->num_of_pcf_ip].port =
                                    IpEndPoint->port;
                            }
                            sess->num_of_pcf_ip++;
                        }
                    }
                }
            }

            if (RecvPcfBinding->supi) {
                if (sess->supi)
                    ogs_free(sess->supi);
                sess->supi = ogs_strdup(RecvPcfBinding->supi);
            }
            if (RecvPcfBinding->gpsi) {
                if (sess->gpsi)
                    ogs_free(sess->gpsi);
                sess->gpsi = ogs_strdup(RecvPcfBinding->gpsi);
            }

            memset(&header, 0, sizeof(header));
            header.service.name =
                (char *)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT;
            header.api.version = (char *)OGS_SBI_API_V1;
            header.resource.component[0] =
                (char *)OGS_SBI_RESOURCE_NAME_PCF_BINDINGS;
            header.resource.component[1] = sess->binding_id;

            if (RecvPcfBinding->supp_feat) {
                uint64_t supported_features =
                    ogs_uint64_from_string(RecvPcfBinding->supp_feat);
                sess->management_features &= supported_features;

                if (sess->management_features != supported_features) {
                    ogs_free(RecvPcfBinding->supp_feat);
                    RecvPcfBinding->supp_feat =
                        ogs_uint64_to_string(sess->management_features);
                    ogs_assert(RecvPcfBinding->supp_feat);
                }
            } else {
                sess->management_features = 0;
            }

            memset(&sendmsg, 0, sizeof(sendmsg));
            sendmsg.PcfBinding = RecvPcfBinding;
            sendmsg.http.location = ogs_sbi_server_uri(server, &header);

            response = ogs_sbi_build_response(
                    &sendmsg, OGS_SBI_HTTP_STATUS_CREATED);
            ogs_assert(response);
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));

            ogs_free(sendmsg.http.location);
            break;

        CASE(OGS_SBI_HTTP_METHOD_GET)
            if (sess->num_of_pcf_ip) {
                memset(&Snssai, 0, sizeof(Snssai));
                Snssai.sst = sess->s_nssai.sst;
                Snssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);

                memset(&SendPcfBinding, 0, sizeof(SendPcfBinding));

                SendPcfBinding.dnn = sess->dnn;
                SendPcfBinding.snssai = &Snssai;

                PcfIpEndPointList = OpenAPI_list_create();
                ogs_assert(PcfIpEndPointList);

                if (sess->pcf_fqdn && strlen(sess->pcf_fqdn))
                    SendPcfBinding.pcf_fqdn = ogs_strdup(sess->pcf_fqdn);

                for (i = 0; i < sess->num_of_pcf_ip; i++) {
                    OpenAPI_ip_end_point_t *PcfIpEndPoint = NULL;

                    ogs_assert(sess->pcf_ip[i].addr || sess->pcf_ip[i].addr6);

                    PcfIpEndPoint = ogs_calloc(1, sizeof(*PcfIpEndPoint));
                    ogs_assert(PcfIpEndPoint);
                    PcfIpEndPoint->ipv4_address = sess->pcf_ip[i].addr;
                    PcfIpEndPoint->ipv6_address = sess->pcf_ip[i].addr6;

                    PcfIpEndPoint->is_port = sess->pcf_ip[i].is_port;
                    PcfIpEndPoint->port = sess->pcf_ip[i].port;

                    OpenAPI_list_add(PcfIpEndPointList, PcfIpEndPoint);
                }

                if (PcfIpEndPointList->count)
                    SendPcfBinding.pcf_ip_end_points = PcfIpEndPointList;
                else
                    OpenAPI_list_free(PcfIpEndPointList);

                memset(&sendmsg, 0, sizeof(sendmsg));
                sendmsg.PcfBinding = &SendPcfBinding;

                response = ogs_sbi_build_response(
                            &sendmsg, OGS_SBI_HTTP_STATUS_OK);
                ogs_assert(response);

                ogs_assert(true ==
                        ogs_sbi_server_send_response(stream, response));

                OpenAPI_list_for_each(SendPcfBinding.pcf_ip_end_points, node) {
                    OpenAPI_ip_end_point_t *PcfIpEndPoint = node->data;
                    if (PcfIpEndPoint)
                        ogs_free(PcfIpEndPoint);
                }
                OpenAPI_list_free(SendPcfBinding.pcf_ip_end_points);

                if (SendPcfBinding.pcf_fqdn)
                    ogs_free(SendPcfBinding.pcf_fqdn);
            } else {
                memset(&sendmsg, 0, sizeof(sendmsg));
                response = ogs_sbi_build_response(
                            &sendmsg, OGS_SBI_HTTP_STATUS_NO_CONTENT);
                ogs_assert(response);

                ogs_assert(true ==
                    ogs_sbi_server_send_response(stream, response));
            }
            break;

        DEFAULT
            strerror = ogs_msprintf("Invalid HTTP method [%s]",
                    recvmsg->h.method);
            status = OGS_SBI_HTTP_STATUS_BAD_REQUEST;
            goto cleanup;
        END
    }

    return true;

cleanup:
    ogs_assert(strerror);
    ogs_assert(status);
    ogs_error("%s", strerror);
    ogs_assert(true ==
        ogs_sbi_server_send_error(stream, status, recvmsg, strerror, NULL,
                NULL));
    ogs_free(strerror);

    return false;
}
