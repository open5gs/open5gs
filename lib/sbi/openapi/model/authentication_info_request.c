
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "authentication_info_request.h"

OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_create(
    char *supported_features,
    char *serving_network_name,
    OpenAPI_resynchronization_info_t *resynchronization_info,
    char *ausf_instance_id,
    char *cag_id
    )
{
    OpenAPI_authentication_info_request_t *authentication_info_request_local_var = OpenAPI_malloc(sizeof(OpenAPI_authentication_info_request_t));
    if (!authentication_info_request_local_var) {
        return NULL;
    }
    authentication_info_request_local_var->supported_features = supported_features;
    authentication_info_request_local_var->serving_network_name = serving_network_name;
    authentication_info_request_local_var->resynchronization_info = resynchronization_info;
    authentication_info_request_local_var->ausf_instance_id = ausf_instance_id;
    authentication_info_request_local_var->cag_id = cag_id;

    return authentication_info_request_local_var;
}

void OpenAPI_authentication_info_request_free(OpenAPI_authentication_info_request_t *authentication_info_request)
{
    if (NULL == authentication_info_request) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(authentication_info_request->supported_features);
    ogs_free(authentication_info_request->serving_network_name);
    OpenAPI_resynchronization_info_free(authentication_info_request->resynchronization_info);
    ogs_free(authentication_info_request->ausf_instance_id);
    ogs_free(authentication_info_request->cag_id);
    ogs_free(authentication_info_request);
}

cJSON *OpenAPI_authentication_info_request_convertToJSON(OpenAPI_authentication_info_request_t *authentication_info_request)
{
    cJSON *item = NULL;

    if (authentication_info_request == NULL) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [AuthenticationInfoRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (authentication_info_request->supported_features) {
        if (cJSON_AddStringToObject(item, "supportedFeatures", authentication_info_request->supported_features) == NULL) {
            ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [supported_features]");
            goto end;
        }
    }

    if (!authentication_info_request->serving_network_name) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [serving_network_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "servingNetworkName", authentication_info_request->serving_network_name) == NULL) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [serving_network_name]");
        goto end;
    }

    if (authentication_info_request->resynchronization_info) {
        cJSON *resynchronization_info_local_JSON = OpenAPI_resynchronization_info_convertToJSON(authentication_info_request->resynchronization_info);
        if (resynchronization_info_local_JSON == NULL) {
            ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [resynchronization_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "resynchronizationInfo", resynchronization_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [resynchronization_info]");
            goto end;
        }
    }

    if (!authentication_info_request->ausf_instance_id) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [ausf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "ausfInstanceId", authentication_info_request->ausf_instance_id) == NULL) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [ausf_instance_id]");
        goto end;
    }

    if (authentication_info_request->cag_id) {
        if (cJSON_AddStringToObject(item, "cagId", authentication_info_request->cag_id) == NULL) {
            ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed [cag_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_parseFromJSON(cJSON *authentication_info_requestJSON)
{
    OpenAPI_authentication_info_request_t *authentication_info_request_local_var = NULL;
    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(authentication_info_requestJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *serving_network_name = cJSON_GetObjectItemCaseSensitive(authentication_info_requestJSON, "servingNetworkName");
    if (!serving_network_name) {
        ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [serving_network_name]");
        goto end;
    }


    if (!cJSON_IsString(serving_network_name)) {
        ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [serving_network_name]");
        goto end;
    }

    cJSON *resynchronization_info = cJSON_GetObjectItemCaseSensitive(authentication_info_requestJSON, "resynchronizationInfo");

    OpenAPI_resynchronization_info_t *resynchronization_info_local_nonprim = NULL;
    if (resynchronization_info) {
        resynchronization_info_local_nonprim = OpenAPI_resynchronization_info_parseFromJSON(resynchronization_info);
    }

    cJSON *ausf_instance_id = cJSON_GetObjectItemCaseSensitive(authentication_info_requestJSON, "ausfInstanceId");
    if (!ausf_instance_id) {
        ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [ausf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(ausf_instance_id)) {
        ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [ausf_instance_id]");
        goto end;
    }

    cJSON *cag_id = cJSON_GetObjectItemCaseSensitive(authentication_info_requestJSON, "cagId");

    if (cag_id) {
        if (!cJSON_IsString(cag_id)) {
            ogs_error("OpenAPI_authentication_info_request_parseFromJSON() failed [cag_id]");
            goto end;
        }
    }

    authentication_info_request_local_var = OpenAPI_authentication_info_request_create (
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        ogs_strdup(serving_network_name->valuestring),
        resynchronization_info ? resynchronization_info_local_nonprim : NULL,
        ogs_strdup(ausf_instance_id->valuestring),
        cag_id ? ogs_strdup(cag_id->valuestring) : NULL
        );

    return authentication_info_request_local_var;
end:
    return NULL;
}

OpenAPI_authentication_info_request_t *OpenAPI_authentication_info_request_copy(OpenAPI_authentication_info_request_t *dst, OpenAPI_authentication_info_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_authentication_info_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_authentication_info_request_convertToJSON() failed");
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

    OpenAPI_authentication_info_request_free(dst);
    dst = OpenAPI_authentication_info_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

