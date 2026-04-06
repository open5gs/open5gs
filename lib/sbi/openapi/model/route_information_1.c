
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "route_information_1.h"

OpenAPI_route_information_1_t *OpenAPI_route_information_1_create(
    char *ipv4_addr,
    char *ipv6_addr,
    int port_number
)
{
    OpenAPI_route_information_1_t *route_information_1_local_var = ogs_malloc(sizeof(OpenAPI_route_information_1_t));
    ogs_assert(route_information_1_local_var);

    route_information_1_local_var->ipv4_addr = ipv4_addr;
    route_information_1_local_var->ipv6_addr = ipv6_addr;
    route_information_1_local_var->port_number = port_number;

    return route_information_1_local_var;
}

void OpenAPI_route_information_1_free(OpenAPI_route_information_1_t *route_information_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == route_information_1) {
        return;
    }
    if (route_information_1->ipv4_addr) {
        ogs_free(route_information_1->ipv4_addr);
        route_information_1->ipv4_addr = NULL;
    }
    if (route_information_1->ipv6_addr) {
        ogs_free(route_information_1->ipv6_addr);
        route_information_1->ipv6_addr = NULL;
    }
    ogs_free(route_information_1);
}

cJSON *OpenAPI_route_information_1_convertToJSON(OpenAPI_route_information_1_t *route_information_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (route_information_1 == NULL) {
        ogs_error("OpenAPI_route_information_1_convertToJSON() failed [RouteInformation_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (route_information_1->ipv4_addr) {
    if (cJSON_AddStringToObject(item, "ipv4Addr", route_information_1->ipv4_addr) == NULL) {
        ogs_error("OpenAPI_route_information_1_convertToJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    if (route_information_1->ipv6_addr) {
    if (cJSON_AddStringToObject(item, "ipv6Addr", route_information_1->ipv6_addr) == NULL) {
        ogs_error("OpenAPI_route_information_1_convertToJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    if (cJSON_AddNumberToObject(item, "portNumber", route_information_1->port_number) == NULL) {
        ogs_error("OpenAPI_route_information_1_convertToJSON() failed [port_number]");
        goto end;
    }

end:
    return item;
}

OpenAPI_route_information_1_t *OpenAPI_route_information_1_parseFromJSON(cJSON *route_information_1JSON)
{
    OpenAPI_route_information_1_t *route_information_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_addr = NULL;
    cJSON *ipv6_addr = NULL;
    cJSON *port_number = NULL;
    ipv4_addr = cJSON_GetObjectItemCaseSensitive(route_information_1JSON, "ipv4Addr");
    if (ipv4_addr) {
    if (!cJSON_IsString(ipv4_addr) && !cJSON_IsNull(ipv4_addr)) {
        ogs_error("OpenAPI_route_information_1_parseFromJSON() failed [ipv4_addr]");
        goto end;
    }
    }

    ipv6_addr = cJSON_GetObjectItemCaseSensitive(route_information_1JSON, "ipv6Addr");
    if (ipv6_addr) {
    if (!cJSON_IsString(ipv6_addr) && !cJSON_IsNull(ipv6_addr)) {
        ogs_error("OpenAPI_route_information_1_parseFromJSON() failed [ipv6_addr]");
        goto end;
    }
    }

    port_number = cJSON_GetObjectItemCaseSensitive(route_information_1JSON, "portNumber");
    if (!port_number) {
        ogs_error("OpenAPI_route_information_1_parseFromJSON() failed [port_number]");
        goto end;
    }
    if (!cJSON_IsNumber(port_number)) {
        ogs_error("OpenAPI_route_information_1_parseFromJSON() failed [port_number]");
        goto end;
    }

    route_information_1_local_var = OpenAPI_route_information_1_create (
        ipv4_addr && !cJSON_IsNull(ipv4_addr) ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr && !cJSON_IsNull(ipv6_addr) ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        
        port_number->valuedouble
    );

    return route_information_1_local_var;
end:
    return NULL;
}

OpenAPI_route_information_1_t *OpenAPI_route_information_1_copy(OpenAPI_route_information_1_t *dst, OpenAPI_route_information_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_route_information_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_route_information_1_convertToJSON() failed");
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

    OpenAPI_route_information_1_free(dst);
    dst = OpenAPI_route_information_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

