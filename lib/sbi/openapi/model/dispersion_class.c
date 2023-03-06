
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_class.h"

OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_create(
)
{
    OpenAPI_dispersion_class_t *dispersion_class_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_class_t));
    ogs_assert(dispersion_class_local_var);


    return dispersion_class_local_var;
}

void OpenAPI_dispersion_class_free(OpenAPI_dispersion_class_t *dispersion_class)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_class) {
        return;
    }
    ogs_free(dispersion_class);
}

cJSON *OpenAPI_dispersion_class_convertToJSON(OpenAPI_dispersion_class_t *dispersion_class)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_class == NULL) {
        ogs_error("OpenAPI_dispersion_class_convertToJSON() failed [DispersionClass]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_parseFromJSON(cJSON *dispersion_classJSON)
{
    OpenAPI_dispersion_class_t *dispersion_class_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    dispersion_class_local_var = OpenAPI_dispersion_class_create (
    );

    return dispersion_class_local_var;
end:
    return NULL;
}

OpenAPI_dispersion_class_t *OpenAPI_dispersion_class_copy(OpenAPI_dispersion_class_t *dst, OpenAPI_dispersion_class_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_class_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_class_convertToJSON() failed");
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

    OpenAPI_dispersion_class_free(dst);
    dst = OpenAPI_dispersion_class_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

