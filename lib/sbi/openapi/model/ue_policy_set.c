
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
    bool is_andsp_ind,
    int andsp_ind,
    char *pei,
    OpenAPI_list_t *os_ids,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_ue_policy_set_t *ue_policy_set_local_var = ogs_malloc(sizeof(OpenAPI_ue_policy_set_t));
    ogs_assert(ue_policy_set_local_var);

    ue_policy_set_local_var->pra_infos = pra_infos;
    ue_policy_set_local_var->subsc_cats = subsc_cats;
    ue_policy_set_local_var->ue_policy_sections = ue_policy_sections;
    ue_policy_set_local_var->upsis = upsis;
    ue_policy_set_local_var->allowed_route_sel_descs = allowed_route_sel_descs;
    ue_policy_set_local_var->is_andsp_ind = is_andsp_ind;
    ue_policy_set_local_var->andsp_ind = andsp_ind;
    ue_policy_set_local_var->pei = pei;
    ue_policy_set_local_var->os_ids = os_ids;
    ue_policy_set_local_var->supp_feat = supp_feat;
    ue_policy_set_local_var->reset_ids = reset_ids;

    return ue_policy_set_local_var;
}

void OpenAPI_ue_policy_set_free(OpenAPI_ue_policy_set_t *ue_policy_set)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_policy_set) {
        return;
    }
    if (ue_policy_set->pra_infos) {
        OpenAPI_list_for_each(ue_policy_set->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_set->pra_infos);
        ue_policy_set->pra_infos = NULL;
    }
    if (ue_policy_set->subsc_cats) {
        OpenAPI_list_for_each(ue_policy_set->subsc_cats, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set->subsc_cats);
        ue_policy_set->subsc_cats = NULL;
    }
    if (ue_policy_set->ue_policy_sections) {
        OpenAPI_list_for_each(ue_policy_set->ue_policy_sections, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_ue_policy_section_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_set->ue_policy_sections);
        ue_policy_set->ue_policy_sections = NULL;
    }
    if (ue_policy_set->upsis) {
        OpenAPI_list_for_each(ue_policy_set->upsis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set->upsis);
        ue_policy_set->upsis = NULL;
    }
    if (ue_policy_set->allowed_route_sel_descs) {
        OpenAPI_list_for_each(ue_policy_set->allowed_route_sel_descs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_plmn_route_selection_descriptor_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ue_policy_set->allowed_route_sel_descs);
        ue_policy_set->allowed_route_sel_descs = NULL;
    }
    if (ue_policy_set->pei) {
        ogs_free(ue_policy_set->pei);
        ue_policy_set->pei = NULL;
    }
    if (ue_policy_set->os_ids) {
        OpenAPI_list_for_each(ue_policy_set->os_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set->os_ids);
        ue_policy_set->os_ids = NULL;
    }
    if (ue_policy_set->supp_feat) {
        ogs_free(ue_policy_set->supp_feat);
        ue_policy_set->supp_feat = NULL;
    }
    if (ue_policy_set->reset_ids) {
        OpenAPI_list_for_each(ue_policy_set->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_policy_set->reset_ids);
        ue_policy_set->reset_ids = NULL;
    }
    ogs_free(ue_policy_set);
}

cJSON *OpenAPI_ue_policy_set_convertToJSON(OpenAPI_ue_policy_set_t *ue_policy_set)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    if (ue_policy_set->pra_infos) {
        OpenAPI_list_for_each(ue_policy_set->pra_infos, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [pra_infos]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [pra_infos]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_policy_set->subsc_cats) {
    cJSON *subsc_catsList = cJSON_AddArrayToObject(item, "subscCats");
    if (subsc_catsList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [subsc_cats]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set->subsc_cats, node) {
        if (cJSON_AddStringToObject(subsc_catsList, "", (char*)node->data) == NULL) {
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
    if (ue_policy_set->ue_policy_sections) {
        OpenAPI_list_for_each(ue_policy_set->ue_policy_sections, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [ue_policy_sections]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [ue_policy_sections]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_ue_policy_section_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_policy_set->upsis) {
    cJSON *upsisList = cJSON_AddArrayToObject(item, "upsis");
    if (upsisList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [upsis]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set->upsis, node) {
        if (cJSON_AddStringToObject(upsisList, "", (char*)node->data) == NULL) {
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
    if (ue_policy_set->allowed_route_sel_descs) {
        OpenAPI_list_for_each(ue_policy_set->allowed_route_sel_descs, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [allowed_route_sel_descs]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [allowed_route_sel_descs]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_plmn_route_selection_descriptor_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (ue_policy_set->is_andsp_ind) {
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
    cJSON *os_idsList = cJSON_AddArrayToObject(item, "osIds");
    if (os_idsList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [os_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set->os_ids, node) {
        if (cJSON_AddStringToObject(os_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [os_ids]");
            goto end;
        }
    }
    }

    if (ue_policy_set->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", ue_policy_set->supp_feat) == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (ue_policy_set->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(ue_policy_set->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_policy_set_convertToJSON() failed [reset_ids]");
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *pra_infos = NULL;
    OpenAPI_list_t *pra_infosList = NULL;
    cJSON *subsc_cats = NULL;
    OpenAPI_list_t *subsc_catsList = NULL;
    cJSON *ue_policy_sections = NULL;
    OpenAPI_list_t *ue_policy_sectionsList = NULL;
    cJSON *upsis = NULL;
    OpenAPI_list_t *upsisList = NULL;
    cJSON *allowed_route_sel_descs = NULL;
    OpenAPI_list_t *allowed_route_sel_descsList = NULL;
    cJSON *andsp_ind = NULL;
    cJSON *pei = NULL;
    cJSON *os_ids = NULL;
    OpenAPI_list_t *os_idsList = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    pra_infos = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "praInfos");
    if (pra_infos) {
        cJSON *pra_infos_local_map = NULL;
        if (!cJSON_IsObject(pra_infos) && !cJSON_IsNull(pra_infos)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [pra_infos]");
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
                    ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_infosList, localMapKeyPair);
            }
        }
    }

    subsc_cats = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "subscCats");
    if (subsc_cats) {
        cJSON *subsc_cats_local = NULL;
        if (!cJSON_IsArray(subsc_cats)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [subsc_cats]");
            goto end;
        }

        subsc_catsList = OpenAPI_list_create();

        cJSON_ArrayForEach(subsc_cats_local, subsc_cats) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(subsc_cats_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [subsc_cats]");
                goto end;
            }
            OpenAPI_list_add(subsc_catsList, ogs_strdup(subsc_cats_local->valuestring));
        }
    }

    ue_policy_sections = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "uePolicySections");
    if (ue_policy_sections) {
        cJSON *ue_policy_sections_local_map = NULL;
        if (!cJSON_IsObject(ue_policy_sections) && !cJSON_IsNull(ue_policy_sections)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [ue_policy_sections]");
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
                    ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(ue_policy_sectionsList, localMapKeyPair);
            }
        }
    }

    upsis = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "upsis");
    if (upsis) {
        cJSON *upsis_local = NULL;
        if (!cJSON_IsArray(upsis)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [upsis]");
            goto end;
        }

        upsisList = OpenAPI_list_create();

        cJSON_ArrayForEach(upsis_local, upsis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(upsis_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [upsis]");
                goto end;
            }
            OpenAPI_list_add(upsisList, ogs_strdup(upsis_local->valuestring));
        }
    }

    allowed_route_sel_descs = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "allowedRouteSelDescs");
    if (allowed_route_sel_descs) {
        cJSON *allowed_route_sel_descs_local_map = NULL;
        if (!cJSON_IsObject(allowed_route_sel_descs) && !cJSON_IsNull(allowed_route_sel_descs)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [allowed_route_sel_descs]");
            goto end;
        }
        if (cJSON_IsObject(allowed_route_sel_descs)) {
            allowed_route_sel_descsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(allowed_route_sel_descs_local_map, allowed_route_sel_descs) {
                cJSON *localMapObject = allowed_route_sel_descs_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_plmn_route_selection_descriptor_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(allowed_route_sel_descsList, localMapKeyPair);
            }
        }
    }

    andsp_ind = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "andspInd");
    if (andsp_ind) {
    if (!cJSON_IsBool(andsp_ind)) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [andsp_ind]");
        goto end;
    }
    }

    pei = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    os_ids = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "osIds");
    if (os_ids) {
        cJSON *os_ids_local = NULL;
        if (!cJSON_IsArray(os_ids)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [os_ids]");
            goto end;
        }

        os_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(os_ids_local, os_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(os_ids_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [os_ids]");
                goto end;
            }
            OpenAPI_list_add(os_idsList, ogs_strdup(os_ids_local->valuestring));
        }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(ue_policy_setJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_ue_policy_set_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    ue_policy_set_local_var = OpenAPI_ue_policy_set_create (
        pra_infos ? pra_infosList : NULL,
        subsc_cats ? subsc_catsList : NULL,
        ue_policy_sections ? ue_policy_sectionsList : NULL,
        upsis ? upsisList : NULL,
        allowed_route_sel_descs ? allowed_route_sel_descsList : NULL,
        andsp_ind ? true : false,
        andsp_ind ? andsp_ind->valueint : 0,
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        os_ids ? os_idsList : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return ue_policy_set_local_var;
end:
    if (pra_infosList) {
        OpenAPI_list_for_each(pra_infosList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
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
    if (allowed_route_sel_descsList) {
        OpenAPI_list_for_each(allowed_route_sel_descsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_plmn_route_selection_descriptor_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(allowed_route_sel_descsList);
        allowed_route_sel_descsList = NULL;
    }
    if (os_idsList) {
        OpenAPI_list_for_each(os_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(os_idsList);
        os_idsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

