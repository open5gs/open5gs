
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_retrieved_data.h"

OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_create(
    char *ue_eps_pdn_connection,
    OpenAPI_sm_context_t *sm_context,
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status,
    bool is_dl_data_waiting_ind,
    int dl_data_waiting_ind,
    OpenAPI_af_coordination_info_t *af_coordination_info
)
{
    OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_retrieved_data_t));
    ogs_assert(sm_context_retrieved_data_local_var);

    sm_context_retrieved_data_local_var->ue_eps_pdn_connection = ue_eps_pdn_connection;
    sm_context_retrieved_data_local_var->sm_context = sm_context;
    sm_context_retrieved_data_local_var->small_data_rate_status = small_data_rate_status;
    sm_context_retrieved_data_local_var->apn_rate_status = apn_rate_status;
    sm_context_retrieved_data_local_var->is_dl_data_waiting_ind = is_dl_data_waiting_ind;
    sm_context_retrieved_data_local_var->dl_data_waiting_ind = dl_data_waiting_ind;
    sm_context_retrieved_data_local_var->af_coordination_info = af_coordination_info;

    return sm_context_retrieved_data_local_var;
}

void OpenAPI_sm_context_retrieved_data_free(OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_retrieved_data) {
        return;
    }
    if (sm_context_retrieved_data->ue_eps_pdn_connection) {
        ogs_free(sm_context_retrieved_data->ue_eps_pdn_connection);
        sm_context_retrieved_data->ue_eps_pdn_connection = NULL;
    }
    if (sm_context_retrieved_data->sm_context) {
        OpenAPI_sm_context_free(sm_context_retrieved_data->sm_context);
        sm_context_retrieved_data->sm_context = NULL;
    }
    if (sm_context_retrieved_data->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(sm_context_retrieved_data->small_data_rate_status);
        sm_context_retrieved_data->small_data_rate_status = NULL;
    }
    if (sm_context_retrieved_data->apn_rate_status) {
        OpenAPI_apn_rate_status_free(sm_context_retrieved_data->apn_rate_status);
        sm_context_retrieved_data->apn_rate_status = NULL;
    }
    if (sm_context_retrieved_data->af_coordination_info) {
        OpenAPI_af_coordination_info_free(sm_context_retrieved_data->af_coordination_info);
        sm_context_retrieved_data->af_coordination_info = NULL;
    }
    ogs_free(sm_context_retrieved_data);
}

cJSON *OpenAPI_sm_context_retrieved_data_convertToJSON(OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_retrieved_data == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [SmContextRetrievedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_context_retrieved_data->ue_eps_pdn_connection) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [ue_eps_pdn_connection]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ueEpsPdnConnection", sm_context_retrieved_data->ue_eps_pdn_connection) == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }

    if (sm_context_retrieved_data->sm_context) {
    cJSON *sm_context_local_JSON = OpenAPI_sm_context_convertToJSON(sm_context_retrieved_data->sm_context);
    if (sm_context_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [sm_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smContext", sm_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [sm_context]");
        goto end;
    }
    }

    if (sm_context_retrieved_data->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(sm_context_retrieved_data->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (sm_context_retrieved_data->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(sm_context_retrieved_data->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

    if (sm_context_retrieved_data->is_dl_data_waiting_ind) {
    if (cJSON_AddBoolToObject(item, "dlDataWaitingInd", sm_context_retrieved_data->dl_data_waiting_ind) == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [dl_data_waiting_ind]");
        goto end;
    }
    }

    if (sm_context_retrieved_data->af_coordination_info) {
    cJSON *af_coordination_info_local_JSON = OpenAPI_af_coordination_info_convertToJSON(sm_context_retrieved_data->af_coordination_info);
    if (af_coordination_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [af_coordination_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afCoordinationInfo", af_coordination_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed [af_coordination_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_parseFromJSON(cJSON *sm_context_retrieved_dataJSON)
{
    OpenAPI_sm_context_retrieved_data_t *sm_context_retrieved_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_eps_pdn_connection = NULL;
    cJSON *sm_context = NULL;
    OpenAPI_sm_context_t *sm_context_local_nonprim = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    cJSON *dl_data_waiting_ind = NULL;
    cJSON *af_coordination_info = NULL;
    OpenAPI_af_coordination_info_t *af_coordination_info_local_nonprim = NULL;
    ue_eps_pdn_connection = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "ueEpsPdnConnection");
    if (!ue_eps_pdn_connection) {
        ogs_error("OpenAPI_sm_context_retrieved_data_parseFromJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }
    if (!cJSON_IsString(ue_eps_pdn_connection)) {
        ogs_error("OpenAPI_sm_context_retrieved_data_parseFromJSON() failed [ue_eps_pdn_connection]");
        goto end;
    }

    sm_context = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "smContext");
    if (sm_context) {
    sm_context_local_nonprim = OpenAPI_sm_context_parseFromJSON(sm_context);
    if (!sm_context_local_nonprim) {
        ogs_error("OpenAPI_sm_context_parseFromJSON failed [sm_context]");
        goto end;
    }
    }

    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    dl_data_waiting_ind = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "dlDataWaitingInd");
    if (dl_data_waiting_ind) {
    if (!cJSON_IsBool(dl_data_waiting_ind)) {
        ogs_error("OpenAPI_sm_context_retrieved_data_parseFromJSON() failed [dl_data_waiting_ind]");
        goto end;
    }
    }

    af_coordination_info = cJSON_GetObjectItemCaseSensitive(sm_context_retrieved_dataJSON, "afCoordinationInfo");
    if (af_coordination_info) {
    af_coordination_info_local_nonprim = OpenAPI_af_coordination_info_parseFromJSON(af_coordination_info);
    if (!af_coordination_info_local_nonprim) {
        ogs_error("OpenAPI_af_coordination_info_parseFromJSON failed [af_coordination_info]");
        goto end;
    }
    }

    sm_context_retrieved_data_local_var = OpenAPI_sm_context_retrieved_data_create (
        ogs_strdup(ue_eps_pdn_connection->valuestring),
        sm_context ? sm_context_local_nonprim : NULL,
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL,
        dl_data_waiting_ind ? true : false,
        dl_data_waiting_ind ? dl_data_waiting_ind->valueint : 0,
        af_coordination_info ? af_coordination_info_local_nonprim : NULL
    );

    return sm_context_retrieved_data_local_var;
end:
    if (sm_context_local_nonprim) {
        OpenAPI_sm_context_free(sm_context_local_nonprim);
        sm_context_local_nonprim = NULL;
    }
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (apn_rate_status_local_nonprim) {
        OpenAPI_apn_rate_status_free(apn_rate_status_local_nonprim);
        apn_rate_status_local_nonprim = NULL;
    }
    if (af_coordination_info_local_nonprim) {
        OpenAPI_af_coordination_info_free(af_coordination_info_local_nonprim);
        af_coordination_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_retrieved_data_t *OpenAPI_sm_context_retrieved_data_copy(OpenAPI_sm_context_retrieved_data_t *dst, OpenAPI_sm_context_retrieved_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_retrieved_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_retrieved_data_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_sm_context_retrieved_data_free(dst);
    dst = OpenAPI_sm_context_retrieved_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

