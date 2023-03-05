
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api_signature.h"

OpenAPI_api_signature_t *OpenAPI_api_signature_create(
    char *callback_type
)
{
    OpenAPI_api_signature_t *api_signature_local_var = ogs_malloc(sizeof(OpenAPI_api_signature_t));
    ogs_assert(api_signature_local_var);

    api_signature_local_var->callback_type = callback_type;

    return api_signature_local_var;
}

void OpenAPI_api_signature_free(OpenAPI_api_signature_t *api_signature)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == api_signature) {
        return;
    }
    if (api_signature->callback_type) {
        ogs_free(api_signature->callback_type);
        api_signature->callback_type = NULL;
    }
    ogs_free(api_signature);
}

cJSON *OpenAPI_api_signature_convertToJSON(OpenAPI_api_signature_t *api_signature)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (api_signature == NULL) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed [ApiSignature]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!api_signature->callback_type) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed [callback_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackType", api_signature->callback_type) == NULL) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed [callback_type]");
        goto end;
    }

end:
    return item;
}

OpenAPI_api_signature_t *OpenAPI_api_signature_parseFromJSON(cJSON *api_signatureJSON)
{
    OpenAPI_api_signature_t *api_signature_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_type = NULL;
    callback_type = cJSON_GetObjectItemCaseSensitive(api_signatureJSON, "callbackType");
    if (!callback_type) {
        ogs_error("OpenAPI_api_signature_parseFromJSON() failed [callback_type]");
        goto end;
    }
    if (!cJSON_IsString(callback_type)) {
        ogs_error("OpenAPI_api_signature_parseFromJSON() failed [callback_type]");
        goto end;
    }

    api_signature_local_var = OpenAPI_api_signature_create (
        ogs_strdup(callback_type->valuestring)
    );

    return api_signature_local_var;
end:
    return NULL;
}

OpenAPI_api_signature_t *OpenAPI_api_signature_copy(OpenAPI_api_signature_t *dst, OpenAPI_api_signature_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_api_signature_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed");
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

    OpenAPI_api_signature_free(dst);
    dst = OpenAPI_api_signature_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

