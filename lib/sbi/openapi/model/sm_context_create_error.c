
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_create_error.h"

OpenAPI_sm_context_create_error_t *OpenAPI_sm_context_create_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    char *recovery_time
    )
{
    OpenAPI_sm_context_create_error_t *sm_context_create_error_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_context_create_error_t));
    if (!sm_context_create_error_local_var) {
        return NULL;
    }
    sm_context_create_error_local_var->error = error;
    sm_context_create_error_local_var->n1_sm_msg = n1_sm_msg;
    sm_context_create_error_local_var->recovery_time = recovery_time;

    return sm_context_create_error_local_var;
}

void OpenAPI_sm_context_create_error_free(OpenAPI_sm_context_create_error_t *sm_context_create_error)
{
    if (NULL == sm_context_create_error) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_problem_details_free(sm_context_create_error->error);
    OpenAPI_ref_to_binary_data_free(sm_context_create_error->n1_sm_msg);
    ogs_free(sm_context_create_error->recovery_time);
    ogs_free(sm_context_create_error);
}

cJSON *OpenAPI_sm_context_create_error_convertToJSON(OpenAPI_sm_context_create_error_t *sm_context_create_error)
{
    cJSON *item = NULL;

    if (sm_context_create_error == NULL) {
        ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [SmContextCreateError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_context_create_error->error) {
        ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(sm_context_create_error->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [error]");
        goto end;
    }

    if (sm_context_create_error->n1_sm_msg) {
        cJSON *n1_sm_msg_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(sm_context_create_error->n1_sm_msg);
        if (n1_sm_msg_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [n1_sm_msg]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n1SmMsg", n1_sm_msg_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [n1_sm_msg]");
            goto end;
        }
    }

    if (sm_context_create_error->recovery_time) {
        if (cJSON_AddStringToObject(item, "recoveryTime", sm_context_create_error->recovery_time) == NULL) {
            ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed [recovery_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sm_context_create_error_t *OpenAPI_sm_context_create_error_parseFromJSON(cJSON *sm_context_create_errorJSON)
{
    OpenAPI_sm_context_create_error_t *sm_context_create_error_local_var = NULL;
    cJSON *error = cJSON_GetObjectItemCaseSensitive(sm_context_create_errorJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_sm_context_create_error_parseFromJSON() failed [error]");
        goto end;
    }

    OpenAPI_problem_details_t *error_local_nonprim = NULL;

    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);

    cJSON *n1_sm_msg = cJSON_GetObjectItemCaseSensitive(sm_context_create_errorJSON, "n1SmMsg");

    OpenAPI_ref_to_binary_data_t *n1_sm_msg_local_nonprim = NULL;
    if (n1_sm_msg) {
        n1_sm_msg_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_msg);
    }

    cJSON *recovery_time = cJSON_GetObjectItemCaseSensitive(sm_context_create_errorJSON, "recoveryTime");

    if (recovery_time) {
        if (!cJSON_IsString(recovery_time)) {
            ogs_error("OpenAPI_sm_context_create_error_parseFromJSON() failed [recovery_time]");
            goto end;
        }
    }

    sm_context_create_error_local_var = OpenAPI_sm_context_create_error_create (
        error_local_nonprim,
        n1_sm_msg ? n1_sm_msg_local_nonprim : NULL,
        recovery_time ? ogs_strdup(recovery_time->valuestring) : NULL
        );

    return sm_context_create_error_local_var;
end:
    return NULL;
}

OpenAPI_sm_context_create_error_t *OpenAPI_sm_context_create_error_copy(OpenAPI_sm_context_create_error_t *dst, OpenAPI_sm_context_create_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_create_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_create_error_convertToJSON() failed");
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

    OpenAPI_sm_context_create_error_free(dst);
    dst = OpenAPI_sm_context_create_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

