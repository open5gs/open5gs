
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pending_policy_counter_status.h"

OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_create(
    char *policy_counter_status,
    char *activation_time
)
{
    OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status_local_var = ogs_malloc(sizeof(OpenAPI_pending_policy_counter_status_t));
    ogs_assert(pending_policy_counter_status_local_var);

    pending_policy_counter_status_local_var->policy_counter_status = policy_counter_status;
    pending_policy_counter_status_local_var->activation_time = activation_time;

    return pending_policy_counter_status_local_var;
}

void OpenAPI_pending_policy_counter_status_free(OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pending_policy_counter_status) {
        return;
    }
    if (pending_policy_counter_status->policy_counter_status) {
        ogs_free(pending_policy_counter_status->policy_counter_status);
        pending_policy_counter_status->policy_counter_status = NULL;
    }
    if (pending_policy_counter_status->activation_time) {
        ogs_free(pending_policy_counter_status->activation_time);
        pending_policy_counter_status->activation_time = NULL;
    }
    ogs_free(pending_policy_counter_status);
}

cJSON *OpenAPI_pending_policy_counter_status_convertToJSON(OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pending_policy_counter_status == NULL) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed [PendingPolicyCounterStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pending_policy_counter_status->policy_counter_status) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed [policy_counter_status]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "policyCounterStatus", pending_policy_counter_status->policy_counter_status) == NULL) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed [policy_counter_status]");
        goto end;
    }

    if (!pending_policy_counter_status->activation_time) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed [activation_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "activationTime", pending_policy_counter_status->activation_time) == NULL) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed [activation_time]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_parseFromJSON(cJSON *pending_policy_counter_statusJSON)
{
    OpenAPI_pending_policy_counter_status_t *pending_policy_counter_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *policy_counter_status = NULL;
    cJSON *activation_time = NULL;
    policy_counter_status = cJSON_GetObjectItemCaseSensitive(pending_policy_counter_statusJSON, "policyCounterStatus");
    if (!policy_counter_status) {
        ogs_error("OpenAPI_pending_policy_counter_status_parseFromJSON() failed [policy_counter_status]");
        goto end;
    }
    if (!cJSON_IsString(policy_counter_status)) {
        ogs_error("OpenAPI_pending_policy_counter_status_parseFromJSON() failed [policy_counter_status]");
        goto end;
    }

    activation_time = cJSON_GetObjectItemCaseSensitive(pending_policy_counter_statusJSON, "activationTime");
    if (!activation_time) {
        ogs_error("OpenAPI_pending_policy_counter_status_parseFromJSON() failed [activation_time]");
        goto end;
    }
    if (!cJSON_IsString(activation_time) && !cJSON_IsNull(activation_time)) {
        ogs_error("OpenAPI_pending_policy_counter_status_parseFromJSON() failed [activation_time]");
        goto end;
    }

    pending_policy_counter_status_local_var = OpenAPI_pending_policy_counter_status_create (
        ogs_strdup(policy_counter_status->valuestring),
        ogs_strdup(activation_time->valuestring)
    );

    return pending_policy_counter_status_local_var;
end:
    return NULL;
}

OpenAPI_pending_policy_counter_status_t *OpenAPI_pending_policy_counter_status_copy(OpenAPI_pending_policy_counter_status_t *dst, OpenAPI_pending_policy_counter_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pending_policy_counter_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pending_policy_counter_status_convertToJSON() failed");
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

    OpenAPI_pending_policy_counter_status_free(dst);
    dst = OpenAPI_pending_policy_counter_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

