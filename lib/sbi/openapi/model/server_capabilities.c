
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "server_capabilities.h"

OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_create(
    OpenAPI_list_t *mandatory_capability,
    OpenAPI_list_t *optional_capability,
    OpenAPI_list_t *server_name
)
{
    OpenAPI_server_capabilities_t *server_capabilities_local_var = ogs_malloc(sizeof(OpenAPI_server_capabilities_t));
    ogs_assert(server_capabilities_local_var);

    server_capabilities_local_var->mandatory_capability = mandatory_capability;
    server_capabilities_local_var->optional_capability = optional_capability;
    server_capabilities_local_var->server_name = server_name;

    return server_capabilities_local_var;
}

void OpenAPI_server_capabilities_free(OpenAPI_server_capabilities_t *server_capabilities)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == server_capabilities) {
        return;
    }
    if (server_capabilities->mandatory_capability) {
        OpenAPI_list_for_each(server_capabilities->mandatory_capability, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_capabilities->mandatory_capability);
        server_capabilities->mandatory_capability = NULL;
    }
    if (server_capabilities->optional_capability) {
        OpenAPI_list_for_each(server_capabilities->optional_capability, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_capabilities->optional_capability);
        server_capabilities->optional_capability = NULL;
    }
    if (server_capabilities->server_name) {
        OpenAPI_list_for_each(server_capabilities->server_name, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_capabilities->server_name);
        server_capabilities->server_name = NULL;
    }
    ogs_free(server_capabilities);
}

cJSON *OpenAPI_server_capabilities_convertToJSON(OpenAPI_server_capabilities_t *server_capabilities)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (server_capabilities == NULL) {
        ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [ServerCapabilities]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (server_capabilities->mandatory_capability) {
    cJSON *mandatory_capabilityList = cJSON_AddArrayToObject(item, "mandatoryCapability");
    if (mandatory_capabilityList == NULL) {
        ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [mandatory_capability]");
        goto end;
    }
    OpenAPI_list_for_each(server_capabilities->mandatory_capability, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [mandatory_capability]");
            goto end;
        }
        if (cJSON_AddNumberToObject(mandatory_capabilityList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [mandatory_capability]");
            goto end;
        }
    }
    }

    if (server_capabilities->optional_capability) {
    cJSON *optional_capabilityList = cJSON_AddArrayToObject(item, "optionalCapability");
    if (optional_capabilityList == NULL) {
        ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [optional_capability]");
        goto end;
    }
    OpenAPI_list_for_each(server_capabilities->optional_capability, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [optional_capability]");
            goto end;
        }
        if (cJSON_AddNumberToObject(optional_capabilityList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [optional_capability]");
            goto end;
        }
    }
    }

    if (server_capabilities->server_name) {
    cJSON *server_nameList = cJSON_AddArrayToObject(item, "serverName");
    if (server_nameList == NULL) {
        ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [server_name]");
        goto end;
    }
    OpenAPI_list_for_each(server_capabilities->server_name, node) {
        if (cJSON_AddStringToObject(server_nameList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_server_capabilities_convertToJSON() failed [server_name]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_parseFromJSON(cJSON *server_capabilitiesJSON)
{
    OpenAPI_server_capabilities_t *server_capabilities_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mandatory_capability = NULL;
    OpenAPI_list_t *mandatory_capabilityList = NULL;
    cJSON *optional_capability = NULL;
    OpenAPI_list_t *optional_capabilityList = NULL;
    cJSON *server_name = NULL;
    OpenAPI_list_t *server_nameList = NULL;
    mandatory_capability = cJSON_GetObjectItemCaseSensitive(server_capabilitiesJSON, "mandatoryCapability");
    if (mandatory_capability) {
        cJSON *mandatory_capability_local = NULL;
        if (!cJSON_IsArray(mandatory_capability)) {
            ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [mandatory_capability]");
            goto end;
        }

        mandatory_capabilityList = OpenAPI_list_create();

        cJSON_ArrayForEach(mandatory_capability_local, mandatory_capability) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(mandatory_capability_local)) {
                ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [mandatory_capability]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [mandatory_capability]");
                goto end;
            }
            *localDouble = mandatory_capability_local->valuedouble;
            OpenAPI_list_add(mandatory_capabilityList, localDouble);
        }
    }

    optional_capability = cJSON_GetObjectItemCaseSensitive(server_capabilitiesJSON, "optionalCapability");
    if (optional_capability) {
        cJSON *optional_capability_local = NULL;
        if (!cJSON_IsArray(optional_capability)) {
            ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [optional_capability]");
            goto end;
        }

        optional_capabilityList = OpenAPI_list_create();

        cJSON_ArrayForEach(optional_capability_local, optional_capability) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(optional_capability_local)) {
                ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [optional_capability]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [optional_capability]");
                goto end;
            }
            *localDouble = optional_capability_local->valuedouble;
            OpenAPI_list_add(optional_capabilityList, localDouble);
        }
    }

    server_name = cJSON_GetObjectItemCaseSensitive(server_capabilitiesJSON, "serverName");
    if (server_name) {
        cJSON *server_name_local = NULL;
        if (!cJSON_IsArray(server_name)) {
            ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [server_name]");
            goto end;
        }

        server_nameList = OpenAPI_list_create();

        cJSON_ArrayForEach(server_name_local, server_name) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(server_name_local)) {
                ogs_error("OpenAPI_server_capabilities_parseFromJSON() failed [server_name]");
                goto end;
            }
            OpenAPI_list_add(server_nameList, ogs_strdup(server_name_local->valuestring));
        }
    }

    server_capabilities_local_var = OpenAPI_server_capabilities_create (
        mandatory_capability ? mandatory_capabilityList : NULL,
        optional_capability ? optional_capabilityList : NULL,
        server_name ? server_nameList : NULL
    );

    return server_capabilities_local_var;
end:
    if (mandatory_capabilityList) {
        OpenAPI_list_for_each(mandatory_capabilityList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mandatory_capabilityList);
        mandatory_capabilityList = NULL;
    }
    if (optional_capabilityList) {
        OpenAPI_list_for_each(optional_capabilityList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(optional_capabilityList);
        optional_capabilityList = NULL;
    }
    if (server_nameList) {
        OpenAPI_list_for_each(server_nameList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(server_nameList);
        server_nameList = NULL;
    }
    return NULL;
}

OpenAPI_server_capabilities_t *OpenAPI_server_capabilities_copy(OpenAPI_server_capabilities_t *dst, OpenAPI_server_capabilities_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_server_capabilities_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_server_capabilities_convertToJSON() failed");
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

    OpenAPI_server_capabilities_free(dst);
    dst = OpenAPI_server_capabilities_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

