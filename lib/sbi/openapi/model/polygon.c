
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "polygon.h"

OpenAPI_polygon_t *OpenAPI_polygon_create(
    OpenAPI_supported_gad_shapes_t *shape,
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
    if (NULL == polygon) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_supported_gad_shapes_free(polygon->shape);
    OpenAPI_list_for_each(polygon->point_list, node) {
        OpenAPI_geographical_coordinates_free(node->data);
    }
    OpenAPI_list_free(polygon->point_list);
    ogs_free(polygon);
}

cJSON *OpenAPI_polygon_convertToJSON(OpenAPI_polygon_t *polygon)
{
    cJSON *item = NULL;

    if (polygon == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [Polygon]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *shape_local_JSON = OpenAPI_supported_gad_shapes_convertToJSON(polygon->shape);
    if (shape_local_JSON == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [shape]");
        goto end;
    }
    cJSON_AddItemToObject(item, "shape", shape_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [shape]");
        goto end;
    }

    cJSON *point_listList = cJSON_AddArrayToObject(item, "pointList");
    if (point_listList == NULL) {
        ogs_error("OpenAPI_polygon_convertToJSON() failed [point_list]");
        goto end;
    }

    OpenAPI_lnode_t *point_list_node;
    if (polygon->point_list) {
        OpenAPI_list_for_each(polygon->point_list, point_list_node) {
            cJSON *itemLocal = OpenAPI_geographical_coordinates_convertToJSON(point_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_polygon_convertToJSON() failed [point_list]");
                goto end;
            }
            cJSON_AddItemToArray(point_listList, itemLocal);
        }
    }

end:
    return item;
}

OpenAPI_polygon_t *OpenAPI_polygon_parseFromJSON(cJSON *polygonJSON)
{
    OpenAPI_polygon_t *polygon_local_var = NULL;
    cJSON *shape = cJSON_GetObjectItemCaseSensitive(polygonJSON, "shape");
    if (!shape) {
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [shape]");
        goto end;
    }

    OpenAPI_supported_gad_shapes_t *shape_local_nonprim = NULL;
    shape_local_nonprim = OpenAPI_supported_gad_shapes_parseFromJSON(shape);

    cJSON *point_list = cJSON_GetObjectItemCaseSensitive(polygonJSON, "pointList");
    if (!point_list) {
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
        goto end;
    }

    OpenAPI_list_t *point_listList;
    cJSON *point_list_local_nonprimitive;
    if (!cJSON_IsArray(point_list)){
        ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
        goto end;
    }

    point_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(point_list_local_nonprimitive, point_list ) {
        if (!cJSON_IsObject(point_list_local_nonprimitive)) {
            ogs_error("OpenAPI_polygon_parseFromJSON() failed [point_list]");
            goto end;
        }
        OpenAPI_geographical_coordinates_t *point_listItem = OpenAPI_geographical_coordinates_parseFromJSON(point_list_local_nonprimitive);

        if (!point_listItem) {
            ogs_error("No point_listItem");
            OpenAPI_list_free(point_listList);
            goto end;
        }

        OpenAPI_list_add(point_listList, point_listItem);
    }

    polygon_local_var = OpenAPI_polygon_create (
        shape_local_nonprim,
        point_listList
    );

    return polygon_local_var;
end:
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

