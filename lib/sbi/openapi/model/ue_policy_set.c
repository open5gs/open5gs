
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_policy_set.h"

OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_create(
    OpenAPI_list_t* pra_infos,
    OpenAPI_list_t *subsc_cats,
    OpenAPI_list_t* ue_policy_sections,
    OpenAPI_list_t *upsis,
    OpenAPI_list_t* allowed_route_sel_descs,
    int andsp_ind,
    char *pei,
    OpenAPI_list_t *os_ids
    )
{
    OpenAPI_ue_policy_set_t *ue_policy_set_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_policy_set_t));
    if (!ue_policy_set_local_var) {
        return NULL;
    }
    ue_policy_set_local_var->pra_infos = pra_infos;
    ue_policy_set_local_var->subsc_cats = subsc_cats;
    ue_policy_set_local_var->ue_policy_sections = ue_policy_sections;
    ue_policy_set_local_var->upsis = upsis;
    ue_policy_set_local_var->allowed_route_sel_descs = allowed_route_sel_descs;
    ue_policy_set_local_var->andsp_ind = andsp_ind;
    ue_policy_set_local_var->pei = pei;
    ue_policy_set_local_var->os_ids = os_ids;

    return ue_policy_set_local_var;
}

void OpenAPI_ue_policy_set_free(OpenAPI_ue_policy_set_t *ue_policy_set)
{
    if (NULL == ue_policy_set) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ue_policy_set->pra_infos, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_presence_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_policy_set->pra_infos);
    OpenAPI_list_for_each(ue_policy_set->subsc_cats, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_policy_set->subsc_cats);
    OpenAPI_list_for_each(ue_policy_set->ue_policy_sections, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_ue_policy_section_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_policy_set->ue_policy_sections);
    OpenAPI_list_for_each(ue_policy_set->upsis, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_policy_set->upsis);
    OpenAPI_list_for_each(ue_policy_set->allowed_route_sel_descs, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_plmn_route_selection_descriptor_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_policy_set->allowed_route_sel_descs);
    ogs_free(ue_policy_set->pei);
    OpenAPI_list_for_each(ue_policy_set->os_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(ue_policy_set->os_ids);
    ogs_free(ue_policy_set);
}

cJSON *OpenAPI_ue_policy_set_convertToJSON(OpenAPI_ue_policy_set_t *ue_policy_set)
{
    cJSON *item = NULL;

    if (ue_policy_set == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [UePolicySet]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_policy_set->pra_infos) {
        cJSON *pra_infos = cJSON_AddObjectToObject(item, "praInfos");
        if (pra_infos == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [pra_infos]");
            goto end;
        }
        cJSON *localMapObject = pra_infos;
        OpenAPI_lnode_t *pra_infos_node;
        if (ue_policy_set->pra_infos) {
            OpenAPI_list_for_each(ue_policy_set->pra_infos, pra_infos_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pra_infos_node->data;
                cJSON *itemLocal = OpenAPI_presence_info_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [pra_infos]");
                    goto end;
                }
                cJSON_AddItemToObject(pra_infos, localKeyValue->key, itemLocal);
            }
        }
    }

    if (ue_policy_set->subsc_cats) {
        cJSON *subsc_cats = cJSON_AddArrayToObject(item, "subscCats");
        if (subsc_cats == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [subsc_cats]");
            goto end;
        }

        OpenAPI_lnode_t *subsc_cats_node;
        OpenAPI_list_for_each(ue_policy_set->subsc_cats, subsc_cats_node)  {
            if (cJSON_AddStringToObject(subsc_cats, "", (char*)subsc_cats_node->data) == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [subsc_cats]");
                goto end;
            }
        }
    }

    if (ue_policy_set->ue_policy_sections) {
        cJSON *ue_policy_sections = cJSON_AddObjectToObject(item, "uePolicySections");
        if (ue_policy_sections == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [ue_policy_sections]");
            goto end;
        }
        cJSON *localMapObject = ue_policy_sections;
        OpenAPI_lnode_t *ue_policy_sections_node;
        if (ue_policy_set->ue_policy_sections) {
            OpenAPI_list_for_each(ue_policy_set->ue_policy_sections, ue_policy_sections_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)ue_policy_sections_node->data;
                cJSON *itemLocal = OpenAPI_ue_policy_section_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [ue_policy_sections]");
                    goto end;
                }
                cJSON_AddItemToObject(ue_policy_sections, localKeyValue->key, itemLocal);
            }
        }
    }

    if (ue_policy_set->upsis) {
        cJSON *upsis = cJSON_AddArrayToObject(item, "upsis");
        if (upsis == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [upsis]");
            goto end;
        }

        OpenAPI_lnode_t *upsis_node;
        OpenAPI_list_for_each(ue_policy_set->upsis, upsis_node)  {
            if (cJSON_AddStringToObject(upsis, "", (char*)upsis_node->data) == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [upsis]");
                goto end;
            }
        }
    }

    if (ue_policy_set->allowed_route_sel_descs) {
        cJSON *allowed_route_sel_descs = cJSON_AddObjectToObject(item, "allowedRouteSelDescs");
        if (allowed_route_sel_descs == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [allowed_route_sel_descs]");
            goto end;
        }
        cJSON *localMapObject = allowed_route_sel_descs;
        OpenAPI_lnode_t *allowed_route_sel_descs_node;
        if (ue_policy_set->allowed_route_sel_descs) {
            OpenAPI_list_for_each(ue_policy_set->allowed_route_sel_descs, allowed_route_sel_descs_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)allowed_route_sel_descs_node->data;
                cJSON *itemLocal = OpenAPI_plmn_route_selection_descriptor_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [allowed_route_sel_descs]");
                    goto end;
                }
                cJSON_AddItemToObject(allowed_route_sel_descs, localKeyValue->key, itemLocal);
            }
        }
    }

    if (ue_policy_set->andsp_ind) {
        if (cJSON_AddBoolToObject(item, "andspInd", ue_policy_set->andsp_ind) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [andsp_ind]");
            goto end;
        }
    }

    if (ue_policy_set->pei) {
        if (cJSON_AddStringToObject(item, "pei", ue_policy_set->pei) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [pei]");
            goto end;
        }
    }

    if (ue_policy_set->os_ids) {
        cJSON *os_ids = cJSON_AddArrayToObject(item, "osIds");
        if (os_ids == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [os_ids]");
            goto end;
        }

        OpenAPI_lnode_t *os_ids_node;
        OpenAPI_list_for_each(ue_policy_set->os_ids, os_ids_node)  {
            if (cJSON_AddStringToObject(os_ids, "", (char*)os_ids_node->data) == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [os_ids]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_parseFromJSON(cJSON *ue_policy_setJSON)
{
    OpenAPI_ue_policy_set_t *ue_policy_set_local_var = NULL;
    cJSON *pra_infos = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "praInfos");

    OpenAPI_list_t *pra_infosList;
    if (pra_infos) {
        cJSON *pra_infos_local_map;
        if (!cJSON_IsObject(pra_infos)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [pra_infos]");
            goto end;
        }
        pra_infosList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(pra_infos_local_map, pra_infos) {
            cJSON *localMapObject = pra_infos_local_map;
            if (!cJSON_IsObject(pra_infos_local_map)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [pra_infos]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_presence_info_parseFromJSON(localMapObject));
            OpenAPI_list_add(pra_infosList, localMapKeyPair);
        }
    }

    cJSON *subsc_cats = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "subscCats");

    OpenAPI_list_t *subsc_catsList;
    if (subsc_cats) {
        cJSON *subsc_cats_local;
        if (!cJSON_IsArray(subsc_cats)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [subsc_cats]");
            goto end;
        }
        subsc_catsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subsc_cats_local, subsc_cats) {
            if (!cJSON_IsString(subsc_cats_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [subsc_cats]");
                goto end;
            }
            OpenAPI_list_add(subsc_catsList, ogs_strdup(subsc_cats_local->valuestring));
        }
    }

    cJSON *ue_policy_sections = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "uePolicySections");

    OpenAPI_list_t *ue_policy_sectionsList;
    if (ue_policy_sections) {
        cJSON *ue_policy_sections_local_map;
        if (!cJSON_IsObject(ue_policy_sections)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [ue_policy_sections]");
            goto end;
        }
        ue_policy_sectionsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(ue_policy_sections_local_map, ue_policy_sections) {
            cJSON *localMapObject = ue_policy_sections_local_map;
            if (!cJSON_IsObject(ue_policy_sections_local_map)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [ue_policy_sections]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_ue_policy_section_parseFromJSON(localMapObject));
            OpenAPI_list_add(ue_policy_sectionsList, localMapKeyPair);
        }
    }

    cJSON *upsis = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "upsis");

    OpenAPI_list_t *upsisList;
    if (upsis) {
        cJSON *upsis_local;
        if (!cJSON_IsArray(upsis)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [upsis]");
            goto end;
        }
        upsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(upsis_local, upsis) {
            if (!cJSON_IsString(upsis_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [upsis]");
                goto end;
            }
            OpenAPI_list_add(upsisList, ogs_strdup(upsis_local->valuestring));
        }
    }

    cJSON *allowed_route_sel_descs = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "allowedRouteSelDescs");

    OpenAPI_list_t *allowed_route_sel_descsList;
    if (allowed_route_sel_descs) {
        cJSON *allowed_route_sel_descs_local_map;
        if (!cJSON_IsObject(allowed_route_sel_descs)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [allowed_route_sel_descs]");
            goto end;
        }
        allowed_route_sel_descsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(allowed_route_sel_descs_local_map, allowed_route_sel_descs) {
            cJSON *localMapObject = allowed_route_sel_descs_local_map;
            if (!cJSON_IsObject(allowed_route_sel_descs_local_map)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [allowed_route_sel_descs]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_plmn_route_selection_descriptor_parseFromJSON(localMapObject));
            OpenAPI_list_add(allowed_route_sel_descsList, localMapKeyPair);
        }
    }

    cJSON *andsp_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "andspInd");

    if (andsp_ind) {
        if (!cJSON_IsBool(andsp_ind)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [andsp_ind]");
            goto end;
        }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "pei");

    if (pei) {
        if (!cJSON_IsString(pei)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [pei]");
            goto end;
        }
    }

    cJSON *os_ids = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "osIds");

    OpenAPI_list_t *os_idsList;
    if (os_ids) {
        cJSON *os_ids_local;
        if (!cJSON_IsArray(os_ids)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [os_ids]");
            goto end;
        }
        os_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(os_ids_local, os_ids) {
            if (!cJSON_IsString(os_ids_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [os_ids]");
                goto end;
            }
            OpenAPI_list_add(os_idsList, ogs_strdup(os_ids_local->valuestring));
        }
    }

    ue_policy_set_local_var = OpenAPI_ue_policy_set_create (
        pra_infos ? pra_infosList : NULL,
        subsc_cats ? subsc_catsList : NULL,
        ue_policy_sections ? ue_policy_sectionsList : NULL,
        upsis ? upsisList : NULL,
        allowed_route_sel_descs ? allowed_route_sel_descsList : NULL,
        andsp_ind ? andsp_ind->valueint : 0,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        os_ids ? os_idsList : NULL
        );

    return ue_policy_set_local_var;
end:
    return NULL;
}

OpenAPI_ue_policy_set_t *OpenAPI_ue_policy_set_copy(OpenAPI_ue_policy_set_t *dst, OpenAPI_ue_policy_set_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_policy_set_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed");
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

    OpenAPI_ue_policy_set_free(dst);
    dst = OpenAPI_ue_policy_set_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

