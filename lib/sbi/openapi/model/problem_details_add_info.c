
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "problem_details_add_info.h"

OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_create(
    bool is_remote_error,
    int remote_error
)
{
    OpenAPI_problem_details_add_info_t *problem_details_add_info_local_var = ogs_malloc(sizeof(OpenAPI_problem_details_add_info_t));
    ogs_assert(problem_details_add_info_local_var);

    problem_details_add_info_local_var->is_remote_error = is_remote_error;
    problem_details_add_info_local_var->remote_error = remote_error;

    return problem_details_add_info_local_var;
}

void OpenAPI_problem_details_add_info_free(OpenAPI_problem_details_add_info_t *problem_details_add_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == problem_details_add_info) {
        return;
    }
    ogs_free(problem_details_add_info);
}

cJSON *OpenAPI_problem_details_add_info_convertToJSON(OpenAPI_problem_details_add_info_t *problem_details_add_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (problem_details_add_info == NULL) {
        ogs_error("OpenAPI_problem_details_add_info_convertToJSON() failed [ProblemDetailsAddInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (problem_details_add_info->is_remote_error) {
    if (cJSON_AddBoolToObject(item, "remoteError", problem_details_add_info->remote_error) == NULL) {
        ogs_error("OpenAPI_problem_details_add_info_convertToJSON() failed [remote_error]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_parseFromJSON(cJSON *problem_details_add_infoJSON)
{
    OpenAPI_problem_details_add_info_t *problem_details_add_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *remote_error = NULL;
    remote_error = cJSON_GetObjectItemCaseSensitive(problem_details_add_infoJSON, "remoteError");
    if (remote_error) {
    if (!cJSON_IsBool(remote_error)) {
        ogs_error("OpenAPI_problem_details_add_info_parseFromJSON() failed [remote_error]");
        goto end;
    }
    }

    problem_details_add_info_local_var = OpenAPI_problem_details_add_info_create (
        remote_error ? true : false,
        remote_error ? remote_error->valueint : 0
    );

    return problem_details_add_info_local_var;
end:
    return NULL;
}

OpenAPI_problem_details_add_info_t *OpenAPI_problem_details_add_info_copy(OpenAPI_problem_details_add_info_t *dst, OpenAPI_problem_details_add_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_problem_details_add_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_problem_details_add_info_convertToJSON() failed");
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

    OpenAPI_problem_details_add_info_free(dst);
    dst = OpenAPI_problem_details_add_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

