
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_subscription_data.h"

OpenAPI_sms_subscription_data_t *OpenAPI_sms_subscription_data_create(
    int sms_subscribed,
    char *shared_sms_subs_data_id
    )
{
    OpenAPI_sms_subscription_data_t *sms_subscription_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_sms_subscription_data_t));
    if (!sms_subscription_data_local_var) {
        return NULL;
    }
    sms_subscription_data_local_var->sms_subscribed = sms_subscribed;
    sms_subscription_data_local_var->shared_sms_subs_data_id = shared_sms_subs_data_id;

    return sms_subscription_data_local_var;
}

void OpenAPI_sms_subscription_data_free(OpenAPI_sms_subscription_data_t *sms_subscription_data)
{
    if (NULL == sms_subscription_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sms_subscription_data->shared_sms_subs_data_id);
    ogs_free(sms_subscription_data);
}

cJSON *OpenAPI_sms_subscription_data_convertToJSON(OpenAPI_sms_subscription_data_t *sms_subscription_data)
{
    cJSON *item = NULL;

    if (sms_subscription_data == NULL) {
        ogs_error("OpenAPI_sms_subscription_data_convertToJSON() failed [SmsSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sms_subscription_data->sms_subscribed >= 0) {
        if (cJSON_AddBoolToObject(item, "smsSubscribed", sms_subscription_data->sms_subscribed) == NULL) {
            ogs_error("OpenAPI_sms_subscription_data_convertToJSON() failed [sms_subscribed]");
            goto end;
        }
    }

    if (sms_subscription_data->shared_sms_subs_data_id) {
        if (cJSON_AddStringToObject(item, "sharedSmsSubsDataId", sms_subscription_data->shared_sms_subs_data_id) == NULL) {
            ogs_error("OpenAPI_sms_subscription_data_convertToJSON() failed [shared_sms_subs_data_id]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sms_subscription_data_t *OpenAPI_sms_subscription_data_parseFromJSON(cJSON *sms_subscription_dataJSON)
{
    OpenAPI_sms_subscription_data_t *sms_subscription_data_local_var = NULL;
    cJSON *sms_subscribed = cJSON_GetObjectItemCaseSensitive(sms_subscription_dataJSON, "smsSubscribed");

    if (sms_subscribed) {
        if (!cJSON_IsBool(sms_subscribed)) {
            ogs_error("OpenAPI_sms_subscription_data_parseFromJSON() failed [sms_subscribed]");
            goto end;
        }
    }

    cJSON *shared_sms_subs_data_id = cJSON_GetObjectItemCaseSensitive(sms_subscription_dataJSON, "sharedSmsSubsDataId");

    if (shared_sms_subs_data_id) {
        if (!cJSON_IsString(shared_sms_subs_data_id)) {
            ogs_error("OpenAPI_sms_subscription_data_parseFromJSON() failed [shared_sms_subs_data_id]");
            goto end;
        }
    }

    sms_subscription_data_local_var = OpenAPI_sms_subscription_data_create (
        sms_subscribed ? sms_subscribed->valueint : 0,
        shared_sms_subs_data_id ? ogs_strdup(shared_sms_subs_data_id->valuestring) : NULL
        );

    return sms_subscription_data_local_var;
end:
    return NULL;
}

