
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_interface.h"

OpenAPI_sm_interface_t *OpenAPI_sm_interface_create(
    char *interface_id,
    char *interface_text,
    char *interface_port,
    OpenAPI_interface_type_e interface_type
)
{
    OpenAPI_sm_interface_t *sm_interface_local_var = ogs_malloc(sizeof(OpenAPI_sm_interface_t));
    ogs_assert(sm_interface_local_var);

    sm_interface_local_var->interface_id = interface_id;
    sm_interface_local_var->interface_text = interface_text;
    sm_interface_local_var->interface_port = interface_port;
    sm_interface_local_var->interface_type = interface_type;

    return sm_interface_local_var;
}

void OpenAPI_sm_interface_free(OpenAPI_sm_interface_t *sm_interface)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_interface) {
        return;
    }
    if (sm_interface->interface_id) {
        ogs_free(sm_interface->interface_id);
        sm_interface->interface_id = NULL;
    }
    if (sm_interface->interface_text) {
        ogs_free(sm_interface->interface_text);
        sm_interface->interface_text = NULL;
    }
    if (sm_interface->interface_port) {
        ogs_free(sm_interface->interface_port);
        sm_interface->interface_port = NULL;
    }
    ogs_free(sm_interface);
}

cJSON *OpenAPI_sm_interface_convertToJSON(OpenAPI_sm_interface_t *sm_interface)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_interface == NULL) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed [SMInterface]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_interface->interface_id) {
    if (cJSON_AddStringToObject(item, "interfaceId", sm_interface->interface_id) == NULL) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed [interface_id]");
        goto end;
    }
    }

    if (sm_interface->interface_text) {
    if (cJSON_AddStringToObject(item, "interfaceText", sm_interface->interface_text) == NULL) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed [interface_text]");
        goto end;
    }
    }

    if (sm_interface->interface_port) {
    if (cJSON_AddStringToObject(item, "interfacePort", sm_interface->interface_port) == NULL) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed [interface_port]");
        goto end;
    }
    }

    if (sm_interface->interface_type != OpenAPI_interface_type_NULL) {
    if (cJSON_AddStringToObject(item, "interfaceType", OpenAPI_interface_type_ToString(sm_interface->interface_type)) == NULL) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed [interface_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_interface_t *OpenAPI_sm_interface_parseFromJSON(cJSON *sm_interfaceJSON)
{
    OpenAPI_sm_interface_t *sm_interface_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *interface_id = NULL;
    cJSON *interface_text = NULL;
    cJSON *interface_port = NULL;
    cJSON *interface_type = NULL;
    OpenAPI_interface_type_e interface_typeVariable = 0;
    interface_id = cJSON_GetObjectItemCaseSensitive(sm_interfaceJSON, "interfaceId");
    if (interface_id) {
    if (!cJSON_IsString(interface_id) && !cJSON_IsNull(interface_id)) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON() failed [interface_id]");
        goto end;
    }
    }

    interface_text = cJSON_GetObjectItemCaseSensitive(sm_interfaceJSON, "interfaceText");
    if (interface_text) {
    if (!cJSON_IsString(interface_text) && !cJSON_IsNull(interface_text)) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON() failed [interface_text]");
        goto end;
    }
    }

    interface_port = cJSON_GetObjectItemCaseSensitive(sm_interfaceJSON, "interfacePort");
    if (interface_port) {
    if (!cJSON_IsString(interface_port) && !cJSON_IsNull(interface_port)) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON() failed [interface_port]");
        goto end;
    }
    }

    interface_type = cJSON_GetObjectItemCaseSensitive(sm_interfaceJSON, "interfaceType");
    if (interface_type) {
    if (!cJSON_IsString(interface_type)) {
        ogs_error("OpenAPI_sm_interface_parseFromJSON() failed [interface_type]");
        goto end;
    }
    interface_typeVariable = OpenAPI_interface_type_FromString(interface_type->valuestring);
    }

    sm_interface_local_var = OpenAPI_sm_interface_create (
        interface_id && !cJSON_IsNull(interface_id) ? ogs_strdup(interface_id->valuestring) : NULL,
        interface_text && !cJSON_IsNull(interface_text) ? ogs_strdup(interface_text->valuestring) : NULL,
        interface_port && !cJSON_IsNull(interface_port) ? ogs_strdup(interface_port->valuestring) : NULL,
        interface_type ? interface_typeVariable : 0
    );

    return sm_interface_local_var;
end:
    return NULL;
}

OpenAPI_sm_interface_t *OpenAPI_sm_interface_copy(OpenAPI_sm_interface_t *dst, OpenAPI_sm_interface_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_interface_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_interface_convertToJSON() failed");
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

    OpenAPI_sm_interface_free(dst);
    dst = OpenAPI_sm_interface_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

