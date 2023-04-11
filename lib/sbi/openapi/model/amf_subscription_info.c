
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_subscription_info.h"

OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_create(
    char *amf_instance_id,
    char *subscription_id,
    char *subs_change_notify_correlation_id,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_amf_subscription_info_t *amf_subscription_info_local_var = ogs_malloc(sizeof(OpenAPI_amf_subscription_info_t));
    ogs_assert(amf_subscription_info_local_var);

    amf_subscription_info_local_var->amf_instance_id = amf_instance_id;
    amf_subscription_info_local_var->subscription_id = subscription_id;
    amf_subscription_info_local_var->subs_change_notify_correlation_id = subs_change_notify_correlation_id;
    amf_subscription_info_local_var->context_info = context_info;

    return amf_subscription_info_local_var;
}

void OpenAPI_amf_subscription_info_free(OpenAPI_amf_subscription_info_t *amf_subscription_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_subscription_info) {
        return;
    }
    if (amf_subscription_info->amf_instance_id) {
        ogs_free(amf_subscription_info->amf_instance_id);
        amf_subscription_info->amf_instance_id = NULL;
    }
    if (amf_subscription_info->subscription_id) {
        ogs_free(amf_subscription_info->subscription_id);
        amf_subscription_info->subscription_id = NULL;
    }
    if (amf_subscription_info->subs_change_notify_correlation_id) {
        ogs_free(amf_subscription_info->subs_change_notify_correlation_id);
        amf_subscription_info->subs_change_notify_correlation_id = NULL;
    }
    if (amf_subscription_info->context_info) {
        OpenAPI_context_info_free(amf_subscription_info->context_info);
        amf_subscription_info->context_info = NULL;
    }
    ogs_free(amf_subscription_info);
}

cJSON *OpenAPI_amf_subscription_info_convertToJSON(OpenAPI_amf_subscription_info_t *amf_subscription_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_subscription_info == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [AmfSubscriptionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!amf_subscription_info->amf_instance_id) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [amf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfInstanceId", amf_subscription_info->amf_instance_id) == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [amf_instance_id]");
        goto end;
    }

    if (!amf_subscription_info->subscription_id) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "subscriptionId", amf_subscription_info->subscription_id) == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [subscription_id]");
        goto end;
    }

    if (amf_subscription_info->subs_change_notify_correlation_id) {
    if (cJSON_AddStringToObject(item, "subsChangeNotifyCorrelationId", amf_subscription_info->subs_change_notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [subs_change_notify_correlation_id]");
        goto end;
    }
    }

    if (amf_subscription_info->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(amf_subscription_info->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_parseFromJSON(cJSON *amf_subscription_infoJSON)
{
    OpenAPI_amf_subscription_info_t *amf_subscription_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_instance_id = NULL;
    cJSON *subscription_id = NULL;
    cJSON *subs_change_notify_correlation_id = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    amf_instance_id = cJSON_GetObjectItemCaseSensitive(amf_subscription_infoJSON, "amfInstanceId");
    if (!amf_instance_id) {
        ogs_error("OpenAPI_amf_subscription_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_instance_id)) {
        ogs_error("OpenAPI_amf_subscription_info_parseFromJSON() failed [amf_instance_id]");
        goto end;
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(amf_subscription_infoJSON, "subscriptionId");
    if (!subscription_id) {
        ogs_error("OpenAPI_amf_subscription_info_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_amf_subscription_info_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    subs_change_notify_correlation_id = cJSON_GetObjectItemCaseSensitive(amf_subscription_infoJSON, "subsChangeNotifyCorrelationId");
    if (subs_change_notify_correlation_id) {
    if (!cJSON_IsString(subs_change_notify_correlation_id) && !cJSON_IsNull(subs_change_notify_correlation_id)) {
        ogs_error("OpenAPI_amf_subscription_info_parseFromJSON() failed [subs_change_notify_correlation_id]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(amf_subscription_infoJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    amf_subscription_info_local_var = OpenAPI_amf_subscription_info_create (
        ogs_strdup(amf_instance_id->valuestring),
        ogs_strdup(subscription_id->valuestring),
        subs_change_notify_correlation_id && !cJSON_IsNull(subs_change_notify_correlation_id) ? ogs_strdup(subs_change_notify_correlation_id->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL
    );

    return amf_subscription_info_local_var;
end:
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_amf_subscription_info_t *OpenAPI_amf_subscription_info_copy(OpenAPI_amf_subscription_info_t *dst, OpenAPI_amf_subscription_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_subscription_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_subscription_info_convertToJSON() failed");
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

    OpenAPI_amf_subscription_info_free(dst);
    dst = OpenAPI_amf_subscription_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

