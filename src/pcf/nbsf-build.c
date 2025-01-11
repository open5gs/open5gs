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

#include "nbsf-build.h"

ogs_sbi_request_t *pcf_nbsf_management_build_register(
        pcf_sess_t *sess, void *data)
{
    pcf_ue_t *pcf_ue = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_pcf_binding_t PcfBinding;
    OpenAPI_list_t *PcfIpEndPointList = NULL;
    OpenAPI_snssai_t sNssai;
    OpenAPI_lnode_t *node = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_nf_service_t *nf_service = NULL;

    OpenAPI_nf_type_e requester_nf_type = OpenAPI_nf_type_NULL;

    int i;

    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_PCF_BINDINGS;

    memset(&PcfBinding, 0, sizeof(PcfBinding));
    memset(&sNssai, 0, sizeof(sNssai));

    PcfBinding.supi = pcf_ue->supi;
    PcfBinding.gpsi = pcf_ue->gpsi;

    PcfBinding.ipv4_addr = sess->ipv4addr_string;
    PcfBinding.ipv6_prefix = sess->ipv6prefix_string;

    PcfBinding.ipv4_frame_route_list = sess->ipv4_frame_route_list;
    PcfBinding.ipv6_frame_route_list = sess->ipv6_frame_route_list;

    if (!sess->dnn) {
        ogs_error("No DNN");
        goto end;
    }
    PcfBinding.dnn = sess->dnn;

    requester_nf_type = NF_INSTANCE_TYPE(ogs_sbi_self()->nf_instance);
    ogs_assert(requester_nf_type);
    nf_instance = ogs_sbi_nf_instance_find_by_service_type(
                    OGS_SBI_SERVICE_TYPE_NPCF_POLICYAUTHORIZATION,
                    requester_nf_type);
    ogs_assert(nf_instance);

    nf_service = ogs_sbi_nf_service_find_by_name(
            nf_instance, (char *)OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION);
    if (!nf_service) {
        ogs_error("No NF-Service");
        goto end;
    }

    if (nf_service->fqdn)
        PcfBinding.pcf_fqdn = ogs_strdup(nf_service->fqdn);

    PcfIpEndPointList = OpenAPI_list_create();
    if (!PcfIpEndPointList) {
        ogs_error("No PcfIpEndPointList");
        goto end;
    }

    for (i = 0; i < nf_service->num_of_addr; i++) {
        ogs_sockaddr_t *ipv4 = NULL;
        ogs_sockaddr_t *ipv6 = NULL;

        OpenAPI_ip_end_point_t *IpEndPoint = NULL;

        ipv4 = nf_service->addr[i].ipv4;
        ipv6 = nf_service->addr[i].ipv6;

        if (ipv4 || ipv6) {
            IpEndPoint = ogs_calloc(1, sizeof(*IpEndPoint));
            if (!IpEndPoint) {
                ogs_error("No IpEndPoint");
                goto end;
            }
            if (ipv4) {
                IpEndPoint->ipv4_address = ogs_ipstrdup(ipv4);
                if (!IpEndPoint->ipv4_address) {
                    ogs_error("No IpEndPoint->ipv4_address");
                    if (IpEndPoint)
                        ogs_free(IpEndPoint);
                    goto end;
                }
            }
            if (ipv6) {
                IpEndPoint->ipv6_address = ogs_ipstrdup(ipv6);
                if (!IpEndPoint->ipv6_address) {
                    ogs_error("No IpEndPoint->ipv6_address");
                    if (IpEndPoint) {
                        if (IpEndPoint->ipv6_address)
                            ogs_free(IpEndPoint->ipv6_address);
                        ogs_free(IpEndPoint);
                    }
                    goto end;
                }
            }
            IpEndPoint->is_port = nf_service->addr[i].is_port;
            IpEndPoint->port = nf_service->addr[i].port;
            OpenAPI_list_add(PcfIpEndPointList, IpEndPoint);
        }
    }

    if (PcfIpEndPointList->count)
        PcfBinding.pcf_ip_end_points = PcfIpEndPointList;
    else
        OpenAPI_list_free(PcfIpEndPointList);

    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    PcfBinding.snssai = &sNssai;

    if (sess->management_features) {
        PcfBinding.supp_feat = ogs_uint64_to_string(sess->management_features);
        if (!PcfBinding.supp_feat) {
            ogs_error("No supp_feat");
            goto end;
        }
    }

    message.PcfBinding = &PcfBinding;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    
    if (sNssai.sd)
        ogs_free(sNssai.sd);

    if (PcfBinding.supp_feat)
        ogs_free(PcfBinding.supp_feat);

    OpenAPI_list_for_each(PcfBinding.pcf_ip_end_points, node) {
        OpenAPI_ip_end_point_t *PcfIpEndPoint = node->data;
        ogs_assert(PcfIpEndPoint);
        if (PcfIpEndPoint->ipv4_address)
            ogs_free(PcfIpEndPoint->ipv4_address);
        if (PcfIpEndPoint->ipv6_address)
            ogs_free(PcfIpEndPoint->ipv6_address);
        ogs_free(PcfIpEndPoint);
    }
    OpenAPI_list_free(PcfBinding.pcf_ip_end_points);

    if (PcfBinding.pcf_fqdn)
        ogs_free(PcfBinding.pcf_fqdn);

    return request;
}

ogs_sbi_request_t *pcf_nbsf_management_build_de_register(
        pcf_sess_t *sess, void *data)
{
    pcf_ue_t *pcf_ue = NULL;

    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);
    ogs_assert(sess->binding.resource_uri);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.uri = sess->binding.resource_uri;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
