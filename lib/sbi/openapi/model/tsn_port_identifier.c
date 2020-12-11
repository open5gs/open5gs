
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tsn_port_identifier.h"

OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_create(
    char *port_mac,
    int port_number
    )
{
    OpenAPI_tsn_port_identifier_t *tsn_port_identifier_local_var = OpenAPI_malloc(sizeof(OpenAPI_tsn_port_identifier_t));
    if (!tsn_port_identifier_local_var) {
        return NULL;
    }
    tsn_port_identifier_local_var->port_mac = port_mac;
    tsn_port_identifier_local_var->port_number = port_number;

    return tsn_port_identifier_local_var;
}

void OpenAPI_tsn_port_identifier_free(OpenAPI_tsn_port_identifier_t *tsn_port_identifier)
{
    if (NULL == tsn_port_identifier) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(tsn_port_identifier->port_mac);
    ogs_free(tsn_port_identifier);
}

cJSON *OpenAPI_tsn_port_identifier_convertToJSON(OpenAPI_tsn_port_identifier_t *tsn_port_identifier)
{
    cJSON *item = NULL;

    if (tsn_port_identifier == NULL) {
        ogs_error("OpenAPI_tsn_port_identifier_convertToJSON() failed [TsnPortIdentifier]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!tsn_port_identifier->port_mac) {
        ogs_error("OpenAPI_tsn_port_identifier_convertToJSON() failed [port_mac]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "portMac", tsn_port_identifier->port_mac) == NULL) {
        ogs_error("OpenAPI_tsn_port_identifier_convertToJSON() failed [port_mac]");
        goto end;
    }

    if (tsn_port_identifier->port_number) {
        if (cJSON_AddNumberToObject(item, "portNumber", tsn_port_identifier->port_number) == NULL) {
            ogs_error("OpenAPI_tsn_port_identifier_convertToJSON() failed [port_number]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_parseFromJSON(cJSON *tsn_port_identifierJSON)
{
    OpenAPI_tsn_port_identifier_t *tsn_port_identifier_local_var = NULL;
    cJSON *port_mac = cJSON_GetObjectItemCaseSensitive(tsn_port_identifierJSON, "portMac");
    if (!port_mac) {
        ogs_error("OpenAPI_tsn_port_identifier_parseFromJSON() failed [port_mac]");
        goto end;
    }


    if (!cJSON_IsString(port_mac)) {
        ogs_error("OpenAPI_tsn_port_identifier_parseFromJSON() failed [port_mac]");
        goto end;
    }

    cJSON *port_number = cJSON_GetObjectItemCaseSensitive(tsn_port_identifierJSON, "portNumber");

    if (port_number) {
        if (!cJSON_IsNumber(port_number)) {
            ogs_error("OpenAPI_tsn_port_identifier_parseFromJSON() failed [port_number]");
            goto end;
        }
    }

    tsn_port_identifier_local_var = OpenAPI_tsn_port_identifier_create (
        ogs_strdup(port_mac->valuestring),
        port_number ? port_number->valuedouble : 0
        );

    return tsn_port_identifier_local_var;
end:
    return NULL;
}

OpenAPI_tsn_port_identifier_t *OpenAPI_tsn_port_identifier_copy(OpenAPI_tsn_port_identifier_t *dst, OpenAPI_tsn_port_identifier_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tsn_port_identifier_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tsn_port_identifier_convertToJSON() failed");
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

    OpenAPI_tsn_port_identifier_free(dst);
    dst = OpenAPI_tsn_port_identifier_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

