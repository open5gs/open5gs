
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qmc_area_scope.h"

OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_create(
    OpenAPI_list_t *nr_cell_id_list,
    OpenAPI_list_t *tac_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *plmn_list
)
{
    OpenAPI_qmc_area_scope_t *qmc_area_scope_local_var = ogs_malloc(sizeof(OpenAPI_qmc_area_scope_t));
    ogs_assert(qmc_area_scope_local_var);

    qmc_area_scope_local_var->nr_cell_id_list = nr_cell_id_list;
    qmc_area_scope_local_var->tac_list = tac_list;
    qmc_area_scope_local_var->tai_list = tai_list;
    qmc_area_scope_local_var->plmn_list = plmn_list;

    return qmc_area_scope_local_var;
}

void OpenAPI_qmc_area_scope_free(OpenAPI_qmc_area_scope_t *qmc_area_scope)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qmc_area_scope) {
        return;
    }
    if (qmc_area_scope->nr_cell_id_list) {
        OpenAPI_list_for_each(qmc_area_scope->nr_cell_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qmc_area_scope->nr_cell_id_list);
        qmc_area_scope->nr_cell_id_list = NULL;
    }
    if (qmc_area_scope->tac_list) {
        OpenAPI_list_for_each(qmc_area_scope->tac_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qmc_area_scope->tac_list);
        qmc_area_scope->tac_list = NULL;
    }
    if (qmc_area_scope->tai_list) {
        OpenAPI_list_for_each(qmc_area_scope->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(qmc_area_scope->tai_list);
        qmc_area_scope->tai_list = NULL;
    }
    if (qmc_area_scope->plmn_list) {
        OpenAPI_list_for_each(qmc_area_scope->plmn_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(qmc_area_scope->plmn_list);
        qmc_area_scope->plmn_list = NULL;
    }
    ogs_free(qmc_area_scope);
}

cJSON *OpenAPI_qmc_area_scope_convertToJSON(OpenAPI_qmc_area_scope_t *qmc_area_scope)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qmc_area_scope == NULL) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [QmcAreaScope]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qmc_area_scope->nr_cell_id_list) {
    cJSON *nr_cell_id_listList = cJSON_AddArrayToObject(item, "nrCellIdList");
    if (nr_cell_id_listList == NULL) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [nr_cell_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_area_scope->nr_cell_id_list, node) {
        if (cJSON_AddStringToObject(nr_cell_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [nr_cell_id_list]");
            goto end;
        }
    }
    }

    if (qmc_area_scope->tac_list) {
    cJSON *tac_listList = cJSON_AddArrayToObject(item, "tacList");
    if (tac_listList == NULL) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [tac_list]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_area_scope->tac_list, node) {
        if (cJSON_AddStringToObject(tac_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [tac_list]");
            goto end;
        }
    }
    }

    if (qmc_area_scope->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_area_scope->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (qmc_area_scope->plmn_list) {
    cJSON *plmn_listList = cJSON_AddArrayToObject(item, "plmnList");
    if (plmn_listList == NULL) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [plmn_list]");
        goto end;
    }
    OpenAPI_list_for_each(qmc_area_scope->plmn_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed [plmn_list]");
            goto end;
        }
        cJSON_AddItemToArray(plmn_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_parseFromJSON(cJSON *qmc_area_scopeJSON)
{
    OpenAPI_qmc_area_scope_t *qmc_area_scope_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nr_cell_id_list = NULL;
    OpenAPI_list_t *nr_cell_id_listList = NULL;
    cJSON *tac_list = NULL;
    OpenAPI_list_t *tac_listList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *plmn_list = NULL;
    OpenAPI_list_t *plmn_listList = NULL;
    nr_cell_id_list = cJSON_GetObjectItemCaseSensitive(qmc_area_scopeJSON, "nrCellIdList");
    if (nr_cell_id_list) {
        cJSON *nr_cell_id_list_local = NULL;
        if (!cJSON_IsArray(nr_cell_id_list)) {
            ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [nr_cell_id_list]");
            goto end;
        }

        nr_cell_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(nr_cell_id_list_local, nr_cell_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(nr_cell_id_list_local)) {
                ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [nr_cell_id_list]");
                goto end;
            }
            OpenAPI_list_add(nr_cell_id_listList, ogs_strdup(nr_cell_id_list_local->valuestring));
        }
    }

    tac_list = cJSON_GetObjectItemCaseSensitive(qmc_area_scopeJSON, "tacList");
    if (tac_list) {
        cJSON *tac_list_local = NULL;
        if (!cJSON_IsArray(tac_list)) {
            ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [tac_list]");
            goto end;
        }

        tac_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tac_list_local, tac_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(tac_list_local)) {
                ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [tac_list]");
                goto end;
            }
            OpenAPI_list_add(tac_listList, ogs_strdup(tac_list_local->valuestring));
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(qmc_area_scopeJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    plmn_list = cJSON_GetObjectItemCaseSensitive(qmc_area_scopeJSON, "plmnList");
    if (plmn_list) {
        cJSON *plmn_list_local = NULL;
        if (!cJSON_IsArray(plmn_list)) {
            ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [plmn_list]");
            goto end;
        }

        plmn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(plmn_list_local, plmn_list) {
            if (!cJSON_IsObject(plmn_list_local)) {
                ogs_error("OpenAPI_qmc_area_scope_parseFromJSON() failed [plmn_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *plmn_listItem = OpenAPI_plmn_id_parseFromJSON(plmn_list_local);
            if (!plmn_listItem) {
                ogs_error("No plmn_listItem");
                goto end;
            }
            OpenAPI_list_add(plmn_listList, plmn_listItem);
        }
    }

    qmc_area_scope_local_var = OpenAPI_qmc_area_scope_create (
        nr_cell_id_list ? nr_cell_id_listList : NULL,
        tac_list ? tac_listList : NULL,
        tai_list ? tai_listList : NULL,
        plmn_list ? plmn_listList : NULL
    );

    return qmc_area_scope_local_var;
end:
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
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (plmn_listList) {
        OpenAPI_list_for_each(plmn_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(plmn_listList);
        plmn_listList = NULL;
    }
    return NULL;
}

OpenAPI_qmc_area_scope_t *OpenAPI_qmc_area_scope_copy(OpenAPI_qmc_area_scope_t *dst, OpenAPI_qmc_area_scope_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qmc_area_scope_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qmc_area_scope_convertToJSON() failed");
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

    OpenAPI_qmc_area_scope_free(dst);
    dst = OpenAPI_qmc_area_scope_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

