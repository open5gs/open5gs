
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_subscription_data_1.h"

OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_create(
    bool is_sms_subscribed,
    int sms_subscribed,
    char *shared_sms_subs_data_id,
    char *supported_features
)
{
    OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1_local_var = ogs_malloc(sizeof(OpenAPI_sms_subscription_data_1_t));
    ogs_assert(sms_subscription_data_1_local_var);

    sms_subscription_data_1_local_var->is_sms_subscribed = is_sms_subscribed;
    sms_subscription_data_1_local_var->sms_subscribed = sms_subscribed;
    sms_subscription_data_1_local_var->shared_sms_subs_data_id = shared_sms_subs_data_id;
    sms_subscription_data_1_local_var->supported_features = supported_features;

    return sms_subscription_data_1_local_var;
}

void OpenAPI_sms_subscription_data_1_free(OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_subscription_data_1) {
        return;
    }
    if (sms_subscription_data_1->shared_sms_subs_data_id) {
        ogs_free(sms_subscription_data_1->shared_sms_subs_data_id);
        sms_subscription_data_1->shared_sms_subs_data_id = NULL;
    }
    if (sms_subscription_data_1->supported_features) {
        ogs_free(sms_subscription_data_1->supported_features);
        sms_subscription_data_1->supported_features = NULL;
    }
    ogs_free(sms_subscription_data_1);
}

cJSON *OpenAPI_sms_subscription_data_1_convertToJSON(OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_subscription_data_1 == NULL) {
        ogs_error("OpenAPI_sms_subscription_data_1_convertToJSON() failed [SmsSubscriptionData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sms_subscription_data_1->is_sms_subscribed) {
    if (cJSON_AddBoolToObject(item, "smsSubscribed", sms_subscription_data_1->sms_subscribed) == NULL) {
        ogs_error("OpenAPI_sms_subscription_data_1_convertToJSON() failed [sms_subscribed]");
        goto end;
    }
    }

    if (sms_subscription_data_1->shared_sms_subs_data_id) {
    if (cJSON_AddStringToObject(item, "sharedSmsSubsDataId", sms_subscription_data_1->shared_sms_subs_data_id) == NULL) {
        ogs_error("OpenAPI_sms_subscription_data_1_convertToJSON() failed [shared_sms_subs_data_id]");
        goto end;
    }
    }

    if (sms_subscription_data_1->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sms_subscription_data_1->supported_features) == NULL) {
        ogs_error("OpenAPI_sms_subscription_data_1_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_parseFromJSON(cJSON *sms_subscription_data_1JSON)
{
    OpenAPI_sms_subscription_data_1_t *sms_subscription_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *sms_subscribed = NULL;
    cJSON *shared_sms_subs_data_id = NULL;
    cJSON *supported_features = NULL;
    sms_subscribed = cJSON_GetObjectItemCaseSensitive(sms_subscription_data_1JSON, "smsSubscribed");
    if (sms_subscribed) {
    if (!cJSON_IsBool(sms_subscribed)) {
        ogs_error("OpenAPI_sms_subscription_data_1_parseFromJSON() failed [sms_subscribed]");
        goto end;
    }
    }

    shared_sms_subs_data_id = cJSON_GetObjectItemCaseSensitive(sms_subscription_data_1JSON, "sharedSmsSubsDataId");
    if (shared_sms_subs_data_id) {
    if (!cJSON_IsString(shared_sms_subs_data_id) && !cJSON_IsNull(shared_sms_subs_data_id)) {
        ogs_error("OpenAPI_sms_subscription_data_1_parseFromJSON() failed [shared_sms_subs_data_id]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sms_subscription_data_1JSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sms_subscription_data_1_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sms_subscription_data_1_local_var = OpenAPI_sms_subscription_data_1_create (
        sms_subscribed ? true : false,
        sms_subscribed ? sms_subscribed->valueint : 0,
        shared_sms_subs_data_id && !cJSON_IsNull(shared_sms_subs_data_id) ? ogs_strdup(shared_sms_subs_data_id->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return sms_subscription_data_1_local_var;
end:
    return NULL;
}

OpenAPI_sms_subscription_data_1_t *OpenAPI_sms_subscription_data_1_copy(OpenAPI_sms_subscription_data_1_t *dst, OpenAPI_sms_subscription_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_subscription_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_subscription_data_1_convertToJSON() failed");
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

    OpenAPI_sms_subscription_data_1_free(dst);
    dst = OpenAPI_sms_subscription_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

