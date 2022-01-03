
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bridge_management_container.h"

OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_create(
    char bridge_man_cont
)
{
    OpenAPI_bridge_management_container_t *bridge_management_container_local_var = ogs_malloc(sizeof(OpenAPI_bridge_management_container_t));
    ogs_assert(bridge_management_container_local_var);

    bridge_management_container_local_var->bridge_man_cont = bridge_man_cont;

    return bridge_management_container_local_var;
}

void OpenAPI_bridge_management_container_free(OpenAPI_bridge_management_container_t *bridge_management_container)
{
    if (NULL == bridge_management_container) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(bridge_management_container);
}

cJSON *OpenAPI_bridge_management_container_convertToJSON(OpenAPI_bridge_management_container_t *bridge_management_container)
{
    cJSON *item = NULL;

    if (bridge_management_container == NULL) {
        ogs_error("OpenAPI_bridge_management_container_convertToJSON() failed [BridgeManagementContainer]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "bridgeManCont", bridge_management_container->bridge_man_cont) == NULL) {
        ogs_error("OpenAPI_bridge_management_container_convertToJSON() failed [bridge_man_cont]");
        goto end;
    }

end:
    return item;
}

OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_parseFromJSON(cJSON *bridge_management_containerJSON)
{
    OpenAPI_bridge_management_container_t *bridge_management_container_local_var = NULL;
    cJSON *bridge_man_cont = cJSON_GetObjectItemCaseSensitive(bridge_management_containerJSON, "bridgeManCont");
    if (!bridge_man_cont) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON() failed [bridge_man_cont]");
        goto end;
    }

    if (!cJSON_IsNumber(bridge_man_cont)) {
        ogs_error("OpenAPI_bridge_management_container_parseFromJSON() failed [bridge_man_cont]");
        goto end;
    }

    bridge_management_container_local_var = OpenAPI_bridge_management_container_create (
        bridge_man_cont->valueint
    );

    return bridge_management_container_local_var;
end:
    return NULL;
}

OpenAPI_bridge_management_container_t *OpenAPI_bridge_management_container_copy(OpenAPI_bridge_management_container_t *dst, OpenAPI_bridge_management_container_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bridge_management_container_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bridge_management_container_convertToJSON() failed");
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

    OpenAPI_bridge_management_container_free(dst);
    dst = OpenAPI_bridge_management_container_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

