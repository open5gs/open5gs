
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "eas_server_address.h"

OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_create(
    OpenAPI_ip_addr_t *ip,
    int port
)
{
    OpenAPI_eas_server_address_t *eas_server_address_local_var = ogs_malloc(sizeof(OpenAPI_eas_server_address_t));
    ogs_assert(eas_server_address_local_var);

    eas_server_address_local_var->ip = ip;
    eas_server_address_local_var->port = port;

    return eas_server_address_local_var;
}

void OpenAPI_eas_server_address_free(OpenAPI_eas_server_address_t *eas_server_address)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == eas_server_address) {
        return;
    }
    if (eas_server_address->ip) {
        OpenAPI_ip_addr_free(eas_server_address->ip);
        eas_server_address->ip = NULL;
    }
    ogs_free(eas_server_address);
}

cJSON *OpenAPI_eas_server_address_convertToJSON(OpenAPI_eas_server_address_t *eas_server_address)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (eas_server_address == NULL) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed [EasServerAddress]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!eas_server_address->ip) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed [ip]");
        return NULL;
    }
    cJSON *ip_local_JSON = OpenAPI_ip_addr_convertToJSON(eas_server_address->ip);
    if (ip_local_JSON == NULL) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed [ip]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ip", ip_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed [ip]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "port", eas_server_address->port) == NULL) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed [port]");
        goto end;
    }

end:
    return item;
}

OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_parseFromJSON(cJSON *eas_server_addressJSON)
{
    OpenAPI_eas_server_address_t *eas_server_address_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip = NULL;
    OpenAPI_ip_addr_t *ip_local_nonprim = NULL;
    cJSON *port = NULL;
    ip = cJSON_GetObjectItemCaseSensitive(eas_server_addressJSON, "ip");
    if (!ip) {
        ogs_error("OpenAPI_eas_server_address_parseFromJSON() failed [ip]");
        goto end;
    }
    ip_local_nonprim = OpenAPI_ip_addr_parseFromJSON(ip);
    if (!ip_local_nonprim) {
        ogs_error("OpenAPI_ip_addr_parseFromJSON failed [ip]");
        goto end;
    }

    port = cJSON_GetObjectItemCaseSensitive(eas_server_addressJSON, "port");
    if (!port) {
        ogs_error("OpenAPI_eas_server_address_parseFromJSON() failed [port]");
        goto end;
    }
    if (!cJSON_IsNumber(port)) {
        ogs_error("OpenAPI_eas_server_address_parseFromJSON() failed [port]");
        goto end;
    }

    eas_server_address_local_var = OpenAPI_eas_server_address_create (
        ip_local_nonprim,
        
        port->valuedouble
    );

    return eas_server_address_local_var;
end:
    if (ip_local_nonprim) {
        OpenAPI_ip_addr_free(ip_local_nonprim);
        ip_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_eas_server_address_t *OpenAPI_eas_server_address_copy(OpenAPI_eas_server_address_t *dst, OpenAPI_eas_server_address_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_eas_server_address_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_eas_server_address_convertToJSON() failed");
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

    OpenAPI_eas_server_address_free(dst);
    dst = OpenAPI_eas_server_address_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

