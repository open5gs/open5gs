
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_subscription_item.h"

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_create(
    char *smf_instance_id,
    char *subscription_id,
    OpenAPI_context_info_t *context_info
)
{
    OpenAPI_smf_subscription_item_t *smf_subscription_item_local_var = ogs_malloc(sizeof(OpenAPI_smf_subscription_item_t));
    ogs_assert(smf_subscription_item_local_var);

    smf_subscription_item_local_var->smf_instance_id = smf_instance_id;
    smf_subscription_item_local_var->subscription_id = subscription_id;
    smf_subscription_item_local_var->context_info = context_info;

    return smf_subscription_item_local_var;
}

void OpenAPI_smf_subscription_item_free(OpenAPI_smf_subscription_item_t *smf_subscription_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smf_subscription_item) {
        return;
    }
    if (smf_subscription_item->smf_instance_id) {
        ogs_free(smf_subscription_item->smf_instance_id);
        smf_subscription_item->smf_instance_id = NULL;
    }
    if (smf_subscription_item->subscription_id) {
        ogs_free(smf_subscription_item->subscription_id);
        smf_subscription_item->subscription_id = NULL;
    }
    if (smf_subscription_item->context_info) {
        OpenAPI_context_info_free(smf_subscription_item->context_info);
        smf_subscription_item->context_info = NULL;
    }
    ogs_free(smf_subscription_item);
}

cJSON *OpenAPI_smf_subscription_item_convertToJSON(OpenAPI_smf_subscription_item_t *smf_subscription_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smf_subscription_item == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [SmfSubscriptionItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!smf_subscription_item->smf_instance_id) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [smf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "smfInstanceId", smf_subscription_item->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!smf_subscription_item->subscription_id) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "subscriptionId", smf_subscription_item->subscription_id) == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [subscription_id]");
        goto end;
    }

    if (smf_subscription_item->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(smf_subscription_item->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [context_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_parseFromJSON(cJSON *smf_subscription_itemJSON)
{
    OpenAPI_smf_subscription_item_t *smf_subscription_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *smf_instance_id = NULL;
    cJSON *subscription_id = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    smf_instance_id = cJSON_GetObjectItemCaseSensitive(smf_subscription_itemJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(smf_subscription_itemJSON, "subscriptionId");
    if (!subscription_id) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    context_info = cJSON_GetObjectItemCaseSensitive(smf_subscription_itemJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    smf_subscription_item_local_var = OpenAPI_smf_subscription_item_create (
        ogs_strdup(smf_instance_id->valuestring),
        ogs_strdup(subscription_id->valuestring),
        context_info ? context_info_local_nonprim : NULL
    );

    return smf_subscription_item_local_var;
end:
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_copy(OpenAPI_smf_subscription_item_t *dst, OpenAPI_smf_subscription_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_subscription_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed");
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

    OpenAPI_smf_subscription_item_free(dst);
    dst = OpenAPI_smf_subscription_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

