
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lcs_subscription_data_1.h"

OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_create(
    char *configured_lmf_id,
    OpenAPI_pru_ind_e pru_ind,
    OpenAPI_lp_hap_type_e lp_hap_type,
    bool is_user_plane_pos_ind_lmf,
    int user_plane_pos_ind_lmf
)
{
    OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_lcs_subscription_data_1_t));
    ogs_assert(lcs_subscription_data_1_local_var);

    lcs_subscription_data_1_local_var->configured_lmf_id = configured_lmf_id;
    lcs_subscription_data_1_local_var->pru_ind = pru_ind;
    lcs_subscription_data_1_local_var->lp_hap_type = lp_hap_type;
    lcs_subscription_data_1_local_var->is_user_plane_pos_ind_lmf = is_user_plane_pos_ind_lmf;
    lcs_subscription_data_1_local_var->user_plane_pos_ind_lmf = user_plane_pos_ind_lmf;

    return lcs_subscription_data_1_local_var;
}

void OpenAPI_lcs_subscription_data_1_free(OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lcs_subscription_data_1) {
        return;
    }
    if (lcs_subscription_data_1->configured_lmf_id) {
        ogs_free(lcs_subscription_data_1->configured_lmf_id);
        lcs_subscription_data_1->configured_lmf_id = NULL;
    }
    ogs_free(lcs_subscription_data_1);
}

cJSON *OpenAPI_lcs_subscription_data_1_convertToJSON(OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lcs_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed [LcsSubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lcs_subscription_data_1->configured_lmf_id) {
    if (cJSON_AddStringToObject(item, "configuredLmfId", lcs_subscription_data_1->configured_lmf_id) == NULL) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed [configured_lmf_id]");
        goto end;
    }
    }

    if (lcs_subscription_data_1->pru_ind != OpenAPI_pru_ind_NULL) {
    if (cJSON_AddStringToObject(item, "pruInd", OpenAPI_pru_ind_ToString(lcs_subscription_data_1->pru_ind)) == NULL) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed [pru_ind]");
        goto end;
    }
    }

    if (lcs_subscription_data_1->lp_hap_type != OpenAPI_lp_hap_type_NULL) {
    if (cJSON_AddStringToObject(item, "lpHapType", OpenAPI_lp_hap_type_ToString(lcs_subscription_data_1->lp_hap_type)) == NULL) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed [lp_hap_type]");
        goto end;
    }
    }

    if (lcs_subscription_data_1->is_user_plane_pos_ind_lmf) {
    if (cJSON_AddBoolToObject(item, "userPlanePosIndLmf", lcs_subscription_data_1->user_plane_pos_ind_lmf) == NULL) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed [user_plane_pos_ind_lmf]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_parseFromJSON(cJSON *lcs_subscription_data_1JSON)
{
    OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *configured_lmf_id = NULL;
    cJSON *pru_ind = NULL;
    OpenAPI_pru_ind_e pru_indVariable = 0;
    cJSON *lp_hap_type = NULL;
    OpenAPI_lp_hap_type_e lp_hap_typeVariable = 0;
    cJSON *user_plane_pos_ind_lmf = NULL;
    configured_lmf_id = cJSON_GetObjectItemCaseSensitive(lcs_subscription_data_1JSON, "configuredLmfId");
    if (configured_lmf_id) {
    if (!cJSON_IsString(configured_lmf_id) && !cJSON_IsNull(configured_lmf_id)) {
        ogs_error("OpenAPI_lcs_subscription_data_1_parseFromJSON() failed [configured_lmf_id]");
        goto end;
    }
    }

    pru_ind = cJSON_GetObjectItemCaseSensitive(lcs_subscription_data_1JSON, "pruInd");
    if (pru_ind) {
    if (!cJSON_IsString(pru_ind)) {
        ogs_error("OpenAPI_lcs_subscription_data_1_parseFromJSON() failed [pru_ind]");
        goto end;
    }
    pru_indVariable = OpenAPI_pru_ind_FromString(pru_ind->valuestring);
    }

    lp_hap_type = cJSON_GetObjectItemCaseSensitive(lcs_subscription_data_1JSON, "lpHapType");
    if (lp_hap_type) {
    if (!cJSON_IsString(lp_hap_type)) {
        ogs_error("OpenAPI_lcs_subscription_data_1_parseFromJSON() failed [lp_hap_type]");
        goto end;
    }
    lp_hap_typeVariable = OpenAPI_lp_hap_type_FromString(lp_hap_type->valuestring);
    }

    user_plane_pos_ind_lmf = cJSON_GetObjectItemCaseSensitive(lcs_subscription_data_1JSON, "userPlanePosIndLmf");
    if (user_plane_pos_ind_lmf) {
    if (!cJSON_IsBool(user_plane_pos_ind_lmf)) {
        ogs_error("OpenAPI_lcs_subscription_data_1_parseFromJSON() failed [user_plane_pos_ind_lmf]");
        goto end;
    }
    }

    lcs_subscription_data_1_local_var = OpenAPI_lcs_subscription_data_1_create (
        configured_lmf_id && !cJSON_IsNull(configured_lmf_id) ? ogs_strdup(configured_lmf_id->valuestring) : NULL,
        pru_ind ? pru_indVariable : 0,
        lp_hap_type ? lp_hap_typeVariable : 0,
        user_plane_pos_ind_lmf ? true : false,
        user_plane_pos_ind_lmf ? user_plane_pos_ind_lmf->valueint : 0
    );

    return lcs_subscription_data_1_local_var;
end:
    return NULL;
}

OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_copy(OpenAPI_lcs_subscription_data_1_t *dst, OpenAPI_lcs_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lcs_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lcs_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_lcs_subscription_data_1_free(dst);
    dst = OpenAPI_lcs_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

