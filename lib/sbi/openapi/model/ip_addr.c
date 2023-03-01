
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_addr.h"

OpenAPI_ip_addr_t *OpenAPI_ip_addr_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *ipv6_prefix
)
{
    OpenAPI_ip_addr_t *ip_addr_local_var = ogs_malloc(sizeof(OpenAPI_ip_addr_t));
    ogs_assert(ip_addr_local_var);

    ip_addr_local_var->ipv4_addr = ipv4_addr;
    ip_addr_local_var->ipv6_addr = ipv6_addr;
    ip_addr_local_var->ipv6_prefix = ipv6_prefix;

    return ip_addr_local_var;
}

void OpenAPI_ip_addr_free(OpenAPI_ip_addr_t *ip_addr)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_addr) {
        return;
    }
    if (ip_addr->ipv4_addr) {
        ogs_free(ip_addr->ipv4_addr);
        ip_addr->ipv4_addr = NULL;
    }
    if (ip_addr->ipv6_addr) {
        ogs_free(ip_addr->ipv6_addr);
        ip_addr->ipv6_addr = NULL;
    }
    if (ip_addr->ipv6_prefix) {
        ogs_free(ip_addr->ipv6_prefix);
        ip_addr->ipv6_prefix = NULL;
    }
    ogs_free(ip_addr);
}

cJSON *OpenAPI_ip_addr_convertToJSON(OpenAPI_ip_addr_t *ip_addr)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_addr == NULL) {
        ogs_error("OpenAPI_ip_addr_convertToJSON() failed [IpAddr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_addr->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", ip_addr->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ip_addr_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (ip_addr->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", ip_addr->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ip_addr_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (ip_addr->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", ip_addr->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_ip_addr_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_addr_t *OpenAPI_ip_addr_parseFromJSON(cJSON *ip_addrJSON)
{
    OpenAPI_ip_addr_t *ip_addr_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *ipv6_prefix = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(ip_addrJSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(ip_addrJSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    ipv6_prefix = cJSON_GetObjectItemCaseSensitive(ip_addrJSON, "ipv6Prefix");
    if (ipv6_prefix) {
    if (!cJSON_IsString(ipv6_prefix) && !cJSON_IsNull(ipv6_prefix)) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    ip_addr_local_var = OpenAPI_ip_addr_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        ipv6_prefix && !cJSON_IsNull(ipv6_prefix) ? ogs_strdup(ipv6_prefix->valuestring) : NULL
    );

    return ip_addr_local_var;
end:
    return NULL;
}

OpenAPI_ip_addr_t *OpenAPI_ip_addr_copy(OpenAPI_ip_addr_t *dst, OpenAPI_ip_addr_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_addr_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_addr_convertToJSON() failed");
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

    OpenAPI_ip_addr_free(dst);
    dst = OpenAPI_ip_addr_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

