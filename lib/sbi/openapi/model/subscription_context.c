
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_context.h"

OpenAPI_subscription_context_t *OpenAPI_subscription_context_create(
    char *subscription_id,
    OpenAPI_subscr_cond_t *subscr_cond
)
{
    OpenAPI_subscription_context_t *subscription_context_local_var = ogs_malloc(sizeof(OpenAPI_subscription_context_t));
    ogs_assert(subscription_context_local_var);

    subscription_context_local_var->subscription_id = subscription_id;
    subscription_context_local_var->subscr_cond = subscr_cond;

    return subscription_context_local_var;
}

void OpenAPI_subscription_context_free(OpenAPI_subscription_context_t *subscription_context)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscription_context) {
        return;
    }
    if (subscription_context->subscription_id) {
        ogs_free(subscription_context->subscription_id);
        subscription_context->subscription_id = NULL;
    }
    if (subscription_context->subscr_cond) {
        OpenAPI_subscr_cond_free(subscription_context->subscr_cond);
        subscription_context->subscr_cond = NULL;
    }
    ogs_free(subscription_context);
}

cJSON *OpenAPI_subscription_context_convertToJSON(OpenAPI_subscription_context_t *subscription_context)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscription_context == NULL) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed [SubscriptionContext]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!subscription_context->subscription_id) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed [subscription_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "subscriptionId", subscription_context->subscription_id) == NULL) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed [subscription_id]");
        goto end;
    }

    if (subscription_context->subscr_cond) {
    cJSON *subscr_cond_local_JSON = OpenAPI_subscr_cond_convertToJSON(subscription_context->subscr_cond);
    if (subscr_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed [subscr_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscrCond", subscr_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed [subscr_cond]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_subscription_context_t *OpenAPI_subscription_context_parseFromJSON(cJSON *subscription_contextJSON)
{
    OpenAPI_subscription_context_t *subscription_context_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *subscription_id = NULL;
    cJSON *subscr_cond = NULL;
    OpenAPI_subscr_cond_t *subscr_cond_local_nonprim = NULL;
    subscription_id = cJSON_GetObjectItemCaseSensitive(subscription_contextJSON, "subscriptionId");
    if (!subscription_id) {
        ogs_error("OpenAPI_subscription_context_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    if (!cJSON_IsString(subscription_id)) {
        ogs_error("OpenAPI_subscription_context_parseFromJSON() failed [subscription_id]");
        goto end;
    }

    subscr_cond = cJSON_GetObjectItemCaseSensitive(subscription_contextJSON, "subscrCond");
    if (subscr_cond) {
    subscr_cond_local_nonprim = OpenAPI_subscr_cond_parseFromJSON(subscr_cond);
    if (!subscr_cond_local_nonprim) {
        ogs_error("OpenAPI_subscr_cond_parseFromJSON failed [subscr_cond]");
        goto end;
    }
    }

    subscription_context_local_var = OpenAPI_subscription_context_create (
        ogs_strdup(subscription_id->valuestring),
        subscr_cond ? subscr_cond_local_nonprim : NULL
    );

    return subscription_context_local_var;
end:
    if (subscr_cond_local_nonprim) {
        OpenAPI_subscr_cond_free(subscr_cond_local_nonprim);
        subscr_cond_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_subscription_context_t *OpenAPI_subscription_context_copy(OpenAPI_subscription_context_t *dst, OpenAPI_subscription_context_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_context_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_context_convertToJSON() failed");
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

    OpenAPI_subscription_context_free(dst);
    dst = OpenAPI_subscription_context_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

