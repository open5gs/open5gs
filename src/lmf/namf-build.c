/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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
#include "nrppa-build.h"

#include "openapi/model/n1_n2_message_transfer_req_data.h"
#include "openapi/model/n2_info_container.h"
#include "openapi/model/nrppa_information.h"
#include "openapi/model/n2_info_content.h"
#include "openapi/model/ref_to_binary_data.h"

ogs_sbi_request_t *lmf_namf_build_nrppa_measurement_request(
        lmf_location_request_t *location_request, void *data)
{
    int i;
    ogs_sbi_message_t message;
    ogs_sbi_request_t *request = NULL;
    ogs_pkbuf_t *nrppa_pdu = NULL;
    uint64_t ue_ngap_id = 0;  /* TODO: Get from UE context via AMF */
    char *content_id_plain = NULL;
    char *content_id_header = NULL;

    OpenAPI_n1_n2_message_transfer_req_data_t N1N2MessageTransferReqData;
    OpenAPI_n2_info_container_t n2InfoContainer;
    OpenAPI_nrppa_information_t nrppaInformation;
    OpenAPI_n2_info_content_t n2InfoContent;
    OpenAPI_ref_to_binary_data_t nrppaBinary;

    ogs_assert(location_request);

    memset(&message, 0, sizeof(message));
    message.h.method = ogs_strdup(OGS_SBI_HTTP_METHOD_POST);
    ogs_assert(message.h.method);
    message.h.service.name = ogs_strdup(OGS_SBI_SERVICE_NAME_NAMF_COMM);
    ogs_assert(message.h.service.name);
    message.h.api.version = ogs_strdup(OGS_SBI_API_V1);
    ogs_assert(message.h.api.version);
    message.h.resource.component[0] = ogs_strdup(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS);
    ogs_assert(message.h.resource.component[0]);
    
    /* Extract SUPI from input message or use placeholder */
    if (location_request->supi) {
        message.h.resource.component[1] = ogs_strdup(location_request->supi);
        ogs_assert(message.h.resource.component[1]);
    } else {
        /* TODO: Extract from input_message */
        ogs_error("No SUPI in location request");
        ogs_sbi_message_free(&message);
        return NULL;
    }
    
    message.h.resource.component[2] =
        ogs_strdup(OGS_SBI_RESOURCE_NAME_N1_N2_MESSAGES);
    ogs_assert(message.h.resource.component[2]);

    /* Generate measurement ID if not set */
    /* Note: UE-Measurement-ID in v17.2.0 is constrained to 1..256 */
    if (location_request->measurement_id == 0 ||
            location_request->measurement_id > 256) {
        location_request->measurement_id = (ogs_random32() % 256) + 1;
        ogs_info("[%s] Generated measurement_id=%u (range: 1..256 for v17.2.0)",
                location_request->supi, location_request->measurement_id);
    }

    /* Build NRPPa ECID Measurement Request PDU */
    /* Request RSRP, RSRQ, and Timing Advance measurements */
    /* Request measurements supported by Ericsson gNB:
     * - Cell-ID (always included)
     * - SS-RSRP
     * - NR Timing Advance (required for AoA)
     * - NR Angle of Arrival (only works if TA is also requested)
     * Note: RSRQ not supported by Ericsson
     */
    uint8_t requested_measurements = 
        NRPPA_ECID_MEASUREMENT_TYPE_RSRP |
        NRPPA_ECID_MEASUREMENT_TYPE_TA |
        NRPPA_ECID_MEASUREMENT_TYPE_AOA;  /* AoA requires TA */

    nrppa_pdu = lmf_nrppa_build_ecid_measurement_request(
            location_request->measurement_id,
            ue_ngap_id,  /* Will be set by AMF based on UE context */
            requested_measurements);
    if (!nrppa_pdu) {
        ogs_error("[%s] lmf_nrppa_build_ecid_measurement_request() failed",
                location_request->supi);
        ogs_sbi_message_free(&message);
        return NULL;
    }

    /* Prepare JSON control plane via OpenAPI structures */
    memset(&N1N2MessageTransferReqData, 0,
            sizeof(N1N2MessageTransferReqData));
    message.N1N2MessageTransferReqData = &N1N2MessageTransferReqData;
    N1N2MessageTransferReqData.supported_features = (char *)"1";
    N1N2MessageTransferReqData.n2_info_container = &n2InfoContainer;

    memset(&n2InfoContainer, 0, sizeof(n2InfoContainer));
    n2InfoContainer.n2_information_class = OpenAPI_n2_information_class_NRPPa;
    n2InfoContainer.nrppa_info = &nrppaInformation;

    memset(&nrppaInformation, 0, sizeof(nrppaInformation));
    nrppaInformation.nrppa_pdu = &n2InfoContent;
    nrppaInformation.nf_id = ogs_sbi_self()->nf_instance->id;

    memset(&n2InfoContent, 0, sizeof(n2InfoContent));
    n2InfoContent.is_ngap_message_type = false;
    n2InfoContent.ngap_ie_type = OpenAPI_ngap_ie_type_NRPPA_PDU;
    n2InfoContent.ngap_data = &nrppaBinary;

    ogs_assert(message.num_of_part < OGS_SBI_MAX_NUM_OF_PART);
    content_id_plain = ogs_msprintf("n2-blob-%u",
            (unsigned)location_request->measurement_id);
    if (!content_id_plain) {
        ogs_error("ogs_msprintf() failed");
        ogs_pkbuf_free(nrppa_pdu);
        ogs_sbi_message_free(&message);
        return NULL;
    }

    memset(&nrppaBinary, 0, sizeof(nrppaBinary));
    nrppaBinary.content_id = content_id_plain;

    content_id_header = ogs_msprintf("<%s>", content_id_plain);
    if (!content_id_header) {
        ogs_error("ogs_msprintf() failed");
        ogs_free(content_id_plain);
        content_id_plain = NULL;
        ogs_pkbuf_free(nrppa_pdu);
        ogs_sbi_message_free(&message);
        return NULL;
    }

    message.part[message.num_of_part].content_type =
        (char *)OGS_SBI_CONTENT_NRPPa_TYPE;
    message.part[message.num_of_part].content_id = content_id_header;
    message.part[message.num_of_part].pkbuf = nrppa_pdu;
    message.num_of_part++;

    request = ogs_sbi_build_request(&message);
    ogs_expect(request);

    if (!request)
        goto build_error;

    message.N1N2MessageTransferReqData = NULL;

    if (content_id_header) {
        ogs_free(content_id_header);
        message.part[0].content_id = NULL;
        content_id_header = NULL;
    }
    if (content_id_plain) {
        ogs_free(content_id_plain);
        nrppaBinary.content_id = NULL;
        content_id_plain = NULL;
    }

    /* Free manually allocated header strings after ogs_sbi_build_request() duplicates them */
    if (message.h.method)
        ogs_free(message.h.method);
    if (message.h.service.name)
        ogs_free(message.h.service.name);
    if (message.h.api.version)
        ogs_free(message.h.api.version);
    for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                    message.h.resource.component[i]; i++)
        ogs_free(message.h.resource.component[i]);

    /* ogs_sbi_message_free() will free the pkbuf */
    ogs_sbi_message_free(&message);

    return request;

build_error:
    message.N1N2MessageTransferReqData = NULL;
    if (content_id_header) {
        ogs_free(content_id_header);
        content_id_header = NULL;
    }
    if (content_id_plain) {
        ogs_free(content_id_plain);
        content_id_plain = NULL;
    }
    if (message.h.method)
        ogs_free(message.h.method);
    if (message.h.service.name)
        ogs_free(message.h.service.name);
    if (message.h.api.version)
        ogs_free(message.h.api.version);
    for (i = 0; i < OGS_SBI_MAX_NUM_OF_RESOURCE_COMPONENT &&
                    message.h.resource.component[i]; i++)
        ogs_free(message.h.resource.component[i]);
    ogs_sbi_message_free(&message);
    return NULL;
}

