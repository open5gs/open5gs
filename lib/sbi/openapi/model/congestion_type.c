
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "congestion_type.h"

OpenAPI_congestion_type_t *OpenAPI_congestion_type_create(
)
{
    OpenAPI_congestion_type_t *congestion_type_local_var = ogs_malloc(sizeof(OpenAPI_congestion_type_t));
    ogs_assert(congestion_type_local_var);


    return congestion_type_local_var;
}

void OpenAPI_congestion_type_free(OpenAPI_congestion_type_t *congestion_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == congestion_type) {
        return;
    }
    ogs_free(congestion_type);
}

cJSON *OpenAPI_congestion_type_convertToJSON(OpenAPI_congestion_type_t *congestion_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (congestion_type == NULL) {
        ogs_error("OpenAPI_congestion_type_convertToJSON() failed [CongestionType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_congestion_type_t *OpenAPI_congestion_type_parseFromJSON(cJSON *congestion_typeJSON)
{
    OpenAPI_congestion_type_t *congestion_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    congestion_type_local_var = OpenAPI_congestion_type_create (
    );

    return congestion_type_local_var;
end:
    return NULL;
}

OpenAPI_congestion_type_t *OpenAPI_congestion_type_copy(OpenAPI_congestion_type_t *dst, OpenAPI_congestion_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_congestion_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_congestion_type_convertToJSON() failed");
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

    OpenAPI_congestion_type_free(dst);
    dst = OpenAPI_congestion_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

