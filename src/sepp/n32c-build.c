/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "n32c-build.h"

ogs_sbi_request_t *sepp_n32c_handshake_build_security_capability_request(
        sepp_node_t *sepp_node, bool none)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;

    OpenAPI_sec_negotiate_req_data_t SecNegotiateReqData;
    OpenAPI_list_t *SupportedSecCapabilityList = NULL;

    OpenAPI_list_t *PlmnIdList = NULL;
    OpenAPI_plmn_id_t *PlmnId = NULL;

    int i;
    OpenAPI_lnode_t *node = NULL;

    ogs_assert(sepp_self()->sender);
    ogs_assert(sepp_node);

    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] =
        (char *)OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY;

    memset(&SecNegotiateReqData, 0, sizeof(SecNegotiateReqData));
    SecNegotiateReqData.sender = sepp_self()->sender;

    SupportedSecCapabilityList = OpenAPI_list_create();
    if (!SupportedSecCapabilityList) {
        ogs_error("No SupportedSecCapabilityList");
        goto end;
    }

    if (none == false) {
        if (sepp_self()->security_capability.tls == true)
            OpenAPI_list_add(SupportedSecCapabilityList,
                    (void *)OpenAPI_security_capability_TLS);
        if (sepp_self()->security_capability.prins == true)
            OpenAPI_list_add(SupportedSecCapabilityList,
                    (void *)OpenAPI_security_capability_PRINS);
    } else {
        OpenAPI_list_add(SupportedSecCapabilityList,
                (void *)OpenAPI_security_capability_NONE);
    }

    ogs_assert(SupportedSecCapabilityList->count);
    SecNegotiateReqData.supported_sec_capability_list =
        SupportedSecCapabilityList;

    if (none == false) {
        if (sepp_self()->target_apiroot_supported == true) {
            SecNegotiateReqData.is__3_gpp_sbi_target_api_root_supported = true;
            SecNegotiateReqData._3_gpp_sbi_target_api_root_supported = 1;
        }
    }

    PlmnIdList = OpenAPI_list_create();
    if (!PlmnIdList) {
        ogs_error("No PlmnIdList");
        goto end;
    }

    for (i = 0; i < ogs_local_conf()->num_of_serving_plmn_id; i++) {
        PlmnId = ogs_sbi_build_plmn_id(&ogs_local_conf()->serving_plmn_id[i]);
        ogs_assert(PlmnId);
        OpenAPI_list_add(PlmnIdList, PlmnId);
    }

    if (PlmnIdList->count)
        SecNegotiateReqData.plmn_id_list = PlmnIdList;
    else
        OpenAPI_list_free(PlmnIdList);

    if (sepp_node->target_plmn_id_presence)
        SecNegotiateReqData.target_plmn_id =
            ogs_sbi_build_plmn_id(&sepp_node->target_plmn_id);

    OGS_SBI_FEATURES_SET(sepp_node->supported_features,
            OGS_SBI_N32_HANDSHAKE_NFTLST);
    SecNegotiateReqData.supported_features =
        ogs_uint64_to_string(sepp_node->supported_features);
    if (!SecNegotiateReqData.supported_features) {
        ogs_error("No supported_features");
        goto end;
    }

    message.SecNegotiateReqData = &SecNegotiateReqData;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

end:
    OpenAPI_list_free(SecNegotiateReqData.supported_sec_capability_list);
    OpenAPI_list_for_each(SecNegotiateReqData.plmn_id_list, node) {
        PlmnId = node->data;
        if (PlmnId)
            ogs_sbi_free_plmn_id(PlmnId);
    }
    OpenAPI_list_free(SecNegotiateReqData.plmn_id_list);
    if (SecNegotiateReqData.target_plmn_id)
        ogs_sbi_free_plmn_id(SecNegotiateReqData.target_plmn_id);
    if (SecNegotiateReqData.supported_features)
        ogs_free(SecNegotiateReqData.supported_features);

    return request;
}
