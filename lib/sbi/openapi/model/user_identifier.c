
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "user_identifier.h"

OpenAPI_user_identifier_t *OpenAPI_user_identifier_create(
    char *supi,
    char *gpsi,
    char *validity_time
    )
{
    OpenAPI_user_identifier_t *user_identifier_local_var = OpenAPI_malloc(sizeof(OpenAPI_user_identifier_t));
    if (!user_identifier_local_var) {
        return NULL;
    }
    user_identifier_local_var->supi = supi;
    user_identifier_local_var->gpsi = gpsi;
    user_identifier_local_var->validity_time = validity_time;

    return user_identifier_local_var;
}

void OpenAPI_user_identifier_free(OpenAPI_user_identifier_t *user_identifier)
{
    if (NULL == user_identifier) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(user_identifier->supi);
    ogs_free(user_identifier->gpsi);
    ogs_free(user_identifier->validity_time);
    ogs_free(user_identifier);
}

cJSON *OpenAPI_user_identifier_convertToJSON(OpenAPI_user_identifier_t *user_identifier)
{
    cJSON *item = NULL;

    if (user_identifier == NULL) {
        ogs_error("OpenAPI_user_identifier_convertToJSON() failed [UserIdentifier]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!user_identifier->supi) {
        ogs_error("OpenAPI_user_identifier_convertToJSON() failed [supi]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "supi", user_identifier->supi) == NULL) {
        ogs_error("OpenAPI_user_identifier_convertToJSON() failed [supi]");
        goto end;
    }

    if (user_identifier->gpsi) {
        if (cJSON_AddStringToObject(item, "gpsi", user_identifier->gpsi) == NULL) {
            ogs_error("OpenAPI_user_identifier_convertToJSON() failed [gpsi]");
            goto end;
        }
    }

    if (user_identifier->validity_time) {
        if (cJSON_AddStringToObject(item, "validityTime", user_identifier->validity_time) == NULL) {
            ogs_error("OpenAPI_user_identifier_convertToJSON() failed [validity_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_user_identifier_t *OpenAPI_user_identifier_parseFromJSON(cJSON *user_identifierJSON)
{
    OpenAPI_user_identifier_t *user_identifier_local_var = NULL;
    cJSON *supi = cJSON_GetObjectItemCaseSensitive(user_identifierJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_user_identifier_parseFromJSON() failed [supi]");
        goto end;
    }


    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_user_identifier_parseFromJSON() failed [supi]");
        goto end;
    }

    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(user_identifierJSON, "gpsi");

    if (gpsi) {
        if (!cJSON_IsString(gpsi)) {
            ogs_error("OpenAPI_user_identifier_parseFromJSON() failed [gpsi]");
            goto end;
        }
    }

    cJSON *validity_time = cJSON_GetObjectItemCaseSensitive(user_identifierJSON, "validityTime");

    if (validity_time) {
        if (!cJSON_IsString(validity_time)) {
            ogs_error("OpenAPI_user_identifier_parseFromJSON() failed [validity_time]");
            goto end;
        }
    }

    user_identifier_local_var = OpenAPI_user_identifier_create (
        ogs_strdup(supi->valuestring),
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        validity_time ? ogs_strdup(validity_time->valuestring) : NULL
        );

    return user_identifier_local_var;
end:
    return NULL;
}

OpenAPI_user_identifier_t *OpenAPI_user_identifier_copy(OpenAPI_user_identifier_t *dst, OpenAPI_user_identifier_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_user_identifier_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_user_identifier_convertToJSON() failed");
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

    OpenAPI_user_identifier_free(dst);
    dst = OpenAPI_user_identifier_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

