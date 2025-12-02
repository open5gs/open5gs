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
#include "nsmf-handler.h"

/*
 * Returns true if duplicate found, false otherwise 
 * Note: Multiple PLMNs can exist, but PLMN + SST + SD combinations must be unique
 */
static bool is_duplicate_slice(const ogs_plmn_id_t *plmn_id, const ogs_s_nssai_t *s_nssai)
{
    int i, k;
    
    ogs_assert(plmn_id);
    ogs_assert(s_nssai);
    
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        /* Check if PLMN matches */
        if (memcmp(&amf_self()->plmn_support[i].plmn_id, 
                   plmn_id, 
                   sizeof(ogs_plmn_id_t)) != 0)
            continue;
        
        /* Same PLMN found, check all its S-NSSAIs */
        for (k = 0; k < amf_self()->plmn_support[i].num_of_s_nssai; k++) {
            ogs_s_nssai_t *existing = &amf_self()->plmn_support[i].s_nssai[k];
            
            /* Check if SST and SD match */
            if (existing->sst == s_nssai->sst && existing->sd.v == s_nssai->sd.v) {
                return true;  /* Duplicate found */
            }
        }
    }
    
    return false;  /* No duplicate */
}

/*
 * Returns the number of unique PLMN IDs (same PLMN with different slices counts as one)
 */
static int count_unique_plmns(void)
{
    int i, j;
    int unique_count = 0;
    bool already_counted[OGS_MAX_NUM_OF_PLMN];
    
    memset(already_counted, 0, sizeof(already_counted));
    
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        if (already_counted[i])
            continue;
            
        unique_count++;
        already_counted[i] = true;
        
        /* Mark all other entries with same PLMN ID as counted */
        for (j = i + 1; j < amf_self()->num_of_plmn_support; j++) {
            if (memcmp(&amf_self()->plmn_support[i].plmn_id,
                       &amf_self()->plmn_support[j].plmn_id,
                       sizeof(ogs_plmn_id_t)) == 0) {
                already_counted[j] = true;
            }
        }
    }
    
    return unique_count;
}


/*
 * Disconnect gNBs that have the specified PLMN being deleted
 * Returns the number of gNBs disconnected
 */
/* static int disconnect_gnbs(const ogs_plmn_id_t *deleted_plmn_id)
{
    amf_gnb_t *gnb = NULL, *next_gnb = NULL;
    int disconnected = 0;
    char buf[OGS_ADDRSTRLEN];
    char deleted_plmn_str[OGS_PLMNIDSTRLEN];
    ogs_plmn_id_to_string(deleted_plmn_id, deleted_plmn_str);
    ogs_list_for_each_safe(&amf_self()->gnb_list, next_gnb, gnb) {
        bool has_deleted_plmn = false;
        int ta_idx, bplmn_idx;
        for (ta_idx = 0; ta_idx < gnb->num_of_supported_ta_list && !has_deleted_plmn; ta_idx++) {
            for (bplmn_idx = 0; bplmn_idx < gnb->supported_ta_list[ta_idx].num_of_bplmn_list; bplmn_idx++) {
                ogs_plmn_id_t *gnb_plmn = &gnb->supported_ta_list[ta_idx].bplmn_list[bplmn_idx].plmn_id;
                if (memcmp(gnb_plmn, deleted_plmn_id, sizeof(ogs_plmn_id_t)) == 0) {
                    has_deleted_plmn = true;
                    break;
                }
            }
        }
        if (has_deleted_plmn) {
            char gnb_plmn_str[OGS_PLMNIDSTRLEN];
            ogs_plmn_id_to_string(&gnb->plmn_id, gnb_plmn_str);
            ogs_warn("[OAM] Disconnecting gNB [%s, PLMN=%s] - using deleted PLMN %s",
                gnb->sctp.addr ? OGS_ADDR(&gnb->sctp.addr->sa, buf) : "unknown",
                gnb_plmn_str, deleted_plmn_str);
            amf_gnb_remove(gnb);
            disconnected++;
        }
    }
    if (disconnected > 0) {
        ogs_info("[OAM] Disconnected %d gNB(s) using deleted PLMN %s", disconnected, deleted_plmn_str);
    }
    return disconnected;
} */

/*
 * Release all UEs associated with the deleted PLMN
 * Returns the number of UEs released
 * 
 * This function releases PDU sessions via SMF and sends RAN context release.
 * The actual UE removal is handled asynchronously by the framework after all
 * sessions are released and RAN context is freed.
 */
static int release_ues_of_plmn(const ogs_plmn_id_t *deleted_plmn_id)
{
    amf_ue_t *amf_ue = NULL, *next_ue = NULL;
    int released = 0;
    char deleted_plmn_str[OGS_PLMNIDSTRLEN];
    
    ogs_plmn_id_to_string(deleted_plmn_id, deleted_plmn_str);
    
    ogs_list_for_each_safe(&amf_self()->amf_ue_list, next_ue, amf_ue) {
        
        if (!(AMF_UE_HAVE_SUCI(amf_ue) || AMF_UE_HAVE_SUPI(amf_ue)))
            continue;
        
        if (memcmp(&amf_ue->nr_tai.plmn_id, deleted_plmn_id, sizeof(ogs_plmn_id_t)) == 0) {
            ran_ue_t *ran_ue = NULL;
            amf_nsmf_pdusession_sm_context_param_t param;
            int state = AMF_RELEASE_SM_CONTEXT_NO_STATE;
            
            ogs_warn("[OAM] Releasing UE [SUPI=%s] - attached to deleted PLMN %s", 
                     amf_ue->supi, deleted_plmn_str);
            
            ran_ue = ran_ue_find_by_id(amf_ue->ran_ue_id);
            
            /* Prepare release parameters */
            memset(&param, 0, sizeof(param));
            param.cause = OpenAPI_cause_REL_DUE_TO_UNSPECIFIED_REASON;
            param.ngApCause.group = NGAP_Cause_PR_nas;
            param.ngApCause.value = NGAP_CauseNas_deregister;
            param.ue_location = true;
            param.ue_timezone = true;
            
            /* Release all PDU sessions via SBI (this will properly notify SMF/UPF) */
            amf_sbi_send_release_all_sessions(ran_ue, amf_ue, state, &param);
            
            /* Release RAN context */
            if (ran_ue) {
                int rv = ngap_send_ran_ue_context_release_command(ran_ue,
                        NGAP_Cause_PR_nas, NGAP_CauseNas_deregister,
                        NGAP_UE_CTX_REL_NG_REMOVE_AND_UNLINK, 0);
                ogs_expect(rv == OGS_OK);
            }
            
            released++;
        }
    }
    
    if (released > 0) {
        ogs_info("[OAM] Initiated release for %d UE(s) and their PDU sessions for deleted PLMN %s", 
                 released, deleted_plmn_str);
    }
    
    return released;
}

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
 * Returns list of configured PLMNs with their slices and general statistics
 */
bool namf_oam_handle_plmns_get(ogs_sbi_stream_t *stream,  ogs_sbi_message_t *message)
{
    cJSON *root = NULL;
    cJSON *plmns_array = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    
    int i, j, k;
    int status = OGS_SBI_HTTP_STATUS_OK;
    amf_gnb_t *gnb = NULL;
    amf_ue_t *amf_ue = NULL;
    int total_gnbs = 0;
    int total_ues = 0;
    bool already_added[OGS_MAX_NUM_OF_PLMN];

    /* Create JSON response */
    root = cJSON_CreateObject();
    plmns_array = cJSON_CreateArray();
    cJSON_AddItemToObject(root, "plmns", plmns_array);

    /* Initialize tracking array */
    memset(already_added, 0, sizeof(already_added));

    /* Iterate through configured PLMNs and group by PLMN ID */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char plmn_id_str[OGS_PLMNIDSTRLEN];
        cJSON *plmn_obj = NULL;
        cJSON *nssai_array = NULL;
        
        /* Skip if already processed in a previous iteration */
        if (already_added[i])
            continue;
        
        ogs_plmn_id_to_string(plmn_id, plmn_id_str);
        
        /* Create PLMN object */
        plmn_obj = cJSON_CreateObject();
        nssai_array = cJSON_CreateArray();
        
        cJSON_AddStringToObject(plmn_obj, "plmn_id", plmn_id_str);
        cJSON_AddNumberToObject(plmn_obj, "mcc", ogs_plmn_id_mcc(plmn_id));
        cJSON_AddNumberToObject(plmn_obj, "mnc", ogs_plmn_id_mnc(plmn_id));
        
        /* Collect all S-NSSAIs for this PLMN ID */
        for (j = i; j < amf_self()->num_of_plmn_support; j++) {
            ogs_plmn_id_t *other_plmn = &amf_self()->plmn_support[j].plmn_id;
            char other_plmn_str[OGS_PLMNIDSTRLEN];
            
            ogs_plmn_id_to_string(other_plmn, other_plmn_str);
            
            /* If same PLMN ID, add its S-NSSAIs */
            if (strcmp(plmn_id_str, other_plmn_str) == 0) {
                already_added[j] = true;
                
                for (k = 0; k < amf_self()->plmn_support[j].num_of_s_nssai; k++) {
                    cJSON *nssai_obj = cJSON_CreateObject();
                    ogs_s_nssai_t *s_nssai = &amf_self()->plmn_support[j].s_nssai[k];
                    
                    cJSON_AddNumberToObject(nssai_obj, "sst", s_nssai->sst);
                    if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                        char *sd_str = ogs_s_nssai_sd_to_string(s_nssai->sd);
                        cJSON_AddStringToObject(nssai_obj, "sd", sd_str);
                        ogs_free(sd_str);
                    }
                    
                    cJSON_AddItemToArray(nssai_array, nssai_obj);
                }
            }
        }
        
        cJSON_AddItemToObject(plmn_obj, "s_nssai", nssai_array);
        cJSON_AddItemToArray(plmns_array, plmn_obj);
    }

    /* Count total connected gNBs */
    ogs_list_for_each(&amf_self()->gnb_list, gnb) {
        if (gnb->state.ng_setup_success)
            total_gnbs++;
    }

    /* Count total registered UEs */
    ogs_list_for_each(&amf_self()->amf_ue_list, amf_ue) {
        if (AMF_UE_HAVE_SUCI(amf_ue))
            total_ues++;
    }

    /* Add statistics to response */
    cJSON_AddNumberToObject(root, "total_plmns", count_unique_plmns());
    cJSON_AddNumberToObject(root, "connected_gnbs", total_gnbs);
    cJSON_AddNumberToObject(root, "registered_ues", total_ues);

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

    plmn_id_str = message->h.resource.component[1];
    
    if (!plmn_id_str) {
        ogs_error("[OAM] Missing PLMN ID in path");
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Missing PLMN ID", NULL, NULL));
        return false;
    }

    /* Create response object */
    root = cJSON_CreateObject();
    s_nssai_array = cJSON_CreateArray();

    /* Search for ALL entries with this PLMN ID and collect all S-NSSAIs */
    for (i = 0; i < amf_self()->num_of_plmn_support; i++) {
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char current_plmn_str[OGS_PLMNIDSTRLEN];
        
        ogs_plmn_id_to_string(plmn_id, current_plmn_str);
        
        if (strcmp(current_plmn_str, plmn_id_str) == 0) {
            /* First match: add PLMN info */
            if (!found) {
                cJSON_AddStringToObject(root, "plmn_id", current_plmn_str);
                cJSON_AddNumberToObject(root, "mcc", ogs_plmn_id_mcc(plmn_id));
                cJSON_AddNumberToObject(root, "mnc", ogs_plmn_id_mnc(plmn_id));
                found = true;
            }
            
            /* Add all S-NSSAIs from this entry */
            for (k = 0; k < amf_self()->plmn_support[i].num_of_s_nssai; k++) {
                cJSON *nssai_obj = cJSON_CreateObject();
                ogs_s_nssai_t *s_nssai = &amf_self()->plmn_support[i].s_nssai[k];
                
                cJSON_AddNumberToObject(nssai_obj, "sst", s_nssai->sst);
                if (s_nssai->sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                    char *sd_str = ogs_s_nssai_sd_to_string(s_nssai->sd);
                    cJSON_AddStringToObject(nssai_obj, "sd", sd_str);
                    ogs_free(sd_str);
                }
                cJSON_AddItemToArray(s_nssai_array, nssai_obj);
            }
        }
    }

    if (!found) {
        cJSON_Delete(root);
        cJSON_Delete(s_nssai_array);
        ogs_error("[OAM] PLMN not found: %s", plmn_id_str);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
            message, "PLMN not found", plmn_id_str, NULL));
        return false;
    }

    /* Add collected S-NSSAI array to response */
    cJSON_AddItemToObject(root, "s_nssai", s_nssai_array);

    response_body = cJSON_Print(root);
    
    char plmn_id_str_copy[OGS_PLMNIDSTRLEN] = {0};
    if (plmn_id_str)
        strncpy(plmn_id_str_copy, plmn_id_str, sizeof(plmn_id_str_copy) - 1);
    cJSON_Delete(root);

    /* Build SBI response */
    memset(&response_message, 0, sizeof(response_message));
    response_message.res_status = OGS_SBI_HTTP_STATUS_OK;
    
    response = ogs_sbi_build_response(&response_message, status);
    ogs_assert(response);
    
    response->http.content = response_body;
    response->http.content_length = strlen(response_body);
    ogs_assert(true == ogs_sbi_server_send_response(stream, response));

    ogs_info("[OAM] Retrieved PLMN: %s", plmn_id_str_copy);

    return true;
}

/*
 * DELETE /namf-oam/v1/plmns/{plmn_id}
 * 
 * Remove ALL entries with this PLMN ID from the configuration
 */
bool namf_oam_handle_plmns_delete(ogs_sbi_stream_t *stream, ogs_sbi_message_t *message)
{
    cJSON *response_json = NULL;
    char *response_body = NULL;
    ogs_sbi_message_t response_message;
    ogs_sbi_response_t *response = NULL;
    const char *plmn_id_str = NULL;
    int status = OGS_SBI_HTTP_STATUS_OK;
    int i;
    bool found = false;
    int deleted_count = 0;
    ogs_plmn_id_t deleted_plmn_id;
    
    plmn_id_str = message->h.resource.component[1];
    
    if (!plmn_id_str) {
        ogs_error("[OAM] Missing PLMN ID in path");
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
            message, "Missing PLMN ID", NULL, NULL));
        return false;
    }

    /* Delete ALL entries with this PLMN ID */
    i = 0;
    while (i < amf_self()->num_of_plmn_support) {
        ogs_plmn_id_t *plmn_id = &amf_self()->plmn_support[i].plmn_id;
        char current_plmn_str[OGS_PLMNIDSTRLEN];
        
        ogs_plmn_id_to_string(plmn_id, current_plmn_str);
        
        if (strcmp(current_plmn_str, plmn_id_str) == 0) {
            int j;
            
            /* Save the PLMN ID before deleting (for gNB disconnection) */
            if (!found) {
                memcpy(&deleted_plmn_id, plmn_id, sizeof(ogs_plmn_id_t));
            }
            
            found = true;
            deleted_count++;
            
            /* Shift remaining entries to fill the gap */
            for (j = i; j < amf_self()->num_of_plmn_support - 1; j++) {
                memcpy(&amf_self()->plmn_support[j], 
                       &amf_self()->plmn_support[j + 1],
                       sizeof(amf_self()->plmn_support[0]));
            }
            
            /* Clear the last entry */
            memset(&amf_self()->plmn_support[amf_self()->num_of_plmn_support - 1],
                   0, sizeof(amf_self()->plmn_support[0]));
            amf_self()->num_of_plmn_support--;
            
        } else {
            i++;
        }
    }

    if (!found) {
        ogs_error("[OAM] PLMN not found for deletion: %s", plmn_id_str);
        ogs_assert(true == ogs_sbi_server_send_error(
            stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
            message, "PLMN not found", plmn_id_str, NULL));
        return false;
    }


    /* Send AMFConfigurationUpdate to all connected gNBs */
    ngap_send_amf_configuration_update_all();

    /* Release UEs that are using the deleted PLMN */
    release_ues_of_plmn(&deleted_plmn_id);

    /* Build success response */
    response_json = cJSON_CreateObject();
    cJSON_AddStringToObject(response_json, "status", "success");
    
    if (deleted_count == 1) {
        cJSON_AddStringToObject(response_json, "message", "PLMN deleted successfully");
    } else {
        char *msg = ogs_msprintf("PLMN deleted successfully (%d entries removed)", deleted_count);
        cJSON_AddStringToObject(response_json, "message", msg);
        ogs_free(msg);
    }
    
    cJSON_AddStringToObject(response_json, "deleted_plmn_id", plmn_id_str);
    cJSON_AddNumberToObject(response_json, "deleted_entries", deleted_count);
    cJSON_AddNumberToObject(response_json, "remaining_plmns", count_unique_plmns());

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

    ogs_info("[OAM] PLMN deleted: %s (%d entries), remaining: %d",
        plmn_id_str, deleted_count, amf_self()->num_of_plmn_support);
    
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

    /* Check for duplicate PLMN + S-NSSAI combinations before adding */
    for (i = 0; i < num_slices; i++) {
        cJSON *slice_obj = cJSON_GetArrayItem(s_nssai_array, i);
        cJSON *sst_obj = cJSON_GetObjectItem(slice_obj, "sst");
        cJSON *sd_obj = cJSON_GetObjectItem(slice_obj, "sd");
        ogs_s_nssai_t temp_nssai;
        char plmn_id_str[OGS_PLMNIDSTRLEN];
        
        if (!sst_obj || !cJSON_IsNumber(sst_obj)) {
            ogs_error("[OAM] Slice %d: missing or invalid 'sst'", i);
            cJSON_Delete(root);
            ogs_assert(true == ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                message, "Each slice must have 'sst' (number)", NULL, NULL));
            return false;
        }
        
        temp_nssai.sst = (uint8_t)sst_obj->valueint;
        
        if (sd_obj && cJSON_IsString(sd_obj)) {
            temp_nssai.sd.v = strtoul(sd_obj->valuestring, NULL, 16);
        } else {
            temp_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
        }
        
        /* Check if this PLMN + S-NSSAI combination already exists */
        if (is_duplicate_slice(&plmn_id, &temp_nssai)) {
            char *sd_str = NULL;
            char *error_msg = NULL;
            
            ogs_plmn_id_to_string(&plmn_id, plmn_id_str);
            
            if (temp_nssai.sd.v != OGS_S_NSSAI_NO_SD_VALUE) {
                sd_str = ogs_s_nssai_sd_to_string(temp_nssai.sd);
                ogs_error("[OAM] Duplicate slice: PLMN=%s, SST=%d, SD=%s", 
                    plmn_id_str, temp_nssai.sst, sd_str);
                
                error_msg = ogs_msprintf("Slice already exists: PLMN=%s, SST=%d, SD=%s",
                    plmn_id_str, temp_nssai.sst, sd_str);
                ogs_free(sd_str);
            } else {
                ogs_error("[OAM] Duplicate slice: PLMN=%s, SST=%d (no SD)", 
                    plmn_id_str, temp_nssai.sst);
                
                error_msg = ogs_msprintf("Slice already exists: PLMN=%s, SST=%d",
                    plmn_id_str, temp_nssai.sst);
            }
            
            cJSON_Delete(root);
            ogs_assert(error_msg);
            ogs_assert(true == ogs_sbi_server_send_error(
                stream, OGS_SBI_HTTP_STATUS_CONFLICT,
                message, error_msg, NULL, NULL));
            ogs_free(error_msg);
            
            return false;
        }
    }

    /* Add new PLMN entry */
    int plmn_idx = amf_self()->num_of_plmn_support;
    
    memcpy(&amf_self()->plmn_support[plmn_idx].plmn_id, 
           &plmn_id, sizeof(ogs_plmn_id_t));
    
    amf_self()->plmn_support[plmn_idx].num_of_s_nssai = 0;
    
    for (i = 0; i < num_slices; i++) {
        cJSON *slice_obj = cJSON_GetArrayItem(s_nssai_array, i);
        cJSON *sst_obj = cJSON_GetObjectItem(slice_obj, "sst");
        cJSON *sd_obj = cJSON_GetObjectItem(slice_obj, "sd");

        uint8_t sst = (uint8_t)sst_obj->valueint;
        amf_self()->plmn_support[plmn_idx].s_nssai[i].sst = sst;

        /* SD is optional */
        if (sd_obj && cJSON_IsString(sd_obj)) {
            ogs_uint24_t sd;
            sd.v = strtoul(sd_obj->valuestring, NULL, 16);
            amf_self()->plmn_support[plmn_idx].s_nssai[i].sd = sd;
        } else {
            amf_self()->plmn_support[plmn_idx].s_nssai[i].sd.v = OGS_S_NSSAI_NO_SD_VALUE;
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
