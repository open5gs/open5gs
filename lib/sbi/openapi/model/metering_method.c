
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "metering_method.h"

OpenAPI_metering_method_t *OpenAPI_metering_method_create(
    )
{
    OpenAPI_metering_method_t *metering_method_local_var = OpenAPI_malloc(sizeof(OpenAPI_metering_method_t));
    if (!metering_method_local_var) {
        return NULL;
    }

    return metering_method_local_var;
}

void OpenAPI_metering_method_free(OpenAPI_metering_method_t *metering_method)
{
    if (NULL == metering_method) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(metering_method);
}

cJSON *OpenAPI_metering_method_convertToJSON(OpenAPI_metering_method_t *metering_method)
{
    cJSON *item = NULL;

    if (metering_method == NULL) {
        ogs_error("OpenAPI_metering_method_convertToJSON() failed [MeteringMethod]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_metering_method_t *OpenAPI_metering_method_parseFromJSON(cJSON *metering_methodJSON)
{
    OpenAPI_metering_method_t *metering_method_local_var = NULL;
    metering_method_local_var = OpenAPI_metering_method_create (
        );

    return metering_method_local_var;
end:
    return NULL;
}

OpenAPI_metering_method_t *OpenAPI_metering_method_copy(OpenAPI_metering_method_t *dst, OpenAPI_metering_method_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_metering_method_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_metering_method_convertToJSON() failed");
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

    OpenAPI_metering_method_free(dst);
    dst = OpenAPI_metering_method_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

