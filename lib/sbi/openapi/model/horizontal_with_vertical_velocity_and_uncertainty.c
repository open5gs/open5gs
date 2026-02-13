
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "horizontal_with_vertical_velocity_and_uncertainty.h"

OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction,
    float h_uncertainty,
    float v_uncertainty
)
{
    OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty_local_var = ogs_malloc(sizeof(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t));
    ogs_assert(horizontal_with_vertical_velocity_and_uncertainty_local_var);

    horizontal_with_vertical_velocity_and_uncertainty_local_var->h_speed = h_speed;
    horizontal_with_vertical_velocity_and_uncertainty_local_var->bearing = bearing;
    horizontal_with_vertical_velocity_and_uncertainty_local_var->v_speed = v_speed;
    horizontal_with_vertical_velocity_and_uncertainty_local_var->v_direction = v_direction;
    horizontal_with_vertical_velocity_and_uncertainty_local_var->h_uncertainty = h_uncertainty;
    horizontal_with_vertical_velocity_and_uncertainty_local_var->v_uncertainty = v_uncertainty;

    return horizontal_with_vertical_velocity_and_uncertainty_local_var;
}

void OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_free(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == horizontal_with_vertical_velocity_and_uncertainty) {
        return;
    }
    ogs_free(horizontal_with_vertical_velocity_and_uncertainty);
}

cJSON *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (horizontal_with_vertical_velocity_and_uncertainty == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [HorizontalWithVerticalVelocityAndUncertainty]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "hSpeed", horizontal_with_vertical_velocity_and_uncertainty->h_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [h_speed]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "bearing", horizontal_with_vertical_velocity_and_uncertainty->bearing) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [bearing]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vSpeed", horizontal_with_vertical_velocity_and_uncertainty->v_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [v_speed]");
        goto end;
    }

    if (horizontal_with_vertical_velocity_and_uncertainty->v_direction == OpenAPI_vertical_direction_NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [v_direction]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "vDirection", OpenAPI_vertical_direction_ToString(horizontal_with_vertical_velocity_and_uncertainty->v_direction)) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [v_direction]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "hUncertainty", horizontal_with_vertical_velocity_and_uncertainty->h_uncertainty) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [h_uncertainty]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vUncertainty", horizontal_with_vertical_velocity_and_uncertainty->v_uncertainty) == NULL) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed [v_uncertainty]");
        goto end;
    }

end:
    return item;
}

OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON(cJSON *horizontal_with_vertical_velocity_and_uncertaintyJSON)
{
    OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *horizontal_with_vertical_velocity_and_uncertainty_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_speed = NULL;
    cJSON *bearing = NULL;
    cJSON *v_speed = NULL;
    cJSON *v_direction = NULL;
    OpenAPI_vertical_direction_e v_directionVariable = 0;
    cJSON *h_uncertainty = NULL;
    cJSON *v_uncertainty = NULL;
    h_speed = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "hSpeed");
    if (!h_speed) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [h_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(h_speed)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [h_speed]");
        goto end;
    }

    bearing = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "bearing");
    if (!bearing) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [bearing]");
        goto end;
    }
    if (!cJSON_IsNumber(bearing)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [bearing]");
        goto end;
    }

    v_speed = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "vSpeed");
    if (!v_speed) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(v_speed)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_speed]");
        goto end;
    }

    v_direction = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "vDirection");
    if (!v_direction) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_direction]");
        goto end;
    }
    if (!cJSON_IsString(v_direction)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_direction]");
        goto end;
    }
    v_directionVariable = OpenAPI_vertical_direction_FromString(v_direction->valuestring);

    h_uncertainty = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "hUncertainty");
    if (!h_uncertainty) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(h_uncertainty)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }

    v_uncertainty = cJSON_GetObjectItemCaseSensitive(horizontal_with_vertical_velocity_and_uncertaintyJSON, "vUncertainty");
    if (!v_uncertainty) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(v_uncertainty)) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON() failed [v_uncertainty]");
        goto end;
    }

    horizontal_with_vertical_velocity_and_uncertainty_local_var = OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_create (
        
        h_speed->valuedouble,
        
        bearing->valuedouble,
        
        v_speed->valuedouble,
        v_directionVariable,
        
        h_uncertainty->valuedouble,
        
        v_uncertainty->valuedouble
    );

    return horizontal_with_vertical_velocity_and_uncertainty_local_var;
end:
    return NULL;
}

OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_copy(OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *dst, OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_convertToJSON() failed");
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

    OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_free(dst);
    dst = OpenAPI_horizontal_with_vertical_velocity_and_uncertainty_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

