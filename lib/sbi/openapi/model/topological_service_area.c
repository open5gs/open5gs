
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "topological_service_area.h"

OpenAPI_topological_service_area_t *OpenAPI_topological_service_area_create(
    OpenAPI_list_t *cell_id_list,
    OpenAPI_list_t *tracking_area_id_list,
    OpenAPI_charging_plmn_id_t *serving_plmn
)
{
    OpenAPI_topological_service_area_t *topological_service_area_local_var = ogs_malloc(sizeof(OpenAPI_topological_service_area_t));
    ogs_assert(topological_service_area_local_var);

    topological_service_area_local_var->cell_id_list = cell_id_list;
    topological_service_area_local_var->tracking_area_id_list = tracking_area_id_list;
    topological_service_area_local_var->serving_plmn = serving_plmn;

    return topological_service_area_local_var;
}

void OpenAPI_topological_service_area_free(OpenAPI_topological_service_area_t *topological_service_area)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == topological_service_area) {
        return;
    }
    if (topological_service_area->cell_id_list) {
        OpenAPI_list_for_each(topological_service_area->cell_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(topological_service_area->cell_id_list);
        topological_service_area->cell_id_list = NULL;
    }
    if (topological_service_area->tracking_area_id_list) {
        OpenAPI_list_for_each(topological_service_area->tracking_area_id_list, node) {
            OpenAPI_charging_tai_free(node->data);
        }
        OpenAPI_list_free(topological_service_area->tracking_area_id_list);
        topological_service_area->tracking_area_id_list = NULL;
    }
    if (topological_service_area->serving_plmn) {
        OpenAPI_charging_plmn_id_free(topological_service_area->serving_plmn);
        topological_service_area->serving_plmn = NULL;
    }
    ogs_free(topological_service_area);
}

cJSON *OpenAPI_topological_service_area_convertToJSON(OpenAPI_topological_service_area_t *topological_service_area)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (topological_service_area == NULL) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [TopologicalServiceArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (topological_service_area->cell_id_list) {
    cJSON *cell_id_listList = cJSON_AddArrayToObject(item, "cellIdList");
    if (cell_id_listList == NULL) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [cell_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(topological_service_area->cell_id_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [cell_id_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(cell_id_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [cell_id_list]");
            goto end;
        }
    }
    }

    if (topological_service_area->tracking_area_id_list) {
    cJSON *tracking_area_id_listList = cJSON_AddArrayToObject(item, "trackingAreaIdList");
    if (tracking_area_id_listList == NULL) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [tracking_area_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(topological_service_area->tracking_area_id_list, node) {
        cJSON *itemLocal = OpenAPI_charging_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [tracking_area_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(tracking_area_id_listList, itemLocal);
    }
    }

    if (topological_service_area->serving_plmn) {
    cJSON *serving_plmn_local_JSON = OpenAPI_charging_plmn_id_convertToJSON(topological_service_area->serving_plmn);
    if (serving_plmn_local_JSON == NULL) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [serving_plmn]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servingPLMN", serving_plmn_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed [serving_plmn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_topological_service_area_t *OpenAPI_topological_service_area_parseFromJSON(cJSON *topological_service_areaJSON)
{
    OpenAPI_topological_service_area_t *topological_service_area_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *cell_id_list = NULL;
    OpenAPI_list_t *cell_id_listList = NULL;
    cJSON *tracking_area_id_list = NULL;
    OpenAPI_list_t *tracking_area_id_listList = NULL;
    cJSON *serving_plmn = NULL;
    OpenAPI_charging_plmn_id_t *serving_plmn_local_nonprim = NULL;
    cell_id_list = cJSON_GetObjectItemCaseSensitive(topological_service_areaJSON, "cellIdList");
    if (cell_id_list) {
        cJSON *cell_id_list_local = NULL;
        if (!cJSON_IsArray(cell_id_list)) {
            ogs_error("OpenAPI_topological_service_area_parseFromJSON() failed [cell_id_list]");
            goto end;
        }

        cell_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(cell_id_list_local, cell_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(cell_id_list_local)) {
                ogs_error("OpenAPI_topological_service_area_parseFromJSON() failed [cell_id_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_topological_service_area_parseFromJSON() failed [cell_id_list]");
                goto end;
            }
            *localDouble = cell_id_list_local->valuedouble;
            OpenAPI_list_add(cell_id_listList, localDouble);
        }
    }

    tracking_area_id_list = cJSON_GetObjectItemCaseSensitive(topological_service_areaJSON, "trackingAreaIdList");
    if (tracking_area_id_list) {
        cJSON *tracking_area_id_list_local = NULL;
        if (!cJSON_IsArray(tracking_area_id_list)) {
            ogs_error("OpenAPI_topological_service_area_parseFromJSON() failed [tracking_area_id_list]");
            goto end;
        }

        tracking_area_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tracking_area_id_list_local, tracking_area_id_list) {
            if (!cJSON_IsObject(tracking_area_id_list_local)) {
                ogs_error("OpenAPI_topological_service_area_parseFromJSON() failed [tracking_area_id_list]");
                goto end;
            }
            OpenAPI_charging_tai_t *tracking_area_id_listItem = OpenAPI_charging_tai_parseFromJSON(tracking_area_id_list_local);
            if (!tracking_area_id_listItem) {
                ogs_error("No tracking_area_id_listItem");
                goto end;
            }
            OpenAPI_list_add(tracking_area_id_listList, tracking_area_id_listItem);
        }
    }

    serving_plmn = cJSON_GetObjectItemCaseSensitive(topological_service_areaJSON, "servingPLMN");
    if (serving_plmn) {
    serving_plmn_local_nonprim = OpenAPI_charging_plmn_id_parseFromJSON(serving_plmn);
    if (!serving_plmn_local_nonprim) {
        ogs_error("OpenAPI_charging_plmn_id_parseFromJSON failed [serving_plmn]");
        goto end;
    }
    }

    topological_service_area_local_var = OpenAPI_topological_service_area_create (
        cell_id_list ? cell_id_listList : NULL,
        tracking_area_id_list ? tracking_area_id_listList : NULL,
        serving_plmn ? serving_plmn_local_nonprim : NULL
    );

    return topological_service_area_local_var;
end:
    if (cell_id_listList) {
        OpenAPI_list_for_each(cell_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(cell_id_listList);
        cell_id_listList = NULL;
    }
    if (tracking_area_id_listList) {
        OpenAPI_list_for_each(tracking_area_id_listList, node) {
            OpenAPI_charging_tai_free(node->data);
        }
        OpenAPI_list_free(tracking_area_id_listList);
        tracking_area_id_listList = NULL;
    }
    if (serving_plmn_local_nonprim) {
        OpenAPI_charging_plmn_id_free(serving_plmn_local_nonprim);
        serving_plmn_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_topological_service_area_t *OpenAPI_topological_service_area_copy(OpenAPI_topological_service_area_t *dst, OpenAPI_topological_service_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_topological_service_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_topological_service_area_convertToJSON() failed");
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

    OpenAPI_topological_service_area_free(dst);
    dst = OpenAPI_topological_service_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

