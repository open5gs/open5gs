
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispersion_type.h"

OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_create(
)
{
    OpenAPI_dispersion_type_t *dispersion_type_local_var = ogs_malloc(sizeof(OpenAPI_dispersion_type_t));
    ogs_assert(dispersion_type_local_var);


    return dispersion_type_local_var;
}

void OpenAPI_dispersion_type_free(OpenAPI_dispersion_type_t *dispersion_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dispersion_type) {
        return;
    }
    ogs_free(dispersion_type);
}

cJSON *OpenAPI_dispersion_type_convertToJSON(OpenAPI_dispersion_type_t *dispersion_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dispersion_type == NULL) {
        ogs_error("OpenAPI_dispersion_type_convertToJSON() failed [DispersionType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_parseFromJSON(cJSON *dispersion_typeJSON)
{
    OpenAPI_dispersion_type_t *dispersion_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    dispersion_type_local_var = OpenAPI_dispersion_type_create (
    );

    return dispersion_type_local_var;
end:
    return NULL;
}

OpenAPI_dispersion_type_t *OpenAPI_dispersion_type_copy(OpenAPI_dispersion_type_t *dst, OpenAPI_dispersion_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dispersion_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dispersion_type_convertToJSON() failed");
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

    OpenAPI_dispersion_type_free(dst);
    dst = OpenAPI_dispersion_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

