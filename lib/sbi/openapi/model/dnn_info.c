
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_info.h"

OpenAPI_dnn_info_t *OpenAPI_dnn_info_create(
    char *dnn,
    bool is_default_dnn_indicator,
    int default_dnn_indicator,
    bool is_lbo_roaming_allowed,
    int lbo_roaming_allowed,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    bool is_dnn_barred,
    int dnn_barred,
    bool is_invoke_nef_ind,
    int invoke_nef_ind,
    OpenAPI_list_t *smf_list,
    bool is_same_smf_ind,
    int same_smf_ind
)
{
    OpenAPI_dnn_info_t *dnn_info_local_var = ogs_malloc(sizeof(OpenAPI_dnn_info_t));
    ogs_assert(dnn_info_local_var);

    dnn_info_local_var->dnn = dnn;
    dnn_info_local_var->is_default_dnn_indicator = is_default_dnn_indicator;
    dnn_info_local_var->default_dnn_indicator = default_dnn_indicator;
    dnn_info_local_var->is_lbo_roaming_allowed = is_lbo_roaming_allowed;
    dnn_info_local_var->lbo_roaming_allowed = lbo_roaming_allowed;
    dnn_info_local_var->is_iwk_eps_ind = is_iwk_eps_ind;
    dnn_info_local_var->iwk_eps_ind = iwk_eps_ind;
    dnn_info_local_var->is_dnn_barred = is_dnn_barred;
    dnn_info_local_var->dnn_barred = dnn_barred;
    dnn_info_local_var->is_invoke_nef_ind = is_invoke_nef_ind;
    dnn_info_local_var->invoke_nef_ind = invoke_nef_ind;
    dnn_info_local_var->smf_list = smf_list;
    dnn_info_local_var->is_same_smf_ind = is_same_smf_ind;
    dnn_info_local_var->same_smf_ind = same_smf_ind;

    return dnn_info_local_var;
}

void OpenAPI_dnn_info_free(OpenAPI_dnn_info_t *dnn_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dnn_info) {
        return;
    }
    if (dnn_info->dnn) {
        ogs_free(dnn_info->dnn);
        dnn_info->dnn = NULL;
    }
    if (dnn_info->smf_list) {
        OpenAPI_list_for_each(dnn_info->smf_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_info->smf_list);
        dnn_info->smf_list = NULL;
    }
    ogs_free(dnn_info);
}

cJSON *OpenAPI_dnn_info_convertToJSON(OpenAPI_dnn_info_t *dnn_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dnn_info == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [DnnInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_info->dnn) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_info->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_info->is_default_dnn_indicator) {
    if (cJSON_AddBoolToObject(item, "defaultDnnIndicator", dnn_info->default_dnn_indicator) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [default_dnn_indicator]");
        goto end;
    }
    }

    if (dnn_info->is_lbo_roaming_allowed) {
    if (cJSON_AddBoolToObject(item, "lboRoamingAllowed", dnn_info->lbo_roaming_allowed) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [lbo_roaming_allowed]");
        goto end;
    }
    }

    if (dnn_info->is_iwk_eps_ind) {
    if (cJSON_AddBoolToObject(item, "iwkEpsInd", dnn_info->iwk_eps_ind) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    if (dnn_info->is_dnn_barred) {
    if (cJSON_AddBoolToObject(item, "dnnBarred", dnn_info->dnn_barred) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [dnn_barred]");
        goto end;
    }
    }

    if (dnn_info->is_invoke_nef_ind) {
    if (cJSON_AddBoolToObject(item, "invokeNefInd", dnn_info->invoke_nef_ind) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [invoke_nef_ind]");
        goto end;
    }
    }

    if (dnn_info->smf_list) {
    cJSON *smf_listList = cJSON_AddArrayToObject(item, "smfList");
    if (smf_listList == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [smf_list]");
        goto end;
    }
    OpenAPI_list_for_each(dnn_info->smf_list, node) {
        if (cJSON_AddStringToObject(smf_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_dnn_info_convertToJSON() failed [smf_list]");
            goto end;
        }
    }
    }

    if (dnn_info->is_same_smf_ind) {
    if (cJSON_AddBoolToObject(item, "sameSmfInd", dnn_info->same_smf_ind) == NULL) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed [same_smf_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dnn_info_t *OpenAPI_dnn_info_parseFromJSON(cJSON *dnn_infoJSON)
{
    OpenAPI_dnn_info_t *dnn_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *default_dnn_indicator = NULL;
    cJSON *lbo_roaming_allowed = NULL;
    cJSON *iwk_eps_ind = NULL;
    cJSON *dnn_barred = NULL;
    cJSON *invoke_nef_ind = NULL;
    cJSON *smf_list = NULL;
    OpenAPI_list_t *smf_listList = NULL;
    cJSON *same_smf_ind = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    default_dnn_indicator = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "defaultDnnIndicator");
    if (default_dnn_indicator) {
    if (!cJSON_IsBool(default_dnn_indicator)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [default_dnn_indicator]");
        goto end;
    }
    }

    lbo_roaming_allowed = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "lboRoamingAllowed");
    if (lbo_roaming_allowed) {
    if (!cJSON_IsBool(lbo_roaming_allowed)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [lbo_roaming_allowed]");
        goto end;
    }
    }

    iwk_eps_ind = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "iwkEpsInd");
    if (iwk_eps_ind) {
    if (!cJSON_IsBool(iwk_eps_ind)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [iwk_eps_ind]");
        goto end;
    }
    }

    dnn_barred = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "dnnBarred");
    if (dnn_barred) {
    if (!cJSON_IsBool(dnn_barred)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [dnn_barred]");
        goto end;
    }
    }

    invoke_nef_ind = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "invokeNefInd");
    if (invoke_nef_ind) {
    if (!cJSON_IsBool(invoke_nef_ind)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [invoke_nef_ind]");
        goto end;
    }
    }

    smf_list = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "smfList");
    if (smf_list) {
        cJSON *smf_list_local = NULL;
        if (!cJSON_IsArray(smf_list)) {
            ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [smf_list]");
            goto end;
        }

        smf_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(smf_list_local, smf_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(smf_list_local)) {
                ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [smf_list]");
                goto end;
            }
            OpenAPI_list_add(smf_listList, ogs_strdup(smf_list_local->valuestring));
        }
    }

    same_smf_ind = cJSON_GetObjectItemCaseSensitive(dnn_infoJSON, "sameSmfInd");
    if (same_smf_ind) {
    if (!cJSON_IsBool(same_smf_ind)) {
        ogs_error("OpenAPI_dnn_info_parseFromJSON() failed [same_smf_ind]");
        goto end;
    }
    }

    dnn_info_local_var = OpenAPI_dnn_info_create (
        ogs_strdup(dnn->valuestring),
        default_dnn_indicator ? true : false,
        default_dnn_indicator ? default_dnn_indicator->valueint : 0,
        lbo_roaming_allowed ? true : false,
        lbo_roaming_allowed ? lbo_roaming_allowed->valueint : 0,
        iwk_eps_ind ? true : false,
        iwk_eps_ind ? iwk_eps_ind->valueint : 0,
        dnn_barred ? true : false,
        dnn_barred ? dnn_barred->valueint : 0,
        invoke_nef_ind ? true : false,
        invoke_nef_ind ? invoke_nef_ind->valueint : 0,
        smf_list ? smf_listList : NULL,
        same_smf_ind ? true : false,
        same_smf_ind ? same_smf_ind->valueint : 0
    );

    return dnn_info_local_var;
end:
    if (smf_listList) {
        OpenAPI_list_for_each(smf_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(smf_listList);
        smf_listList = NULL;
    }
    return NULL;
}

OpenAPI_dnn_info_t *OpenAPI_dnn_info_copy(OpenAPI_dnn_info_t *dst, OpenAPI_dnn_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dnn_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dnn_info_convertToJSON() failed");
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

    OpenAPI_dnn_info_free(dst);
    dst = OpenAPI_dnn_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

