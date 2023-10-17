
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ddd_traffic_descriptor_1.h"

OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    bool is_port_number,
    int port_number,
    char *mac_addr
)
{
    OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1_local_var = ogs_malloc(sizeof(OpenAPI_ddd_traffic_descriptor_1_t));
    ogs_assert(ddd_traffic_descriptor_1_local_var);

    ddd_traffic_descriptor_1_local_var->ipv4_addr = ipv4_addr;
    ddd_traffic_descriptor_1_local_var->ipv6_addr = ipv6_addr;
    ddd_traffic_descriptor_1_local_var->is_port_number = is_port_number;
    ddd_traffic_descriptor_1_local_var->port_number = port_number;
    ddd_traffic_descriptor_1_local_var->mac_addr = mac_addr;

    return ddd_traffic_descriptor_1_local_var;
}

void OpenAPI_ddd_traffic_descriptor_1_free(OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ddd_traffic_descriptor_1) {
        return;
    }
    if (ddd_traffic_descriptor_1->ipv4_addr) {
        ogs_free(ddd_traffic_descriptor_1->ipv4_addr);
        ddd_traffic_descriptor_1->ipv4_addr = NULL;
    }
    if (ddd_traffic_descriptor_1->ipv6_addr) {
        ogs_free(ddd_traffic_descriptor_1->ipv6_addr);
        ddd_traffic_descriptor_1->ipv6_addr = NULL;
    }
    if (ddd_traffic_descriptor_1->mac_addr) {
        ogs_free(ddd_traffic_descriptor_1->mac_addr);
        ddd_traffic_descriptor_1->mac_addr = NULL;
    }
    ogs_free(ddd_traffic_descriptor_1);
}

cJSON *OpenAPI_ddd_traffic_descriptor_1_convertToJSON(OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ddd_traffic_descriptor_1 == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed [DddTrafficDescriptor_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ddd_traffic_descriptor_1->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", ddd_traffic_descriptor_1->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor_1->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", ddd_traffic_descriptor_1->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor_1->is_port_number) {
    if (cJSON_AddNumberToObject(item, "portNumber", ddd_traffic_descriptor_1->port_number) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed [port_number]");
        goto end;
    }
    }

    if (ddd_traffic_descriptor_1->mac_addr) {
    if (cJSON_AddStringToObject(item, "macAddr", ddd_traffic_descriptor_1->mac_addr) == NULL) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed [mac_addr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_parseFromJSON(cJSON *ddd_traffic_descriptor_1JSON)
{
    OpenAPI_ddd_traffic_descriptor_1_t *ddd_traffic_descriptor_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *port_number = NULL;
    cJSON *mac_addr = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptor_1JSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptor_1JSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    port_number = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptor_1JSON, "portNumber");
    if (port_number) {
    if (!cJSON_IsNumber(port_number)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_parseFromJSON() failed [port_number]");
        goto end;
    }
    }

    mac_addr = cJSON_GetObjectItemCaseSensitive(ddd_traffic_descriptor_1JSON, "macAddr");
    if (mac_addr) {
    if (!cJSON_IsString(mac_addr) && !cJSON_IsNull(mac_addr)) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_parseFromJSON() failed [mac_addr]");
        goto end;
    }
    }

    ddd_traffic_descriptor_1_local_var = OpenAPI_ddd_traffic_descriptor_1_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        port_number ? true : false,
        port_number ? port_number->valuedouble : 0,
        mac_addr && !cJSON_IsNull(mac_addr) ? ogs_strdup(mac_addr->valuestring) : NULL
    );

    return ddd_traffic_descriptor_1_local_var;
end:
    return NULL;
}

OpenAPI_ddd_traffic_descriptor_1_t *OpenAPI_ddd_traffic_descriptor_1_copy(OpenAPI_ddd_traffic_descriptor_1_t *dst, OpenAPI_ddd_traffic_descriptor_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ddd_traffic_descriptor_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ddd_traffic_descriptor_1_convertToJSON() failed");
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

    OpenAPI_ddd_traffic_descriptor_1_free(dst);
    dst = OpenAPI_ddd_traffic_descriptor_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

