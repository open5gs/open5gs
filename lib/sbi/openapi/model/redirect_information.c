
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redirect_information.h"

OpenAPI_redirect_information_t *OpenAPI_redirect_information_create(
    int redirect_enabled,
    OpenAPI_redirect_address_type_t *redirect_address_type,
    char *redirect_server_address
    )
{
    OpenAPI_redirect_information_t *redirect_information_local_var = OpenAPI_malloc(sizeof(OpenAPI_redirect_information_t));
    if (!redirect_information_local_var) {
        return NULL;
    }
    redirect_information_local_var->redirect_enabled = redirect_enabled;
    redirect_information_local_var->redirect_address_type = redirect_address_type;
    redirect_information_local_var->redirect_server_address = redirect_server_address;

    return redirect_information_local_var;
}

void OpenAPI_redirect_information_free(OpenAPI_redirect_information_t *redirect_information)
{
    if (NULL == redirect_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_redirect_address_type_free(redirect_information->redirect_address_type);
    ogs_free(redirect_information->redirect_server_address);
    ogs_free(redirect_information);
}

cJSON *OpenAPI_redirect_information_convertToJSON(OpenAPI_redirect_information_t *redirect_information)
{
    cJSON *item = NULL;

    if (redirect_information == NULL) {
        ogs_error("OpenAPI_redirect_information_convertToJSON() failed [RedirectInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redirect_information->redirect_enabled) {
        if (cJSON_AddBoolToObject(item, "redirectEnabled", redirect_information->redirect_enabled) == NULL) {
            ogs_error("OpenAPI_redirect_information_convertToJSON() failed [redirect_enabled]");
            goto end;
        }
    }

    if (redirect_information->redirect_address_type) {
        cJSON *redirect_address_type_local_JSON = OpenAPI_redirect_address_type_convertToJSON(redirect_information->redirect_address_type);
        if (redirect_address_type_local_JSON == NULL) {
            ogs_error("OpenAPI_redirect_information_convertToJSON() failed [redirect_address_type]");
            goto end;
        }
        cJSON_AddItemToObject(item, "redirectAddressType", redirect_address_type_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_redirect_information_convertToJSON() failed [redirect_address_type]");
            goto end;
        }
    }

    if (redirect_information->redirect_server_address) {
        if (cJSON_AddStringToObject(item, "redirectServerAddress", redirect_information->redirect_server_address) == NULL) {
            ogs_error("OpenAPI_redirect_information_convertToJSON() failed [redirect_server_address]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_redirect_information_t *OpenAPI_redirect_information_parseFromJSON(cJSON *redirect_informationJSON)
{
    OpenAPI_redirect_information_t *redirect_information_local_var = NULL;
    cJSON *redirect_enabled = cJSON_GetObjectItemCaseSensitive(redirect_informationJSON, "redirectEnabled");

    if (redirect_enabled) {
        if (!cJSON_IsBool(redirect_enabled)) {
            ogs_error("OpenAPI_redirect_information_parseFromJSON() failed [redirect_enabled]");
            goto end;
        }
    }

    cJSON *redirect_address_type = cJSON_GetObjectItemCaseSensitive(redirect_informationJSON, "redirectAddressType");

    OpenAPI_redirect_address_type_t *redirect_address_type_local_nonprim = NULL;
    if (redirect_address_type) {
        redirect_address_type_local_nonprim = OpenAPI_redirect_address_type_parseFromJSON(redirect_address_type);
    }

    cJSON *redirect_server_address = cJSON_GetObjectItemCaseSensitive(redirect_informationJSON, "redirectServerAddress");

    if (redirect_server_address) {
        if (!cJSON_IsString(redirect_server_address)) {
            ogs_error("OpenAPI_redirect_information_parseFromJSON() failed [redirect_server_address]");
            goto end;
        }
    }

    redirect_information_local_var = OpenAPI_redirect_information_create (
        redirect_enabled ? redirect_enabled->valueint : 0,
        redirect_address_type ? redirect_address_type_local_nonprim : NULL,
        redirect_server_address ? ogs_strdup(redirect_server_address->valuestring) : NULL
        );

    return redirect_information_local_var;
end:
    return NULL;
}

OpenAPI_redirect_information_t *OpenAPI_redirect_information_copy(OpenAPI_redirect_information_t *dst, OpenAPI_redirect_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redirect_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redirect_information_convertToJSON() failed");
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

    OpenAPI_redirect_information_free(dst);
    dst = OpenAPI_redirect_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

