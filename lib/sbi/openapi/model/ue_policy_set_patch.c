
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_policy_set_patch.h"

OpenAPI_ue_policy_set_patch_t *OpenAPI_ue_policy_set_patch_create(
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    bool is_andsp_ind,
    int andsp_ind,
    char *pei,
    OpenAPI_list_t *os_ids
)
{
    OpenAPI_ue_policy_set_patch_t *ue_policy_set_patch_local_var = ogs_malloc(sizeof(OpenAPI_ue_policy_set_patch_t));
    ogs_assert(ue_policy_set_patch_local_var);

    ue_policy_set_patch_local_var->ue_policy_sections = ue_policy_sections;
    ue_policy_set_patch_local_var->upsis = upsis;
    ue_policy_set_patch_local_var->is_andsp_ind = is_andsp_ind;
    ue_policy_set_patch_local_var->andsp_ind = andsp_ind;
    ue_policy_set_patch_local_var->pei = pei;
    ue_policy_set_patch_local_var->os_ids = os_ids;

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
    cJSON *pei = NULL;
    cJSON *os_ids = NULL;
    OpenAPI_list_t *os_idsList = NULL;
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

    ue_policy_set_patch_local_var = OpenAPI_ue_policy_set_patch_create (
        ue_policy_sections ? ue_policy_sectionsList : NULL,
        upsis ? upsisList : NULL,
        andsp_ind ? true : false,
        andsp_ind ? andsp_ind->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        os_ids ? os_idsList : NULL
    );

    return ue_policy_set_patch_local_var;
end:
    if (ue_policy_sectionsList) {
        OpenAPI_list_for_each(ue_policy_sectionsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
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

