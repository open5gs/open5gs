
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "restriction_type.h"

OpenAPI_restriction_type_t *OpenAPI_restriction_type_create(
    )
{
    OpenAPI_restriction_type_t *restriction_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_restriction_type_t));
    if (!restriction_type_local_var) {
        return NULL;
    }

    return restriction_type_local_var;
}

void OpenAPI_restriction_type_free(OpenAPI_restriction_type_t *restriction_type)
{
    if (NULL == restriction_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(restriction_type);
}

cJSON *OpenAPI_restriction_type_convertToJSON(OpenAPI_restriction_type_t *restriction_type)
{
    cJSON *item = NULL;

    if (restriction_type == NULL) {
        ogs_error("OpenAPI_restriction_type_convertToJSON() failed [RestrictionType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_restriction_type_t *OpenAPI_restriction_type_parseFromJSON(cJSON *restriction_typeJSON)
{
    OpenAPI_restriction_type_t *restriction_type_local_var = NULL;
    restriction_type_local_var = OpenAPI_restriction_type_create (
        );

    return restriction_type_local_var;
end:
    return NULL;
}

OpenAPI_restriction_type_t *OpenAPI_restriction_type_copy(OpenAPI_restriction_type_t *dst, OpenAPI_restriction_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_restriction_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_restriction_type_convertToJSON() failed");
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

    OpenAPI_restriction_type_free(dst);
    dst = OpenAPI_restriction_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

