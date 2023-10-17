
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "hss_subscription_item.h"

OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_create(
    char *hss_instance_id,
    char *subscription_id,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_hss_subscription_item_t *hss_subscription_item_local_var = ogs_malloc(sizeof(OpenAPI_hss_subscription_item_t));
    ogs_assert(hss_subscription_item_local_var);

    hss_subscription_item_local_var->hss_instance_id = hss_instance_id;
    hss_subscription_item_local_var->subscription_id = subscription_id;
    hss_subscription_item_local_var->context_info = context_info;

    return hss_subscription_item_local_var;
}

void OpenAPI_hss_subscription_item_free(OpenAPI_hss_subscription_item_t *hss_subscription_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == hss_subscription_item) {
        return;
    }
    if (hss_subscription_item->hss_instance_id) {
        ogs_free(hss_subscription_item->hss_instance_id);
        hss_subscription_item->hss_instance_id = NULL;
    }
    if (hss_subscription_item->subscription_id) {
        ogs_free(hss_subscription_item->subscription_id);
        hss_subscription_item->subscription_id = NULL;
    }
    if (hss_subscription_item->context_info) {
        OpenAPI_context_info_free(hss_subscription_item->context_info);
        hss_subscription_item->context_info = NULL;
    }
    ogs_free(hss_subscription_item);
}

cJSON *OpenAPI_hss_subscription_item_convertToJSON(OpenAPI_hss_subscription_item_t *hss_subscription_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (hss_subscription_item == NULL) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [HssSubscriptionItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!hss_subscription_item->hss_instance_id) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [hss_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "hssInstanceId", hss_subscription_item->hss_instance_id) == NULL) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [hss_instance_id]");
        goto end;
    }

    if (!hss_subscription_item->subscription_id) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "subscriptionId", hss_subscription_item->subscription_id) == NULL) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [subscription_id]");
        goto end;
    }

    if (hss_subscription_item->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(hss_subscription_item->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_parseFromJSON(cJSON *hss_subscription_itemJSON)
{
    OpenAPI_hss_subscription_item_t *hss_subscription_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *hss_instance_id = NULL;
    cJSON *subscription_id = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    hss_instance_id = cJSON_GetObjectItemCaseSensitive(hss_subscription_itemJSON, "hssInstanceId");
    if (!hss_instance_id) {
        ogs_error("OpenAPI_hss_subscription_item_parseFromJSON() failed [hss_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(hss_instance_id)) {
        ogs_error("OpenAPI_hss_subscription_item_parseFromJSON() failed [hss_instance_id]");
        goto end;
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(hss_subscription_itemJSON, "subscriptionId");
    if (!subscription_id) {
        ogs_error("OpenAPI_hss_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_hss_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    context_info = cJSON_GetObjectItemCaseSensitive(hss_subscription_itemJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    hss_subscription_item_local_var = OpenAPI_hss_subscription_item_create (
        ogs_strdup(hss_instance_id->valuestring),
        ogs_strdup(subscription_id->valuestring),
        context_info ? context_info_local_nonprim : NULL
    );

    return hss_subscription_item_local_var;
end:
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_hss_subscription_item_t *OpenAPI_hss_subscription_item_copy(OpenAPI_hss_subscription_item_t *dst, OpenAPI_hss_subscription_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_hss_subscription_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_hss_subscription_item_convertToJSON() failed");
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

    OpenAPI_hss_subscription_item_free(dst);
    dst = OpenAPI_hss_subscription_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

