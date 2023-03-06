
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uncertainty_ellipsoid.h"

OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_create(
    float semi_major,
    float semi_minor,
    float vertical,
    int orientation_major
)
{
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid_local_var = ogs_malloc(sizeof(OpenAPI_uncertainty_ellipsoid_t));
    ogs_assert(uncertainty_ellipsoid_local_var);

    uncertainty_ellipsoid_local_var->semi_major = semi_major;
    uncertainty_ellipsoid_local_var->semi_minor = semi_minor;
    uncertainty_ellipsoid_local_var->vertical = vertical;
    uncertainty_ellipsoid_local_var->orientation_major = orientation_major;

    return uncertainty_ellipsoid_local_var;
}

void OpenAPI_uncertainty_ellipsoid_free(OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == uncertainty_ellipsoid) {
        return;
    }
    ogs_free(uncertainty_ellipsoid);
}

cJSON *OpenAPI_uncertainty_ellipsoid_convertToJSON(OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (uncertainty_ellipsoid == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed [UncertaintyEllipsoid]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "semiMajor", uncertainty_ellipsoid->semi_major) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed [semi_major]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "semiMinor", uncertainty_ellipsoid->semi_minor) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed [semi_minor]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "vertical", uncertainty_ellipsoid->vertical) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed [vertical]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "orientationMajor", uncertainty_ellipsoid->orientation_major) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed [orientation_major]");
        goto end;
    }

end:
    return item;
}

OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_parseFromJSON(cJSON *uncertainty_ellipsoidJSON)
{
    OpenAPI_uncertainty_ellipsoid_t *uncertainty_ellipsoid_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *semi_major = NULL;
    cJSON *semi_minor = NULL;
    cJSON *vertical = NULL;
    cJSON *orientation_major = NULL;
    semi_major = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipsoidJSON, "semiMajor");
    if (!semi_major) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [semi_major]");
        goto end;
    }
    if (!cJSON_IsNumber(semi_major)) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [semi_major]");
        goto end;
    }

    semi_minor = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipsoidJSON, "semiMinor");
    if (!semi_minor) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [semi_minor]");
        goto end;
    }
    if (!cJSON_IsNumber(semi_minor)) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [semi_minor]");
        goto end;
    }

    vertical = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipsoidJSON, "vertical");
    if (!vertical) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [vertical]");
        goto end;
    }
    if (!cJSON_IsNumber(vertical)) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [vertical]");
        goto end;
    }

    orientation_major = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipsoidJSON, "orientationMajor");
    if (!orientation_major) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [orientation_major]");
        goto end;
    }
    if (!cJSON_IsNumber(orientation_major)) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_parseFromJSON() failed [orientation_major]");
        goto end;
    }

    uncertainty_ellipsoid_local_var = OpenAPI_uncertainty_ellipsoid_create (
        
        semi_major->valuedouble,
        
        semi_minor->valuedouble,
        
        vertical->valuedouble,
        
        orientation_major->valuedouble
    );

    return uncertainty_ellipsoid_local_var;
end:
    return NULL;
}

OpenAPI_uncertainty_ellipsoid_t *OpenAPI_uncertainty_ellipsoid_copy(OpenAPI_uncertainty_ellipsoid_t *dst, OpenAPI_uncertainty_ellipsoid_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_uncertainty_ellipsoid_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_uncertainty_ellipsoid_convertToJSON() failed");
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

    OpenAPI_uncertainty_ellipsoid_free(dst);
    dst = OpenAPI_uncertainty_ellipsoid_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

