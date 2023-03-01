
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_policy_status.h"

OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_create(
)
{
    OpenAPI_bdt_policy_status_t *bdt_policy_status_local_var = ogs_malloc(sizeof(OpenAPI_bdt_policy_status_t));
    ogs_assert(bdt_policy_status_local_var);


    return bdt_policy_status_local_var;
}

void OpenAPI_bdt_policy_status_free(OpenAPI_bdt_policy_status_t *bdt_policy_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bdt_policy_status) {
        return;
    }
    ogs_free(bdt_policy_status);
}

cJSON *OpenAPI_bdt_policy_status_convertToJSON(OpenAPI_bdt_policy_status_t *bdt_policy_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (bdt_policy_status == NULL) {
        ogs_error("OpenAPI_bdt_policy_status_convertToJSON() failed [BdtPolicyStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_parseFromJSON(cJSON *bdt_policy_statusJSON)
{
    OpenAPI_bdt_policy_status_t *bdt_policy_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    bdt_policy_status_local_var = OpenAPI_bdt_policy_status_create (
    );

    return bdt_policy_status_local_var;
end:
    return NULL;
}

OpenAPI_bdt_policy_status_t *OpenAPI_bdt_policy_status_copy(OpenAPI_bdt_policy_status_t *dst, OpenAPI_bdt_policy_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_policy_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_policy_status_convertToJSON() failed");
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

    OpenAPI_bdt_policy_status_free(dst);
    dst = OpenAPI_bdt_policy_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

