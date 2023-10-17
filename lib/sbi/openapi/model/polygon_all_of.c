
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "polygon_all_of.h"

OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_create(
    OpenAPI_list_t *point_list
)
{
    OpenAPI_polygon_all_of_t *polygon_all_of_local_var = ogs_malloc(sizeof(OpenAPI_polygon_all_of_t));
    ogs_assert(polygon_all_of_local_var);

    polygon_all_of_local_var->point_list = point_list;

    return polygon_all_of_local_var;
}

void OpenAPI_polygon_all_of_free(OpenAPI_polygon_all_of_t *polygon_all_of)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == polygon_all_of) {
        return;
    }
    if (polygon_all_of->point_list) {
        OpenAPI_list_for_each(polygon_all_of->point_list, node) {
            OpenAPI_geographical_coordinates_free(node->data);
        }
        OpenAPI_list_free(polygon_all_of->point_list);
        polygon_all_of->point_list = NULL;
    }
    ogs_free(polygon_all_of);
}

cJSON *OpenAPI_polygon_all_of_convertToJSON(OpenAPI_polygon_all_of_t *polygon_all_of)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (polygon_all_of == NULL) {
        ogs_error("OpenAPI_polygon_all_of_convertToJSON() failed [Polygon_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!polygon_all_of->point_list) {
        ogs_error("OpenAPI_polygon_all_of_convertToJSON() failed [point_list]");
        return NULL;
    }
    cJSON *point_listList = cJSON_AddArrayToObject(item, "pointList");
    if (point_listList == NULL) {
        ogs_error("OpenAPI_polygon_all_of_convertToJSON() failed [point_list]");
        goto end;
    }
    OpenAPI_list_for_each(polygon_all_of->point_list, node) {
        cJSON *itemLocal = OpenAPI_geographical_coordinates_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_polygon_all_of_convertToJSON() failed [point_list]");
            goto end;
        }
        cJSON_AddItemToArray(point_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_parseFromJSON(cJSON *polygon_all_ofJSON)
{
    OpenAPI_polygon_all_of_t *polygon_all_of_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *point_list = NULL;
    OpenAPI_list_t *point_listList = NULL;
    point_list = cJSON_GetObjectItemCaseSensitive(polygon_all_ofJSON, "pointList");
    if (!point_list) {
        ogs_error("OpenAPI_polygon_all_of_parseFromJSON() failed [point_list]");
        goto end;
    }
        cJSON *point_list_local = NULL;
        if (!cJSON_IsArray(point_list)) {
            ogs_error("OpenAPI_polygon_all_of_parseFromJSON() failed [point_list]");
            goto end;
        }

        point_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(point_list_local, point_list) {
            if (!cJSON_IsObject(point_list_local)) {
                ogs_error("OpenAPI_polygon_all_of_parseFromJSON() failed [point_list]");
                goto end;
            }
            OpenAPI_geographical_coordinates_t *point_listItem = OpenAPI_geographical_coordinates_parseFromJSON(point_list_local);
            if (!point_listItem) {
                ogs_error("No point_listItem");
                goto end;
            }
            OpenAPI_list_add(point_listList, point_listItem);
        }

    polygon_all_of_local_var = OpenAPI_polygon_all_of_create (
        point_listList
    );

    return polygon_all_of_local_var;
end:
    if (point_listList) {
        OpenAPI_list_for_each(point_listList, node) {
            OpenAPI_geographical_coordinates_free(node->data);
        }
        OpenAPI_list_free(point_listList);
        point_listList = NULL;
    }
    return NULL;
}

OpenAPI_polygon_all_of_t *OpenAPI_polygon_all_of_copy(OpenAPI_polygon_all_of_t *dst, OpenAPI_polygon_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_polygon_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_polygon_all_of_convertToJSON() failed");
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

    OpenAPI_polygon_all_of_free(dst);
    dst = OpenAPI_polygon_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

