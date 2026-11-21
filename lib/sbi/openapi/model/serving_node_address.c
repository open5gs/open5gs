
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "serving_node_address.h"

OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_create(
    char *ipv4_addr,
    char *ipv6_addr
)
{
    OpenAPI_serving_node_address_t *serving_node_address_local_var = ogs_malloc(sizeof(OpenAPI_serving_node_address_t));
    ogs_assert(serving_node_address_local_var);

    serving_node_address_local_var->ipv4_addr = ipv4_addr;
    serving_node_address_local_var->ipv6_addr = ipv6_addr;

    return serving_node_address_local_var;
}

void OpenAPI_serving_node_address_free(OpenAPI_serving_node_address_t *serving_node_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == serving_node_address) {
        return;
    }
    if (serving_node_address->ipv4_addr) {
        ogs_free(serving_node_address->ipv4_addr);
        serving_node_address->ipv4_addr = NULL;
    }
    if (serving_node_address->ipv6_addr) {
        ogs_free(serving_node_address->ipv6_addr);
        serving_node_address->ipv6_addr = NULL;
    }
    ogs_free(serving_node_address);
}

cJSON *OpenAPI_serving_node_address_convertToJSON(OpenAPI_serving_node_address_t *serving_node_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (serving_node_address == NULL) {
        ogs_error("OpenAPI_serving_node_address_convertToJSON() failed [ServingNodeAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (serving_node_address->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", serving_node_address->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_serving_node_address_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (serving_node_address->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", serving_node_address->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_serving_node_address_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_parseFromJSON(cJSON *serving_node_addressJSON)
{
    OpenAPI_serving_node_address_t *serving_node_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(serving_node_addressJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_serving_node_address_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(serving_node_addressJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_serving_node_address_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    serving_node_address_local_var = OpenAPI_serving_node_address_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL
    );

    return serving_node_address_local_var;
end:
    return NULL;
}

OpenAPI_serving_node_address_t *OpenAPI_serving_node_address_copy(OpenAPI_serving_node_address_t *dst, OpenAPI_serving_node_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_serving_node_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_serving_node_address_convertToJSON() failed");
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

    OpenAPI_serving_node_address_free(dst);
    dst = OpenAPI_serving_node_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

