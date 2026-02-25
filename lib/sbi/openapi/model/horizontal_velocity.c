
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "horizontal_velocity.h"

OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_create(
    float h_speed,
    int bearing
)
{
    OpenAPI_horizontal_velocity_t *horizontal_velocity_local_var = ogs_malloc(sizeof(OpenAPI_horizontal_velocity_t));
    ogs_assert(horizontal_velocity_local_var);

    horizontal_velocity_local_var->h_speed = h_speed;
    horizontal_velocity_local_var->bearing = bearing;

    return horizontal_velocity_local_var;
}

void OpenAPI_horizontal_velocity_free(OpenAPI_horizontal_velocity_t *horizontal_velocity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == horizontal_velocity) {
        return;
    }
    ogs_free(horizontal_velocity);
}

cJSON *OpenAPI_horizontal_velocity_convertToJSON(OpenAPI_horizontal_velocity_t *horizontal_velocity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (horizontal_velocity == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_convertToJSON() failed [HorizontalVelocity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "hSpeed", horizontal_velocity->h_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_convertToJSON() failed [h_speed]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "bearing", horizontal_velocity->bearing) == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_convertToJSON() failed [bearing]");
        goto end;
    }

end:
    return item;
}

OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_parseFromJSON(cJSON *horizontal_velocityJSON)
{
    OpenAPI_horizontal_velocity_t *horizontal_velocity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_speed = NULL;
    cJSON *bearing = NULL;
    h_speed = cJSON_GetObjectItemCaseSensitive(horizontal_velocityJSON, "hSpeed");
    if (!h_speed) {
        ogs_error("OpenAPI_horizontal_velocity_parseFromJSON() failed [h_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(h_speed)) {
        ogs_error("OpenAPI_horizontal_velocity_parseFromJSON() failed [h_speed]");
        goto end;
    }

    bearing = cJSON_GetObjectItemCaseSensitive(horizontal_velocityJSON, "bearing");
    if (!bearing) {
        ogs_error("OpenAPI_horizontal_velocity_parseFromJSON() failed [bearing]");
        goto end;
    }
    if (!cJSON_IsNumber(bearing)) {
        ogs_error("OpenAPI_horizontal_velocity_parseFromJSON() failed [bearing]");
        goto end;
    }

    horizontal_velocity_local_var = OpenAPI_horizontal_velocity_create (
        
        h_speed->valuedouble,
        
        bearing->valuedouble
    );

    return horizontal_velocity_local_var;
end:
    return NULL;
}

OpenAPI_horizontal_velocity_t *OpenAPI_horizontal_velocity_copy(OpenAPI_horizontal_velocity_t *dst, OpenAPI_horizontal_velocity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_horizontal_velocity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_horizontal_velocity_convertToJSON() failed");
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

    OpenAPI_horizontal_velocity_free(dst);
    dst = OpenAPI_horizontal_velocity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

