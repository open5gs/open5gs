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

#include "namf-oam.h"
#include "sbi-path.h"
#include "ngap-path.h"

/*
 * Main router for Namf_OAM API
 */
bool amf_namf_oam_handler(
    ogs_sbi_stream_t *stream,
    ogs_sbi_message_t *message,
    ogs_sbi_request_t *request)
{
    const char *resource = NULL;

    ogs_assert(stream);
    ogs_assert(message);
    ogs_assert(request);

    resource = message->h.resource.component[0];

    if (!resource) {
        ogs_error("No resource specified");
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST, 
            message, "No resource specified", NULL, NULL));
        return false;
    }

    /*
     * Route based on resource path:
     * - /namf-oam/v1/plmns (GET, POST)
     * - /namf-oam/v1/plmns/{plmn_id} (GET, DELETE)
     * - /namf-oam/v1/status
     */
    if (!strcmp(resource, "plmns")) {
        const char *plmn_id_param = message->h.resource.component[1];
        
        if (plmn_id_param) {
            /* /plmns/{plmn_id} */
            if (!strcmp(message->h.method, OGS_SBI_HTTP_METHOD_GET)) {
                return namf_oam_handle_plmns_get_by_id(stream, message);

            } else if (!strcmp(message->h.method, OGS_SBI_HTTP_METHOD_DELETE)) {
                return namf_oam_handle_plmns_delete(stream, message);

            } else {
                ogs_error("Invalid HTTP method [%s] for /plmns/{id}",
                        message->h.method);
                ogs_assert(true == ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, 
                    message, "Method not allowed", message->h.method, NULL));
                return false;
            }
        } else {
            /* /plmns */
            if (!strcmp(message->h.method, OGS_SBI_HTTP_METHOD_GET)) {
                return namf_oam_handle_plmns_get(stream, message);

            } else if (!strcmp(message->h.method, OGS_SBI_HTTP_METHOD_POST)) {
                return namf_oam_handle_plmns_post(stream, message, request);

            } else {
                ogs_error("Invalid HTTP method [%s] for /plmns",
                        message->h.method);
                ogs_assert(true == ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, 
                    message, "Method not allowed", message->h.method, NULL));
                return false;
            }
        }
    }

    /* Resource not found */
    ogs_error("Invalid OAM resource [%s]", resource);
    ogs_assert(true == ogs_sbi_server_send_error(
        stream, OGS_SBI_HTTP_STATUS_NOT_FOUND, 
        message, "Resource not found", resource, NULL));
    
    return false;
}

/*
 * GET /namf-oam/v1/plmns
 * 
 * Returns list of configured PLMNs with simple statistics
 */
bool namf_oam_handle_plmns_get(ogs_sbi_stream_t *stream,  ogs_sbi_message_t *message)
{
    cJSON *root = NULL;
    cJSON *plmns_array = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    
    int i;
    int status = OGS_SBI_HTTP_STATUS_OK;
    amf_gnb_t *gnb = NULL;
    amf_ue_t *amf_ue = NULL;


    /* Create JSON response */
    root = cJSON_CreateObject();
    ogs_assert(root);

    plmns_array = cJSON_CreateArray();
    ogs_assert(plmns_array);
    cJSON_AddItemToObject(root, "plmns", plmns_array);

    /* Iterate through configured PLMNs */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        cJSON *plmn_obj = cJSON_CreateObject();
        cJSON *s_nssai_array = cJSON_CreateArray();
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char plmn_id_str[OGS_PLMNIDSTRLEN];
        int k;
        
        ogs_plmn_id_to_string(plmn_id, plmn_id_str);
        
        cJSON_AddStringToObject(plmn_obj, "plmn_id", plmn_id_str);
        cJSON_AddNumberToObject(plmn_obj, "mcc", ogs_plmn_id_mcc(plmn_id));
        cJSON_AddNumberToObject(plmn_obj, "mnc", ogs_plmn_id_mnc(plmn_id));
        cJSON_AddNumberToObject(plmn_obj, "mnc_len", ogs_plmn_id_mnc_len(plmn_id));
        
        /* Add S-NSSAI list */
        for (k = 0; k < amf_self()->plmn_support[i].num_of_s_nssai; k++) {
            cJSON *slice_obj = cJSON_CreateObject();
            ogs_s_nssai_t *s_nssai = &amf_self()->plmn_support[i].s_nssai[k];
            
            cJSON_AddNumberToObject(slice_obj, "sst", s_nssai->sst);
            if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                char *sd_str = ogs_s_nssai_sd_to_string(s_nssai->sd);
                cJSON_AddStringToObject(slice_obj, "sd", sd_str);
                ogs_free(sd_str);
            }
            cJSON_AddItemToArray(s_nssai_array, slice_obj);
        }
        cJSON_AddItemToObject(plmn_obj, "s_nssai_list", s_nssai_array);

        /* Count connected gNBs */
        int gnb_count = 0;
        ogs_list_for_each(&amf_self()->gnb_list, gnb) {
            if (gnb->state.ng_setup_success)
                gnb_count++;
        }
        cJSON_AddNumberToObject(plmn_obj, "connected_gnbs", gnb_count);

        /* Count registered UEs */
        int ue_count = 0;
        ogs_list_for_each(&amf_self()->amf_ue_list, amf_ue) {
            if (AMF_UE_HAVE_SUCI(amf_ue))
                ue_count++;
        }
        cJSON_AddNumberToObject(plmn_obj, "registered_ues", ue_count);

        cJSON_AddItemToArray(plmns_array, plmn_obj);
    }

    cJSON_AddNumberToObject(root, "total_entries", amf_self()->num_of_plmn_support);

    /* Convert to string */
    response_body = cJSON_Print(root);
    ogs_assert(response_body);
    cJSON_Delete(root);

    /* Build SBI response */
    memset(&response_message, 0, sizeof(response_message));
    response_message.res_status = OGS_SBI_HTTP_STATUS_OK;
    
    response = ogs_sbi_build_response(&response_message, status);
    ogs_assert(response);
    
    response->http.content = response_body;
    response->http.content_length = strlen(response_body);
    
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_info("[OAM] Listed %d PLMNs", amf_self()->num_of_plmn_support);

    return true;
}

/*
 * GET /namf-oam/v1/plmns/{plmn_id}
 * 
 * Get details of a specific PLMN by ID (format: "99970" or "999-70")
 */
bool namf_oam_handle_plmns_get_by_id(ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    cJSON *root = NULL;
    cJSON *s_nssai_array = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    const char *plmn_id_str = NULL;
    int i, k;
    bool found = false;
    int status = OGS_SBI_HTTP_STATUS_OK;

    plmn_id_str = message->h.resource.component[2];

    /* Search for the PLMN */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char current_plmn_str[OGS_PLMNIDSTRLEN];
        
        ogs_plmn_id_to_string(plmn_id, current_plmn_str);
        
        if (strcmp(current_plmn_str, plmn_id_str) == 0) {
            found = true;
            
            root = cJSON_CreateObject();
            cJSON_AddStringToObject(root, "plmn_id", current_plmn_str);
            cJSON_AddNumberToObject(root, "mcc", ogs_plmn_id_mcc(plmn_id));
            cJSON_AddNumberToObject(root, "mnc", ogs_plmn_id_mnc(plmn_id));
            cJSON_AddNumberToObject(root, "mnc_len", ogs_plmn_id_mnc_len(plmn_id));
            
            /* Add S-NSSAI list */
            s_nssai_array = cJSON_CreateArray();
            for (k = 0; k < amf_self()->plmn_support[i].num_of_s_nssai; k++) {
                cJSON *slice_obj = cJSON_CreateObject();
                ogs_s_nssai_t *s_nssai = &amf_self()->plmn_support[i].s_nssai[k];
                
                cJSON_AddNumberToObject(slice_obj, "sst", s_nssai->sst);
                if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                    char *sd_str = ogs_s_nssai_sd_to_string(s_nssai->sd);
                    cJSON_AddStringToObject(slice_obj, "sd", sd_str);
                    ogs_free(sd_str);
                }
                cJSON_AddItemToArray(s_nssai_array, slice_obj);
            }
            cJSON_AddItemToObject(root, "s_nssai_list", s_nssai_array);

            /* Count connected gNBs for this PLMN */
            amf_gnb_t *gnb = NULL;
            int gnb_count = 0;
            ogs_list_for_each(&amf_self()->gnb_list, gnb) {
                if (gnb->state.ng_setup_success)
                    gnb_count++;
            }
            cJSON_AddNumberToObject(root, "connected_gnbs", gnb_count);

            /* Count registered UEs for this PLMN */
            amf_ue_t *amf_ue = NULL;
            int ue_count = 0;
            ogs_list_for_each(&amf_self()->amf_ue_list, amf_ue) {
                if (AMF_UE_HAVE_SUCI(amf_ue))
                    ue_count++;
            }
            cJSON_AddNumberToObject(root, "registered_ues", ue_count);

            break;
        }
    }

    if (!found) {
        ogs_error("[OAM] PLMN not found: %s", plmn_id_str);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
            message, "PLMN not found", plmn_id_str, NULL));
        return false;
    }

    response_body = cJSON_Print(root);
    cJSON_Delete(root);

    /* Build SBI response */
    memset(&response_message, 0, sizeof(response_message));
    response_message.res_status = OGS_SBI_HTTP_STATUS_OK;
    
    response = ogs_sbi_build_response(&response_message, status);
    ogs_assert(response);
    
    response->http.content = response_body;
    response->http.content_length = strlen(response_body);
    
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_info("[OAM] Retrieved PLMN: %s", plmn_id_str);

    return true;
}

/*
 * DELETE /namf-oam/v1/plmns/{plmn_id}
 * 
 * Remove a PLMN from the configuration
 */
bool namf_oam_handle_plmns_delete(ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    cJSON *response_json = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    const char *plmn_id_str = NULL;
    int status = OGS_SBI_HTTP_STATUS_OK;
    int i, j;
    bool found = false;
    int deleted_index = -1;
    
    plmn_id_str = message->h.resource.component[1];

    /* Search for the PLMN to delete */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char current_plmn_str[OGS_PLMNIDSTRLEN];
        
        ogs_plmn_id_to_string(plmn_id, current_plmn_str);
        
        if (strcmp(current_plmn_str, plmn_id_str) == 0) {
            found = true;
            deleted_index = i;
            break;
        }
    }

    if (!found) {
        ogs_error("[OAM] PLMN not found for deletion: %s", plmn_id_str);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
            message, "PLMN not found", plmn_id_str, NULL));
        return false;
    }

    /* Shift remaining PLMNs to fill the gap */
    for (j = deleted_index; j < amf_self()->num_of_plmn_support - 1; j++) {
        memcpy(&amf_self()->plmn_support[j], 
               &amf_self()->plmn_support[j + 1],
               sizeof(amf_self()->plmn_support[0]));
    }

    /* Clear the last entry */
    memset(&amf_self()->plmn_support[amf_self()->num_of_plmn_support - 1],
           0, sizeof(amf_self()->plmn_support[0]));

    /* Decrement counter */
    amf_self()->num_of_plmn_support--;

    /* Send AMFConfigurationUpdate to all connected gNBs */
    ngap_send_amf_configuration_update_all();

    /* Build success response */
    response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "status", "success");
    cJSON_AddStringToObject(response_json, "message", "PLMN deleted successfully");
    cJSON_AddStringToObject(response_json, "deleted_plmn_id", plmn_id_str);
    cJSON_AddNumberToObject(response_json, "total_plmns", amf_self()->num_of_plmn_support);

    response_body = cJSON_Print(response_json);
    cJSON_Delete(response_json);

    /* Build SBI response */
    memset(&response_message, 0, sizeof(response_message));
    response_message.res_status = OGS_SBI_HTTP_STATUS_OK;
    
    response = ogs_sbi_build_response(&response_message, status);
    ogs_assert(response);
    
    response->http.content = response_body;
    response->http.content_length = strlen(response_body);
    
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_info("[OAM] PLMN deleted: %s, remaining: %d",
        plmn_id_str, amf_self()->num_of_plmn_support);

    return true;
}

/*
 * POST /namf-oam/v1/plmns
 * 
 * Add a new PLMN to the AMF configuration
 * 
 * Request body example:
 * {
 *   "plmn_id": {
 *     "mcc": "999",
 *     "mnc": "70"
 *   },
 *   "s_nssai": [
 *     {"sst": 1, "sd": "000001"},
 *     {"sst": 1}
 *   ]
 * }
 */
bool namf_oam_handle_plmns_post(
    ogs_sbi_stream_t *stream,
    ogs_sbi_message_t *message,
    ogs_sbi_request_t *request)
{
    cJSON *root = NULL;
    cJSON *plmn_id_obj = NULL;
    cJSON *mcc_obj = NULL;
    cJSON *mnc_obj = NULL;
    cJSON *s_nssai_array = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    int i;
    ogs_plmn_id_t plmn_id;
    uint16_t mcc, mnc;
    int mnc_len;

    ogs_assert(request);

    /* Check if we have reached the maximum number of PLMNs */
    if (amf_self()->num_of_plmn_support >= OGS_MAX_NUM_OF_PLMN) {
        ogs_error("[OAM] Cannot add PLMN: maximum limit reached (%d)",
            OGS_MAX_NUM_OF_PLMN);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_CONFLICT,
            message, "Maximum number of PLMNs reached", NULL, NULL));
        return false;
    }

    /* Parse JSON request body */
    if (!request->http.content || request->http.content_length == 0) {
        ogs_error("[OAM] POST /plmns: empty body");
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Request body is required", NULL, NULL));
        return false;
    }

    root = cJSON_Parse(request->http.content);
    if (!root) {
        ogs_error("[OAM] Failed to parse JSON request");
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Invalid JSON format", NULL, NULL));
        return false;
    }

    /* Extract PLMN ID */
    plmn_id_obj = cJSON_GetObjectItem(root, "plmn_id");
    if (!plmn_id_obj) {
        ogs_error("[OAM] Missing 'plmn_id' in request");
        cJSON_Delete(root);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Missing 'plmn_id' field", NULL, NULL));
        return false;
    }

    mcc_obj = cJSON_GetObjectItem(plmn_id_obj, "mcc");
    mnc_obj = cJSON_GetObjectItem(plmn_id_obj, "mnc");

    if (!mcc_obj || !cJSON_IsString(mcc_obj) ||
        !mnc_obj || !cJSON_IsString(mnc_obj)) {
        ogs_error("[OAM] Invalid 'plmn_id' format");
        cJSON_Delete(root);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "plmn_id must contain 'mcc' and 'mnc' strings", NULL, NULL));
        return false;
    }

    /* Parse MCC and MNC */
    mcc = atoi(mcc_obj->valuestring);
    mnc = atoi(mnc_obj->valuestring);
    mnc_len = strlen(mnc_obj->valuestring);

    if (mcc > 999 || mnc > 999 || mnc_len < 2 || mnc_len > 3) {
        ogs_error("[OAM] Invalid MCC (%d) or MNC (%d, len=%d)", 
            mcc, mnc, mnc_len);
        cJSON_Delete(root);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Invalid MCC/MNC values", NULL, NULL));
        return false;
    }

    /* Build PLMN ID */
    ogs_plmn_id_build(&plmn_id, mcc, mnc, mnc_len);

    /* Extract S-NSSAI array */
    s_nssai_array = cJSON_GetObjectItem(root, "s_nssai");
    if (!s_nssai_array || !cJSON_IsArray(s_nssai_array)) {
        ogs_error("[OAM] Missing or invalid 's_nssai' array");
        cJSON_Delete(root);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "'s_nssai' must be an array", NULL, NULL));
        return false;
    }

    int num_slices = cJSON_GetArraySize(s_nssai_array);
    if (num_slices == 0 || num_slices > OGS_MAX_NUM_OF_SLICE) {
        ogs_error("[OAM] Invalid number of slices: %d (max: %d)",
            num_slices, OGS_MAX_NUM_OF_SLICE);
        cJSON_Delete(root);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Invalid number of slices", NULL, NULL));
        return false;
    }

    /* Add new PLMN entry */
    int plmn_idx = amf_self()->num_of_plmn_support;
    
    memcpy(&amf_self()->plmn_support[plmn_idx].plmn_id, 
           &plmn_id, sizeof(ogs_plmn_id_t));
    
    /* Parse and add S-NSSAIs */
    amf_self()->plmn_support[plmn_idx].num_of_s_nssai = 0;
    
    for (i = 0; i < num_slices; i++) {
        cJSON *slice_obj = cJSON_GetArrayItem(s_nssai_array, i);
        cJSON *sst_obj = cJSON_GetObjectItem(slice_obj, "sst");
        cJSON *sd_obj = cJSON_GetObjectItem(slice_obj, "sd");

        if (!sst_obj || !cJSON_IsNumber(sst_obj)) {
            ogs_error("[OAM] Slice %d: missing or invalid 'sst'", i);
            cJSON_Delete(root);
            ogs_assert(true == ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "Each slice must have 'sst' (number)", NULL, NULL));
            return false;
        }

        uint8_t sst = (uint8_t)sst_obj->valueint;
        amf_self()->plmn_support[plmn_idx].s_nssai[i].sst = sst;

        /* SD is optional */
        if (sd_obj && cJSON_IsString(sd_obj)) {
            ogs_uint24_t sd;
            sd.v = strtoul(sd_obj->valuestring, NULL, 16);
            amf_self()->plmn_support[plmn_idx].s_nssai[i].sd = sd;
        } else {
            amf_self()->plmn_support[plmn_idx].s_nssai[i].sd.v = 
                OGS_S_NSSAI_NO_SD_VALUE;
        }

        amf_self()->plmn_support[plmn_idx].num_of_s_nssai++;
    }

    /* Increment PLMN counter */
    amf_self()->num_of_plmn_support++;

    cJSON_Delete(root);

    /* Send AMFConfigurationUpdate to all connected gNBs */
    ngap_send_amf_configuration_update_all();

    /* Build success response */
    cJSON *response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "status", "success");
    cJSON_AddStringToObject(response_json, "message", "PLMN added successfully");
    cJSON_AddNumberToObject(response_json, "total_plmns", amf_self()->num_of_plmn_support);

    response_body = cJSON_Print(response_json);
    ogs_assert(response_body);
    cJSON_Delete(response_json);

    /* Build SBI response */
    memset(&response_message, 0, sizeof(response_message));
    response_message.res_status = OGS_SBI_HTTP_STATUS_CREATED;
    
    response = ogs_sbi_build_response(&response_message, OGS_SBI_HTTP_STATUS_CREATED);
    ogs_assert(response);
    
    response->http.content = response_body;
    response->http.content_length = strlen(response_body);
    
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_info("[OAM] PLMN added: MCC=%d MNC=%d (%d slices), notified to gNBs",
        mcc, mnc, num_slices);

    return true;
}
