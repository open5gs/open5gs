
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "routing_info_sm_request.h"

OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_create(
    bool is_ip_sm_gw_ind,
    int ip_sm_gw_ind,
    char *supported_features
)
{
    OpenAPI_routing_info_sm_request_t *routing_info_sm_request_local_var = ogs_malloc(sizeof(OpenAPI_routing_info_sm_request_t));
    ogs_assert(routing_info_sm_request_local_var);

    routing_info_sm_request_local_var->is_ip_sm_gw_ind = is_ip_sm_gw_ind;
    routing_info_sm_request_local_var->ip_sm_gw_ind = ip_sm_gw_ind;
    routing_info_sm_request_local_var->supported_features = supported_features;

    return routing_info_sm_request_local_var;
}

void OpenAPI_routing_info_sm_request_free(OpenAPI_routing_info_sm_request_t *routing_info_sm_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == routing_info_sm_request) {
        return;
    }
    if (routing_info_sm_request->supported_features) {
        ogs_free(routing_info_sm_request->supported_features);
        routing_info_sm_request->supported_features = NULL;
    }
    ogs_free(routing_info_sm_request);
}

cJSON *OpenAPI_routing_info_sm_request_convertToJSON(OpenAPI_routing_info_sm_request_t *routing_info_sm_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (routing_info_sm_request == NULL) {
        ogs_error("OpenAPI_routing_info_sm_request_convertToJSON() failed [RoutingInfoSmRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (routing_info_sm_request->is_ip_sm_gw_ind) {
    if (cJSON_AddBoolToObject(item, "ipSmGwInd", routing_info_sm_request->ip_sm_gw_ind) == NULL) {
        ogs_error("OpenAPI_routing_info_sm_request_convertToJSON() failed [ip_sm_gw_ind]");
        goto end;
    }
    }

    if (routing_info_sm_request->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", routing_info_sm_request->supported_features) == NULL) {
        ogs_error("OpenAPI_routing_info_sm_request_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_parseFromJSON(cJSON *routing_info_sm_requestJSON)
{
    OpenAPI_routing_info_sm_request_t *routing_info_sm_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ip_sm_gw_ind = NULL;
    cJSON *supported_features = NULL;
    ip_sm_gw_ind = cJSON_GetObjectItemCaseSensitive(routing_info_sm_requestJSON, "ipSmGwInd");
    if (ip_sm_gw_ind) {
    if (!cJSON_IsBool(ip_sm_gw_ind)) {
        ogs_error("OpenAPI_routing_info_sm_request_parseFromJSON() failed [ip_sm_gw_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(routing_info_sm_requestJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_routing_info_sm_request_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    routing_info_sm_request_local_var = OpenAPI_routing_info_sm_request_create (
        ip_sm_gw_ind ? true : false,
        ip_sm_gw_ind ? ip_sm_gw_ind->valueint : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return routing_info_sm_request_local_var;
end:
    return NULL;
}

OpenAPI_routing_info_sm_request_t *OpenAPI_routing_info_sm_request_copy(OpenAPI_routing_info_sm_request_t *dst, OpenAPI_routing_info_sm_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_routing_info_sm_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_routing_info_sm_request_convertToJSON() failed");
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

    OpenAPI_routing_info_sm_request_free(dst);
    dst = OpenAPI_routing_info_sm_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

