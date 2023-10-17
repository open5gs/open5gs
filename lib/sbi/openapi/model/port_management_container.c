
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "port_management_container.h"

OpenAPI_port_management_container_t *OpenAPI_port_management_container_create(
    char *port_man_cont,
    int port_num
)
{
    OpenAPI_port_management_container_t *port_management_container_local_var = ogs_malloc(sizeof(OpenAPI_port_management_container_t));
    ogs_assert(port_management_container_local_var);

    port_management_container_local_var->port_man_cont = port_man_cont;
    port_management_container_local_var->port_num = port_num;

    return port_management_container_local_var;
}

void OpenAPI_port_management_container_free(OpenAPI_port_management_container_t *port_management_container)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == port_management_container) {
        return;
    }
    if (port_management_container->port_man_cont) {
        ogs_free(port_management_container->port_man_cont);
        port_management_container->port_man_cont = NULL;
    }
    ogs_free(port_management_container);
}

cJSON *OpenAPI_port_management_container_convertToJSON(OpenAPI_port_management_container_t *port_management_container)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (port_management_container == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [PortManagementContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!port_management_container->port_man_cont) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_man_cont]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "portManCont", port_management_container->port_man_cont) == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_man_cont]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "portNum", port_management_container->port_num) == NULL) {
        ogs_error("OpenAPI_port_management_container_convertToJSON() failed [port_num]");
        goto end;
    }

end:
    return item;
}

OpenAPI_port_management_container_t *OpenAPI_port_management_container_parseFromJSON(cJSON *port_management_containerJSON)
{
    OpenAPI_port_management_container_t *port_management_container_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *port_man_cont = NULL;
    cJSON *port_num = NULL;
    port_man_cont = cJSON_GetObjectItemCaseSensitive(port_management_containerJSON, "portManCont");
    if (!port_man_cont) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_man_cont]");
        goto end;
    }
    if (!cJSON_IsString(port_man_cont)) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_man_cont]");
        goto end;
    }

    port_num = cJSON_GetObjectItemCaseSensitive(port_management_containerJSON, "portNum");
    if (!port_num) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_num]");
        goto end;
    }
    if (!cJSON_IsNumber(port_num)) {
        ogs_error("OpenAPI_port_management_container_parseFromJSON() failed [port_num]");
        goto end;
    }

    port_management_container_local_var = OpenAPI_port_management_container_create (
        ogs_strdup(port_man_cont->valuestring),
        
        port_num->valuedouble
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

