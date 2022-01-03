
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ipv4_address_range.h"

OpenAPI_ipv4_address_range_t *OpenAPI_ipv4_address_range_create(
    char *start,
    char *end
)
{
    OpenAPI_ipv4_address_range_t *ipv4_address_range_local_var = ogs_malloc(sizeof(OpenAPI_ipv4_address_range_t));
    ogs_assert(ipv4_address_range_local_var);

    ipv4_address_range_local_var->start = start;
    ipv4_address_range_local_var->end = end;

    return ipv4_address_range_local_var;
}

void OpenAPI_ipv4_address_range_free(OpenAPI_ipv4_address_range_t *ipv4_address_range)
{
    if (NULL == ipv4_address_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ipv4_address_range->start);
    ogs_free(ipv4_address_range->end);
    ogs_free(ipv4_address_range);
}

cJSON *OpenAPI_ipv4_address_range_convertToJSON(OpenAPI_ipv4_address_range_t *ipv4_address_range)
{
    cJSON *item = NULL;

    if (ipv4_address_range == NULL) {
        ogs_error("OpenAPI_ipv4_address_range_convertToJSON() failed [Ipv4AddressRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ipv4_address_range->start) {
    if (cJSON_AddStringToObject(item, "start", ipv4_address_range->start) == NULL) {
        ogs_error("OpenAPI_ipv4_address_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (ipv4_address_range->end) {
    if (cJSON_AddStringToObject(item, "end", ipv4_address_range->end) == NULL) {
        ogs_error("OpenAPI_ipv4_address_range_convertToJSON() failed [end]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ipv4_address_range_t *OpenAPI_ipv4_address_range_parseFromJSON(cJSON *ipv4_address_rangeJSON)
{
    OpenAPI_ipv4_address_range_t *ipv4_address_range_local_var = NULL;
    cJSON *start = cJSON_GetObjectItemCaseSensitive(ipv4_address_rangeJSON, "start");

    if (start) {
    if (!cJSON_IsString(start)) {
        ogs_error("OpenAPI_ipv4_address_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(ipv4_address_rangeJSON, "end");

    if (end) {
    if (!cJSON_IsString(end)) {
        ogs_error("OpenAPI_ipv4_address_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    ipv4_address_range_local_var = OpenAPI_ipv4_address_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL
    );

    return ipv4_address_range_local_var;
end:
    return NULL;
}

OpenAPI_ipv4_address_range_t *OpenAPI_ipv4_address_range_copy(OpenAPI_ipv4_address_range_t *dst, OpenAPI_ipv4_address_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ipv4_address_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ipv4_address_range_convertToJSON() failed");
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

    OpenAPI_ipv4_address_range_free(dst);
    dst = OpenAPI_ipv4_address_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

