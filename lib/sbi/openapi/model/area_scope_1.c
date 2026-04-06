
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "area_scope_1.h"

OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_create(
    OpenAPI_list_t *eutra_cell_id_list,
    OpenAPI_list_t *nr_cell_id_list,
    OpenAPI_list_t *tac_list,
    OpenAPI_list_t* tac_info_per_plmn,
    OpenAPI_list_t* cag_info_per_plmn,
    OpenAPI_list_t* nid_info_per_plmn,
    OpenAPI_list_t* cell_id_nid_info_per_plmn,
    OpenAPI_list_t* tac_nid_info_per_plmn,
    OpenAPI_list_t *cag_list
)
{
    OpenAPI_area_scope_1_t *area_scope_1_local_var = ogs_malloc(sizeof(OpenAPI_area_scope_1_t));
    ogs_assert(area_scope_1_local_var);

    area_scope_1_local_var->eutra_cell_id_list = eutra_cell_id_list;
    area_scope_1_local_var->nr_cell_id_list = nr_cell_id_list;
    area_scope_1_local_var->tac_list = tac_list;
    area_scope_1_local_var->tac_info_per_plmn = tac_info_per_plmn;
    area_scope_1_local_var->cag_info_per_plmn = cag_info_per_plmn;
    area_scope_1_local_var->nid_info_per_plmn = nid_info_per_plmn;
    area_scope_1_local_var->cell_id_nid_info_per_plmn = cell_id_nid_info_per_plmn;
    area_scope_1_local_var->tac_nid_info_per_plmn = tac_nid_info_per_plmn;
    area_scope_1_local_var->cag_list = cag_list;

    return area_scope_1_local_var;
}

void OpenAPI_area_scope_1_free(OpenAPI_area_scope_1_t *area_scope_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == area_scope_1) {
        return;
    }
    if (area_scope_1->eutra_cell_id_list) {
        OpenAPI_list_for_each(area_scope_1->eutra_cell_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_scope_1->eutra_cell_id_list);
        area_scope_1->eutra_cell_id_list = NULL;
    }
    if (area_scope_1->nr_cell_id_list) {
        OpenAPI_list_for_each(area_scope_1->nr_cell_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_scope_1->nr_cell_id_list);
        area_scope_1->nr_cell_id_list = NULL;
    }
    if (area_scope_1->tac_list) {
        OpenAPI_list_for_each(area_scope_1->tac_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_scope_1->tac_list);
        area_scope_1->tac_list = NULL;
    }
    if (area_scope_1->tac_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->tac_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tac_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(area_scope_1->tac_info_per_plmn);
        area_scope_1->tac_info_per_plmn = NULL;
    }
    if (area_scope_1->cag_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->cag_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cag_info_2_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(area_scope_1->cag_info_per_plmn);
        area_scope_1->cag_info_per_plmn = NULL;
    }
    if (area_scope_1->nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(area_scope_1->nid_info_per_plmn);
        area_scope_1->nid_info_per_plmn = NULL;
    }
    if (area_scope_1->cell_id_nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->cell_id_nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cell_id_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(area_scope_1->cell_id_nid_info_per_plmn);
        area_scope_1->cell_id_nid_info_per_plmn = NULL;
    }
    if (area_scope_1->tac_nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->tac_nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tac_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(area_scope_1->tac_nid_info_per_plmn);
        area_scope_1->tac_nid_info_per_plmn = NULL;
    }
    if (area_scope_1->cag_list) {
        OpenAPI_list_for_each(area_scope_1->cag_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(area_scope_1->cag_list);
        area_scope_1->cag_list = NULL;
    }
    ogs_free(area_scope_1);
}

cJSON *OpenAPI_area_scope_1_convertToJSON(OpenAPI_area_scope_1_t *area_scope_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (area_scope_1 == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [AreaScope_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (area_scope_1->eutra_cell_id_list) {
    cJSON *eutra_cell_id_listList = cJSON_AddArrayToObject(item, "eutraCellIdList");
    if (eutra_cell_id_listList == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [eutra_cell_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(area_scope_1->eutra_cell_id_list, node) {
        if (cJSON_AddStringToObject(eutra_cell_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [eutra_cell_id_list]");
            goto end;
        }
    }
    }

    if (area_scope_1->nr_cell_id_list) {
    cJSON *nr_cell_id_listList = cJSON_AddArrayToObject(item, "nrCellIdList");
    if (nr_cell_id_listList == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [nr_cell_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(area_scope_1->nr_cell_id_list, node) {
        if (cJSON_AddStringToObject(nr_cell_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [nr_cell_id_list]");
            goto end;
        }
    }
    }

    if (area_scope_1->tac_list) {
    cJSON *tac_listList = cJSON_AddArrayToObject(item, "tacList");
    if (tac_listList == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_list]");
        goto end;
    }
    OpenAPI_list_for_each(area_scope_1->tac_list, node) {
        if (cJSON_AddStringToObject(tac_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_list]");
            goto end;
        }
    }
    }

    if (area_scope_1->tac_info_per_plmn) {
    cJSON *tac_info_per_plmn = cJSON_AddObjectToObject(item, "tacInfoPerPlmn");
    if (tac_info_per_plmn == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_info_per_plmn]");
        goto end;
    }
    cJSON *localMapObject = tac_info_per_plmn;
    if (area_scope_1->tac_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->tac_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_info_per_plmn]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_info_per_plmn]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_tac_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (area_scope_1->cag_info_per_plmn) {
    cJSON *cag_info_per_plmn = cJSON_AddObjectToObject(item, "cagInfoPerPlmn");
    if (cag_info_per_plmn == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cag_info_per_plmn]");
        goto end;
    }
    cJSON *localMapObject = cag_info_per_plmn;
    if (area_scope_1->cag_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->cag_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cag_info_per_plmn]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cag_info_per_plmn]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_cag_info_2_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (area_scope_1->nid_info_per_plmn) {
    cJSON *nid_info_per_plmn = cJSON_AddObjectToObject(item, "nidInfoPerPlmn");
    if (nid_info_per_plmn == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [nid_info_per_plmn]");
        goto end;
    }
    cJSON *localMapObject = nid_info_per_plmn;
    if (area_scope_1->nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [nid_info_per_plmn]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [nid_info_per_plmn]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_nid_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (area_scope_1->cell_id_nid_info_per_plmn) {
    cJSON *cell_id_nid_info_per_plmn = cJSON_AddObjectToObject(item, "cellIdNidInfoPerPlmn");
    if (cell_id_nid_info_per_plmn == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cell_id_nid_info_per_plmn]");
        goto end;
    }
    cJSON *localMapObject = cell_id_nid_info_per_plmn;
    if (area_scope_1->cell_id_nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->cell_id_nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cell_id_nid_info_per_plmn]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cell_id_nid_info_per_plmn]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_cell_id_nid_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (area_scope_1->tac_nid_info_per_plmn) {
    cJSON *tac_nid_info_per_plmn = cJSON_AddObjectToObject(item, "tacNidInfoPerPlmn");
    if (tac_nid_info_per_plmn == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_nid_info_per_plmn]");
        goto end;
    }
    cJSON *localMapObject = tac_nid_info_per_plmn;
    if (area_scope_1->tac_nid_info_per_plmn) {
        OpenAPI_list_for_each(area_scope_1->tac_nid_info_per_plmn, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_nid_info_per_plmn]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [tac_nid_info_per_plmn]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_tac_nid_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (area_scope_1->cag_list) {
    cJSON *cag_listList = cJSON_AddArrayToObject(item, "cagList");
    if (cag_listList == NULL) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cag_list]");
        goto end;
    }
    OpenAPI_list_for_each(area_scope_1->cag_list, node) {
        if (cJSON_AddStringToObject(cag_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_area_scope_1_convertToJSON() failed [cag_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_parseFromJSON(cJSON *area_scope_1JSON)
{
    OpenAPI_area_scope_1_t *area_scope_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *eutra_cell_id_list = NULL;
    OpenAPI_list_t *eutra_cell_id_listList = NULL;
    cJSON *nr_cell_id_list = NULL;
    OpenAPI_list_t *nr_cell_id_listList = NULL;
    cJSON *tac_list = NULL;
    OpenAPI_list_t *tac_listList = NULL;
    cJSON *tac_info_per_plmn = NULL;
    OpenAPI_list_t *tac_info_per_plmnList = NULL;
    cJSON *cag_info_per_plmn = NULL;
    OpenAPI_list_t *cag_info_per_plmnList = NULL;
    cJSON *nid_info_per_plmn = NULL;
    OpenAPI_list_t *nid_info_per_plmnList = NULL;
    cJSON *cell_id_nid_info_per_plmn = NULL;
    OpenAPI_list_t *cell_id_nid_info_per_plmnList = NULL;
    cJSON *tac_nid_info_per_plmn = NULL;
    OpenAPI_list_t *tac_nid_info_per_plmnList = NULL;
    cJSON *cag_list = NULL;
    OpenAPI_list_t *cag_listList = NULL;
    eutra_cell_id_list = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "eutraCellIdList");
    if (eutra_cell_id_list) {
        cJSON *eutra_cell_id_list_local = NULL;
        if (!cJSON_IsArray(eutra_cell_id_list)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [eutra_cell_id_list]");
            goto end;
        }

        eutra_cell_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(eutra_cell_id_list_local, eutra_cell_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(eutra_cell_id_list_local)) {
                ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [eutra_cell_id_list]");
                goto end;
            }
            OpenAPI_list_add(eutra_cell_id_listList, ogs_strdup(eutra_cell_id_list_local->valuestring));
        }
    }

    nr_cell_id_list = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "nrCellIdList");
    if (nr_cell_id_list) {
        cJSON *nr_cell_id_list_local = NULL;
        if (!cJSON_IsArray(nr_cell_id_list)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [nr_cell_id_list]");
            goto end;
        }

        nr_cell_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nr_cell_id_list_local, nr_cell_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nr_cell_id_list_local)) {
                ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [nr_cell_id_list]");
                goto end;
            }
            OpenAPI_list_add(nr_cell_id_listList, ogs_strdup(nr_cell_id_list_local->valuestring));
        }
    }

    tac_list = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "tacList");
    if (tac_list) {
        cJSON *tac_list_local = NULL;
        if (!cJSON_IsArray(tac_list)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [tac_list]");
            goto end;
        }

        tac_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_list_local, tac_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tac_list_local)) {
                ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [tac_list]");
                goto end;
            }
            OpenAPI_list_add(tac_listList, ogs_strdup(tac_list_local->valuestring));
        }
    }

    tac_info_per_plmn = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "tacInfoPerPlmn");
    if (tac_info_per_plmn) {
        cJSON *tac_info_per_plmn_local_map = NULL;
        if (!cJSON_IsObject(tac_info_per_plmn) && !cJSON_IsNull(tac_info_per_plmn)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [tac_info_per_plmn]");
            goto end;
        }
        if (cJSON_IsObject(tac_info_per_plmn)) {
            tac_info_per_plmnList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(tac_info_per_plmn_local_map, tac_info_per_plmn) {
                cJSON *localMapObject = tac_info_per_plmn_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_tac_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(tac_info_per_plmnList, localMapKeyPair);
            }
        }
    }

    cag_info_per_plmn = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "cagInfoPerPlmn");
    if (cag_info_per_plmn) {
        cJSON *cag_info_per_plmn_local_map = NULL;
        if (!cJSON_IsObject(cag_info_per_plmn) && !cJSON_IsNull(cag_info_per_plmn)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [cag_info_per_plmn]");
            goto end;
        }
        if (cJSON_IsObject(cag_info_per_plmn)) {
            cag_info_per_plmnList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(cag_info_per_plmn_local_map, cag_info_per_plmn) {
                cJSON *localMapObject = cag_info_per_plmn_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_cag_info_2_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(cag_info_per_plmnList, localMapKeyPair);
            }
        }
    }

    nid_info_per_plmn = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "nidInfoPerPlmn");
    if (nid_info_per_plmn) {
        cJSON *nid_info_per_plmn_local_map = NULL;
        if (!cJSON_IsObject(nid_info_per_plmn) && !cJSON_IsNull(nid_info_per_plmn)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [nid_info_per_plmn]");
            goto end;
        }
        if (cJSON_IsObject(nid_info_per_plmn)) {
            nid_info_per_plmnList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(nid_info_per_plmn_local_map, nid_info_per_plmn) {
                cJSON *localMapObject = nid_info_per_plmn_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_nid_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(nid_info_per_plmnList, localMapKeyPair);
            }
        }
    }

    cell_id_nid_info_per_plmn = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "cellIdNidInfoPerPlmn");
    if (cell_id_nid_info_per_plmn) {
        cJSON *cell_id_nid_info_per_plmn_local_map = NULL;
        if (!cJSON_IsObject(cell_id_nid_info_per_plmn) && !cJSON_IsNull(cell_id_nid_info_per_plmn)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [cell_id_nid_info_per_plmn]");
            goto end;
        }
        if (cJSON_IsObject(cell_id_nid_info_per_plmn)) {
            cell_id_nid_info_per_plmnList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(cell_id_nid_info_per_plmn_local_map, cell_id_nid_info_per_plmn) {
                cJSON *localMapObject = cell_id_nid_info_per_plmn_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_cell_id_nid_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(cell_id_nid_info_per_plmnList, localMapKeyPair);
            }
        }
    }

    tac_nid_info_per_plmn = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "tacNidInfoPerPlmn");
    if (tac_nid_info_per_plmn) {
        cJSON *tac_nid_info_per_plmn_local_map = NULL;
        if (!cJSON_IsObject(tac_nid_info_per_plmn) && !cJSON_IsNull(tac_nid_info_per_plmn)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [tac_nid_info_per_plmn]");
            goto end;
        }
        if (cJSON_IsObject(tac_nid_info_per_plmn)) {
            tac_nid_info_per_plmnList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(tac_nid_info_per_plmn_local_map, tac_nid_info_per_plmn) {
                cJSON *localMapObject = tac_nid_info_per_plmn_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_tac_nid_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(tac_nid_info_per_plmnList, localMapKeyPair);
            }
        }
    }

    cag_list = cJSON_GetObjectItemCaseSensitive(area_scope_1JSON, "cagList");
    if (cag_list) {
        cJSON *cag_list_local = NULL;
        if (!cJSON_IsArray(cag_list)) {
            ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [cag_list]");
            goto end;
        }

        cag_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(cag_list_local, cag_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(cag_list_local)) {
                ogs_error("OpenAPI_area_scope_1_parseFromJSON() failed [cag_list]");
                goto end;
            }
            OpenAPI_list_add(cag_listList, ogs_strdup(cag_list_local->valuestring));
        }
    }

    area_scope_1_local_var = OpenAPI_area_scope_1_create (
        eutra_cell_id_list ? eutra_cell_id_listList : NULL,
        nr_cell_id_list ? nr_cell_id_listList : NULL,
        tac_list ? tac_listList : NULL,
        tac_info_per_plmn ? tac_info_per_plmnList : NULL,
        cag_info_per_plmn ? cag_info_per_plmnList : NULL,
        nid_info_per_plmn ? nid_info_per_plmnList : NULL,
        cell_id_nid_info_per_plmn ? cell_id_nid_info_per_plmnList : NULL,
        tac_nid_info_per_plmn ? tac_nid_info_per_plmnList : NULL,
        cag_list ? cag_listList : NULL
    );

    return area_scope_1_local_var;
end:
    if (eutra_cell_id_listList) {
        OpenAPI_list_for_each(eutra_cell_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(eutra_cell_id_listList);
        eutra_cell_id_listList = NULL;
    }
    if (nr_cell_id_listList) {
        OpenAPI_list_for_each(nr_cell_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nr_cell_id_listList);
        nr_cell_id_listList = NULL;
    }
    if (tac_listList) {
        OpenAPI_list_for_each(tac_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(tac_listList);
        tac_listList = NULL;
    }
    if (tac_info_per_plmnList) {
        OpenAPI_list_for_each(tac_info_per_plmnList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tac_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(tac_info_per_plmnList);
        tac_info_per_plmnList = NULL;
    }
    if (cag_info_per_plmnList) {
        OpenAPI_list_for_each(cag_info_per_plmnList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cag_info_2_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cag_info_per_plmnList);
        cag_info_per_plmnList = NULL;
    }
    if (nid_info_per_plmnList) {
        OpenAPI_list_for_each(nid_info_per_plmnList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(nid_info_per_plmnList);
        nid_info_per_plmnList = NULL;
    }
    if (cell_id_nid_info_per_plmnList) {
        OpenAPI_list_for_each(cell_id_nid_info_per_plmnList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_cell_id_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(cell_id_nid_info_per_plmnList);
        cell_id_nid_info_per_plmnList = NULL;
    }
    if (tac_nid_info_per_plmnList) {
        OpenAPI_list_for_each(tac_nid_info_per_plmnList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_tac_nid_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(tac_nid_info_per_plmnList);
        tac_nid_info_per_plmnList = NULL;
    }
    if (cag_listList) {
        OpenAPI_list_for_each(cag_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cag_listList);
        cag_listList = NULL;
    }
    return NULL;
}

OpenAPI_area_scope_1_t *OpenAPI_area_scope_1_copy(OpenAPI_area_scope_1_t *dst, OpenAPI_area_scope_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_area_scope_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_area_scope_1_convertToJSON() failed");
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

    OpenAPI_area_scope_1_free(dst);
    dst = OpenAPI_area_scope_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

