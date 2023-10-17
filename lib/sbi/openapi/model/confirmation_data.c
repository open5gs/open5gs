
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "confirmation_data.h"

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_create(
    bool is_res_star_null,
    char *res_star,
    char *supported_features
)
{
    OpenAPI_confirmation_data_t *confirmation_data_local_var = ogs_malloc(sizeof(OpenAPI_confirmation_data_t));
    ogs_assert(confirmation_data_local_var);

    confirmation_data_local_var->is_res_star_null = is_res_star_null;
    confirmation_data_local_var->res_star = res_star;
    confirmation_data_local_var->supported_features = supported_features;

    return confirmation_data_local_var;
}

void OpenAPI_confirmation_data_free(OpenAPI_confirmation_data_t *confirmation_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == confirmation_data) {
        return;
    }
    if (confirmation_data->res_star) {
        ogs_free(confirmation_data->res_star);
        confirmation_data->res_star = NULL;
    }
    if (confirmation_data->supported_features) {
        ogs_free(confirmation_data->supported_features);
        confirmation_data->supported_features = NULL;
    }
    ogs_free(confirmation_data);
}

cJSON *OpenAPI_confirmation_data_convertToJSON(OpenAPI_confirmation_data_t *confirmation_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (confirmation_data == NULL) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [ConfirmationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!confirmation_data->res_star) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [res_star]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resStar", confirmation_data->res_star) == NULL) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [res_star]");
        goto end;
    }

    if (confirmation_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", confirmation_data->supported_features) == NULL) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_parseFromJSON(cJSON *confirmation_dataJSON)
{
    OpenAPI_confirmation_data_t *confirmation_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *res_star = NULL;
    cJSON *supported_features = NULL;
    res_star = cJSON_GetObjectItemCaseSensitive(confirmation_dataJSON, "resStar");
    if (!res_star) {
        ogs_error("OpenAPI_confirmation_data_parseFromJSON() failed [res_star]");
        goto end;
    }
    if (!cJSON_IsString(res_star)) {
        ogs_error("OpenAPI_confirmation_data_parseFromJSON() failed [res_star]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(confirmation_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_confirmation_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    confirmation_data_local_var = OpenAPI_confirmation_data_create (
        res_star && cJSON_IsNull(res_star) ? true : false,
        ogs_strdup(res_star->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return confirmation_data_local_var;
end:
    return NULL;
}

OpenAPI_confirmation_data_t *OpenAPI_confirmation_data_copy(OpenAPI_confirmation_data_t *dst, OpenAPI_confirmation_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_confirmation_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_confirmation_data_convertToJSON() failed");
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

    OpenAPI_confirmation_data_free(dst);
    dst = OpenAPI_confirmation_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

