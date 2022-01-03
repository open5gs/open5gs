
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "options_response.h"

OpenAPI_options_response_t *OpenAPI_options_response_create(
    char *supported_features
)
{
    OpenAPI_options_response_t *options_response_local_var = ogs_malloc(sizeof(OpenAPI_options_response_t));
    ogs_assert(options_response_local_var);

    options_response_local_var->supported_features = supported_features;

    return options_response_local_var;
}

void OpenAPI_options_response_free(OpenAPI_options_response_t *options_response)
{
    if (NULL == options_response) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(options_response->supported_features);
    ogs_free(options_response);
}

cJSON *OpenAPI_options_response_convertToJSON(OpenAPI_options_response_t *options_response)
{
    cJSON *item = NULL;

    if (options_response == NULL) {
        ogs_error("OpenAPI_options_response_convertToJSON() failed [OptionsResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (options_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", options_response->supported_features) == NULL) {
        ogs_error("OpenAPI_options_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_options_response_t *OpenAPI_options_response_parseFromJSON(cJSON *options_responseJSON)
{
    OpenAPI_options_response_t *options_response_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(options_responseJSON, "supportedFeatures");

    if (supported_features) {
    if (!cJSON_IsString(supported_features)) {
        ogs_error("OpenAPI_options_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    options_response_local_var = OpenAPI_options_response_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return options_response_local_var;
end:
    return NULL;
}

OpenAPI_options_response_t *OpenAPI_options_response_copy(OpenAPI_options_response_t *dst, OpenAPI_options_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_options_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_options_response_convertToJSON() failed");
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

    OpenAPI_options_response_free(dst);
    dst = OpenAPI_options_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

