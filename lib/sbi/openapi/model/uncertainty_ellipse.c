
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uncertainty_ellipse.h"

OpenAPI_uncertainty_ellipse_t *OpenAPI_uncertainty_ellipse_create(
    float semi_major,
    float semi_minor,
    int orientation_major
    )
{
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse_local_var = OpenAPI_malloc(sizeof(OpenAPI_uncertainty_ellipse_t));
    if (!uncertainty_ellipse_local_var) {
        return NULL;
    }
    uncertainty_ellipse_local_var->semi_major = semi_major;
    uncertainty_ellipse_local_var->semi_minor = semi_minor;
    uncertainty_ellipse_local_var->orientation_major = orientation_major;

    return uncertainty_ellipse_local_var;
}

void OpenAPI_uncertainty_ellipse_free(OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse)
{
    if (NULL == uncertainty_ellipse) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(uncertainty_ellipse);
}

cJSON *OpenAPI_uncertainty_ellipse_convertToJSON(OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse)
{
    cJSON *item = NULL;

    if (uncertainty_ellipse == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [UncertaintyEllipse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!uncertainty_ellipse->semi_major) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [semi_major]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "semiMajor", uncertainty_ellipse->semi_major) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [semi_major]");
        goto end;
    }

    if (!uncertainty_ellipse->semi_minor) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [semi_minor]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "semiMinor", uncertainty_ellipse->semi_minor) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [semi_minor]");
        goto end;
    }

    if (!uncertainty_ellipse->orientation_major) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [orientation_major]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "orientationMajor", uncertainty_ellipse->orientation_major) == NULL) {
        ogs_error("OpenAPI_uncertainty_ellipse_convertToJSON() failed [orientation_major]");
        goto end;
    }

end:
    return item;
}

OpenAPI_uncertainty_ellipse_t *OpenAPI_uncertainty_ellipse_parseFromJSON(cJSON *uncertainty_ellipseJSON)
{
    OpenAPI_uncertainty_ellipse_t *uncertainty_ellipse_local_var = NULL;
    cJSON *semi_major = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipseJSON, "semiMajor");
    if (!semi_major) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [semi_major]");
        goto end;
    }


    if (!cJSON_IsNumber(semi_major)) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [semi_major]");
        goto end;
    }

    cJSON *semi_minor = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipseJSON, "semiMinor");
    if (!semi_minor) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [semi_minor]");
        goto end;
    }


    if (!cJSON_IsNumber(semi_minor)) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [semi_minor]");
        goto end;
    }

    cJSON *orientation_major = cJSON_GetObjectItemCaseSensitive(uncertainty_ellipseJSON, "orientationMajor");
    if (!orientation_major) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [orientation_major]");
        goto end;
    }


    if (!cJSON_IsNumber(orientation_major)) {
        ogs_error("OpenAPI_uncertainty_ellipse_parseFromJSON() failed [orientation_major]");
        goto end;
    }

    uncertainty_ellipse_local_var = OpenAPI_uncertainty_ellipse_create (
        semi_major->valuedouble,
        semi_minor->valuedouble,
        orientation_major->valuedouble
        );

    return uncertainty_ellipse_local_var;
end:
    return NULL;
}

