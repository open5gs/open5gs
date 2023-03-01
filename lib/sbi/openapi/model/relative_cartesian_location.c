
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "relative_cartesian_location.h"

OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_create(
    float x,
    float y,
    bool is_z,
    float z
)
{
    OpenAPI_relative_cartesian_location_t *relative_cartesian_location_local_var = ogs_malloc(sizeof(OpenAPI_relative_cartesian_location_t));
    ogs_assert(relative_cartesian_location_local_var);

    relative_cartesian_location_local_var->x = x;
    relative_cartesian_location_local_var->y = y;
    relative_cartesian_location_local_var->is_z = is_z;
    relative_cartesian_location_local_var->z = z;

    return relative_cartesian_location_local_var;
}

void OpenAPI_relative_cartesian_location_free(OpenAPI_relative_cartesian_location_t *relative_cartesian_location)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == relative_cartesian_location) {
        return;
    }
    ogs_free(relative_cartesian_location);
}

cJSON *OpenAPI_relative_cartesian_location_convertToJSON(OpenAPI_relative_cartesian_location_t *relative_cartesian_location)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (relative_cartesian_location == NULL) {
        ogs_error("OpenAPI_relative_cartesian_location_convertToJSON() failed [RelativeCartesianLocation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "x", relative_cartesian_location->x) == NULL) {
        ogs_error("OpenAPI_relative_cartesian_location_convertToJSON() failed [x]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "y", relative_cartesian_location->y) == NULL) {
        ogs_error("OpenAPI_relative_cartesian_location_convertToJSON() failed [y]");
        goto end;
    }

    if (relative_cartesian_location->is_z) {
    if (cJSON_AddNumberToObject(item, "z", relative_cartesian_location->z) == NULL) {
        ogs_error("OpenAPI_relative_cartesian_location_convertToJSON() failed [z]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_parseFromJSON(cJSON *relative_cartesian_locationJSON)
{
    OpenAPI_relative_cartesian_location_t *relative_cartesian_location_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *x = NULL;
    cJSON *y = NULL;
    cJSON *z = NULL;
    x = cJSON_GetObjectItemCaseSensitive(relative_cartesian_locationJSON, "x");
    if (!x) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON() failed [x]");
        goto end;
    }
    if (!cJSON_IsNumber(x)) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON() failed [x]");
        goto end;
    }

    y = cJSON_GetObjectItemCaseSensitive(relative_cartesian_locationJSON, "y");
    if (!y) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON() failed [y]");
        goto end;
    }
    if (!cJSON_IsNumber(y)) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON() failed [y]");
        goto end;
    }

    z = cJSON_GetObjectItemCaseSensitive(relative_cartesian_locationJSON, "z");
    if (z) {
    if (!cJSON_IsNumber(z)) {
        ogs_error("OpenAPI_relative_cartesian_location_parseFromJSON() failed [z]");
        goto end;
    }
    }

    relative_cartesian_location_local_var = OpenAPI_relative_cartesian_location_create (
        
        x->valuedouble,
        
        y->valuedouble,
        z ? true : false,
        z ? z->valuedouble : 0
    );

    return relative_cartesian_location_local_var;
end:
    return NULL;
}

OpenAPI_relative_cartesian_location_t *OpenAPI_relative_cartesian_location_copy(OpenAPI_relative_cartesian_location_t *dst, OpenAPI_relative_cartesian_location_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_relative_cartesian_location_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_relative_cartesian_location_convertToJSON() failed");
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

    OpenAPI_relative_cartesian_location_free(dst);
    dst = OpenAPI_relative_cartesian_location_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

