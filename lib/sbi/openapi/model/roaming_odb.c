
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "roaming_odb.h"

OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_create(
    )
{
    OpenAPI_roaming_odb_t *roaming_odb_local_var = OpenAPI_malloc(sizeof(OpenAPI_roaming_odb_t));
    if (!roaming_odb_local_var) {
        return NULL;
    }

    return roaming_odb_local_var;
}

void OpenAPI_roaming_odb_free(OpenAPI_roaming_odb_t *roaming_odb)
{
    if (NULL == roaming_odb) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(roaming_odb);
}

cJSON *OpenAPI_roaming_odb_convertToJSON(OpenAPI_roaming_odb_t *roaming_odb)
{
    cJSON *item = NULL;

    if (roaming_odb == NULL) {
        ogs_error("OpenAPI_roaming_odb_convertToJSON() failed [RoamingOdb]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_parseFromJSON(cJSON *roaming_odbJSON)
{
    OpenAPI_roaming_odb_t *roaming_odb_local_var = NULL;
    roaming_odb_local_var = OpenAPI_roaming_odb_create (
        );

    return roaming_odb_local_var;
end:
    return NULL;
}

OpenAPI_roaming_odb_t *OpenAPI_roaming_odb_copy(OpenAPI_roaming_odb_t *dst, OpenAPI_roaming_odb_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_roaming_odb_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_roaming_odb_convertToJSON() failed");
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

    OpenAPI_roaming_odb_free(dst);
    dst = OpenAPI_roaming_odb_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

