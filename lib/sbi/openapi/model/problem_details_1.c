
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "problem_details_1.h"

OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_create(
    char *type,
    char *title,
    bool is_status,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params
)
{
    OpenAPI_problem_details_1_t *problem_details_1_local_var = ogs_malloc(sizeof(OpenAPI_problem_details_1_t));
    ogs_assert(problem_details_1_local_var);

    problem_details_1_local_var->type = type;
    problem_details_1_local_var->title = title;
    problem_details_1_local_var->is_status = is_status;
    problem_details_1_local_var->status = status;
    problem_details_1_local_var->detail = detail;
    problem_details_1_local_var->instance = instance;
    problem_details_1_local_var->cause = cause;
    problem_details_1_local_var->invalid_params = invalid_params;

    return problem_details_1_local_var;
}

void OpenAPI_problem_details_1_free(OpenAPI_problem_details_1_t *problem_details_1)
{
    if (NULL == problem_details_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(problem_details_1->type);
    ogs_free(problem_details_1->title);
    ogs_free(problem_details_1->detail);
    ogs_free(problem_details_1->instance);
    ogs_free(problem_details_1->cause);
    OpenAPI_list_for_each(problem_details_1->invalid_params, node) {
        OpenAPI_invalid_param_1_free(node->data);
    }
    OpenAPI_list_free(problem_details_1->invalid_params);
    ogs_free(problem_details_1);
}

cJSON *OpenAPI_problem_details_1_convertToJSON(OpenAPI_problem_details_1_t *problem_details_1)
{
    cJSON *item = NULL;

    if (problem_details_1 == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [ProblemDetails_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (problem_details_1->type) {
    if (cJSON_AddStringToObject(item, "type", problem_details_1->type) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [type]");
        goto end;
    }
    }

    if (problem_details_1->title) {
    if (cJSON_AddStringToObject(item, "title", problem_details_1->title) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [title]");
        goto end;
    }
    }

    if (problem_details_1->is_status) {
    if (cJSON_AddNumberToObject(item, "status", problem_details_1->status) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (problem_details_1->detail) {
    if (cJSON_AddStringToObject(item, "detail", problem_details_1->detail) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [detail]");
        goto end;
    }
    }

    if (problem_details_1->instance) {
    if (cJSON_AddStringToObject(item, "instance", problem_details_1->instance) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [instance]");
        goto end;
    }
    }

    if (problem_details_1->cause) {
    if (cJSON_AddStringToObject(item, "cause", problem_details_1->cause) == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (problem_details_1->invalid_params) {
    cJSON *invalid_paramsList = cJSON_AddArrayToObject(item, "invalidParams");
    if (invalid_paramsList == NULL) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [invalid_params]");
        goto end;
    }

    OpenAPI_lnode_t *invalid_params_node;
    if (problem_details_1->invalid_params) {
        OpenAPI_list_for_each(problem_details_1->invalid_params, invalid_params_node) {
            cJSON *itemLocal = OpenAPI_invalid_param_1_convertToJSON(invalid_params_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_problem_details_1_convertToJSON() failed [invalid_params]");
                goto end;
            }
            cJSON_AddItemToArray(invalid_paramsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_parseFromJSON(cJSON *problem_details_1JSON)
{
    OpenAPI_problem_details_1_t *problem_details_1_local_var = NULL;
    cJSON *type = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "type");

    if (type) {
    if (!cJSON_IsString(type)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [type]");
        goto end;
    }
    }

    cJSON *title = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "title");

    if (title) {
    if (!cJSON_IsString(title)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [title]");
        goto end;
    }
    }

    cJSON *status = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "status");

    if (status) {
    if (!cJSON_IsNumber(status)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [status]");
        goto end;
    }
    }

    cJSON *detail = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "detail");

    if (detail) {
    if (!cJSON_IsString(detail)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [detail]");
        goto end;
    }
    }

    cJSON *instance = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "instance");

    if (instance) {
    if (!cJSON_IsString(instance)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [instance]");
        goto end;
    }
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "cause");

    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    cJSON *invalid_params = cJSON_GetObjectItemCaseSensitive(problem_details_1JSON, "invalidParams");

    OpenAPI_list_t *invalid_paramsList;
    if (invalid_params) {
    cJSON *invalid_params_local_nonprimitive;
    if (!cJSON_IsArray(invalid_params)){
        ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [invalid_params]");
        goto end;
    }

    invalid_paramsList = OpenAPI_list_create();

    cJSON_ArrayForEach(invalid_params_local_nonprimitive, invalid_params ) {
        if (!cJSON_IsObject(invalid_params_local_nonprimitive)) {
            ogs_error("OpenAPI_problem_details_1_parseFromJSON() failed [invalid_params]");
            goto end;
        }
        OpenAPI_invalid_param_1_t *invalid_paramsItem = OpenAPI_invalid_param_1_parseFromJSON(invalid_params_local_nonprimitive);

        if (!invalid_paramsItem) {
            ogs_error("No invalid_paramsItem");
            OpenAPI_list_free(invalid_paramsList);
            goto end;
        }

        OpenAPI_list_add(invalid_paramsList, invalid_paramsItem);
    }
    }

    problem_details_1_local_var = OpenAPI_problem_details_1_create (
        type ? ogs_strdup(type->valuestring) : NULL,
        title ? ogs_strdup(title->valuestring) : NULL,
        status ? true : false,
        status ? status->valuedouble : 0,
        detail ? ogs_strdup(detail->valuestring) : NULL,
        instance ? ogs_strdup(instance->valuestring) : NULL,
        cause ? ogs_strdup(cause->valuestring) : NULL,
        invalid_params ? invalid_paramsList : NULL
    );

    return problem_details_1_local_var;
end:
    return NULL;
}

OpenAPI_problem_details_1_t *OpenAPI_problem_details_1_copy(OpenAPI_problem_details_1_t *dst, OpenAPI_problem_details_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_problem_details_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_problem_details_1_convertToJSON() failed");
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

    OpenAPI_problem_details_1_free(dst);
    dst = OpenAPI_problem_details_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

