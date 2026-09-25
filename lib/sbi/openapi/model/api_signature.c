
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "api_signature.h"

OpenAPI_api_signature_t *OpenAPI_api_signature_create(
    char *uri,
    OpenAPI_callback_name_t *callback_name
)
{
    OpenAPI_api_signature_t *api_signature_local_var = ogs_malloc(sizeof(OpenAPI_api_signature_t));
    ogs_assert(api_signature_local_var);

    api_signature_local_var->uri = uri;
    api_signature_local_var->callback_name = callback_name;

    return api_signature_local_var;
}

void OpenAPI_api_signature_free(OpenAPI_api_signature_t *api_signature)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == api_signature) {
        return;
    }
    if (api_signature->uri) {
        ogs_free(api_signature->uri);
        api_signature->uri = NULL;
    }
    if (api_signature->callback_name) {
        OpenAPI_callback_name_free(api_signature->callback_name);
        api_signature->callback_name = NULL;
    }
    ogs_free(api_signature);
}

/*
 * oneOf union (x-open5gs-union): the members are the alternatives of the
 * original oneOf. Exactly one is set, and it is the JSON value itself
 * rather than a property of a wrapper object.
 */
cJSON *OpenAPI_api_signature_convertToJSON(OpenAPI_api_signature_t *api_signature)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (api_signature == NULL) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed [ApiSignature]");
        return NULL;
    }

    if ((api_signature->uri != NULL) +
            (api_signature->callback_name != NULL) != 1) {
        ogs_error("OpenAPI_api_signature_convertToJSON() failed [ApiSignature]: exactly one alternative must be set");
        return NULL;
    }

    if (api_signature->uri) {
        return cJSON_CreateString(api_signature->uri);
    }
    if (api_signature->callback_name) {
        return OpenAPI_callback_name_convertToJSON(api_signature->callback_name);
    }

    ogs_error("OpenAPI_api_signature_convertToJSON() failed [ApiSignature]");
    return NULL;
}

OpenAPI_api_signature_t *OpenAPI_api_signature_parseFromJSON(cJSON *api_signatureJSON)
{
    OpenAPI_lnode_t *node = NULL;
    char *uri = NULL;
    OpenAPI_callback_name_t *callback_name = NULL;

    if (cJSON_IsString(api_signatureJSON)) {
        uri = ogs_strdup(api_signatureJSON->valuestring);
        ogs_assert(uri);
    } else if (cJSON_IsObject(api_signatureJSON)) {
        callback_name = OpenAPI_callback_name_parseFromJSON(api_signatureJSON);
        if (!callback_name) {
            ogs_error("OpenAPI_callback_name_parseFromJSON failed [callback_name]");
            goto end;
        }
    } else {
        ogs_error("OpenAPI_api_signature_parseFromJSON() failed [ApiSignature]");
        goto end;
    }

    return OpenAPI_api_signature_create(uri, callback_name);
end:
    if (uri) {
        ogs_free(uri);
        uri = NULL;
    }
    if (callback_name) {
        OpenAPI_callback_name_free(callback_name);
        callback_name = NULL;
    }
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

