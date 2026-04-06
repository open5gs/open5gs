
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "horizontal_with_vertical_velocity.h"

OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction
)
{
    OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity_local_var = ogs_malloc(sizeof(OpenAPI_horizontal_with_vertical_velocity_t));
    ogs_assert(horizontal_with_vertical_velocity_local_var);

    horizontal_with_vertical_velocity_local_var->h_speed = h_speed;
    horizontal_with_vertical_velocity_local_var->bearing = bearing;
    horizontal_with_vertical_velocity_local_var->v_speed = v_speed;
    horizontal_with_vertical_velocity_local_var->v_direction = v_direction;

    return horizontal_with_vertical_velocity_local_var;
}

void OpenAPI_horizontal_with_vertical_velocity_free(OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == horizontal_with_vertical_velocity) {
        return;
    }
    ogs_free(horizontal_with_vertical_velocity);
}

cJSON *OpenAPI_horizontal_with_vertical_velocity_convertToJSON(OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (horizontal_with_vertical_velocity == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [HorizontalWithVerticalVelocity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "hSpeed", horizontal_with_vertical_velocity->h_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [h_speed]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "bearing", horizontal_with_vertical_velocity->bearing) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [bearing]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vSpeed", horizontal_with_vertical_velocity->v_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [v_speed]");
        goto end;
    }

    if (horizontal_with_vertical_velocity->v_direction == OpenAPI_vertical_direction_NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [v_direction]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "vDirection", OpenAPI_vertical_direction_ToString(horizontal_with_vertical_velocity->v_direction)) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed [v_direction]");
        goto end;
    }

end:
    return item;
}

OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_parseFromJSON(cJSON *horizontal_with_vertical_velocityJSON)
{
    OpenAPI_horizontal_with_vertical_velocity_t *horizontal_with_vertical_velocity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_speed = NULL;
    cJSON *bearing = NULL;
    cJSON *v_speed = NULL;
    cJSON *v_direction = NULL;
    OpenAPI_vertical_direction_e v_directionVariable = 0;
    h_speed = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocityJSON, "hSpeed");
    if (!h_speed) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [h_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(h_speed)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [h_speed]");
        goto end;
    }

    bearing = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocityJSON, "bearing");
    if (!bearing) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [bearing]");
        goto end;
    }
    if (!cJSON_IsNumber(bearing)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [bearing]");
        goto end;
    }

    v_speed = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocityJSON, "vSpeed");
    if (!v_speed) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [v_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(v_speed)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [v_speed]");
        goto end;
    }

    v_direction = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocityJSON, "vDirection");
    if (!v_direction) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [v_direction]");
        goto end;
    }
    if (!cJSON_IsString(v_direction)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_parseFromJSON() failed [v_direction]");
        goto end;
    }
    v_directionVariable = OpenAPI_vertical_direction_FromString(v_direction->valuestring);

    horizontal_with_vertical_velocity_local_var = OpenAPI_horizontal_with_vertical_velocity_create (
        
        h_speed->valuedouble,
        
        bearing->valuedouble,
        
        v_speed->valuedouble,
        v_directionVariable
    );

    return horizontal_with_vertical_velocity_local_var;
end:
    return NULL;
}

OpenAPI_horizontal_with_vertical_velocity_t *OpenAPI_horizontal_with_vertical_velocity_copy(OpenAPI_horizontal_with_vertical_velocity_t *dst, OpenAPI_horizontal_with_vertical_velocity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_horizontal_with_vertical_velocity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_convertToJSON() failed");
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

    OpenAPI_horizontal_with_vertical_velocity_free(dst);
    dst = OpenAPI_horizontal_with_vertical_velocity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

