
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_subscription_item.h"

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_create(
    char *smf_instance_id,
    char *subscription_id
)
{
    OpenAPI_smf_subscription_item_t *smf_subscription_item_local_var = ogs_malloc(sizeof(OpenAPI_smf_subscription_item_t));
    ogs_assert(smf_subscription_item_local_var);

    smf_subscription_item_local_var->smf_instance_id = smf_instance_id;
    smf_subscription_item_local_var->subscription_id = subscription_id;

    return smf_subscription_item_local_var;
}

void OpenAPI_smf_subscription_item_free(OpenAPI_smf_subscription_item_t *smf_subscription_item)
{
    if (NULL == smf_subscription_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(smf_subscription_item->smf_instance_id);
    ogs_free(smf_subscription_item->subscription_id);
    ogs_free(smf_subscription_item);
}

cJSON *OpenAPI_smf_subscription_item_convertToJSON(OpenAPI_smf_subscription_item_t *smf_subscription_item)
{
    cJSON *item = NULL;

    if (smf_subscription_item == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [SmfSubscriptionItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "smfInstanceId", smf_subscription_item->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "subscriptionId", smf_subscription_item->subscription_id) == NULL) {
        ogs_error("OpenAPI_smf_subscription_item_convertToJSON() failed [subscription_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_smf_subscription_item_t *OpenAPI_smf_subscription_item_parseFromJSON(cJSON *smf_subscription_itemJSON)
{
    OpenAPI_smf_subscription_item_t *smf_subscription_item_local_var = NULL;
    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(smf_subscription_itemJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(smf_subscription_itemJSON, "subscriptionId");
    if (!subscription_id) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_smf_subscription_item_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    smf_subscription_item_local_var = OpenAPI_smf_subscription_item_create (
        ogs_strdup(smf_instance_id->valuestring),
        ogs_strdup(subscription_id->valuestring)
    );

    return smf_subscription_item_local_var;
end:
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

