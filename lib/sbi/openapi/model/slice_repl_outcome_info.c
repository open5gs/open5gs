
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_repl_outcome_info.h"

OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_create(
    OpenAPI_slice_repl_outcome_e result,
    OpenAPI_slice_repl_failure_cause_e fail_cause
)
{
    OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info_local_var = ogs_malloc(sizeof(OpenAPI_slice_repl_outcome_info_t));
    ogs_assert(slice_repl_outcome_info_local_var);

    slice_repl_outcome_info_local_var->result = result;
    slice_repl_outcome_info_local_var->fail_cause = fail_cause;

    return slice_repl_outcome_info_local_var;
}

void OpenAPI_slice_repl_outcome_info_free(OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_repl_outcome_info) {
        return;
    }
    ogs_free(slice_repl_outcome_info);
}

cJSON *OpenAPI_slice_repl_outcome_info_convertToJSON(OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_repl_outcome_info == NULL) {
        ogs_error("OpenAPI_slice_repl_outcome_info_convertToJSON() failed [SliceReplOutcomeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_repl_outcome_info->result == OpenAPI_slice_repl_outcome_NULL) {
        ogs_error("OpenAPI_slice_repl_outcome_info_convertToJSON() failed [result]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "result", OpenAPI_slice_repl_outcome_ToString(slice_repl_outcome_info->result)) == NULL) {
        ogs_error("OpenAPI_slice_repl_outcome_info_convertToJSON() failed [result]");
        goto end;
    }

    if (slice_repl_outcome_info->fail_cause != OpenAPI_slice_repl_failure_cause_NULL) {
    if (cJSON_AddStringToObject(item, "failCause", OpenAPI_slice_repl_failure_cause_ToString(slice_repl_outcome_info->fail_cause)) == NULL) {
        ogs_error("OpenAPI_slice_repl_outcome_info_convertToJSON() failed [fail_cause]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_parseFromJSON(cJSON *slice_repl_outcome_infoJSON)
{
    OpenAPI_slice_repl_outcome_info_t *slice_repl_outcome_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *result = NULL;
    OpenAPI_slice_repl_outcome_e resultVariable = 0;
    cJSON *fail_cause = NULL;
    OpenAPI_slice_repl_failure_cause_e fail_causeVariable = 0;
    result = cJSON_GetObjectItemCaseSensitive(slice_repl_outcome_infoJSON, "result");
    if (!result) {
        ogs_error("OpenAPI_slice_repl_outcome_info_parseFromJSON() failed [result]");
        goto end;
    }
    if (!cJSON_IsString(result)) {
        ogs_error("OpenAPI_slice_repl_outcome_info_parseFromJSON() failed [result]");
        goto end;
    }
    resultVariable = OpenAPI_slice_repl_outcome_FromString(result->valuestring);

    fail_cause = cJSON_GetObjectItemCaseSensitive(slice_repl_outcome_infoJSON, "failCause");
    if (fail_cause) {
    if (!cJSON_IsString(fail_cause)) {
        ogs_error("OpenAPI_slice_repl_outcome_info_parseFromJSON() failed [fail_cause]");
        goto end;
    }
    fail_causeVariable = OpenAPI_slice_repl_failure_cause_FromString(fail_cause->valuestring);
    }

    slice_repl_outcome_info_local_var = OpenAPI_slice_repl_outcome_info_create (
        resultVariable,
        fail_cause ? fail_causeVariable : 0
    );

    return slice_repl_outcome_info_local_var;
end:
    return NULL;
}

OpenAPI_slice_repl_outcome_info_t *OpenAPI_slice_repl_outcome_info_copy(OpenAPI_slice_repl_outcome_info_t *dst, OpenAPI_slice_repl_outcome_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_repl_outcome_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_repl_outcome_info_convertToJSON() failed");
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

    OpenAPI_slice_repl_outcome_info_free(dst);
    dst = OpenAPI_slice_repl_outcome_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

