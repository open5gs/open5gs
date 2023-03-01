
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "scp_domain_routing_info_subscription.h"

OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_create(
    char *callback_uri,
    char *validity_time,
    char *req_instance_id,
    bool is_local_ind,
    int local_ind
)
{
    OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription_local_var = ogs_malloc(sizeof(OpenAPI_scp_domain_routing_info_subscription_t));
    ogs_assert(scp_domain_routing_info_subscription_local_var);

    scp_domain_routing_info_subscription_local_var->callback_uri = callback_uri;
    scp_domain_routing_info_subscription_local_var->validity_time = validity_time;
    scp_domain_routing_info_subscription_local_var->req_instance_id = req_instance_id;
    scp_domain_routing_info_subscription_local_var->is_local_ind = is_local_ind;
    scp_domain_routing_info_subscription_local_var->local_ind = local_ind;

    return scp_domain_routing_info_subscription_local_var;
}

void OpenAPI_scp_domain_routing_info_subscription_free(OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == scp_domain_routing_info_subscription) {
        return;
    }
    if (scp_domain_routing_info_subscription->callback_uri) {
        ogs_free(scp_domain_routing_info_subscription->callback_uri);
        scp_domain_routing_info_subscription->callback_uri = NULL;
    }
    if (scp_domain_routing_info_subscription->validity_time) {
        ogs_free(scp_domain_routing_info_subscription->validity_time);
        scp_domain_routing_info_subscription->validity_time = NULL;
    }
    if (scp_domain_routing_info_subscription->req_instance_id) {
        ogs_free(scp_domain_routing_info_subscription->req_instance_id);
        scp_domain_routing_info_subscription->req_instance_id = NULL;
    }
    ogs_free(scp_domain_routing_info_subscription);
}

cJSON *OpenAPI_scp_domain_routing_info_subscription_convertToJSON(OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (scp_domain_routing_info_subscription == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [ScpDomainRoutingInfoSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!scp_domain_routing_info_subscription->callback_uri) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [callback_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackUri", scp_domain_routing_info_subscription->callback_uri) == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [callback_uri]");
        goto end;
    }

    if (scp_domain_routing_info_subscription->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", scp_domain_routing_info_subscription->validity_time) == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (scp_domain_routing_info_subscription->req_instance_id) {
    if (cJSON_AddStringToObject(item, "reqInstanceId", scp_domain_routing_info_subscription->req_instance_id) == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [req_instance_id]");
        goto end;
    }
    }

    if (scp_domain_routing_info_subscription->is_local_ind) {
    if (cJSON_AddBoolToObject(item, "localInd", scp_domain_routing_info_subscription->local_ind) == NULL) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed [local_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_parseFromJSON(cJSON *scp_domain_routing_info_subscriptionJSON)
{
    OpenAPI_scp_domain_routing_info_subscription_t *scp_domain_routing_info_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_uri = NULL;
    cJSON *validity_time = NULL;
    cJSON *req_instance_id = NULL;
    cJSON *local_ind = NULL;
    callback_uri = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_subscriptionJSON, "callbackUri");
    if (!callback_uri) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }
    if (!cJSON_IsString(callback_uri)) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_parseFromJSON() failed [callback_uri]");
        goto end;
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_subscriptionJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    req_instance_id = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_subscriptionJSON, "reqInstanceId");
    if (req_instance_id) {
    if (!cJSON_IsString(req_instance_id) && !cJSON_IsNull(req_instance_id)) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_parseFromJSON() failed [req_instance_id]");
        goto end;
    }
    }

    local_ind = cJSON_GetObjectItemCaseSensitive(scp_domain_routing_info_subscriptionJSON, "localInd");
    if (local_ind) {
    if (!cJSON_IsBool(local_ind)) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_parseFromJSON() failed [local_ind]");
        goto end;
    }
    }

    scp_domain_routing_info_subscription_local_var = OpenAPI_scp_domain_routing_info_subscription_create (
        ogs_strdup(callback_uri->valuestring),
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        req_instance_id && !cJSON_IsNull(req_instance_id) ? ogs_strdup(req_instance_id->valuestring) : NULL,
        local_ind ? true : false,
        local_ind ? local_ind->valueint : 0
    );

    return scp_domain_routing_info_subscription_local_var;
end:
    return NULL;
}

OpenAPI_scp_domain_routing_info_subscription_t *OpenAPI_scp_domain_routing_info_subscription_copy(OpenAPI_scp_domain_routing_info_subscription_t *dst, OpenAPI_scp_domain_routing_info_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_scp_domain_routing_info_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_scp_domain_routing_info_subscription_convertToJSON() failed");
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

    OpenAPI_scp_domain_routing_info_subscription_free(dst);
    dst = OpenAPI_scp_domain_routing_info_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

