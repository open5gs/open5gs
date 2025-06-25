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

#include "namf-build.h"
#include "ngap-build.h"
#include "openapi/model/non_ue_n2_message_transfer_request.h"

ogs_sbi_request_t *pwsiws_nonuen2_comm_build_nonuen2_message_transfer(
        pwsiws_warning_t *warning, pwsiws_nonuen2_message_transfer_param_t *param)
{
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;
    ogs_sbi_header_t header;

    // --- OpenAPI structures ---
    OpenAPI_n2_information_transfer_req_data_t N2InformationTransferReqData;
    OpenAPI_n2_info_container_t n2InfoContainer;
    OpenAPI_pws_information_t pwsInfo;
    OpenAPI_n2_info_content_t pws_container;
    OpenAPI_ref_to_binary_data_t ngapData;

    ogs_assert(warning);
    ogs_assert(param);
    ogs_assert(param->state);
    ogs_assert(param->n2smbuf);

    // Zero out all structures
    memset(&N2InformationTransferReqData, 0, sizeof(N2InformationTransferReqData));
    memset(&n2InfoContainer, 0, sizeof(n2InfoContainer));
    memset(&pwsInfo, 0, sizeof(pwsInfo));
    memset(&pws_container, 0, sizeof(pws_container));
    memset(&ngapData, 0, sizeof(ngapData));




    // Fill in the NGAP binary data
    ngapData.content_id = (char *)OGS_SBI_CONTENT_NGAP_SM_ID;
    // Optionally set other fields if needed

    // Fill in the N2 info content (NGAP IE content)
    pws_container.ngap_data = &ngapData;
    // Optionally set is_ngap_message_type, ngap_message_type, ngap_ie_type if needed

    // Fill in the PWS information
    pwsInfo.message_identifier = warning->message_id;
    pwsInfo.serial_number = warning->warning_data.serial_number;
    pwsInfo.data_coding_scheme = warning ->warning_data.data_coding_scheme;

    

    pws_container.message_identifier = warning->message_id;
    pws_container.serial_number = warning->warning_data.serial_number;
    pws_container.data_coding_scheme = warning->warning_data.data_coding_scheme;
    pws_container.repetition_period = warning->warning_data.repetition_period;
    pws_container.number_of_broadcast = warning->warning_data.number_of_broadcast;
    pws_container.message_length = warning->warning_data.message_length;
    memcpy(pws_container.message_contents, warning->warning_data.message_contents, warning->warning_data.message_length);
    
   
    ogs_info("[pwsiws] PWS: msg_id=%u, serial_number=%u, repetition_period=%u, number_of_broadcast=%u, data_coding_scheme=%u, message_length=%u",
        pws_container.message_identifier,
        pws_container.serial_number,
        pws_container.repetition_period,
        pws_container.number_of_broadcast,
        pws_container.data_coding_scheme,
        pws_container.message_length);




    // Additional PWS parameters (repetition_period, data_coding_scheme, etc.) are not present in OpenAPI_pws_information_t
    // If needed, extend the OpenAPI model and add here
    pwsInfo.pws_container= &pws_container;
    pwsInfo.bc_empty_area_list = NULL; // Set if you have area list info
    pwsInfo.is_send_ran_response = false; // Set as needed
    pwsInfo.send_ran_response = 0; // Set as needed
    pwsInfo.omc_id = NULL; // Set as needed
    pwsInfo.nf_id = NULL; // Set as needed

    // Fill in the N2 info container
    n2InfoContainer.n2_information_class = OpenAPI_n2_information_class_PWS;
    n2InfoContainer.pws_info = &pwsInfo;
    n2InfoContainer.sm_info = NULL;
    n2InfoContainer.ran_info = NULL;
    n2InfoContainer.nrppa_info = NULL;
    n2InfoContainer.v2x_info = NULL;
    n2InfoContainer.prose_info = NULL;

    // Fill in the N2InformationTransferReqData structure
    N2InformationTransferReqData.n2_information = &n2InfoContainer;
    N2InformationTransferReqData.tai_list = NULL; // Set if you have TAI list
    N2InformationTransferReqData.rat_selector = 0; // Set as needed
    N2InformationTransferReqData.global_ran_node_list = NULL; // Set if needed
    N2InformationTransferReqData.supported_features = NULL; // Set if needed

    // --- Build the SBI message ---
    memset(&message, 0, sizeof(message));
    message.h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    message.h.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
    message.h.api.version = (char *)OGS_SBI_API_V1;
    message.h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_NON_UE_N2_MESSAGES;
    message.N2InformationTransferReqData = &N2InformationTransferReqData;

    request = ogs_sbi_build_request(&message);

    // Set SBI header info (redundant, but for clarity)
    request->h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    request->h.service.name = (char *)OGS_SBI_SERVICE_NAME_NAMF_COMM;
    request->h.api.version = (char *)OGS_SBI_API_V1;
    request->h.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_NON_UE_N2_MESSAGES;

    // Set up HTTP message
    request->http.headers = ogs_hash_make();
    ogs_sbi_header_set(request->http.headers, OGS_SBI_CONTENT_TYPE, "application/json");

    // Add failure notification URI if requested
    if (param->nonuen2_failure_txf_notif_uri == true) {
        server = ogs_sbi_server_first();
        if (server) {
            memset(&header, 0, sizeof(header));
            header.service.name = (char *)OGS_SBI_SERVICE_NAME_NPWS_CALLBACK;
            header.api.version = (char *)OGS_SBI_API_V1;
            header.resource.component[0] = (char *)OGS_SBI_RESOURCE_NAME_N1_N2_FAILURE_NOTIFY;
            char *failure_uri = ogs_sbi_server_uri(server, &header);
            if (failure_uri) {
                ogs_sbi_header_set(request->http.headers, OGS_SBI_CUSTOM_CALLBACK, failure_uri);
                ogs_free(failure_uri);
            }
        } else {
            ogs_warn("No SBI server available for failure notification URI");
        }
    }

    // Add skip indicator if requested
    if (param->skip_ind == true) {
        ogs_sbi_header_set(request->http.headers, "Skip-Indicator", "true");
    }

    return request;
}

ogs_sbi_request_t *pwsiws_nonuen2_callback_build_warning_status(
        pwsiws_warning_t *warning, void *data)
{
    ogs_sbi_request_t *request = NULL;
    OpenAPI_pws_information_t pwsInfo;



    ogs_assert(warning);

    // Zero out the structure
    memset(&pwsInfo, 0, sizeof(pwsInfo));

    // Fill in the PWS information fields from the warning struct
    pwsInfo.message_identifier = warning->message_id;
    pwsInfo.serial_number = warning->warning_data.serial_number;
    pwsInfo.bc_empty_area_list = NULL; // Set if you have area list info
    pwsInfo.is_send_ran_response = false; // Set as needed
    pwsInfo.send_ran_response = 0; // Set as needed
    pwsInfo.omc_id = NULL; // Set as needed
    pwsInfo.nf_id = NULL; // Set as needed
    pwsInfo.pws_container = NULL;

    // Build the SBI message
    request = ogs_sbi_request_new();
    ogs_assert(request);

    // Set SBI header info
    request->h.method = (char *)OGS_SBI_HTTP_METHOD_POST;
    request->h.service.name = (char *)OGS_SBI_SERVICE_NAME_NPWS_CALLBACK;
    request->h.api.version = (char *)OGS_SBI_API_V1;
    request->h.resource.component[0] = (char *)"warning-status-notify"; // Use appropriate resource name

    // Set up HTTP message
    request->http.headers = ogs_hash_make();
    ogs_sbi_header_set(request->http.headers, OGS_SBI_CONTENT_TYPE, "application/json");

    // Attach the OpenAPI structure to the request (if your framework supports it)
    // If not, you may need to serialize it to JSON or use as needed by your SBI stack
    // Example: request->PwsInformation = &pwsInfo;
    // (If not supported, remove this line and handle serialization elsewhere)

    return request;
}