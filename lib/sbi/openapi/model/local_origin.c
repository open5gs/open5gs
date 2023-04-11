
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "local_origin.h"

OpenAPI_local_origin_t *OpenAPI_local_origin_create(
    char *coordinate_id,
    OpenAPI_geographical_coordinates_t *point
)
{
    OpenAPI_local_origin_t *local_origin_local_var = ogs_malloc(sizeof(OpenAPI_local_origin_t));
    ogs_assert(local_origin_local_var);

    local_origin_local_var->coordinate_id = coordinate_id;
    local_origin_local_var->point = point;

    return local_origin_local_var;
}

void OpenAPI_local_origin_free(OpenAPI_local_origin_t *local_origin)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == local_origin) {
        return;
    }
    if (local_origin->coordinate_id) {
        ogs_free(local_origin->coordinate_id);
        local_origin->coordinate_id = NULL;
    }
    if (local_origin->point) {
        OpenAPI_geographical_coordinates_free(local_origin->point);
        local_origin->point = NULL;
    }
    ogs_free(local_origin);
}

cJSON *OpenAPI_local_origin_convertToJSON(OpenAPI_local_origin_t *local_origin)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (local_origin == NULL) {
        ogs_error("OpenAPI_local_origin_convertToJSON() failed [LocalOrigin]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (local_origin->coordinate_id) {
    if (cJSON_AddStringToObject(item, "coordinateId", local_origin->coordinate_id) == NULL) {
        ogs_error("OpenAPI_local_origin_convertToJSON() failed [coordinate_id]");
        goto end;
    }
    }

    if (local_origin->point) {
    cJSON *point_local_JSON = OpenAPI_geographical_coordinates_convertToJSON(local_origin->point);
    if (point_local_JSON == NULL) {
        ogs_error("OpenAPI_local_origin_convertToJSON() failed [point]");
        goto end;
    }
    cJSON_AddItemToObject(item, "point", point_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_local_origin_convertToJSON() failed [point]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_local_origin_t *OpenAPI_local_origin_parseFromJSON(cJSON *local_originJSON)
{
    OpenAPI_local_origin_t *local_origin_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *coordinate_id = NULL;
    cJSON *point = NULL;
    OpenAPI_geographical_coordinates_t *point_local_nonprim = NULL;
    coordinate_id = cJSON_GetObjectItemCaseSensitive(local_originJSON, "coordinateId");
    if (coordinate_id) {
    if (!cJSON_IsString(coordinate_id) && !cJSON_IsNull(coordinate_id)) {
        ogs_error("OpenAPI_local_origin_parseFromJSON() failed [coordinate_id]");
        goto end;
    }
    }

    point = cJSON_GetObjectItemCaseSensitive(local_originJSON, "point");
    if (point) {
    point_local_nonprim = OpenAPI_geographical_coordinates_parseFromJSON(point);
    if (!point_local_nonprim) {
        ogs_error("OpenAPI_geographical_coordinates_parseFromJSON failed [point]");
        goto end;
    }
    }

    local_origin_local_var = OpenAPI_local_origin_create (
        coordinate_id && !cJSON_IsNull(coordinate_id) ? ogs_strdup(coordinate_id->valuestring) : NULL,
        point ? point_local_nonprim : NULL
    );

    return local_origin_local_var;
end:
    if (point_local_nonprim) {
        OpenAPI_geographical_coordinates_free(point_local_nonprim);
        point_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_local_origin_t *OpenAPI_local_origin_copy(OpenAPI_local_origin_t *dst, OpenAPI_local_origin_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_local_origin_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_local_origin_convertToJSON() failed");
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

    OpenAPI_local_origin_free(dst);
    dst = OpenAPI_local_origin_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

