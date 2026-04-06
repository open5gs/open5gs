
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "angular_velocity.h"

OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_create(
    OpenAPI_units_angular_velocity_e units_angular_velocity,
    int angular_velocity,
    int a_velocity_uncertainty
)
{
    OpenAPI_angular_velocity_t *angular_velocity_local_var = ogs_malloc(sizeof(OpenAPI_angular_velocity_t));
    ogs_assert(angular_velocity_local_var);

    angular_velocity_local_var->units_angular_velocity = units_angular_velocity;
    angular_velocity_local_var->angular_velocity = angular_velocity;
    angular_velocity_local_var->a_velocity_uncertainty = a_velocity_uncertainty;

    return angular_velocity_local_var;
}

void OpenAPI_angular_velocity_free(OpenAPI_angular_velocity_t *angular_velocity)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == angular_velocity) {
        return;
    }
    ogs_free(angular_velocity);
}

cJSON *OpenAPI_angular_velocity_convertToJSON(OpenAPI_angular_velocity_t *angular_velocity)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (angular_velocity == NULL) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed [AngularVelocity]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (angular_velocity->units_angular_velocity == OpenAPI_units_angular_velocity_NULL) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed [units_angular_velocity]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "unitsAngularVelocity", OpenAPI_units_angular_velocity_ToString(angular_velocity->units_angular_velocity)) == NULL) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed [units_angular_velocity]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "angularVelocity", angular_velocity->angular_velocity) == NULL) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed [angular_velocity]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "aVelocityUncertainty", angular_velocity->a_velocity_uncertainty) == NULL) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed [a_velocity_uncertainty]");
        goto end;
    }

end:
    return item;
}

OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_parseFromJSON(cJSON *angular_velocityJSON)
{
    OpenAPI_angular_velocity_t *angular_velocity_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *units_angular_velocity = NULL;
    OpenAPI_units_angular_velocity_e units_angular_velocityVariable = 0;
    cJSON *angular_velocity = NULL;
    cJSON *a_velocity_uncertainty = NULL;
    units_angular_velocity = cJSON_GetObjectItemCaseSensitive(angular_velocityJSON, "unitsAngularVelocity");
    if (!units_angular_velocity) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [units_angular_velocity]");
        goto end;
    }
    if (!cJSON_IsString(units_angular_velocity)) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [units_angular_velocity]");
        goto end;
    }
    units_angular_velocityVariable = OpenAPI_units_angular_velocity_FromString(units_angular_velocity->valuestring);

    angular_velocity = cJSON_GetObjectItemCaseSensitive(angular_velocityJSON, "angularVelocity");
    if (!angular_velocity) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [angular_velocity]");
        goto end;
    }
    if (!cJSON_IsNumber(angular_velocity)) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [angular_velocity]");
        goto end;
    }

    a_velocity_uncertainty = cJSON_GetObjectItemCaseSensitive(angular_velocityJSON, "aVelocityUncertainty");
    if (!a_velocity_uncertainty) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [a_velocity_uncertainty]");
        goto end;
    }
    if (!cJSON_IsNumber(a_velocity_uncertainty)) {
        ogs_error("OpenAPI_angular_velocity_parseFromJSON() failed [a_velocity_uncertainty]");
        goto end;
    }

    angular_velocity_local_var = OpenAPI_angular_velocity_create (
        units_angular_velocityVariable,
        
        angular_velocity->valuedouble,
        
        a_velocity_uncertainty->valuedouble
    );

    return angular_velocity_local_var;
end:
    return NULL;
}

OpenAPI_angular_velocity_t *OpenAPI_angular_velocity_copy(OpenAPI_angular_velocity_t *dst, OpenAPI_angular_velocity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_angular_velocity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_angular_velocity_convertToJSON() failed");
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

    OpenAPI_angular_velocity_free(dst);
    dst = OpenAPI_angular_velocity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

