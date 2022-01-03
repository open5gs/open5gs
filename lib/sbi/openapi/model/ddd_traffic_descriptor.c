
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ddd_traffic_descriptor.h"

OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_create(
    char *ipv4_addr,
    char *ipv6_addr,
    bool is_port_number,
    int port_number,
    char *mac_addr
)
{
    OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor_local_var = ogs_malloc(sizeof(OpenAPI_ddd_traffic_descriptor_t));
    ogs_assert(ddd_traffic_descriptor_local_var);

    ddd_traffic_descriptor_local_var->ipv4_addr = ipv4_addr;
    ddd_traffic_descriptor_local_var->ipv6_addr = ipv6_addr;
    ddd_traffic_descriptor_local_var->is_port_number = is_port_number;
    ddd_traffic_descriptor_local_var->port_number = port_number;
    ddd_traffic_descriptor_local_var->mac_addr = mac_addr;

    return ddd_traffic_descriptor_local_var;
}

void OpenAPI_ddd_traffic_descriptor_free(OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor)
{
    if (NULL == ddd_traffic_descriptor) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ddd_traffic_descriptor->ipv4_addr);
    ogs_free(ddd_traffic_descriptor->ipv6_addr);
    ogs_free(ddd_traffic_descriptor->mac_addr);
    ogs_free(ddd_traffic_descriptor);
}

cJSON *OpenAPI_ddd_traffic_descriptor_convertToJSON(OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor)
{
    cJSON *item = NULL;

    if (ddd_traffic_descriptor == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed [DddTrafficDescriptor]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ddd_traffic_descriptor->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", ddd_traffic_descriptor->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", ddd_traffic_descriptor->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor->is_port_number) {
    if (cJSON_AddNumberToObject(item, "portNumber", ddd_traffic_descriptor->port_number) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed [port_number]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor->mac_addr) {
    if (cJSON_AddStringToObject(item, "macAddr", ddd_traffic_descriptor->mac_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed [mac_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_parseFromJSON(cJSON *ddd_traffic_descriptorJSON)
{
    OpenAPI_ddd_traffic_descriptor_t *ddd_traffic_descriptor_local_var = NULL;
    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptorJSON, "ipv4Addr");

    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    cJSON *ipv6_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptorJSON, "ipv6Addr");

    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    cJSON *port_number = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptorJSON, "portNumber");

    if (port_number) {
    if (!cJSON_IsNumber(port_number)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_parseFromJSON() failed [port_number]");
        goto end;
    }
    }

    cJSON *mac_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptorJSON, "macAddr");

    if (mac_addr) {
    if (!cJSON_IsString(mac_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_parseFromJSON() failed [mac_addr]");
        goto end;
    }
    }

    ddd_traffic_descriptor_local_var = OpenAPI_ddd_traffic_descriptor_create (
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        port_number ? true : false,
        port_number ? port_number->valuedouble : 0,
        mac_addr ? ogs_strdup(mac_addr->valuestring) : NULL
    );

    return ddd_traffic_descriptor_local_var;
end:
    return NULL;
}

OpenAPI_ddd_traffic_descriptor_t *OpenAPI_ddd_traffic_descriptor_copy(OpenAPI_ddd_traffic_descriptor_t *dst, OpenAPI_ddd_traffic_descriptor_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ddd_traffic_descriptor_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_convertToJSON() failed");
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

    OpenAPI_ddd_traffic_descriptor_free(dst);
    dst = OpenAPI_ddd_traffic_descriptor_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

