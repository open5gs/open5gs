
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "muting_exception_instructions_1.h"

OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_create(
    OpenAPI_buffered_notifications_action_e buffered_notifs,
    OpenAPI_subscription_action_e subscription
)
{
    OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1_local_var = ogs_malloc(sizeof(OpenAPI_muting_exception_instructions_1_t));
    ogs_assert(muting_exception_instructions_1_local_var);

    muting_exception_instructions_1_local_var->buffered_notifs = buffered_notifs;
    muting_exception_instructions_1_local_var->subscription = subscription;

    return muting_exception_instructions_1_local_var;
}

void OpenAPI_muting_exception_instructions_1_free(OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == muting_exception_instructions_1) {
        return;
    }
    ogs_free(muting_exception_instructions_1);
}

cJSON *OpenAPI_muting_exception_instructions_1_convertToJSON(OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (muting_exception_instructions_1 == NULL) {
        ogs_error("OpenAPI_muting_exception_instructions_1_convertToJSON() failed [MutingExceptionInstructions_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (muting_exception_instructions_1->buffered_notifs != OpenAPI_buffered_notifications_action_NULL) {
    if (cJSON_AddStringToObject(item, "bufferedNotifs", OpenAPI_buffered_notifications_action_ToString(muting_exception_instructions_1->buffered_notifs)) == NULL) {
        ogs_error("OpenAPI_muting_exception_instructions_1_convertToJSON() failed [buffered_notifs]");
        goto end;
    }
    }

    if (muting_exception_instructions_1->subscription != OpenAPI_subscription_action_NULL) {
    if (cJSON_AddStringToObject(item, "subscription", OpenAPI_subscription_action_ToString(muting_exception_instructions_1->subscription)) == NULL) {
        ogs_error("OpenAPI_muting_exception_instructions_1_convertToJSON() failed [subscription]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_parseFromJSON(cJSON *muting_exception_instructions_1JSON)
{
    OpenAPI_muting_exception_instructions_1_t *muting_exception_instructions_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *buffered_notifs = NULL;
    OpenAPI_buffered_notifications_action_e buffered_notifsVariable = 0;
    cJSON *subscription = NULL;
    OpenAPI_subscription_action_e subscriptionVariable = 0;
    buffered_notifs = cJSON_GetObjectItemCaseSensitive(muting_exception_instructions_1JSON, "bufferedNotifs");
    if (buffered_notifs) {
    if (!cJSON_IsString(buffered_notifs)) {
        ogs_error("OpenAPI_muting_exception_instructions_1_parseFromJSON() failed [buffered_notifs]");
        goto end;
    }
    buffered_notifsVariable = OpenAPI_buffered_notifications_action_FromString(buffered_notifs->valuestring);
    }

    subscription = cJSON_GetObjectItemCaseSensitive(muting_exception_instructions_1JSON, "subscription");
    if (subscription) {
    if (!cJSON_IsString(subscription)) {
        ogs_error("OpenAPI_muting_exception_instructions_1_parseFromJSON() failed [subscription]");
        goto end;
    }
    subscriptionVariable = OpenAPI_subscription_action_FromString(subscription->valuestring);
    }

    muting_exception_instructions_1_local_var = OpenAPI_muting_exception_instructions_1_create (
        buffered_notifs ? buffered_notifsVariable : 0,
        subscription ? subscriptionVariable : 0
    );

    return muting_exception_instructions_1_local_var;
end:
    return NULL;
}

OpenAPI_muting_exception_instructions_1_t *OpenAPI_muting_exception_instructions_1_copy(OpenAPI_muting_exception_instructions_1_t *dst, OpenAPI_muting_exception_instructions_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_muting_exception_instructions_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_muting_exception_instructions_1_convertToJSON() failed");
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

    OpenAPI_muting_exception_instructions_1_free(dst);
    dst = OpenAPI_muting_exception_instructions_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

