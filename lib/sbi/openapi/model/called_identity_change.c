
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "called_identity_change.h"

OpenAPI_called_identity_change_t *OpenAPI_called_identity_change_create(
    char *called_identity,
    char *change_time
)
{
    OpenAPI_called_identity_change_t *called_identity_change_local_var = ogs_malloc(sizeof(OpenAPI_called_identity_change_t));
    ogs_assert(called_identity_change_local_var);

    called_identity_change_local_var->called_identity = called_identity;
    called_identity_change_local_var->change_time = change_time;

    return called_identity_change_local_var;
}

void OpenAPI_called_identity_change_free(OpenAPI_called_identity_change_t *called_identity_change)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == called_identity_change) {
        return;
    }
    if (called_identity_change->called_identity) {
        ogs_free(called_identity_change->called_identity);
        called_identity_change->called_identity = NULL;
    }
    if (called_identity_change->change_time) {
        ogs_free(called_identity_change->change_time);
        called_identity_change->change_time = NULL;
    }
    ogs_free(called_identity_change);
}

cJSON *OpenAPI_called_identity_change_convertToJSON(OpenAPI_called_identity_change_t *called_identity_change)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (called_identity_change == NULL) {
        ogs_error("OpenAPI_called_identity_change_convertToJSON() failed [CalledIdentityChange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (called_identity_change->called_identity) {
    if (cJSON_AddStringToObject(item, "calledIdentity", called_identity_change->called_identity) == NULL) {
        ogs_error("OpenAPI_called_identity_change_convertToJSON() failed [called_identity]");
        goto end;
    }
    }

    if (called_identity_change->change_time) {
    if (cJSON_AddStringToObject(item, "changeTime", called_identity_change->change_time) == NULL) {
        ogs_error("OpenAPI_called_identity_change_convertToJSON() failed [change_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_called_identity_change_t *OpenAPI_called_identity_change_parseFromJSON(cJSON *called_identity_changeJSON)
{
    OpenAPI_called_identity_change_t *called_identity_change_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *called_identity = NULL;
    cJSON *change_time = NULL;
    called_identity = cJSON_GetObjectItemCaseSensitive(called_identity_changeJSON, "calledIdentity");
    if (called_identity) {
    if (!cJSON_IsString(called_identity) && !cJSON_IsNull(called_identity)) {
        ogs_error("OpenAPI_called_identity_change_parseFromJSON() failed [called_identity]");
        goto end;
    }
    }

    change_time = cJSON_GetObjectItemCaseSensitive(called_identity_changeJSON, "changeTime");
    if (change_time) {
    if (!cJSON_IsString(change_time) && !cJSON_IsNull(change_time)) {
        ogs_error("OpenAPI_called_identity_change_parseFromJSON() failed [change_time]");
        goto end;
    }
    }

    called_identity_change_local_var = OpenAPI_called_identity_change_create (
        called_identity && !cJSON_IsNull(called_identity) ? ogs_strdup(called_identity->valuestring) : NULL,
        change_time && !cJSON_IsNull(change_time) ? ogs_strdup(change_time->valuestring) : NULL
    );

    return called_identity_change_local_var;
end:
    return NULL;
}

OpenAPI_called_identity_change_t *OpenAPI_called_identity_change_copy(OpenAPI_called_identity_change_t *dst, OpenAPI_called_identity_change_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_called_identity_change_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_called_identity_change_convertToJSON() failed");
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

    OpenAPI_called_identity_change_free(dst);
    dst = OpenAPI_called_identity_change_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

