
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_control_request_trigger.h"

OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_create(
    )
{
    OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger_local_var = OpenAPI_malloc(sizeof(OpenAPI_policy_control_request_trigger_t));
    if (!policy_control_request_trigger_local_var) {
        return NULL;
    }

    return policy_control_request_trigger_local_var;
}

void OpenAPI_policy_control_request_trigger_free(OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger)
{
    if (NULL == policy_control_request_trigger) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(policy_control_request_trigger);
}

cJSON *OpenAPI_policy_control_request_trigger_convertToJSON(OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger)
{
    cJSON *item = NULL;

    if (policy_control_request_trigger == NULL) {
        ogs_error("OpenAPI_policy_control_request_trigger_convertToJSON() failed [PolicyControlRequestTrigger]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_parseFromJSON(cJSON *policy_control_request_triggerJSON)
{
    OpenAPI_policy_control_request_trigger_t *policy_control_request_trigger_local_var = NULL;
    policy_control_request_trigger_local_var = OpenAPI_policy_control_request_trigger_create (
        );

    return policy_control_request_trigger_local_var;
end:
    return NULL;
}

OpenAPI_policy_control_request_trigger_t *OpenAPI_policy_control_request_trigger_copy(OpenAPI_policy_control_request_trigger_t *dst, OpenAPI_policy_control_request_trigger_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_control_request_trigger_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_control_request_trigger_convertToJSON() failed");
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

    OpenAPI_policy_control_request_trigger_free(dst);
    dst = OpenAPI_policy_control_request_trigger_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

