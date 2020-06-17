
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "route_information.h"

OpenAPI_route_information_t *OpenAPI_route_information_create(
    char *ipv4_addr,
    char *ipv6_addr,
    int port_number
    )
{
    OpenAPI_route_information_t *route_information_local_var = OpenAPI_malloc(sizeof(OpenAPI_route_information_t));
    if (!route_information_local_var) {
        return NULL;
    }
    route_information_local_var->ipv4_addr = ipv4_addr;
    route_information_local_var->ipv6_addr = ipv6_addr;
    route_information_local_var->port_number = port_number;

    return route_information_local_var;
}

void OpenAPI_route_information_free(OpenAPI_route_information_t *route_information)
{
    if (NULL == route_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(route_information->ipv4_addr);
    ogs_free(route_information->ipv6_addr);
    ogs_free(route_information);
}

cJSON *OpenAPI_route_information_convertToJSON(OpenAPI_route_information_t *route_information)
{
    cJSON *item = NULL;

    if (route_information == NULL) {
        ogs_error("OpenAPI_route_information_convertToJSON() failed [RouteInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (route_information->ipv4_addr) {
        if (cJSON_AddStringToObject(item, "ipv4Addr", route_information->ipv4_addr) == NULL) {
            ogs_error("OpenAPI_route_information_convertToJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    if (route_information->ipv6_addr) {
        if (cJSON_AddStringToObject(item, "ipv6Addr", route_information->ipv6_addr) == NULL) {
            ogs_error("OpenAPI_route_information_convertToJSON() failed [ipv6_addr]");
            goto end;
        }
    }

    if (!route_information->port_number) {
        ogs_error("OpenAPI_route_information_convertToJSON() failed [port_number]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "portNumber", route_information->port_number) == NULL) {
        ogs_error("OpenAPI_route_information_convertToJSON() failed [port_number]");
        goto end;
    }

end:
    return item;
}

OpenAPI_route_information_t *OpenAPI_route_information_parseFromJSON(cJSON *route_informationJSON)
{
    OpenAPI_route_information_t *route_information_local_var = NULL;
    cJSON *ipv4_addr = cJSON_GetObjectItemCaseSensitive(route_informationJSON, "ipv4Addr");

    if (ipv4_addr) {
        if (!cJSON_IsString(ipv4_addr)) {
            ogs_error("OpenAPI_route_information_parseFromJSON() failed [ipv4_addr]");
            goto end;
        }
    }

    cJSON *ipv6_addr = cJSON_GetObjectItemCaseSensitive(route_informationJSON, "ipv6Addr");

    if (ipv6_addr) {
        if (!cJSON_IsString(ipv6_addr)) {
            ogs_error("OpenAPI_route_information_parseFromJSON() failed [ipv6_addr]");
            goto end;
        }
    }

    cJSON *port_number = cJSON_GetObjectItemCaseSensitive(route_informationJSON, "portNumber");
    if (!port_number) {
        ogs_error("OpenAPI_route_information_parseFromJSON() failed [port_number]");
        goto end;
    }


    if (!cJSON_IsNumber(port_number)) {
        ogs_error("OpenAPI_route_information_parseFromJSON() failed [port_number]");
        goto end;
    }

    route_information_local_var = OpenAPI_route_information_create (
        ipv4_addr ? ogs_strdup(ipv4_addr->valuestring) : NULL,
        ipv6_addr ? ogs_strdup(ipv6_addr->valuestring) : NULL,
        port_number->valuedouble
        );

    return route_information_local_var;
end:
    return NULL;
}

OpenAPI_route_information_t *OpenAPI_route_information_copy(OpenAPI_route_information_t *dst, OpenAPI_route_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_route_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_route_information_convertToJSON() failed");
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

    OpenAPI_route_information_free(dst);
    dst = OpenAPI_route_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

