/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

/* Exercise the complete Namf transfer handlers and real SBI JSON codec.
 * Only context lookup, SUPI indexing and outbound transport are replaced. */
#include "amf/namf-handler.h"
#include "core/abts.h"

static amf_ue_t *transfer_ue;
static ogs_sbi_response_t *transfer_response;

static amf_ue_t *transfer_find(char *context_id)
{
    return transfer_ue && !strcmp(context_id, transfer_ue->supi) ?
        transfer_ue : NULL;
}

static void transfer_set_supi(amf_ue_t *ue, char *supi)
{
    if (ue->supi) ogs_free(ue->supi);
    ue->supi = ogs_strdup(supi);
}

static bool transfer_send(ogs_sbi_stream_t *stream,
        ogs_sbi_response_t *response)
{
    ogs_assert(!transfer_response);
    transfer_response = response;
    return true;
}

#define amf_ue_find_by_ue_context_id transfer_find
#define amf_ue_set_supi transfer_set_supi
#define ogs_sbi_server_send_response transfer_send
#define amf_namf_comm_handle_n1_n2_message_transfer transfer_n1_n2
#define amf_namf_callback_handle_sm_context_status transfer_sm_status
#define amf_namf_callback_handle_dereg_notify transfer_dereg_notify
#define amf_namf_callback_handle_sdm_data_change_notify transfer_sdm_notify
#define amf_namf_comm_handle_ue_context_transfer_request transfer_request
#define amf_namf_comm_handle_ue_context_transfer_response transfer_import
#define amf_namf_comm_handle_registration_status_update_request transfer_status_request
#define amf_namf_comm_handle_registration_status_update_response transfer_status_response
#undef AMF_NAMF_HANDLER_H
#include "amf/namf-handler.h"
#include "../../src/amf/namf-handler.c"

static const struct { uint8_t tsc, ksi; } identifiers[] = {
    { 0, 0 }, { 0, 5 }, { 1, 5 }
};

static void unverified_request_omits_seaf(abts_case *tc, void *data)
{
    int i, stream_token;

    for (i = 0; i < OGS_ARRAY_SIZE(identifiers); i++) {
        ogs_sbi_message_t request = {0};
        OpenAPI_ue_context_transfer_rsp_data_t *response_data;
        cJSON *request_json, *response_json;

        transfer_ue = ogs_calloc(1, sizeof(*transfer_ue));
        ogs_assert(transfer_ue);
        transfer_ue->supi = "imsi-001010000000001";
        transfer_ue->auth_result = OpenAPI_auth_result_AUTHENTICATION_SUCCESS;
        transfer_ue->nas.access_type = OGS_ACCESS_TYPE_3GPP;
        transfer_ue->security_context_available = 1;
        transfer_ue->nas.amf.tsc = identifiers[i].tsc;
        transfer_ue->nas.amf.ksi = identifiers[i].ksi;
        transfer_ue->nas.ue.tsc = 1;
        transfer_ue->nas.ue.ksi = 4;
        memset(transfer_ue->kamf, 0x11, sizeof(transfer_ue->kamf));

        /* A known SUPI and valid stored context do not verify a new N1.
         * The request is valid JSON but contains no regRequest or N1 part. */
        request_json = cJSON_Parse(
                "{\"reason\":\"INIT_REG\",\"accessType\":\"3GPP_ACCESS\"}");
        ogs_assert(request_json);
        request.UeContextTransferReqData =
            OpenAPI_ue_context_transfer_req_data_parseFromJSON(request_json);
        ogs_assert(request.UeContextTransferReqData);
        ABTS_PTR_EQUAL(tc, NULL, request.UeContextTransferReqData->reg_request);
        request.h.resource.component[1] = transfer_ue->supi;
        ABTS_INT_EQUAL(tc, OGS_OK,
            transfer_request((ogs_sbi_stream_t *)&stream_token, &request));
        ogs_assert(transfer_response);
        ABTS_INT_EQUAL(tc, OGS_SBI_HTTP_STATUS_OK, transfer_response->status);
        response_json = cJSON_Parse(transfer_response->http.content);
        ogs_assert(response_json);
        response_data =
            OpenAPI_ue_context_transfer_rsp_data_parseFromJSON(response_json);
        ogs_assert(response_data && response_data->ue_context);
        ABTS_STR_EQUAL(tc, transfer_ue->supi, response_data->ue_context->supi);
        ABTS_PTR_EQUAL(tc, NULL, response_data->ue_context->seaf_data);
        ABTS_INT_EQUAL(tc, identifiers[i].tsc, transfer_ue->nas.amf.tsc);
        ABTS_INT_EQUAL(tc, identifiers[i].ksi, transfer_ue->nas.amf.ksi);
        ABTS_INT_EQUAL(tc, 1, transfer_ue->nas.ue.tsc);
        ABTS_INT_EQUAL(tc, 4, transfer_ue->nas.ue.ksi);

        OpenAPI_ue_context_transfer_rsp_data_free(response_data);
        cJSON_Delete(response_json);
        ogs_sbi_response_free(transfer_response);
        transfer_response = NULL;
        OpenAPI_ue_context_transfer_req_data_free(request.UeContextTransferReqData);
        cJSON_Delete(request_json);
        ogs_free(transfer_ue);
        transfer_ue = NULL;
    }
}

static void import_owns_stored_identifier(abts_case *tc, void *data)
{
    int i;
    uint8_t kamf[OGS_SHA256_DIGEST_SIZE];
    memset(kamf, 0x11, sizeof(kamf));

    for (i = 0; i < OGS_ARRAY_SIZE(identifiers); i++) {
        ogs_sbi_message_t response = {0};
        amf_ue_t *ue = ogs_calloc(1, sizeof(*ue));
        char *body = ogs_msprintf(
            "{\"ueContext\":{\"supi\":\"imsi-001010000000001\","
            "\"seafData\":{\"ngKsi\":{\"tsc\":\"%s\",\"ksi\":%u},"
            "\"keyAmf\":{\"keyType\":\"KAMF\",\"keyVal\":"
            "\"1111111111111111111111111111111111111111111111111111111111111111\"}}}}",
            identifiers[i].tsc ? "MAPPED" : "NATIVE", identifiers[i].ksi);
        cJSON *json = cJSON_Parse(body);
        ogs_assert(ue && json);
        response.UeContextTransferRspData =
            OpenAPI_ue_context_transfer_rsp_data_parseFromJSON(json);
        ogs_assert(response.UeContextTransferRspData);
        ue->nas.amf.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
        ue->nas.ue.tsc = 1;
        ue->nas.ue.ksi = 4;

        ABTS_INT_EQUAL(tc, OGS_OK, transfer_import(&response, ue));
        ABTS_INT_EQUAL(tc, identifiers[i].tsc, ue->nas.amf.tsc);
        ABTS_INT_EQUAL(tc, identifiers[i].ksi, ue->nas.amf.ksi);
        ABTS_INT_EQUAL(tc, 0, memcmp(kamf, ue->kamf, sizeof(kamf)));
        ABTS_INT_EQUAL(tc, 1, ue->nas.ue.tsc);
        ABTS_INT_EQUAL(tc, 4, ue->nas.ue.ksi);
        ABTS_INT_EQUAL(tc, 0, ue->security_context_available);

        ogs_free(ue->supi);
        ogs_free(ue);
        OpenAPI_ue_context_transfer_rsp_data_free(response.UeContextTransferRspData);
        cJSON_Delete(json);
        ogs_free(body);
    }
}

abts_suite *test_amf_namf(abts_suite *suite)
{
    suite = ADD_SUITE(suite)
    abts_run_test(suite, unverified_request_omits_seaf, NULL);
    abts_run_test(suite, import_owns_stored_identifier, NULL);
    return suite;
}
