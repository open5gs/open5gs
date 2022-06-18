/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

    int i;
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
    int fqdn_len;
    char fqdn[OGS_MAX_FQDN_LEN];
#endif

    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_PCF_BINDINGS;

    memset(&PcfBinding, 0, sizeof(PcfBinding));

    PcfBinding.supi = pcf_ue->supi;
    PcfBinding.gpsi = pcf_ue->gpsi;

    PcfBinding.ipv4_addr = sess->ipv4addr_string;
    PcfBinding.ipv6_prefix = sess->ipv6prefix_string;

    ogs_expect_or_return_val(sess->dnn, NULL);
    PcfBinding.dnn = sess->dnn;

    nf_instance = ogs_sbi_self()->nf_instance;
    ogs_expect_or_return_val(nf_instance, NULL);
    nf_service = ogs_list_first(&nf_instance->nf_service_list);
    ogs_expect_or_return_val(nf_service, NULL);

    if (nf_service->fqdn) {
#if SBI_FQDN_WITH_ONE_OCTET_LENGTH
        memset(fqdn, 0, sizeof(fqdn));
        fqdn_len = ogs_fqdn_build(fqdn,
                nf_service->fqdn, strlen(nf_service->fqdn));
        PcfBinding.pcf_fqdn = ogs_memdup(fqdn, fqdn_len+1);
        ogs_expect_or_return_val(PcfBinding.pcf_fqdn, NULL);
        PcfBinding.pcf_fqdn[fqdn_len] = 0;
#else
        PcfBinding.pcf_fqdn = ogs_strdup(nf_service->fqdn);
#endif
    }

    PcfIpEndPointList = OpenAPI_list_create();
    ogs_assert(PcfIpEndPointList);

    for (i = 0; i < nf_service->num_of_addr; i++) {
        ogs_sockaddr_t *ipv4 = NULL;
        ogs_sockaddr_t *ipv6 = NULL;

        OpenAPI_ip_end_point_t *IpEndPoint = NULL;

        ipv4 = nf_service->addr[i].ipv4;
        ipv6 = nf_service->addr[i].ipv6;

        if (ipv4 || ipv6) {
            IpEndPoint = ogs_calloc(1, sizeof(*IpEndPoint));
            ogs_expect_or_return_val(IpEndPoint, NULL);
            if (ipv4) {
                IpEndPoint->ipv4_address = ogs_ipstrdup(ipv4);
                ogs_expect_or_return_val(IpEndPoint->ipv4_address, NULL);
            }
            if (ipv6) {
                IpEndPoint->ipv6_address = ogs_ipstrdup(ipv6);
                ogs_expect_or_return_val(IpEndPoint->ipv6_address, NULL);

            }
            IpEndPoint->is_port = true;
            IpEndPoint->port = nf_service->addr[i].port;
            OpenAPI_list_add(PcfIpEndPointList, IpEndPoint);
        }
    }

    if (PcfIpEndPointList->count)
        PcfBinding.pcf_ip_end_points = PcfIpEndPointList;
    else
        OpenAPI_list_free(PcfIpEndPointList);

    ogs_expect_or_return_val(sess->s_nssai.sst, NULL);
    memset(&sNssai, 0, sizeof(sNssai));
    sNssai.sst = sess->s_nssai.sst;
    sNssai.sd = ogs_s_nssai_sd_to_string(sess->s_nssai.sd);
    PcfBinding.snssai = &sNssai;

    if (sess->management_features) {
        PcfBinding.supp_feat = ogs_uint64_to_string(sess->management_features);
        ogs_expect_or_return_val(PcfBinding.supp_feat, NULL);
    }

    message.PcfBinding = &PcfBinding;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);
    
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
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);
    ogs_assert(sess->binding_id);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_DELETE;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_PCF_BINDINGS;
    message.h.resource.component[1] = sess->binding_id;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    return request;
}
