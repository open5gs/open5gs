
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pro_se_authentication_info_request.h"

OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_create(
    char *serving_network_name,
    int relay_service_code,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *supported_features
)
{
    OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request_local_var = ogs_malloc(sizeof(OpenAPI_pro_se_authentication_info_request_t));
    ogs_assert(pro_se_authentication_info_request_local_var);

    pro_se_authentication_info_request_local_var->serving_network_name = serving_network_name;
    pro_se_authentication_info_request_local_var->relay_service_code = relay_service_code;
    pro_se_authentication_info_request_local_var->resynchronization_info = resynchronization_info;
    pro_se_authentication_info_request_local_var->supported_features = supported_features;

    return pro_se_authentication_info_request_local_var;
}

void OpenAPI_pro_se_authentication_info_request_free(OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pro_se_authentication_info_request) {
        return;
    }
    if (pro_se_authentication_info_request->serving_network_name) {
        ogs_free(pro_se_authentication_info_request->serving_network_name);
        pro_se_authentication_info_request->serving_network_name = NULL;
    }
    if (pro_se_authentication_info_request->resynchronization_info) {
        OpenAPI_resynchronization_info_free(pro_se_authentication_info_request->resynchronization_info);
        pro_se_authentication_info_request->resynchronization_info = NULL;
    }
    if (pro_se_authentication_info_request->supported_features) {
        ogs_free(pro_se_authentication_info_request->supported_features);
        pro_se_authentication_info_request->supported_features = NULL;
    }
    ogs_free(pro_se_authentication_info_request);
}

cJSON *OpenAPI_pro_se_authentication_info_request_convertToJSON(OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pro_se_authentication_info_request == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [ProSeAuthenticationInfoRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pro_se_authentication_info_request->serving_network_name) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [serving_network_name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", pro_se_authentication_info_request->serving_network_name) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [serving_network_name]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "relayServiceCode", pro_se_authentication_info_request->relay_service_code) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [relay_service_code]");
        goto end;
    }

    if (pro_se_authentication_info_request->resynchronization_info) {
    cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_convertToJSON(pro_se_authentication_info_request->resynchronization_info);
    if (resynchronization_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [resynchronization_info]");
        goto end;
    }
    }

    if (pro_se_authentication_info_request->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pro_se_authentication_info_request->supported_features) == NULL) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_parseFromJSON(cJSON *pro_se_authentication_info_requestJSON)
{
    OpenAPI_pro_se_authentication_info_request_t *pro_se_authentication_info_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_network_name = NULL;
    cJSON *relay_service_code = NULL;
    cJSON *resynchronization_info = NULL;
    OpenAPI_resynchronization_info_t *resynchronization_info_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    serving_network_name = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_requestJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_parseFromJSON() failed [serving_network_name]");
        goto end;
    }
    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    relay_service_code = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_requestJSON, "relayServiceCode");
    if (!relay_service_code) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_parseFromJSON() failed [relay_service_code]");
        goto end;
    }
    if (!cJSON_IsNumber(relay_service_code)) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_parseFromJSON() failed [relay_service_code]");
        goto end;
    }

    resynchronization_info = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_requestJSON, "resynchronizationInfo");
    if (resynchronization_info) {
    resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_parseFromJSON(resynchronization_info);
    if (!resynchronization_info_local_nonprim) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON failed [resynchronization_info]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pro_se_authentication_info_requestJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    pro_se_authentication_info_request_local_var = OpenAPI_pro_se_authentication_info_request_create (
        ogs_strdup(serving_network_name->valuestring),
        
        relay_service_code->valuedouble,
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return pro_se_authentication_info_request_local_var;
end:
    if (resynchronization_info_local_nonprim) {
        OpenAPI_resynchronization_info_free(resynchronization_info_local_nonprim);
        resynchronization_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pro_se_authentication_info_request_t *OpenAPI_pro_se_authentication_info_request_copy(OpenAPI_pro_se_authentication_info_request_t *dst, OpenAPI_pro_se_authentication_info_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pro_se_authentication_info_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pro_se_authentication_info_request_convertToJSON() failed");
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

    OpenAPI_pro_se_authentication_info_request_free(dst);
    dst = OpenAPI_pro_se_authentication_info_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

