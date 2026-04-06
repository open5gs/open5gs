
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "restricted_status.h"

OpenAPI_restricted_status_t *OpenAPI_restricted_status_create(
    OpenAPI_exception_id_e exception_id,
    char *time_stamp
)
{
    OpenAPI_restricted_status_t *restricted_status_local_var = ogs_malloc(sizeof(OpenAPI_restricted_status_t));
    ogs_assert(restricted_status_local_var);

    restricted_status_local_var->exception_id = exception_id;
    restricted_status_local_var->time_stamp = time_stamp;

    return restricted_status_local_var;
}

void OpenAPI_restricted_status_free(OpenAPI_restricted_status_t *restricted_status)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == restricted_status) {
        return;
    }
    if (restricted_status->time_stamp) {
        ogs_free(restricted_status->time_stamp);
        restricted_status->time_stamp = NULL;
    }
    ogs_free(restricted_status);
}

cJSON *OpenAPI_restricted_status_convertToJSON(OpenAPI_restricted_status_t *restricted_status)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (restricted_status == NULL) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed [RestrictedStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (restricted_status->exception_id == OpenAPI_exception_id_NULL) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed [exception_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "exceptionId", OpenAPI_exception_id_ToString(restricted_status->exception_id)) == NULL) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed [exception_id]");
        goto end;
    }

    if (!restricted_status->time_stamp) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed [time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "timeStamp", restricted_status->time_stamp) == NULL) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed [time_stamp]");
        goto end;
    }

end:
    return item;
}

OpenAPI_restricted_status_t *OpenAPI_restricted_status_parseFromJSON(cJSON *restricted_statusJSON)
{
    OpenAPI_restricted_status_t *restricted_status_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *exception_id = NULL;
    OpenAPI_exception_id_e exception_idVariable = 0;
    cJSON *time_stamp = NULL;
    exception_id = cJSON_GetObjectItemCaseSensitive(restricted_statusJSON, "exceptionId");
    if (!exception_id) {
        ogs_error("OpenAPI_restricted_status_parseFromJSON() failed [exception_id]");
        goto end;
    }
    if (!cJSON_IsString(exception_id)) {
        ogs_error("OpenAPI_restricted_status_parseFromJSON() failed [exception_id]");
        goto end;
    }
    exception_idVariable = OpenAPI_exception_id_FromString(exception_id->valuestring);

    time_stamp = cJSON_GetObjectItemCaseSensitive(restricted_statusJSON, "timeStamp");
    if (!time_stamp) {
        ogs_error("OpenAPI_restricted_status_parseFromJSON() failed [time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(time_stamp) && !cJSON_IsNull(time_stamp)) {
        ogs_error("OpenAPI_restricted_status_parseFromJSON() failed [time_stamp]");
        goto end;
    }

    restricted_status_local_var = OpenAPI_restricted_status_create (
        exception_idVariable,
        ogs_strdup(time_stamp->valuestring)
    );

    return restricted_status_local_var;
end:
    return NULL;
}

OpenAPI_restricted_status_t *OpenAPI_restricted_status_copy(OpenAPI_restricted_status_t *dst, OpenAPI_restricted_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_restricted_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_restricted_status_convertToJSON() failed");
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

    OpenAPI_restricted_status_free(dst);
    dst = OpenAPI_restricted_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

