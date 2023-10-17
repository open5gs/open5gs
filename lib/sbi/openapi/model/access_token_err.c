
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "access_token_err.h"

char *OpenAPI_erroraccess_token_err_ToString(OpenAPI_access_token_err_error_e error)
{
    const char *errorArray[] =  { "NULL", "invalid_request", "invalid_client", "invalid_grant", "unauthorized_client", "unsupported_grant_type", "invalid_scope" };
    size_t sizeofArray = sizeof(errorArray) / sizeof(errorArray[0]);
    if (error < sizeofArray)
        return (char *)errorArray[error];
    else
        return (char *)"Unknown";
}

OpenAPI_access_token_err_error_e OpenAPI_erroraccess_token_err_FromString(char* error)
{
    int stringToReturn = 0;
    const char *errorArray[] =  { "NULL", "invalid_request", "invalid_client", "invalid_grant", "unauthorized_client", "unsupported_grant_type", "invalid_scope" };
    size_t sizeofArray = sizeof(errorArray) / sizeof(errorArray[0]);
    while (stringToReturn < sizeofArray) {
        if (strcmp(error, errorArray[stringToReturn]) == 0) {
            return stringToReturn;
        }
        stringToReturn++;
    }
    return 0;
}
OpenAPI_access_token_err_t *OpenAPI_access_token_err_create(
    OpenAPI_access_token_err_error_e error,
    char *error_description,
    char *error_uri
)
{
    OpenAPI_access_token_err_t *access_token_err_local_var = ogs_malloc(sizeof(OpenAPI_access_token_err_t));
    ogs_assert(access_token_err_local_var);

    access_token_err_local_var->error = error;
    access_token_err_local_var->error_description = error_description;
    access_token_err_local_var->error_uri = error_uri;

    return access_token_err_local_var;
}

void OpenAPI_access_token_err_free(OpenAPI_access_token_err_t *access_token_err)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == access_token_err) {
        return;
    }
    if (access_token_err->error_description) {
        ogs_free(access_token_err->error_description);
        access_token_err->error_description = NULL;
    }
    if (access_token_err->error_uri) {
        ogs_free(access_token_err->error_uri);
        access_token_err->error_uri = NULL;
    }
    ogs_free(access_token_err);
}

cJSON *OpenAPI_access_token_err_convertToJSON(OpenAPI_access_token_err_t *access_token_err)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (access_token_err == NULL) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed [AccessTokenErr]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (access_token_err->error == OpenAPI_access_token_err_ERROR_NULL) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed [error]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "error", OpenAPI_erroraccess_token_err_ToString(access_token_err->error)) == NULL) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed [error]");
        goto end;
    }

    if (access_token_err->error_description) {
    if (cJSON_AddStringToObject(item, "error_description", access_token_err->error_description) == NULL) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed [error_description]");
        goto end;
    }
    }

    if (access_token_err->error_uri) {
    if (cJSON_AddStringToObject(item, "error_uri", access_token_err->error_uri) == NULL) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed [error_uri]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_access_token_err_t *OpenAPI_access_token_err_parseFromJSON(cJSON *access_token_errJSON)
{
    OpenAPI_access_token_err_t *access_token_err_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_access_token_err_error_e errorVariable = 0;
    cJSON *error_description = NULL;
    cJSON *error_uri = NULL;
    error = cJSON_GetObjectItemCaseSensitive(access_token_errJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON() failed [error]");
        goto end;
    }
    if (!cJSON_IsString(error)) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON() failed [error]");
        goto end;
    }
    errorVariable = OpenAPI_erroraccess_token_err_FromString(error->valuestring);

    error_description = cJSON_GetObjectItemCaseSensitive(access_token_errJSON, "error_description");
    if (error_description) {
    if (!cJSON_IsString(error_description) && !cJSON_IsNull(error_description)) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON() failed [error_description]");
        goto end;
    }
    }

    error_uri = cJSON_GetObjectItemCaseSensitive(access_token_errJSON, "error_uri");
    if (error_uri) {
    if (!cJSON_IsString(error_uri) && !cJSON_IsNull(error_uri)) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON() failed [error_uri]");
        goto end;
    }
    }

    access_token_err_local_var = OpenAPI_access_token_err_create (
        errorVariable,
        error_description && !cJSON_IsNull(error_description) ? ogs_strdup(error_description->valuestring) : NULL,
        error_uri && !cJSON_IsNull(error_uri) ? ogs_strdup(error_uri->valuestring) : NULL
    );

    return access_token_err_local_var;
end:
    return NULL;
}

OpenAPI_access_token_err_t *OpenAPI_access_token_err_copy(OpenAPI_access_token_err_t *dst, OpenAPI_access_token_err_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_access_token_err_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_access_token_err_convertToJSON() failed");
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

    OpenAPI_access_token_err_free(dst);
    dst = OpenAPI_access_token_err_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

