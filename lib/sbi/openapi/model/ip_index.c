
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_index.h"

OpenAPI_ip_index_t *OpenAPI_ip_index_create(
)
{
    OpenAPI_ip_index_t *ip_index_local_var = ogs_malloc(sizeof(OpenAPI_ip_index_t));
    ogs_assert(ip_index_local_var);


    return ip_index_local_var;
}

void OpenAPI_ip_index_free(OpenAPI_ip_index_t *ip_index)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_index) {
        return;
    }
    ogs_free(ip_index);
}

cJSON *OpenAPI_ip_index_convertToJSON(OpenAPI_ip_index_t *ip_index)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_index == NULL) {
        ogs_error("OpenAPI_ip_index_convertToJSON() failed [IpIndex]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_ip_index_t *OpenAPI_ip_index_parseFromJSON(cJSON *ip_indexJSON)
{
    OpenAPI_ip_index_t *ip_index_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    ip_index_local_var = OpenAPI_ip_index_create (
    );

    return ip_index_local_var;
end:
    return NULL;
}

OpenAPI_ip_index_t *OpenAPI_ip_index_copy(OpenAPI_ip_index_t *dst, OpenAPI_ip_index_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_index_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_index_convertToJSON() failed");
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

    OpenAPI_ip_index_free(dst);
    dst = OpenAPI_ip_index_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

