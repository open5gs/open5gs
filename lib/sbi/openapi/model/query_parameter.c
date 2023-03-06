
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "query_parameter.h"

OpenAPI_query_parameter_t *OpenAPI_query_parameter_create(
    char *name,
    char *value
)
{
    OpenAPI_query_parameter_t *query_parameter_local_var = ogs_malloc(sizeof(OpenAPI_query_parameter_t));
    ogs_assert(query_parameter_local_var);

    query_parameter_local_var->name = name;
    query_parameter_local_var->value = value;

    return query_parameter_local_var;
}

void OpenAPI_query_parameter_free(OpenAPI_query_parameter_t *query_parameter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == query_parameter) {
        return;
    }
    if (query_parameter->name) {
        ogs_free(query_parameter->name);
        query_parameter->name = NULL;
    }
    if (query_parameter->value) {
        ogs_free(query_parameter->value);
        query_parameter->value = NULL;
    }
    ogs_free(query_parameter);
}

cJSON *OpenAPI_query_parameter_convertToJSON(OpenAPI_query_parameter_t *query_parameter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (query_parameter == NULL) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed [QueryParameter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!query_parameter->name) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed [name]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "name", query_parameter->name) == NULL) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed [name]");
        goto end;
    }

    if (!query_parameter->value) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed [value]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "value", query_parameter->value) == NULL) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed [value]");
        goto end;
    }

end:
    return item;
}

OpenAPI_query_parameter_t *OpenAPI_query_parameter_parseFromJSON(cJSON *query_parameterJSON)
{
    OpenAPI_query_parameter_t *query_parameter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *name = NULL;
    cJSON *value = NULL;
    name = cJSON_GetObjectItemCaseSensitive(query_parameterJSON, "name");
    if (!name) {
        ogs_error("OpenAPI_query_parameter_parseFromJSON() failed [name]");
        goto end;
    }
    if (!cJSON_IsString(name)) {
        ogs_error("OpenAPI_query_parameter_parseFromJSON() failed [name]");
        goto end;
    }

    value = cJSON_GetObjectItemCaseSensitive(query_parameterJSON, "value");
    if (!value) {
        ogs_error("OpenAPI_query_parameter_parseFromJSON() failed [value]");
        goto end;
    }
    if (!cJSON_IsString(value)) {
        ogs_error("OpenAPI_query_parameter_parseFromJSON() failed [value]");
        goto end;
    }

    query_parameter_local_var = OpenAPI_query_parameter_create (
        ogs_strdup(name->valuestring),
        ogs_strdup(value->valuestring)
    );

    return query_parameter_local_var;
end:
    return NULL;
}

OpenAPI_query_parameter_t *OpenAPI_query_parameter_copy(OpenAPI_query_parameter_t *dst, OpenAPI_query_parameter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_query_parameter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_query_parameter_convertToJSON() failed");
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

    OpenAPI_query_parameter_free(dst);
    dst = OpenAPI_query_parameter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

