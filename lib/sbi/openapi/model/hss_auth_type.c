
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_auth_type.h"

OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_create(
    )
{
    OpenAPI_hss_auth_type_t *hss_auth_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_hss_auth_type_t));
    if (!hss_auth_type_local_var) {
        return NULL;
    }

    return hss_auth_type_local_var;
}

void OpenAPI_hss_auth_type_free(OpenAPI_hss_auth_type_t *hss_auth_type)
{
    if (NULL == hss_auth_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(hss_auth_type);
}

cJSON *OpenAPI_hss_auth_type_convertToJSON(OpenAPI_hss_auth_type_t *hss_auth_type)
{
    cJSON *item = NULL;

    if (hss_auth_type == NULL) {
        ogs_error("OpenAPI_hss_auth_type_convertToJSON() failed [HssAuthType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_parseFromJSON(cJSON *hss_auth_typeJSON)
{
    OpenAPI_hss_auth_type_t *hss_auth_type_local_var = NULL;
    hss_auth_type_local_var = OpenAPI_hss_auth_type_create (
        );

    return hss_auth_type_local_var;
end:
    return NULL;
}

OpenAPI_hss_auth_type_t *OpenAPI_hss_auth_type_copy(OpenAPI_hss_auth_type_t *dst, OpenAPI_hss_auth_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_auth_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_auth_type_convertToJSON() failed");
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

    OpenAPI_hss_auth_type_free(dst);
    dst = OpenAPI_hss_auth_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

