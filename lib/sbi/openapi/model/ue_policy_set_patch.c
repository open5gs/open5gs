
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_policy_set_patch.h"

OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_create(
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    bool is_andsp_ind,
    int andsp_ind,
    bool is_eps_ursp_ind,
    int eps_ursp_ind,
    bool is_vps_ursp_ind,
    int vps_ursp_ind,
    bool is_ursp_enf_ind,
    int ursp_enf_ind,
    char *pei,
    OpenAPI_list_t *os_ids,
    OpenAPI_list_t *restri_status,
    bool is_spend_lim_info_null,
    OpenAPI_list_t* spend_lim_info
)
{
    OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch_local_var = ogs_malloc(sizeof(OpenAPI_ue_policy_set_patch_t));
    ogs_assert(ue_policy_set_patch_local_var);

    ue_policy_set_patch_local_var->ue_policy_sections = ue_policy_sections;
    ue_policy_set_patch_local_var->upsis = upsis;
    ue_policy_set_patch_local_var->is_andsp_ind = is_andsp_ind;
    ue_policy_set_patch_local_var->andsp_ind = andsp_ind;
    ue_policy_set_patch_local_var->is_eps_ursp_ind = is_eps_ursp_ind;
    ue_policy_set_patch_local_var->eps_ursp_ind = eps_ursp_ind;
    ue_policy_set_patch_local_var->is_vps_ursp_ind = is_vps_ursp_ind;
    ue_policy_set_patch_local_var->vps_ursp_ind = vps_ursp_ind;
    ue_policy_set_patch_local_var->is_ursp_enf_ind = is_ursp_enf_ind;
    ue_policy_set_patch_local_var->ursp_enf_ind = ursp_enf_ind;
    ue_policy_set_patch_local_var->pei = pei;
    ue_policy_set_patch_local_var->os_ids = os_ids;
    ue_policy_set_patch_local_var->restri_status = restri_status;
    ue_policy_set_patch_local_var->is_spend_lim_info_null = is_spend_lim_info_null;
    ue_policy_set_patch_local_var->spend_lim_info = spend_lim_info;

    return ue_policy_set_patch_local_var;
}

void OpenAPI_ue_policy_set_patch_free(OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_policy_set_patch) {
        return;
    }
    if (ue_policy_set_patch->ue_policy_sections) {
        OpenAPI_list_for_each(ue_policy_set_patch->ue_policy_sections, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_ue_policy_section_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_set_patch->ue_policy_sections);
        ue_policy_set_patch->ue_policy_sections = NULL;
    }
    if (ue_policy_set_patch->upsis) {
        OpenAPI_list_for_each(ue_policy_set_patch->upsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set_patch->upsis);
        ue_policy_set_patch->upsis = NULL;
    }
    if (ue_policy_set_patch->pei) {
        ogs_free(ue_policy_set_patch->pei);
        ue_policy_set_patch->pei = NULL;
    }
    if (ue_policy_set_patch->os_ids) {
        OpenAPI_list_for_each(ue_policy_set_patch->os_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set_patch->os_ids);
        ue_policy_set_patch->os_ids = NULL;
    }
    if (ue_policy_set_patch->restri_status) {
        OpenAPI_list_for_each(ue_policy_set_patch->restri_status, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set_patch->restri_status);
        ue_policy_set_patch->restri_status = NULL;
    }
    if (ue_policy_set_patch->spend_lim_info) {
        OpenAPI_list_for_each(ue_policy_set_patch->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_set_patch->spend_lim_info);
        ue_policy_set_patch->spend_lim_info = NULL;
    }
    ogs_free(ue_policy_set_patch);
}

cJSON *OpenAPI_ue_policy_set_patch_convertToJSON(OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_policy_set_patch == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [UePolicySetPatch]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_policy_set_patch->ue_policy_sections) {
    cJSON *ue_policy_sections = cJSON_AddObjectToObject(item, "uePolicySections");
    if (ue_policy_sections == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [ue_policy_sections]");
        goto end;
    }
    cJSON *localMapObject = ue_policy_sections;
    if (ue_policy_set_patch->ue_policy_sections) {
        OpenAPI_list_for_each(ue_policy_set_patch->ue_policy_sections, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [ue_policy_sections]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [ue_policy_sections]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_ue_policy_section_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_policy_set_patch->upsis) {
    cJSON *upsisList = cJSON_AddArrayToObject(item, "upsis");
    if (upsisList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [upsis]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set_patch->upsis, node) {
        if (cJSON_AddStringToObject(upsisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [upsis]");
            goto end;
        }
    }
    }

    if (ue_policy_set_patch->is_andsp_ind) {
    if (cJSON_AddBoolToObject(item, "andspInd", ue_policy_set_patch->andsp_ind) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [andsp_ind]");
        goto end;
    }
    }

    if (ue_policy_set_patch->is_eps_ursp_ind) {
    if (cJSON_AddBoolToObject(item, "epsUrspInd", ue_policy_set_patch->eps_ursp_ind) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [eps_ursp_ind]");
        goto end;
    }
    }

    if (ue_policy_set_patch->is_vps_ursp_ind) {
    if (cJSON_AddBoolToObject(item, "vpsUrspInd", ue_policy_set_patch->vps_ursp_ind) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [vps_ursp_ind]");
        goto end;
    }
    }

    if (ue_policy_set_patch->is_ursp_enf_ind) {
    if (cJSON_AddBoolToObject(item, "urspEnfInd", ue_policy_set_patch->ursp_enf_ind) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [ursp_enf_ind]");
        goto end;
    }
    }

    if (ue_policy_set_patch->pei) {
    if (cJSON_AddStringToObject(item, "pei", ue_policy_set_patch->pei) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (ue_policy_set_patch->os_ids) {
    cJSON *os_idsList = cJSON_AddArrayToObject(item, "osIds");
    if (os_idsList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [os_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set_patch->os_ids, node) {
        if (cJSON_AddStringToObject(os_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [os_ids]");
            goto end;
        }
    }
    }

    if (ue_policy_set_patch->restri_status) {
    cJSON *restri_statusList = cJSON_AddArrayToObject(item, "restriStatus");
    if (restri_statusList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [restri_status]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set_patch->restri_status, node) {
        cJSON *itemLocal = OpenAPI_restricted_status_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [restri_status]");
            goto end;
        }
        cJSON_AddItemToArray(restri_statusList, itemLocal);
    }
    }

    if (ue_policy_set_patch->spend_lim_info) {
    cJSON *spend_lim_info = cJSON_AddObjectToObject(item, "spendLimInfo");
    if (spend_lim_info == NULL) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [spend_lim_info]");
        goto end;
    }
    cJSON *localMapObject = spend_lim_info;
    if (ue_policy_set_patch->spend_lim_info) {
        OpenAPI_list_for_each(ue_policy_set_patch->spend_lim_info, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [spend_lim_info]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_policy_counter_info_rm_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    } else if (ue_policy_set_patch->is_spend_lim_info_null) {
        if (cJSON_AddNullToObject(item, "spendLimInfo") == NULL) {
            ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed [spend_lim_info]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_parseFromJSON(cJSON *ue_policy_set_patchJSON)
{
    OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_policy_sections = NULL;
    OpenAPI_list_t *ue_policy_sectionsList = NULL;
    cJSON *upsis = NULL;
    OpenAPI_list_t *upsisList = NULL;
    cJSON *andsp_ind = NULL;
    cJSON *eps_ursp_ind = NULL;
    cJSON *vps_ursp_ind = NULL;
    cJSON *ursp_enf_ind = NULL;
    cJSON *pei = NULL;
    cJSON *os_ids = NULL;
    OpenAPI_list_t *os_idsList = NULL;
    cJSON *restri_status = NULL;
    OpenAPI_list_t *restri_statusList = NULL;
    cJSON *spend_lim_info = NULL;
    OpenAPI_list_t *spend_lim_infoList = NULL;
    ue_policy_sections = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "uePolicySections");
    if (ue_policy_sections) {
        cJSON *ue_policy_sections_local_map = NULL;
        if (!cJSON_IsObject(ue_policy_sections) && !cJSON_IsNull(ue_policy_sections)) {
            ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [ue_policy_sections]");
            goto end;
        }
        if (cJSON_IsObject(ue_policy_sections)) {
            ue_policy_sectionsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(ue_policy_sections_local_map, ue_policy_sections) {
                cJSON *localMapObject = ue_policy_sections_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_ue_policy_section_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(ue_policy_sectionsList, localMapKeyPair);
            }
        }
    }

    upsis = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "upsis");
    if (upsis) {
        cJSON *upsis_local = NULL;
        if (!cJSON_IsArray(upsis)) {
            ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [upsis]");
            goto end;
        }

        upsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(upsis_local, upsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(upsis_local)) {
                ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [upsis]");
                goto end;
            }
            OpenAPI_list_add(upsisList, ogs_strdup(upsis_local->valuestring));
        }
    }

    andsp_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "andspInd");
    if (andsp_ind) {
    if (!cJSON_IsBool(andsp_ind)) {
        ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [andsp_ind]");
        goto end;
    }
    }

    eps_ursp_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "epsUrspInd");
    if (eps_ursp_ind) {
    if (!cJSON_IsBool(eps_ursp_ind)) {
        ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [eps_ursp_ind]");
        goto end;
    }
    }

    vps_ursp_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "vpsUrspInd");
    if (vps_ursp_ind) {
    if (!cJSON_IsBool(vps_ursp_ind)) {
        ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [vps_ursp_ind]");
        goto end;
    }
    }

    ursp_enf_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "urspEnfInd");
    if (ursp_enf_ind) {
    if (!cJSON_IsBool(ursp_enf_ind)) {
        ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [ursp_enf_ind]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    os_ids = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "osIds");
    if (os_ids) {
        cJSON *os_ids_local = NULL;
        if (!cJSON_IsArray(os_ids)) {
            ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [os_ids]");
            goto end;
        }

        os_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(os_ids_local, os_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(os_ids_local)) {
                ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [os_ids]");
                goto end;
            }
            OpenAPI_list_add(os_idsList, ogs_strdup(os_ids_local->valuestring));
        }
    }

    restri_status = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "restriStatus");
    if (restri_status) {
        cJSON *restri_status_local = NULL;
        if (!cJSON_IsArray(restri_status)) {
            ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [restri_status]");
            goto end;
        }

        restri_statusList = OpenAPI_list_create();

        cJSON_ArrayForEach(restri_status_local, restri_status) {
            if (!cJSON_IsObject(restri_status_local)) {
                ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [restri_status]");
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

    spend_lim_info = cJSON_GetObjectItemCaseSensitive(ue_policy_set_patchJSON, "spendLimInfo");
    if (spend_lim_info) {
    if (!cJSON_IsNull(spend_lim_info)) {
        cJSON *spend_lim_info_local_map = NULL;
        if (!cJSON_IsObject(spend_lim_info) && !cJSON_IsNull(spend_lim_info)) {
            ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [spend_lim_info]");
            goto end;
        }
        if (cJSON_IsObject(spend_lim_info)) {
            spend_lim_infoList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(spend_lim_info_local_map, spend_lim_info) {
                cJSON *localMapObject = spend_lim_info_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_policy_counter_info_rm_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_policy_set_patch_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(spend_lim_infoList, localMapKeyPair);
            }
        }
    }
    }

    ue_policy_set_patch_local_var = OpenAPI_ue_policy_set_patch_create (
        ue_policy_sections ? ue_policy_sectionsList : NULL,
        upsis ? upsisList : NULL,
        andsp_ind ? true : false,
        andsp_ind ? andsp_ind->valueint : 0,
        eps_ursp_ind ? true : false,
        eps_ursp_ind ? eps_ursp_ind->valueint : 0,
        vps_ursp_ind ? true : false,
        vps_ursp_ind ? vps_ursp_ind->valueint : 0,
        ursp_enf_ind ? true : false,
        ursp_enf_ind ? ursp_enf_ind->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        os_ids ? os_idsList : NULL,
        restri_status ? restri_statusList : NULL,
        spend_lim_info && cJSON_IsNull(spend_lim_info) ? true : false,
        spend_lim_info ? spend_lim_infoList : NULL
    );

    return ue_policy_set_patch_local_var;
end:
    if (ue_policy_sectionsList) {
        OpenAPI_list_for_each(ue_policy_sectionsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_ue_policy_section_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_sectionsList);
        ue_policy_sectionsList = NULL;
    }
    if (upsisList) {
        OpenAPI_list_for_each(upsisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(upsisList);
        upsisList = NULL;
    }
    if (os_idsList) {
        OpenAPI_list_for_each(os_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(os_idsList);
        os_idsList = NULL;
    }
    if (restri_statusList) {
        OpenAPI_list_for_each(restri_statusList, node) {
            OpenAPI_restricted_status_free(node->data);
        }
        OpenAPI_list_free(restri_statusList);
        restri_statusList = NULL;
    }
    if (spend_lim_infoList) {
        OpenAPI_list_for_each(spend_lim_infoList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_policy_counter_info_rm_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(spend_lim_infoList);
        spend_lim_infoList = NULL;
    }
    return NULL;
}

OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_copy(OpenAPI_ue_policy_set_patch_t *dst, OpenAPI_ue_policy_set_patch_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_policy_set_patch_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_policy_set_patch_convertToJSON() failed");
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

    OpenAPI_ue_policy_set_patch_free(dst);
    dst = OpenAPI_ue_policy_set_patch_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

