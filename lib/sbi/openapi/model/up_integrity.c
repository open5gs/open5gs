
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "up_integrity.h"

OpenAPI_up_integrity_t *OpenAPI_up_integrity_create(
    )
{
    OpenAPI_up_integrity_t *up_integrity_local_var = OpenAPI_malloc(sizeof(OpenAPI_up_integrity_t));
    if (!up_integrity_local_var) {
        return NULL;
    }

    return up_integrity_local_var;
}

void OpenAPI_up_integrity_free(OpenAPI_up_integrity_t *up_integrity)
{
    if (NULL == up_integrity) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(up_integrity);
}

cJSON *OpenAPI_up_integrity_convertToJSON(OpenAPI_up_integrity_t *up_integrity)
{
    cJSON *item = NULL;

    if (up_integrity == NULL) {
        ogs_error("OpenAPI_up_integrity_convertToJSON() failed [UpIntegrity]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_up_integrity_t *OpenAPI_up_integrity_parseFromJSON(cJSON *up_integrityJSON)
{
    OpenAPI_up_integrity_t *up_integrity_local_var = NULL;
    up_integrity_local_var = OpenAPI_up_integrity_create (
        );

    return up_integrity_local_var;
end:
    return NULL;
}

OpenAPI_up_integrity_t *OpenAPI_up_integrity_copy(OpenAPI_up_integrity_t *dst, OpenAPI_up_integrity_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_up_integrity_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_up_integrity_convertToJSON() failed");
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

    OpenAPI_up_integrity_free(dst);
    dst = OpenAPI_up_integrity_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

