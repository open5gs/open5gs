
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "point_all_of.h"

OpenAPI_point_all_of_t *OpenAPI_point_all_of_create(
    OpenAPI_geographical_coordinates_t *point
)
{
    OpenAPI_point_all_of_t *point_all_of_local_var = ogs_malloc(sizeof(OpenAPI_point_all_of_t));
    ogs_assert(point_all_of_local_var);

    point_all_of_local_var->point = point;

    return point_all_of_local_var;
}

void OpenAPI_point_all_of_free(OpenAPI_point_all_of_t *point_all_of)
{
    if (NULL == point_all_of) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_geographical_coordinates_free(point_all_of->point);
    ogs_free(point_all_of);
}

cJSON *OpenAPI_point_all_of_convertToJSON(OpenAPI_point_all_of_t *point_all_of)
{
    cJSON *item = NULL;

    if (point_all_of == NULL) {
        ogs_error("OpenAPI_point_all_of_convertToJSON() failed [Point_allOf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(point_all_of->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_point_all_of_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_point_all_of_convertToJSON() failed [point]");
        goto end;
    }

end:
    return item;
}

OpenAPI_point_all_of_t *OpenAPI_point_all_of_parseFromJSON(cJSON *point_all_ofJSON)
{
    OpenAPI_point_all_of_t *point_all_of_local_var = NULL;
    cJSON *point = cJSON_GetObjectItemCaseSensitive(point_all_ofJSON, "point");
    if (!point) {
        ogs_error("OpenAPI_point_all_of_parseFromJSON() failed [point]");
        goto end;
    }

    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);

    point_all_of_local_var = OpenAPI_point_all_of_create (
        point_local_nonprim
    );

    return point_all_of_local_var;
end:
    return NULL;
}

OpenAPI_point_all_of_t *OpenAPI_point_all_of_copy(OpenAPI_point_all_of_t *dst, OpenAPI_point_all_of_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_point_all_of_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_point_all_of_convertToJSON() failed");
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

    OpenAPI_point_all_of_free(dst);
    dst = OpenAPI_point_all_of_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

