
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ip_end_point.h"

OpenAPI_ip_end_point_t *OpenAPI_ip_end_point_create(
    char *ipv4_address,
    char *ipv6_address,
    OpenAPI_transport_protocol_e transport,
    bool is_port,
    int port
)
{
    OpenAPI_ip_end_point_t *ip_end_point_local_var = ogs_malloc(sizeof(OpenAPI_ip_end_point_t));
    ogs_assert(ip_end_point_local_var);

    ip_end_point_local_var->ipv4_address = ipv4_address;
    ip_end_point_local_var->ipv6_address = ipv6_address;
    ip_end_point_local_var->transport = transport;
    ip_end_point_local_var->is_port = is_port;
    ip_end_point_local_var->port = port;

    return ip_end_point_local_var;
}

void OpenAPI_ip_end_point_free(OpenAPI_ip_end_point_t *ip_end_point)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ip_end_point) {
        return;
    }
    if (ip_end_point->ipv4_address) {
        ogs_free(ip_end_point->ipv4_address);
        ip_end_point->ipv4_address = NULL;
    }
    if (ip_end_point->ipv6_address) {
        ogs_free(ip_end_point->ipv6_address);
        ip_end_point->ipv6_address = NULL;
    }
    ogs_free(ip_end_point);
}

cJSON *OpenAPI_ip_end_point_convertToJSON(OpenAPI_ip_end_point_t *ip_end_point)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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

    if (ip_end_point->transport != OpenAPI_transport_protocol_NULL) {
    if (cJSON_AddStringToObject(item, "transport", OpenAPI_transport_protocol_ToString(ip_end_point->transport)) == NULL) {
        ogs_error("OpenAPI_ip_end_point_convertToJSON() failed [transport]");
        goto end;
    }
    }

    if (ip_end_point->is_port) {
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
    OpenAPI_lnode_t *node = NULL;
    cJSON *ipv4_address = NULL;
    cJSON *ipv6_address = NULL;
    cJSON *transport = NULL;
    OpenAPI_transport_protocol_e transportVariable = 0;
    cJSON *port = NULL;
    ipv4_address = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "ipv4Address");
    if (ipv4_address) {
    if (!cJSON_IsString(ipv4_address) && !cJSON_IsNull(ipv4_address)) {
        ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [ipv4_address]");
        goto end;
    }
    }

    ipv6_address = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "ipv6Address");
    if (ipv6_address) {
    if (!cJSON_IsString(ipv6_address) && !cJSON_IsNull(ipv6_address)) {
        ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [ipv6_address]");
        goto end;
    }
    }

    transport = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "transport");
    if (transport) {
    if (!cJSON_IsString(transport)) {
        ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [transport]");
        goto end;
    }
    transportVariable = OpenAPI_transport_protocol_FromString(transport->valuestring);
    }

    port = cJSON_GetObjectItemCaseSensitive(ip_end_pointJSON, "port");
    if (port) {
    if (!cJSON_IsNumber(port)) {
        ogs_error("OpenAPI_ip_end_point_parseFromJSON() failed [port]");
        goto end;
    }
    }

    ip_end_point_local_var = OpenAPI_ip_end_point_create (
        ipv4_address && !cJSON_IsNull(ipv4_address) ? ogs_strdup(ipv4_address->valuestring) : NULL,
        ipv6_address && !cJSON_IsNull(ipv6_address) ? ogs_strdup(ipv6_address->valuestring) : NULL,
        transport ? transportVariable : 0,
        port ? true : false,
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

