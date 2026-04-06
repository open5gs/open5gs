
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "polygon.h"

OpenAPI_polygon_t *OpenAPI_polygon_create(
    OpenAPI_supported_gad_shapes_e shape,
    OpenAPI_list_t *point_list
)
{
    OpenAPI_polygon_t *polygon_local_var = ogs_malloc(sizeof(OpenAPI_polygon_t));
    ogs_assert(polygon_local_var);

    polygon_local_var->shape = shape;
    polygon_local_var->point_list = point_list;

    return polygon_local_var;
}

void OpenAPI_polygon_free(OpenAPI_polygon_t *polygon)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == polygon) {
        return;
    }
    if (polygon->point_list) {
        OpenAPI_list_for_each(polygon->point_list, node) {
            OpenAPI_geographical_coordinates_free(node->data);
        }
        OpenAPI_list_free(polygon->point_list);
        polygon->point_list = NULL;
    }
    ogs_free(polygon);
}

cJSON *OpenAPI_polygon_convertToJSON(OpenAPI_polygon_t *polygon)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (polygon == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [Polygon]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (polygon->shape == OpenAPI_supported_gad_shapes_NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [shape]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "shape", OpenAPI_supported_gad_shapes_ToString(polygon->shape)) == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [shape]");
        goto end;
    }

    if (!polygon->point_list) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [point_list]");
        return NULL;
    }
    cJSON *point_listList = cJSON_AddArrayToObject(item, "pointList");
    if (point_listList == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [point_list]");
        goto end;
    }
    OpenAPI_list_for_each(polygon->point_list, node) {
        cJSON *itemLocal = OpenAPI_geographical_coordinates_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_polygon_convertToJSON() failed [point_list]");
            goto end;
        }
        cJSON_AddItemToArray(point_listList, itemLocal);
    }

end:
    return item;
}

OpenAPI_polygon_t *OpenAPI_polygon_parseFromJSON(cJSON *polygonJSON)
{
    OpenAPI_polygon_t *polygon_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *shape = NULL;
    OpenAPI_supported_gad_shapes_e shapeVariable = 0;
    cJSON *point_list = NULL;
    OpenAPI_list_t *point_listList = NULL;
    shape = cJSON_GetObjectItemCaseSensitive(polygonJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [shape]");
        goto end;
    }
    if (!cJSON_IsString(shape)) {
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [shape]");
        goto end;
    }
    shapeVariable = OpenAPI_supported_gad_shapes_FromString(shape->valuestring);

    point_list = cJSON_GetObjectItemCaseSensitive(polygonJSON, "pointList");
    if (!point_list) {
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
        goto end;
    }
        cJSON *point_list_local = NULL;
        if (!cJSON_IsArray(point_list)) {
            ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
            goto end;
        }

        point_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(point_list_local, point_list) {
            if (!cJSON_IsObject(point_list_local)) {
                ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
                goto end;
            }
            OpenAPI_geographical_coordinates_t *point_listItem = OpenAPI_geographical_coordinates_parseFromJSON(point_list_local);
            if (!point_listItem) {
                ogs_error("No point_listItem");
                goto end;
            }
            OpenAPI_list_add(point_listList, point_listItem);
        }

    polygon_local_var = OpenAPI_polygon_create (
        shapeVariable,
        point_listList
    );

    return polygon_local_var;
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

OpenAPI_polygon_t *OpenAPI_polygon_copy(OpenAPI_polygon_t *dst, OpenAPI_polygon_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_polygon_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed");
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

    OpenAPI_polygon_free(dst);
    dst = OpenAPI_polygon_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

