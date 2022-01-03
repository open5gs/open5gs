
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_address_1.h"

OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    char *ipv6_prefix
)
{
    OpenAPI_ip_address_1_t *ip_address_1_local_var = ogs_malloc(sizeof(OpenAPI_ip_address_1_t));
    ogs_assert(ip_address_1_local_var);

    ip_address_1_local_var->ipv4_addr = ipv4_addr;
    ip_address_1_local_var->ipv6_addr = ipv6_addr;
    ip_address_1_local_var->ipv6_prefix = ipv6_prefix;

    return ip_address_1_local_var;
}

void OpenAPI_ip_address_1_free(OpenAPI_ip_address_1_t *ip_address_1)
{
    if (NULL == ip_address_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ip_address_1->ipv4_addr);
    ogs_free(ip_address_1->ipv6_addr);
    ogs_free(ip_address_1->ipv6_prefix);
    ogs_free(ip_address_1);
}

cJSON *OpenAPI_ip_address_1_convertToJSON(OpenAPI_ip_address_1_t *ip_address_1)
{
    cJSON *item = NULL;

    if (ip_address_1 == NULL) {
        ogs_error("OpenAPI_ip_address_1_convertToJSON() failed [IpAddress_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_address_1->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", ip_address_1->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ip_address_1_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (ip_address_1->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", ip_address_1->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ip_address_1_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (ip_address_1->ipv6_prefix) {
    if (cJSON_AddStringToObject(item, "ipv6Prefix", ip_address_1->ipv6_prefix) == NULL) {
        ogs_error("OpenAPI_ip_address_1_convertToJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_parseFromJSON(cJSON *ip_address_1JSON)
{
    OpenAPI_ip_address_1_t *ip_address_1_local_var = NULL;
    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(ip_address_1JSON, "ipv4Addr");

    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr)) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    cJSON *ipv6_addr = cJSON_GetObjectItemCaseSensitive(ip_address_1JSON, "ipv6Addr");

    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr)) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    cJSON *ipv6_prefix = cJSON_GetObjectItemCaseSensitive(ip_address_1JSON, "ipv6Prefix");

    if (ipv6_prefix) {
    if (!cJSON_IsString(ipv6_prefix)) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON() failed [ipv6_prefix]");
        goto end;
    }
    }

    ip_address_1_local_var = OpenAPI_ip_address_1_create (
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        ipv6_prefix ? ogs_strdup(ipv6_prefix->valuestring) : NULL
    );

    return ip_address_1_local_var;
end:
    return NULL;
}

OpenAPI_ip_address_1_t *OpenAPI_ip_address_1_copy(OpenAPI_ip_address_1_t *dst, OpenAPI_ip_address_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_address_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_address_1_convertToJSON() failed");
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

    OpenAPI_ip_address_1_free(dst);
    dst = OpenAPI_ip_address_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

