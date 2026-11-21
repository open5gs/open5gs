
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "invocation_result.h"

OpenAPI_invocation_result_t *OpenAPI_invocation_result_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_failure_handling_e failure_handling
)
{
    OpenAPI_invocation_result_t *invocation_result_local_var = ogs_malloc(sizeof(OpenAPI_invocation_result_t));
    ogs_assert(invocation_result_local_var);

    invocation_result_local_var->error = error;
    invocation_result_local_var->failure_handling = failure_handling;

    return invocation_result_local_var;
}

void OpenAPI_invocation_result_free(OpenAPI_invocation_result_t *invocation_result)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == invocation_result) {
        return;
    }
    if (invocation_result->error) {
        OpenAPI_problem_details_free(invocation_result->error);
        invocation_result->error = NULL;
    }
    ogs_free(invocation_result);
}

cJSON *OpenAPI_invocation_result_convertToJSON(OpenAPI_invocation_result_t *invocation_result)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (invocation_result == NULL) {
        ogs_error("OpenAPI_invocation_result_convertToJSON() failed [InvocationResult]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (invocation_result->error) {
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(invocation_result->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_invocation_result_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_invocation_result_convertToJSON() failed [error]");
        goto end;
    }
    }

    if (invocation_result->failure_handling != OpenAPI_failure_handling_NULL) {
    if (cJSON_AddStringToObject(item, "failureHandling", OpenAPI_failure_handling_ToString(invocation_result->failure_handling)) == NULL) {
        ogs_error("OpenAPI_invocation_result_convertToJSON() failed [failure_handling]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_invocation_result_t *OpenAPI_invocation_result_parseFromJSON(cJSON *invocation_resultJSON)
{
    OpenAPI_invocation_result_t *invocation_result_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    cJSON *failure_handling = NULL;
    OpenAPI_failure_handling_e failure_handlingVariable = 0;
    error = cJSON_GetObjectItemCaseSensitive(invocation_resultJSON, "error");
    if (error) {
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    if (!error_local_nonprim) {
        ogs_error("OpenAPI_problem_details_parseFromJSON failed [error]");
        goto end;
    }
    }

    failure_handling = cJSON_GetObjectItemCaseSensitive(invocation_resultJSON, "failureHandling");
    if (failure_handling) {
    if (!cJSON_IsString(failure_handling)) {
        ogs_error("OpenAPI_invocation_result_parseFromJSON() failed [failure_handling]");
        goto end;
    }
    failure_handlingVariable = OpenAPI_failure_handling_FromString(failure_handling->valuestring);
    }

    invocation_result_local_var = OpenAPI_invocation_result_create (
        error ? error_local_nonprim : NULL,
        failure_handling ? failure_handlingVariable : 0
    );

    return invocation_result_local_var;
end:
    if (error_local_nonprim) {
        OpenAPI_problem_details_free(error_local_nonprim);
        error_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_invocation_result_t *OpenAPI_invocation_result_copy(OpenAPI_invocation_result_t *dst, OpenAPI_invocation_result_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_invocation_result_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_invocation_result_convertToJSON() failed");
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

    OpenAPI_invocation_result_free(dst);
    dst = OpenAPI_invocation_result_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

