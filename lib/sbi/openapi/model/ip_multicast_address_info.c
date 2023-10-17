
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_multicast_address_info.h"

OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_create(
    char *src_ipv4_addr,
    char *ipv4_mul_addr,
    char *src_ipv6_addr,
    char *ipv6_mul_addr
)
{
    OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info_local_var = ogs_malloc(sizeof(OpenAPI_ip_multicast_address_info_t));
    ogs_assert(ip_multicast_address_info_local_var);

    ip_multicast_address_info_local_var->src_ipv4_addr = src_ipv4_addr;
    ip_multicast_address_info_local_var->ipv4_mul_addr = ipv4_mul_addr;
    ip_multicast_address_info_local_var->src_ipv6_addr = src_ipv6_addr;
    ip_multicast_address_info_local_var->ipv6_mul_addr = ipv6_mul_addr;

    return ip_multicast_address_info_local_var;
}

void OpenAPI_ip_multicast_address_info_free(OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_multicast_address_info) {
        return;
    }
    if (ip_multicast_address_info->src_ipv4_addr) {
        ogs_free(ip_multicast_address_info->src_ipv4_addr);
        ip_multicast_address_info->src_ipv4_addr = NULL;
    }
    if (ip_multicast_address_info->ipv4_mul_addr) {
        ogs_free(ip_multicast_address_info->ipv4_mul_addr);
        ip_multicast_address_info->ipv4_mul_addr = NULL;
    }
    if (ip_multicast_address_info->src_ipv6_addr) {
        ogs_free(ip_multicast_address_info->src_ipv6_addr);
        ip_multicast_address_info->src_ipv6_addr = NULL;
    }
    if (ip_multicast_address_info->ipv6_mul_addr) {
        ogs_free(ip_multicast_address_info->ipv6_mul_addr);
        ip_multicast_address_info->ipv6_mul_addr = NULL;
    }
    ogs_free(ip_multicast_address_info);
}

cJSON *OpenAPI_ip_multicast_address_info_convertToJSON(OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ip_multicast_address_info == NULL) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed [IpMulticastAddressInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_multicast_address_info->src_ipv4_addr) {
    if (cJSON_AddStringToObject(item, "srcIpv4Addr", ip_multicast_address_info->src_ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed [src_ipv4_addr]");
        goto end;
    }
    }

    if (ip_multicast_address_info->ipv4_mul_addr) {
    if (cJSON_AddStringToObject(item, "ipv4MulAddr", ip_multicast_address_info->ipv4_mul_addr) == NULL) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed [ipv4_mul_addr]");
        goto end;
    }
    }

    if (ip_multicast_address_info->src_ipv6_addr) {
    if (cJSON_AddStringToObject(item, "srcIpv6Addr", ip_multicast_address_info->src_ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed [src_ipv6_addr]");
        goto end;
    }
    }

    if (ip_multicast_address_info->ipv6_mul_addr) {
    if (cJSON_AddStringToObject(item, "ipv6MulAddr", ip_multicast_address_info->ipv6_mul_addr) == NULL) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed [ipv6_mul_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_parseFromJSON(cJSON *ip_multicast_address_infoJSON)
{
    OpenAPI_ip_multicast_address_info_t *ip_multicast_address_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *src_ipv4_addr = NULL;
    cJSON *ipv4_mul_addr = NULL;
    cJSON *src_ipv6_addr = NULL;
    cJSON *ipv6_mul_addr = NULL;
    src_ipv4_addr = cJSON_GetObjectItemCaseSensitive(ip_multicast_address_infoJSON, "srcIpv4Addr");
    if (src_ipv4_addr) {
    if (!cJSON_IsString(src_ipv4_addr) && !cJSON_IsNull(src_ipv4_addr)) {
        ogs_error("OpenAPI_ip_multicast_address_info_parseFromJSON() failed [src_ipv4_addr]");
        goto end;
    }
    }

    ipv4_mul_addr = cJSON_GetObjectItemCaseSensitive(ip_multicast_address_infoJSON, "ipv4MulAddr");
    if (ipv4_mul_addr) {
    if (!cJSON_IsString(ipv4_mul_addr) && !cJSON_IsNull(ipv4_mul_addr)) {
        ogs_error("OpenAPI_ip_multicast_address_info_parseFromJSON() failed [ipv4_mul_addr]");
        goto end;
    }
    }

    src_ipv6_addr = cJSON_GetObjectItemCaseSensitive(ip_multicast_address_infoJSON, "srcIpv6Addr");
    if (src_ipv6_addr) {
    if (!cJSON_IsString(src_ipv6_addr) && !cJSON_IsNull(src_ipv6_addr)) {
        ogs_error("OpenAPI_ip_multicast_address_info_parseFromJSON() failed [src_ipv6_addr]");
        goto end;
    }
    }

    ipv6_mul_addr = cJSON_GetObjectItemCaseSensitive(ip_multicast_address_infoJSON, "ipv6MulAddr");
    if (ipv6_mul_addr) {
    if (!cJSON_IsString(ipv6_mul_addr) && !cJSON_IsNull(ipv6_mul_addr)) {
        ogs_error("OpenAPI_ip_multicast_address_info_parseFromJSON() failed [ipv6_mul_addr]");
        goto end;
    }
    }

    ip_multicast_address_info_local_var = OpenAPI_ip_multicast_address_info_create (
        src_ipv4_addr && !cJSON_IsNull(src_ipv4_addr) ? ogs_strdup(src_ipv4_addr->valuestring) : NULL,
        ipv4_mul_addr && !cJSON_IsNull(ipv4_mul_addr) ? ogs_strdup(ipv4_mul_addr->valuestring) : NULL,
        src_ipv6_addr && !cJSON_IsNull(src_ipv6_addr) ? ogs_strdup(src_ipv6_addr->valuestring) : NULL,
        ipv6_mul_addr && !cJSON_IsNull(ipv6_mul_addr) ? ogs_strdup(ipv6_mul_addr->valuestring) : NULL
    );

    return ip_multicast_address_info_local_var;
end:
    return NULL;
}

OpenAPI_ip_multicast_address_info_t *OpenAPI_ip_multicast_address_info_copy(OpenAPI_ip_multicast_address_info_t *dst, OpenAPI_ip_multicast_address_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_multicast_address_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_multicast_address_info_convertToJSON() failed");
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

    OpenAPI_ip_multicast_address_info_free(dst);
    dst = OpenAPI_ip_multicast_address_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

