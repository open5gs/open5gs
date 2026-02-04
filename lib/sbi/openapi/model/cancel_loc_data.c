
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cancel_loc_data.h"

OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_create(
    char *hgmlc_call_back_uri,
    char *ldr_reference,
    char *supported_features
)
{
    OpenAPI_cancel_loc_data_t *cancel_loc_data_local_var = ogs_malloc(sizeof(OpenAPI_cancel_loc_data_t));
    ogs_assert(cancel_loc_data_local_var);

    cancel_loc_data_local_var->hgmlc_call_back_uri = hgmlc_call_back_uri;
    cancel_loc_data_local_var->ldr_reference = ldr_reference;
    cancel_loc_data_local_var->supported_features = supported_features;

    return cancel_loc_data_local_var;
}

void OpenAPI_cancel_loc_data_free(OpenAPI_cancel_loc_data_t *cancel_loc_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cancel_loc_data) {
        return;
    }
    if (cancel_loc_data->hgmlc_call_back_uri) {
        ogs_free(cancel_loc_data->hgmlc_call_back_uri);
        cancel_loc_data->hgmlc_call_back_uri = NULL;
    }
    if (cancel_loc_data->ldr_reference) {
        ogs_free(cancel_loc_data->ldr_reference);
        cancel_loc_data->ldr_reference = NULL;
    }
    if (cancel_loc_data->supported_features) {
        ogs_free(cancel_loc_data->supported_features);
        cancel_loc_data->supported_features = NULL;
    }
    ogs_free(cancel_loc_data);
}

cJSON *OpenAPI_cancel_loc_data_convertToJSON(OpenAPI_cancel_loc_data_t *cancel_loc_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cancel_loc_data == NULL) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [CancelLocData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!cancel_loc_data->hgmlc_call_back_uri) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [hgmlc_call_back_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "hgmlcCallBackURI", cancel_loc_data->hgmlc_call_back_uri) == NULL) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }

    if (!cancel_loc_data->ldr_reference) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [ldr_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ldrReference", cancel_loc_data->ldr_reference) == NULL) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [ldr_reference]");
        goto end;
    }

    if (cancel_loc_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", cancel_loc_data->supported_features) == NULL) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_parseFromJSON(cJSON *cancel_loc_dataJSON)
{
    OpenAPI_cancel_loc_data_t *cancel_loc_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hgmlc_call_back_uri = NULL;
    cJSON *ldr_reference = NULL;
    cJSON *supported_features = NULL;
    hgmlc_call_back_uri = cJSON_GetObjectItemCaseSensitive(cancel_loc_dataJSON, "hgmlcCallBackURI");
    if (!hgmlc_call_back_uri) {
        ogs_error("OpenAPI_cancel_loc_data_parseFromJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }
    if (!cJSON_IsString(hgmlc_call_back_uri)) {
        ogs_error("OpenAPI_cancel_loc_data_parseFromJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }

    ldr_reference = cJSON_GetObjectItemCaseSensitive(cancel_loc_dataJSON, "ldrReference");
    if (!ldr_reference) {
        ogs_error("OpenAPI_cancel_loc_data_parseFromJSON() failed [ldr_reference]");
        goto end;
    }
    if (!cJSON_IsString(ldr_reference)) {
        ogs_error("OpenAPI_cancel_loc_data_parseFromJSON() failed [ldr_reference]");
        goto end;
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(cancel_loc_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_cancel_loc_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    cancel_loc_data_local_var = OpenAPI_cancel_loc_data_create (
        ogs_strdup(hgmlc_call_back_uri->valuestring),
        ogs_strdup(ldr_reference->valuestring),
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return cancel_loc_data_local_var;
end:
    return NULL;
}

OpenAPI_cancel_loc_data_t *OpenAPI_cancel_loc_data_copy(OpenAPI_cancel_loc_data_t *dst, OpenAPI_cancel_loc_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cancel_loc_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cancel_loc_data_convertToJSON() failed");
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

    OpenAPI_cancel_loc_data_free(dst);
    dst = OpenAPI_cancel_loc_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

