
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wireline_area.h"

OpenAPI_wireline_area_t *OpenAPI_wireline_area_create(
    OpenAPI_list_t *global_line_ids,
    OpenAPI_list_t *hfc_n_ids,
    char *area_code_b,
    char *area_code_c
    )
{
    OpenAPI_wireline_area_t *wireline_area_local_var = OpenAPI_malloc(sizeof(OpenAPI_wireline_area_t));
    if (!wireline_area_local_var) {
        return NULL;
    }
    wireline_area_local_var->global_line_ids = global_line_ids;
    wireline_area_local_var->hfc_n_ids = hfc_n_ids;
    wireline_area_local_var->area_code_b = area_code_b;
    wireline_area_local_var->area_code_c = area_code_c;

    return wireline_area_local_var;
}

void OpenAPI_wireline_area_free(OpenAPI_wireline_area_t *wireline_area)
{
    if (NULL == wireline_area) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(wireline_area->global_line_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(wireline_area->global_line_ids);
    OpenAPI_list_for_each(wireline_area->hfc_n_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(wireline_area->hfc_n_ids);
    ogs_free(wireline_area->area_code_b);
    ogs_free(wireline_area->area_code_c);
    ogs_free(wireline_area);
}

cJSON *OpenAPI_wireline_area_convertToJSON(OpenAPI_wireline_area_t *wireline_area)
{
    cJSON *item = NULL;

    if (wireline_area == NULL) {
        ogs_error("OpenAPI_wireline_area_convertToJSON() failed [WirelineArea]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (wireline_area->global_line_ids) {
        cJSON *global_line_ids = cJSON_AddArrayToObject(item, "globalLineIds");
        if (global_line_ids == NULL) {
            ogs_error("OpenAPI_wireline_area_convertToJSON() failed [global_line_ids]");
            goto end;
        }

        OpenAPI_lnode_t *global_line_ids_node;
        OpenAPI_list_for_each(wireline_area->global_line_ids, global_line_ids_node)  {
        }
    }

    if (wireline_area->hfc_n_ids) {
        cJSON *hfc_n_ids = cJSON_AddArrayToObject(item, "hfcNIds");
        if (hfc_n_ids == NULL) {
            ogs_error("OpenAPI_wireline_area_convertToJSON() failed [hfc_n_ids]");
            goto end;
        }

        OpenAPI_lnode_t *hfc_n_ids_node;
        OpenAPI_list_for_each(wireline_area->hfc_n_ids, hfc_n_ids_node)  {
            if (cJSON_AddStringToObject(hfc_n_ids, "", (char*)hfc_n_ids_node->data) == NULL) {
                ogs_error("OpenAPI_wireline_area_convertToJSON() failed [hfc_n_ids]");
                goto end;
            }
        }
    }

    if (wireline_area->area_code_b) {
        if (cJSON_AddStringToObject(item, "areaCodeB", wireline_area->area_code_b) == NULL) {
            ogs_error("OpenAPI_wireline_area_convertToJSON() failed [area_code_b]");
            goto end;
        }
    }

    if (wireline_area->area_code_c) {
        if (cJSON_AddStringToObject(item, "areaCodeC", wireline_area->area_code_c) == NULL) {
            ogs_error("OpenAPI_wireline_area_convertToJSON() failed [area_code_c]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_wireline_area_t *OpenAPI_wireline_area_parseFromJSON(cJSON *wireline_areaJSON)
{
    OpenAPI_wireline_area_t *wireline_area_local_var = NULL;
    cJSON *global_line_ids = cJSON_GetObjectItemCaseSensitive(wireline_areaJSON, "globalLineIds");

    OpenAPI_list_t *global_line_idsList;
    if (global_line_ids) {
        cJSON *global_line_ids_local;
        if (!cJSON_IsArray(global_line_ids)) {
            ogs_error("OpenAPI_wireline_area_parseFromJSON() failed [global_line_ids]");
            goto end;
        }
        global_line_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(global_line_ids_local, global_line_ids) {
        }
    }

    cJSON *hfc_n_ids = cJSON_GetObjectItemCaseSensitive(wireline_areaJSON, "hfcNIds");

    OpenAPI_list_t *hfc_n_idsList;
    if (hfc_n_ids) {
        cJSON *hfc_n_ids_local;
        if (!cJSON_IsArray(hfc_n_ids)) {
            ogs_error("OpenAPI_wireline_area_parseFromJSON() failed [hfc_n_ids]");
            goto end;
        }
        hfc_n_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(hfc_n_ids_local, hfc_n_ids) {
            if (!cJSON_IsString(hfc_n_ids_local)) {
                ogs_error("OpenAPI_wireline_area_parseFromJSON() failed [hfc_n_ids]");
                goto end;
            }
            OpenAPI_list_add(hfc_n_idsList, ogs_strdup(hfc_n_ids_local->valuestring));
        }
    }

    cJSON *area_code_b = cJSON_GetObjectItemCaseSensitive(wireline_areaJSON, "areaCodeB");

    if (area_code_b) {
        if (!cJSON_IsString(area_code_b)) {
            ogs_error("OpenAPI_wireline_area_parseFromJSON() failed [area_code_b]");
            goto end;
        }
    }

    cJSON *area_code_c = cJSON_GetObjectItemCaseSensitive(wireline_areaJSON, "areaCodeC");

    if (area_code_c) {
        if (!cJSON_IsString(area_code_c)) {
            ogs_error("OpenAPI_wireline_area_parseFromJSON() failed [area_code_c]");
            goto end;
        }
    }

    wireline_area_local_var = OpenAPI_wireline_area_create (
        global_line_ids ? global_line_idsList : NULL,
        hfc_n_ids ? hfc_n_idsList : NULL,
        area_code_b ? ogs_strdup(area_code_b->valuestring) : NULL,
        area_code_c ? ogs_strdup(area_code_c->valuestring) : NULL
        );

    return wireline_area_local_var;
end:
    return NULL;
}

OpenAPI_wireline_area_t *OpenAPI_wireline_area_copy(OpenAPI_wireline_area_t *dst, OpenAPI_wireline_area_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_wireline_area_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_wireline_area_convertToJSON() failed");
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

    OpenAPI_wireline_area_free(dst);
    dst = OpenAPI_wireline_area_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

