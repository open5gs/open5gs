
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_association_release_cause.h"

OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_create(
    )
{
    OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_association_release_cause_t));
    if (!sm_policy_association_release_cause_local_var) {
        return NULL;
    }

    return sm_policy_association_release_cause_local_var;
}

void OpenAPI_sm_policy_association_release_cause_free(OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause)
{
    if (NULL == sm_policy_association_release_cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_policy_association_release_cause);
}

cJSON *OpenAPI_sm_policy_association_release_cause_convertToJSON(OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause)
{
    cJSON *item = NULL;

    if (sm_policy_association_release_cause == NULL) {
        ogs_error("OpenAPI_sm_policy_association_release_cause_convertToJSON() failed [SmPolicyAssociationReleaseCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_parseFromJSON(cJSON *sm_policy_association_release_causeJSON)
{
    OpenAPI_sm_policy_association_release_cause_t *sm_policy_association_release_cause_local_var = NULL;
    sm_policy_association_release_cause_local_var = OpenAPI_sm_policy_association_release_cause_create (
        );

    return sm_policy_association_release_cause_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_association_release_cause_t *OpenAPI_sm_policy_association_release_cause_copy(OpenAPI_sm_policy_association_release_cause_t *dst, OpenAPI_sm_policy_association_release_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_association_release_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_association_release_cause_convertToJSON() failed");
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

    OpenAPI_sm_policy_association_release_cause_free(dst);
    dst = OpenAPI_sm_policy_association_release_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

