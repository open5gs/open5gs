
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "horizontal_velocity_with_uncertainty.h"

OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_create(
    float h_speed,
    int bearing,
    float h_uncertainty
)
{
    OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty_local_var = ogs_malloc(sizeof(OpenAPI_horizontal_velocity_with_uncertainty_t));
    ogs_assert(horizontal_velocity_with_uncertainty_local_var);

    horizontal_velocity_with_uncertainty_local_var->h_speed = h_speed;
    horizontal_velocity_with_uncertainty_local_var->bearing = bearing;
    horizontal_velocity_with_uncertainty_local_var->h_uncertainty = h_uncertainty;

    return horizontal_velocity_with_uncertainty_local_var;
}

void OpenAPI_horizontal_velocity_with_uncertainty_free(OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == horizontal_velocity_with_uncertainty) {
        return;
    }
    ogs_free(horizontal_velocity_with_uncertainty);
}

cJSON *OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON(OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (horizontal_velocity_with_uncertainty == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON() failed [HorizontalVelocityWithUncertainty]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "hSpeed", horizontal_velocity_with_uncertainty->h_speed) == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON() failed [h_speed]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "bearing", horizontal_velocity_with_uncertainty->bearing) == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON() failed [bearing]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "hUncertainty", horizontal_velocity_with_uncertainty->h_uncertainty) == NULL) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON() failed [h_uncertainty]");
        goto end;
    }

end:
    return item;
}

OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON(cJSON *horizontal_velocity_with_uncertaintyJSON)
{
    OpenAPI_horizontal_velocity_with_uncertainty_t *horizontal_velocity_with_uncertainty_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *h_speed = NULL;
    cJSON *bearing = NULL;
    cJSON *h_uncertainty = NULL;
    h_speed = cJSON_GetObjectItemCaseSensitive(horizontal_velocity_with_uncertaintyJSON, "hSpeed");
    if (!h_speed) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [h_speed]");
        goto end;
    }
    if (!cJSON_IsNumber(h_speed)) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [h_speed]");
        goto end;
    }

    bearing = cJSON_GetObjectItemCaseSensitive(horizontal_velocity_with_uncertaintyJSON, "bearing");
    if (!bearing) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [bearing]");
        goto end;
    }
    if (!cJSON_IsNumber(bearing)) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [bearing]");
        goto end;
    }

    h_uncertainty = cJSON_GetObjectItemCaseSensitive(horizontal_velocity_with_uncertaintyJSON, "hUncertainty");
    if (!h_uncertainty) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(h_uncertainty)) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON() failed [h_uncertainty]");
        goto end;
    }

    horizontal_velocity_with_uncertainty_local_var = OpenAPI_horizontal_velocity_with_uncertainty_create (
        
        h_speed->valuedouble,
        
        bearing->valuedouble,
        
        h_uncertainty->valuedouble
    );

    return horizontal_velocity_with_uncertainty_local_var;
end:
    return NULL;
}

OpenAPI_horizontal_velocity_with_uncertainty_t *OpenAPI_horizontal_velocity_with_uncertainty_copy(OpenAPI_horizontal_velocity_with_uncertainty_t *dst, OpenAPI_horizontal_velocity_with_uncertainty_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_horizontal_velocity_with_uncertainty_convertToJSON() failed");
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

    OpenAPI_horizontal_velocity_with_uncertainty_free(dst);
    dst = OpenAPI_horizontal_velocity_with_uncertainty_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

