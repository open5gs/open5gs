
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "no_profile_match_reason.h"

OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_create(
)
{
    OpenAPI_no_profile_match_reason_t *no_profile_match_reason_local_var = ogs_malloc(sizeof(OpenAPI_no_profile_match_reason_t));
    ogs_assert(no_profile_match_reason_local_var);


    return no_profile_match_reason_local_var;
}

void OpenAPI_no_profile_match_reason_free(OpenAPI_no_profile_match_reason_t *no_profile_match_reason)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == no_profile_match_reason) {
        return;
    }
    ogs_free(no_profile_match_reason);
}

cJSON *OpenAPI_no_profile_match_reason_convertToJSON(OpenAPI_no_profile_match_reason_t *no_profile_match_reason)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (no_profile_match_reason == NULL) {
        ogs_error("OpenAPI_no_profile_match_reason_convertToJSON() failed [NoProfileMatchReason]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_parseFromJSON(cJSON *no_profile_match_reasonJSON)
{
    OpenAPI_no_profile_match_reason_t *no_profile_match_reason_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    no_profile_match_reason_local_var = OpenAPI_no_profile_match_reason_create (
    );

    return no_profile_match_reason_local_var;
end:
    return NULL;
}

OpenAPI_no_profile_match_reason_t *OpenAPI_no_profile_match_reason_copy(OpenAPI_no_profile_match_reason_t *dst, OpenAPI_no_profile_match_reason_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_no_profile_match_reason_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_no_profile_match_reason_convertToJSON() failed");
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

    OpenAPI_no_profile_match_reason_free(dst);
    dst = OpenAPI_no_profile_match_reason_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

