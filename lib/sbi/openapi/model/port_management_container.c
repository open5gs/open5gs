
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "port_management_container.h"

OpenAPI_port_management_container_t *OpenAPI_port_management_container_create(
    char port_man_cont,
    OpenAPI_tsn_port_identifier_t *port_identifier
    )
{
    OpenAPI_port_management_container_t *port_management_container_local_var = OpenAPI_malloc(sizeof(OpenAPI_port_management_container_t));
    if (!port_management_container_local_var) {
        return NULL;
    }
    port_management_container_local_var->port_man_cont = port_man_cont;
    port_management_container_local_var->port_identifier = port_identifier;

    return port_management_container_local_var;
}

void OpenAPI_port_management_container_free(OpenAPI_port_management_container_t *port_management_container)
{
    if (NULL == port_management_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_tsn_port_identifier_free(port_management_container->port_identifier);
    ogs_free(port_management_container);
}

cJSON *OpenAPI_port_management_container_convertToJSON(OpenAPI_port_management_container_t *port_management_container)
{
    cJSON *item = NULL;

    if (port_management_container == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [PortManagementContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!port_management_container->port_man_cont) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_man_cont]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "portManCont", port_management_container->port_man_cont) == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_man_cont]");
        goto end;
    }

    if (!port_management_container->port_identifier) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_identifier]");
        goto end;
    }
    cJSON *port_identifier_local_JSON = OpenAPI_tsn_port_identifier_convertToJSON(port_management_container->port_identifier);
    if (port_identifier_local_JSON == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_identifier]");
        goto end;
    }
    cJSON_AddItemToObject(item, "portIdentifier", port_identifier_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_identifier]");
        goto end;
    }

end:
    return item;
}

OpenAPI_port_management_container_t *OpenAPI_port_management_container_parseFromJSON(cJSON *port_management_containerJSON)
{
    OpenAPI_port_management_container_t *port_management_container_local_var = NULL;
    cJSON *port_man_cont = cJSON_GetObjectItemCaseSensitive(port_management_containerJSON, "portManCont");
    if (!port_man_cont) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_man_cont]");
        goto end;
    }


    if (!cJSON_IsNumber(port_man_cont)) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_man_cont]");
        goto end;
    }

    cJSON *port_identifier = cJSON_GetObjectItemCaseSensitive(port_management_containerJSON, "portIdentifier");
    if (!port_identifier) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_identifier]");
        goto end;
    }

    OpenAPI_tsn_port_identifier_t *port_identifier_local_nonprim = NULL;

    port_identifier_local_nonprim = OpenAPI_tsn_port_identifier_parseFromJSON(port_identifier);

    port_management_container_local_var = OpenAPI_port_management_container_create (
        port_man_cont->valueint,
        port_identifier_local_nonprim
        );

    return port_management_container_local_var;
end:
    return NULL;
}

OpenAPI_port_management_container_t *OpenAPI_port_management_container_copy(OpenAPI_port_management_container_t *dst, OpenAPI_port_management_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_port_management_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed");
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

    OpenAPI_port_management_container_free(dst);
    dst = OpenAPI_port_management_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

