
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app_port_id.h"

OpenAPI_app_port_id_t *OpenAPI_app_port_id_create(
    int destination_port,
    int originator_port
    )
{
    OpenAPI_app_port_id_t *app_port_id_local_var = OpenAPI_malloc(sizeof(OpenAPI_app_port_id_t));
    if (!app_port_id_local_var) {
        return NULL;
    }
    app_port_id_local_var->destination_port = destination_port;
    app_port_id_local_var->originator_port = originator_port;

    return app_port_id_local_var;
}

void OpenAPI_app_port_id_free(OpenAPI_app_port_id_t *app_port_id)
{
    if (NULL == app_port_id) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(app_port_id);
}

cJSON *OpenAPI_app_port_id_convertToJSON(OpenAPI_app_port_id_t *app_port_id)
{
    cJSON *item = NULL;

    if (app_port_id == NULL) {
        ogs_error("OpenAPI_app_port_id_convertToJSON() failed [AppPortId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (app_port_id->destination_port) {
        if (cJSON_AddNumberToObject(item, "destinationPort", app_port_id->destination_port) == NULL) {
            ogs_error("OpenAPI_app_port_id_convertToJSON() failed [destination_port]");
            goto end;
        }
    }

    if (app_port_id->originator_port) {
        if (cJSON_AddNumberToObject(item, "originatorPort", app_port_id->originator_port) == NULL) {
            ogs_error("OpenAPI_app_port_id_convertToJSON() failed [originator_port]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_app_port_id_t *OpenAPI_app_port_id_parseFromJSON(cJSON *app_port_idJSON)
{
    OpenAPI_app_port_id_t *app_port_id_local_var = NULL;
    cJSON *destination_port = cJSON_GetObjectItemCaseSensitive(app_port_idJSON, "destinationPort");

    if (destination_port) {
        if (!cJSON_IsNumber(destination_port)) {
            ogs_error("OpenAPI_app_port_id_parseFromJSON() failed [destination_port]");
            goto end;
        }
    }

    cJSON *originator_port = cJSON_GetObjectItemCaseSensitive(app_port_idJSON, "originatorPort");

    if (originator_port) {
        if (!cJSON_IsNumber(originator_port)) {
            ogs_error("OpenAPI_app_port_id_parseFromJSON() failed [originator_port]");
            goto end;
        }
    }

    app_port_id_local_var = OpenAPI_app_port_id_create (
        destination_port ? destination_port->valuedouble : 0,
        originator_port ? originator_port->valuedouble : 0
        );

    return app_port_id_local_var;
end:
    return NULL;
}

OpenAPI_app_port_id_t *OpenAPI_app_port_id_copy(OpenAPI_app_port_id_t *dst, OpenAPI_app_port_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_app_port_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_app_port_id_convertToJSON() failed");
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

    OpenAPI_app_port_id_free(dst);
    dst = OpenAPI_app_port_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

