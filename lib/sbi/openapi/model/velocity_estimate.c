
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "velocity_estimate.h"

OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_create(
    float h_speed,
    int bearing,
    float v_speed,
    OpenAPI_vertical_direction_e v_direction,
    float h_uncertainty,
    float v_uncertainty
)
{
    OpenAPI_velocity_estimate_t *velocity_estimate_local_var = ogs_malloc(sizeof(OpenAPI_velocity_estimate_t));
    ogs_assert(velocity_estimate_local_var);

    velocity_estimate_local_var->h_speed = h_speed;
    velocity_estimate_local_var->bearing = bearing;
    velocity_estimate_local_var->v_speed = v_speed;
    velocity_estimate_local_var->v_direction = v_direction;
    velocity_estimate_local_var->h_uncertainty = h_uncertainty;
    velocity_estimate_local_var->v_uncertainty = v_uncertainty;

    return velocity_estimate_local_var;
}

void OpenAPI_velocity_estimate_free(OpenAPI_velocity_estimate_t *velocity_estimate)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == velocity_estimate) {
        return;
    }
    ogs_free(velocity_estimate);
}

cJSON *OpenAPI_velocity_estimate_convertToJSON(OpenAPI_velocity_estimate_t *velocity_estimate)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (velocity_estimate == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [VelocityEstimate]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "hSpeed", velocity_estimate->h_speed) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [h_speed]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "bearing", velocity_estimate->bearing) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [bearing]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vSpeed", velocity_estimate->v_speed) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [v_speed]");
        goto end;
    }

    if (velocity_estimate->v_direction == OpenAPI_vertical_direction_NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [v_direction]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "vDirection", OpenAPI_vertical_direction_ToString(velocity_estimate->v_direction)) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [v_direction]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "hUncertainty", velocity_estimate->h_uncertainty) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [h_uncertainty]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vUncertainty", velocity_estimate->v_uncertainty) == NULL) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed [v_uncertainty]");
        goto end;
    }

end:
    return item;
}

OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_parseFromJSON(cJSON *velocity_estimateJSON)
{
    OpenAPI_velocity_estimate_t *velocity_estimate_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_speed = NULL;
    cJSON *bearing = NULL;
    cJSON *v_speed = NULL;
    cJSON *v_direction = NULL;
    OpenAPI_vertical_direction_e v_directionVariable = 0;
    cJSON *h_uncertainty = NULL;
    cJSON *v_uncertainty = NULL;
    h_speed = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "hSpeed");
    if (!h_speed) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [h_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(h_speed)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [h_speed]");
        goto end;
    }

    bearing = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "bearing");
    if (!bearing) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [bearing]");
        goto end;
    }
    if (!cJSON_IsNumber(bearing)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [bearing]");
        goto end;
    }

    v_speed = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "vSpeed");
    if (!v_speed) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(v_speed)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_speed]");
        goto end;
    }

    v_direction = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "vDirection");
    if (!v_direction) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_direction]");
        goto end;
    }
    if (!cJSON_IsString(v_direction)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_direction]");
        goto end;
    }
    v_directionVariable = OpenAPI_vertical_direction_FromString(v_direction->valuestring);

    h_uncertainty = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "hUncertainty");
    if (!h_uncertainty) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(h_uncertainty)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }

    v_uncertainty = cJSON_GetObjectItemCaseSensitive(velocity_estimateJSON, "vUncertainty");
    if (!v_uncertainty) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(v_uncertainty)) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON() failed [v_uncertainty]");
        goto end;
    }

    velocity_estimate_local_var = OpenAPI_velocity_estimate_create (
        
        h_speed->valuedouble,
        
        bearing->valuedouble,
        
        v_speed->valuedouble,
        v_directionVariable,
        
        h_uncertainty->valuedouble,
        
        v_uncertainty->valuedouble
    );

    return velocity_estimate_local_var;
end:
    return NULL;
}

OpenAPI_velocity_estimate_t *OpenAPI_velocity_estimate_copy(OpenAPI_velocity_estimate_t *dst, OpenAPI_velocity_estimate_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_velocity_estimate_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_velocity_estimate_convertToJSON() failed");
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

    OpenAPI_velocity_estimate_free(dst);
    dst = OpenAPI_velocity_estimate_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

