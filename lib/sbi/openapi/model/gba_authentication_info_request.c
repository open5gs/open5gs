
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gba_authentication_info_request.h"

OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_create(
    OpenAPI_gba_auth_type_e auth_type,
    OpenAPI_resynchronization_info_1_t *resynchronization_info,
    char *supported_features
)
{
    OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request_local_var = ogs_malloc(sizeof(OpenAPI_gba_authentication_info_request_t));
    ogs_assert(gba_authentication_info_request_local_var);

    gba_authentication_info_request_local_var->auth_type = auth_type;
    gba_authentication_info_request_local_var->resynchronization_info = resynchronization_info;
    gba_authentication_info_request_local_var->supported_features = supported_features;

    return gba_authentication_info_request_local_var;
}

void OpenAPI_gba_authentication_info_request_free(OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == gba_authentication_info_request) {
        return;
    }
    if (gba_authentication_info_request->resynchronization_info) {
        OpenAPI_resynchronization_info_1_free(gba_authentication_info_request->resynchronization_info);
        gba_authentication_info_request->resynchronization_info = NULL;
    }
    if (gba_authentication_info_request->supported_features) {
        ogs_free(gba_authentication_info_request->supported_features);
        gba_authentication_info_request->supported_features = NULL;
    }
    ogs_free(gba_authentication_info_request);
}

cJSON *OpenAPI_gba_authentication_info_request_convertToJSON(OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (gba_authentication_info_request == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [GbaAuthenticationInfoRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (gba_authentication_info_request->auth_type == OpenAPI_gba_auth_type_NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [auth_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "authType", OpenAPI_gba_auth_type_ToString(gba_authentication_info_request->auth_type)) == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [auth_type]");
        goto end;
    }

    if (gba_authentication_info_request->resynchronization_info) {
    cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_1_convertToJSON(gba_authentication_info_request->resynchronization_info);
    if (resynchronization_info_local_JSON == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    }

    if (gba_authentication_info_request->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", gba_authentication_info_request->supported_features) == NULL) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_parseFromJSON(cJSON *gba_authentication_info_requestJSON)
{
    OpenAPI_gba_authentication_info_request_t *gba_authentication_info_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *auth_type = NULL;
    OpenAPI_gba_auth_type_e auth_typeVariable = 0;
    cJSON *resynchronization_info = NULL;
    OpenAPI_resynchronization_info_1_t *resynchronization_info_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    auth_type = cJSON_GetObjectItemCaseSensitive(gba_authentication_info_requestJSON, "authType");
    if (!auth_type) {
        ogs_error("OpenAPI_gba_authentication_info_request_parseFromJSON() failed [auth_type]");
        goto end;
    }
    if (!cJSON_IsString(auth_type)) {
        ogs_error("OpenAPI_gba_authentication_info_request_parseFromJSON() failed [auth_type]");
        goto end;
    }
    auth_typeVariable = OpenAPI_gba_auth_type_FromString(auth_type->valuestring);

    resynchronization_info = cJSON_GetObjectItemCaseSensitive(gba_authentication_info_requestJSON, "resynchronizationInfo");
    if (resynchronization_info) {
    resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_1_parseFromJSON(resynchronization_info);
    if (!resynchronization_info_local_nonprim) {
        ogs_error("OpenAPI_resynchronization_info_1_parseFromJSON failed [resynchronization_info]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(gba_authentication_info_requestJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_gba_authentication_info_request_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    gba_authentication_info_request_local_var = OpenAPI_gba_authentication_info_request_create (
        auth_typeVariable,
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return gba_authentication_info_request_local_var;
end:
    if (resynchronization_info_local_nonprim) {
        OpenAPI_resynchronization_info_1_free(resynchronization_info_local_nonprim);
        resynchronization_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_gba_authentication_info_request_t *OpenAPI_gba_authentication_info_request_copy(OpenAPI_gba_authentication_info_request_t *dst, OpenAPI_gba_authentication_info_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_gba_authentication_info_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_gba_authentication_info_request_convertToJSON() failed");
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

    OpenAPI_gba_authentication_info_request_free(dst);
    dst = OpenAPI_gba_authentication_info_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

