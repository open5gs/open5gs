
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_control.h"

OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_create(
    OpenAPI_sm_policy_context_data_t *context,
    OpenAPI_sm_policy_decision_t *policy
    )
{
    OpenAPI_sm_policy_control_t *sm_policy_control_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_control_t));
    if (!sm_policy_control_local_var) {
        return NULL;
    }
    sm_policy_control_local_var->context = context;
    sm_policy_control_local_var->policy = policy;

    return sm_policy_control_local_var;
}

void OpenAPI_sm_policy_control_free(OpenAPI_sm_policy_control_t *sm_policy_control)
{
    if (NULL == sm_policy_control) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_sm_policy_context_data_free(sm_policy_control->context);
    OpenAPI_sm_policy_decision_free(sm_policy_control->policy);
    ogs_free(sm_policy_control);
}

cJSON *OpenAPI_sm_policy_control_convertToJSON(OpenAPI_sm_policy_control_t *sm_policy_control)
{
    cJSON *item = NULL;

    if (sm_policy_control == NULL) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [SmPolicyControl]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_policy_control->context) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [context]");
        goto end;
    }
    cJSON *context_local_JSON = OpenAPI_sm_policy_context_data_convertToJSON(sm_policy_control->context);
    if (context_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "context", context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [context]");
        goto end;
    }

    if (!sm_policy_control->policy) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [policy]");
        goto end;
    }
    cJSON *policy_local_JSON = OpenAPI_sm_policy_decision_convertToJSON(sm_policy_control->policy);
    if (policy_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [policy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "policy", policy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed [policy]");
        goto end;
    }

end:
    return item;
}

OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_parseFromJSON(cJSON *sm_policy_controlJSON)
{
    OpenAPI_sm_policy_control_t *sm_policy_control_local_var = NULL;
    cJSON *context = cJSON_GetObjectItemCaseSensitive(sm_policy_controlJSON, "context");
    if (!context) {
        ogs_error("OpenAPI_sm_policy_control_parseFromJSON() failed [context]");
        goto end;
    }

    OpenAPI_sm_policy_context_data_t *context_local_nonprim = NULL;

    context_local_nonprim = OpenAPI_sm_policy_context_data_parseFromJSON(context);

    cJSON *policy = cJSON_GetObjectItemCaseSensitive(sm_policy_controlJSON, "policy");
    if (!policy) {
        ogs_error("OpenAPI_sm_policy_control_parseFromJSON() failed [policy]");
        goto end;
    }

    OpenAPI_sm_policy_decision_t *policy_local_nonprim = NULL;

    policy_local_nonprim = OpenAPI_sm_policy_decision_parseFromJSON(policy);

    sm_policy_control_local_var = OpenAPI_sm_policy_control_create (
        context_local_nonprim,
        policy_local_nonprim
        );

    return sm_policy_control_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_control_t *OpenAPI_sm_policy_control_copy(OpenAPI_sm_policy_control_t *dst, OpenAPI_sm_policy_control_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_control_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_control_convertToJSON() failed");
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

    OpenAPI_sm_policy_control_free(dst);
    dst = OpenAPI_sm_policy_control_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

