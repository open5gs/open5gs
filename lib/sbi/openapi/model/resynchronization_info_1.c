
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "resynchronization_info_1.h"

OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_create(
    char *rand,
    char *auts
)
{
    OpenAPI_resynchronization_info_1_t *resynchronization_info_1_local_var = ogs_malloc(sizeof(OpenAPI_resynchronization_info_1_t));
    ogs_assert(resynchronization_info_1_local_var);

    resynchronization_info_1_local_var->rand = rand;
    resynchronization_info_1_local_var->auts = auts;

    return resynchronization_info_1_local_var;
}

void OpenAPI_resynchronization_info_1_free(OpenAPI_resynchronization_info_1_t *resynchronization_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == resynchronization_info_1) {
        return;
    }
    if (resynchronization_info_1->rand) {
        ogs_free(resynchronization_info_1->rand);
        resynchronization_info_1->rand = NULL;
    }
    if (resynchronization_info_1->auts) {
        ogs_free(resynchronization_info_1->auts);
        resynchronization_info_1->auts = NULL;
    }
    ogs_free(resynchronization_info_1);
}

cJSON *OpenAPI_resynchronization_info_1_convertToJSON(OpenAPI_resynchronization_info_1_t *resynchronization_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (resynchronization_info_1 == NULL) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed [ResynchronizationInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!resynchronization_info_1->rand) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed [rand]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rand", resynchronization_info_1->rand) == NULL) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed [rand]");
        goto end;
    }

    if (!resynchronization_info_1->auts) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed [auts]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "auts", resynchronization_info_1->auts) == NULL) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed [auts]");
        goto end;
    }

end:
    return item;
}

OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_parseFromJSON(cJSON *resynchronization_info_1JSON)
{
    OpenAPI_resynchronization_info_1_t *resynchronization_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rand = NULL;
    cJSON *auts = NULL;
    rand = cJSON_GetObjectItemCaseSensitive(resynchronization_info_1JSON, "rand");
    if (!rand) {
        ogs_error("OpenAPI_resynchronization_info_1_parseFromJSON() failed [rand]");
        goto end;
    }
    if (!cJSON_IsString(rand)) {
        ogs_error("OpenAPI_resynchronization_info_1_parseFromJSON() failed [rand]");
        goto end;
    }

    auts = cJSON_GetObjectItemCaseSensitive(resynchronization_info_1JSON, "auts");
    if (!auts) {
        ogs_error("OpenAPI_resynchronization_info_1_parseFromJSON() failed [auts]");
        goto end;
    }
    if (!cJSON_IsString(auts)) {
        ogs_error("OpenAPI_resynchronization_info_1_parseFromJSON() failed [auts]");
        goto end;
    }

    resynchronization_info_1_local_var = OpenAPI_resynchronization_info_1_create (
        ogs_strdup(rand->valuestring),
        ogs_strdup(auts->valuestring)
    );

    return resynchronization_info_1_local_var;
end:
    return NULL;
}

OpenAPI_resynchronization_info_1_t *OpenAPI_resynchronization_info_1_copy(OpenAPI_resynchronization_info_1_t *dst, OpenAPI_resynchronization_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_resynchronization_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_resynchronization_info_1_convertToJSON() failed");
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

    OpenAPI_resynchronization_info_1_free(dst);
    dst = OpenAPI_resynchronization_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

