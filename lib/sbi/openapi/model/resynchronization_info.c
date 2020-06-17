
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resynchronization_info.h"

OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_create(
    char *rand,
    char *auts
    )
{
    OpenAPI_resynchronization_info_t *resynchronization_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_resynchronization_info_t));
    if (!resynchronization_info_local_var) {
        return NULL;
    }
    resynchronization_info_local_var->rand = rand;
    resynchronization_info_local_var->auts = auts;

    return resynchronization_info_local_var;
}

void OpenAPI_resynchronization_info_free(OpenAPI_resynchronization_info_t *resynchronization_info)
{
    if (NULL == resynchronization_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(resynchronization_info->rand);
    ogs_free(resynchronization_info->auts);
    ogs_free(resynchronization_info);
}

cJSON *OpenAPI_resynchronization_info_convertToJSON(OpenAPI_resynchronization_info_t *resynchronization_info)
{
    cJSON *item = NULL;

    if (resynchronization_info == NULL) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed [ResynchronizationInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!resynchronization_info->rand) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed [rand]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "rand", resynchronization_info->rand) == NULL) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed [rand]");
        goto end;
    }

    if (!resynchronization_info->auts) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed [auts]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "auts", resynchronization_info->auts) == NULL) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed [auts]");
        goto end;
    }

end:
    return item;
}

OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_parseFromJSON(cJSON *resynchronization_infoJSON)
{
    OpenAPI_resynchronization_info_t *resynchronization_info_local_var = NULL;
    cJSON *rand = cJSON_GetObjectItemCaseSensitive(resynchronization_infoJSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON() failed [rand]");
        goto end;
    }


    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON() failed [rand]");
        goto end;
    }

    cJSON *auts = cJSON_GetObjectItemCaseSensitive(resynchronization_infoJSON, "auts");
    if (!auts) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON() failed [auts]");
        goto end;
    }


    if (!cJSON_IsString(auts)) {
        ogs_error("OpenAPI_resynchronization_info_parseFromJSON() failed [auts]");
        goto end;
    }

    resynchronization_info_local_var = OpenAPI_resynchronization_info_create (
        ogs_strdup(rand->valuestring),
        ogs_strdup(auts->valuestring)
        );

    return resynchronization_info_local_var;
end:
    return NULL;
}

OpenAPI_resynchronization_info_t *OpenAPI_resynchronization_info_copy(OpenAPI_resynchronization_info_t *dst, OpenAPI_resynchronization_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resynchronization_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resynchronization_info_convertToJSON() failed");
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

    OpenAPI_resynchronization_info_free(dst);
    dst = OpenAPI_resynchronization_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

