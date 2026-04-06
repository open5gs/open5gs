
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_policy_data.h"

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats,
    OpenAPI_charging_information_t *chf_info,
    char *chf_group_id,
    bool is_subsc_spending_limits,
    int subsc_spending_limits,
    OpenAPI_list_t* spend_lim_info,
    OpenAPI_list_t *restri_status,
    char *supp_feat
)
{
    OpenAPI_am_policy_data_t *am_policy_data_local_var = ogs_malloc(sizeof(OpenAPI_am_policy_data_t));
    ogs_assert(am_policy_data_local_var);

    am_policy_data_local_var->pra_infos = pra_infos;
    am_policy_data_local_var->subsc_cats = subsc_cats;
    am_policy_data_local_var->chf_info = chf_info;
    am_policy_data_local_var->chf_group_id = chf_group_id;
    am_policy_data_local_var->is_subsc_spending_limits = is_subsc_spending_limits;
    am_policy_data_local_var->subsc_spending_limits = subsc_spending_limits;
    am_policy_data_local_var->spend_lim_info = spend_lim_info;
    am_policy_data_local_var->restri_status = restri_status;
    am_policy_data_local_var->supp_feat = supp_feat;

    return am_policy_data_local_var;
}

void OpenAPI_am_policy_data_free(OpenAPI_am_policy_data_t *am_policy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_policy_data) {
        return;
    }
    if (am_policy_data->pra_infos) {
        OpenAPI_list_for_each(am_policy_data->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_policy_data->pra_infos);
        am_policy_data->pra_infos = NULL;
    }
    if (am_policy_data->subsc_cats) {
        OpenAPI_list_for_each(am_policy_data->subsc_cats, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(am_policy_data->subsc_cats);
        am_policy_data->subsc_cats = NULL;
    }
    if (am_policy_data->chf_info) {
        OpenAPI_charging_information_free(am_policy_data->chf_info);
        am_policy_data->chf_info = NULL;
    }
    if (am_policy_data->chf_group_id) {
        ogs_free(am_policy_data->chf_group_id);
        am_policy_data->chf_group_id = NULL;
    }
    if (am_policy_data->spend_lim_info) {
        OpenAPI_list_for_each(am_policy_data->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(am_policy_data->spend_lim_info);
        am_policy_data->spend_lim_info = NULL;
    }
    if (am_policy_data->restri_status) {
        OpenAPI_list_for_each(am_policy_data->restri_status, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(am_policy_data->restri_status);
        am_policy_data->restri_status = NULL;
    }
    if (am_policy_data->supp_feat) {
        ogs_free(am_policy_data->supp_feat);
        am_policy_data->supp_feat = NULL;
    }
    ogs_free(am_policy_data);
}

cJSON *OpenAPI_am_policy_data_convertToJSON(OpenAPI_am_policy_data_t *am_policy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_policy_data == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [AmPolicyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_policy_data->pra_infos) {
    cJSON *pra_infos = cJSON_AddObjectToObject(item, "praInfos");
    if (pra_infos == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
        goto end;
    }
    cJSON *localMapObject = pra_infos;
    if (am_policy_data->pra_infos) {
        OpenAPI_list_for_each(am_policy_data->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [pra_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (am_policy_data->subsc_cats) {
    cJSON *subsc_catsList = cJSON_AddArrayToObject(item, "subscCats");
    if (subsc_catsList == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [subsc_cats]");
        goto end;
    }
    OpenAPI_list_for_each(am_policy_data->subsc_cats, node) {
        if (cJSON_AddStringToObject(subsc_catsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [subsc_cats]");
            goto end;
        }
    }
    }

    if (am_policy_data->chf_info) {
    cJSON *chf_info_local_JSON = OpenAPI_charging_information_convertToJSON(am_policy_data->chf_info);
    if (chf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [chf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "chfInfo", chf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [chf_info]");
        goto end;
    }
    }

    if (am_policy_data->chf_group_id) {
    if (cJSON_AddStringToObject(item, "chfGroupId", am_policy_data->chf_group_id) == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [chf_group_id]");
        goto end;
    }
    }

    if (am_policy_data->is_subsc_spending_limits) {
    if (cJSON_AddBoolToObject(item, "subscSpendingLimits", am_policy_data->subsc_spending_limits) == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [subsc_spending_limits]");
        goto end;
    }
    }

    if (am_policy_data->spend_lim_info) {
    cJSON *spend_lim_info = cJSON_AddObjectToObject(item, "spendLimInfo");
    if (spend_lim_info == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [spend_lim_info]");
        goto end;
    }
    cJSON *localMapObject = spend_lim_info;
    if (am_policy_data->spend_lim_info) {
        OpenAPI_list_for_each(am_policy_data->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_policy_counter_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (am_policy_data->restri_status) {
    cJSON *restri_statusList = cJSON_AddArrayToObject(item, "restriStatus");
    if (restri_statusList == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [restri_status]");
        goto end;
    }
    OpenAPI_list_for_each(am_policy_data->restri_status, node) {
        cJSON *itemLocal = OpenAPI_restricted_status_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [restri_status]");
            goto end;
        }
        cJSON_AddItemToArray(restri_statusList, itemLocal);
    }
    }

    if (am_policy_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", am_policy_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_parseFromJSON(cJSON *am_policy_dataJSON)
{
    OpenAPI_am_policy_data_t *am_policy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pra_infos = NULL;
    OpenAPI_list_t *pra_infosList = NULL;
    cJSON *subsc_cats = NULL;
    OpenAPI_list_t *subsc_catsList = NULL;
    cJSON *chf_info = NULL;
    OpenAPI_charging_information_t *chf_info_local_nonprim = NULL;
    cJSON *chf_group_id = NULL;
    cJSON *subsc_spending_limits = NULL;
    cJSON *spend_lim_info = NULL;
    OpenAPI_list_t *spend_lim_infoList = NULL;
    cJSON *restri_status = NULL;
    OpenAPI_list_t *restri_statusList = NULL;
    cJSON *supp_feat = NULL;
    pra_infos = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "praInfos");
    if (pra_infos) {
        cJSON *pra_infos_local_map = NULL;
        if (!cJSON_IsObject(pra_infos) && !cJSON_IsNull(pra_infos)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        if (cJSON_IsObject(pra_infos)) {
            pra_infosList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
                cJSON *localMapObject = pra_infos_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_infosList, localMapKeyPair);
            }
        }
    }

    subsc_cats = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "subscCats");
    if (subsc_cats) {
        cJSON *subsc_cats_local = NULL;
        if (!cJSON_IsArray(subsc_cats)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [subsc_cats]");
            goto end;
        }

        subsc_catsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subsc_cats_local, subsc_cats) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subsc_cats_local)) {
                ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [subsc_cats]");
                goto end;
            }
            OpenAPI_list_add(subsc_catsList, ogs_strdup(subsc_cats_local->valuestring));
        }
    }

    chf_info = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "chfInfo");
    if (chf_info) {
    chf_info_local_nonprim = OpenAPI_charging_information_parseFromJSON(chf_info);
    if (!chf_info_local_nonprim) {
        ogs_error("OpenAPI_charging_information_parseFromJSON failed [chf_info]");
        goto end;
    }
    }

    chf_group_id = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "chfGroupId");
    if (chf_group_id) {
    if (!cJSON_IsString(chf_group_id) && !cJSON_IsNull(chf_group_id)) {
        ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [chf_group_id]");
        goto end;
    }
    }

    subsc_spending_limits = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "subscSpendingLimits");
    if (subsc_spending_limits) {
    if (!cJSON_IsBool(subsc_spending_limits)) {
        ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [subsc_spending_limits]");
        goto end;
    }
    }

    spend_lim_info = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "spendLimInfo");
    if (spend_lim_info) {
        cJSON *spend_lim_info_local_map = NULL;
        if (!cJSON_IsObject(spend_lim_info) && !cJSON_IsNull(spend_lim_info)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [spend_lim_info]");
            goto end;
        }
        if (cJSON_IsObject(spend_lim_info)) {
            spend_lim_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(spend_lim_info_local_map, spend_lim_info) {
                cJSON *localMapObject = spend_lim_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_policy_counter_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(spend_lim_infoList, localMapKeyPair);
            }
        }
    }

    restri_status = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "restriStatus");
    if (restri_status) {
        cJSON *restri_status_local = NULL;
        if (!cJSON_IsArray(restri_status)) {
            ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [restri_status]");
            goto end;
        }

        restri_statusList = OpenAPI_list_create();

        cJSON_ArrayForEach(restri_status_local, restri_status) {
            if (!cJSON_IsObject(restri_status_local)) {
                ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [restri_status]");
                goto end;
            }
            OpenAPI_restricted_status_t *restri_statusItem = OpenAPI_restricted_status_parseFromJSON(restri_status_local);
            if (!restri_statusItem) {
                ogs_error("No restri_statusItem");
                goto end;
            }
            OpenAPI_list_add(restri_statusList, restri_statusItem);
        }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(am_policy_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_am_policy_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    am_policy_data_local_var = OpenAPI_am_policy_data_create (
        pra_infos ? pra_infosList : NULL,
        subsc_cats ? subsc_catsList : NULL,
        chf_info ? chf_info_local_nonprim : NULL,
        chf_group_id && !cJSON_IsNull(chf_group_id) ? ogs_strdup(chf_group_id->valuestring) : NULL,
        subsc_spending_limits ? true : false,
        subsc_spending_limits ? subsc_spending_limits->valueint : 0,
        spend_lim_info ? spend_lim_infoList : NULL,
        restri_status ? restri_statusList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL
    );

    return am_policy_data_local_var;
end:
    if (pra_infosList) {
        OpenAPI_list_for_each(pra_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_infosList);
        pra_infosList = NULL;
    }
    if (subsc_catsList) {
        OpenAPI_list_for_each(subsc_catsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subsc_catsList);
        subsc_catsList = NULL;
    }
    if (chf_info_local_nonprim) {
        OpenAPI_charging_information_free(chf_info_local_nonprim);
        chf_info_local_nonprim = NULL;
    }
    if (spend_lim_infoList) {
        OpenAPI_list_for_each(spend_lim_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(spend_lim_infoList);
        spend_lim_infoList = NULL;
    }
    if (restri_statusList) {
        OpenAPI_list_for_each(restri_statusList, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(restri_statusList);
        restri_statusList = NULL;
    }
    return NULL;
}

OpenAPI_am_policy_data_t *OpenAPI_am_policy_data_copy(OpenAPI_am_policy_data_t *dst, OpenAPI_am_policy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_policy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_policy_data_convertToJSON() failed");
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

    OpenAPI_am_policy_data_free(dst);
    dst = OpenAPI_am_policy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

