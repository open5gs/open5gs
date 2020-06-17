
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_end_point.h"

OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_create(
    char *ipv4_address,
    char *ipv6_address,
    OpenAPI_transport_protocol_t *transport,
    int port
    )
{
    OpenAPI_ip_end_point_t *ip_end_point_local_var = OpenAPI_malloc(sizeof(OpenAPI_ip_end_point_t));
    if (!ip_end_point_local_var) {
        return NULL;
    }
    ip_end_point_local_var->ipv4_address = ipv4_address;
    ip_end_point_local_var->ipv6_address = ipv6_address;
    ip_end_point_local_var->transport = transport;
    ip_end_point_local_var->port = port;

    return ip_end_point_local_var;
}

void OpenAPI_ip_end_point_free(OpenAPI_ip_end_point_t *ip_end_point)
{
    if (NULL == ip_end_point) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ip_end_point->ipv4_address);
    ogs_free(ip_end_point->ipv6_address);
    OpenAPI_transport_protocol_free(ip_end_point->transport);
    ogs_free(ip_end_point);
}

cJSON *OpenAPI_ip_end_point_convertToJSON(OpenAPI_ip_end_point_t *ip_end_point)
{
    cJSON *item = NULL;

    if (ip_end_point == NULL) {
        ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [IpEndPoint]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ip_end_point->ipv4_address) {
        if (cJSON_AddStringToObject(item, "ipv4Address", ip_end_point->ipv4_address) == NULL) {
            ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [ipv4_address]");
            goto end;
        }
    }

    if (ip_end_point->ipv6_address) {
        if (cJSON_AddStringToObject(item, "ipv6Address", ip_end_point->ipv6_address) == NULL) {
            ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [ipv6_address]");
            goto end;
        }
    }

    if (ip_end_point->transport) {
        cJSON *transport_local_JSON = OpenAPI_transport_protocol_convertToJSON(ip_end_point->transport);
        if (transport_local_JSON == NULL) {
            ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [transport]");
            goto end;
        }
        cJSON_AddItemToObject(item, "transport", transport_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [transport]");
            goto end;
        }
    }

    if (ip_end_point->port) {
        if (cJSON_AddNumberToObject(item, "port", ip_end_point->port) == NULL) {
            ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [port]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_parseFromJSON(cJSON *ip_end_pointJSON)
{
    OpenAPI_ip_end_point_t *ip_end_point_local_var = NULL;
    cJSON *ipv4_address = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "ipv4Address");

    if (ipv4_address) {
        if (!cJSON_IsString(ipv4_address)) {
            ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [ipv4_address]");
            goto end;
        }
    }

    cJSON *ipv6_address = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "ipv6Address");

    if (ipv6_address) {
        if (!cJSON_IsString(ipv6_address)) {
            ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [ipv6_address]");
            goto end;
        }
    }

    cJSON *transport = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "transport");

    OpenAPI_transport_protocol_t *transport_local_nonprim = NULL;
    if (transport) {
        transport_local_nonprim = OpenAPI_transport_protocol_parseFromJSON(transport);
    }

    cJSON *port = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "port");

    if (port) {
        if (!cJSON_IsNumber(port)) {
            ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [port]");
            goto end;
        }
    }

    ip_end_point_local_var = OpenAPI_ip_end_point_create (
        ipv4_address ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ipv6_address ? ogs_strdup(ipv6_address->valuestring) : NULL,
        transport ? transport_local_nonprim : NULL,
        port ? port->valuedouble : 0
        );

    return ip_end_point_local_var;
end:
    return NULL;
}

OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_copy(OpenAPI_ip_end_point_t *dst, OpenAPI_ip_end_point_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ip_end_point_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ip_end_point_convertToJSON() failed");
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

    OpenAPI_ip_end_point_free(dst);
    dst = OpenAPI_ip_end_point_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

