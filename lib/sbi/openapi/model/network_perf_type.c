
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "network_perf_type.h"

OpenAPI_network_perf_type_t *OpenAPI_network_perf_type_create(
)
{
    OpenAPI_network_perf_type_t *network_perf_type_local_var = ogs_malloc(sizeof(OpenAPI_network_perf_type_t));
    ogs_assert(network_perf_type_local_var);


    return network_perf_type_local_var;
}

void OpenAPI_network_perf_type_free(OpenAPI_network_perf_type_t *network_perf_type)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == network_perf_type) {
        return;
    }
    ogs_free(network_perf_type);
}

cJSON *OpenAPI_network_perf_type_convertToJSON(OpenAPI_network_perf_type_t *network_perf_type)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (network_perf_type == NULL) {
        ogs_error("OpenAPI_network_perf_type_convertToJSON() failed [NetworkPerfType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_network_perf_type_t *OpenAPI_network_perf_type_parseFromJSON(cJSON *network_perf_typeJSON)
{
    OpenAPI_network_perf_type_t *network_perf_type_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    network_perf_type_local_var = OpenAPI_network_perf_type_create (
    );

    return network_perf_type_local_var;
end:
    return NULL;
}

OpenAPI_network_perf_type_t *OpenAPI_network_perf_type_copy(OpenAPI_network_perf_type_t *dst, OpenAPI_network_perf_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_network_perf_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_network_perf_type_convertToJSON() failed");
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

    OpenAPI_network_perf_type_free(dst);
    dst = OpenAPI_network_perf_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

