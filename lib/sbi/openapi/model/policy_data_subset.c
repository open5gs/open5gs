
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_data_subset.h"

OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_create(
)
{
    OpenAPI_policy_data_subset_t *policy_data_subset_local_var = ogs_malloc(sizeof(OpenAPI_policy_data_subset_t));
    ogs_assert(policy_data_subset_local_var);


    return policy_data_subset_local_var;
}

void OpenAPI_policy_data_subset_free(OpenAPI_policy_data_subset_t *policy_data_subset)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_data_subset) {
        return;
    }
    ogs_free(policy_data_subset);
}

cJSON *OpenAPI_policy_data_subset_convertToJSON(OpenAPI_policy_data_subset_t *policy_data_subset)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (policy_data_subset == NULL) {
        ogs_error("OpenAPI_policy_data_subset_convertToJSON() failed [PolicyDataSubset]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_parseFromJSON(cJSON *policy_data_subsetJSON)
{
    OpenAPI_policy_data_subset_t *policy_data_subset_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    policy_data_subset_local_var = OpenAPI_policy_data_subset_create (
    );

    return policy_data_subset_local_var;
end:
    return NULL;
}

OpenAPI_policy_data_subset_t *OpenAPI_policy_data_subset_copy(OpenAPI_policy_data_subset_t *dst, OpenAPI_policy_data_subset_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_data_subset_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_data_subset_convertToJSON() failed");
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

    OpenAPI_policy_data_subset_free(dst);
    dst = OpenAPI_policy_data_subset_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

