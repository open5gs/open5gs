
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "snssai.h"

OpenAPI_snssai_t *OpenAPI_snssai_create(
    int sst,
    char *sd
)
{
    OpenAPI_snssai_t *snssai_local_var = ogs_malloc(sizeof(OpenAPI_snssai_t));
    ogs_assert(snssai_local_var);

    snssai_local_var->sst = sst;
    snssai_local_var->sd = sd;

    return snssai_local_var;
}

void OpenAPI_snssai_free(OpenAPI_snssai_t *snssai)
{
    if (NULL == snssai) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(snssai->sd);
    ogs_free(snssai);
}

cJSON *OpenAPI_snssai_convertToJSON(OpenAPI_snssai_t *snssai)
{
    cJSON *item = NULL;

    if (snssai == NULL) {
        ogs_error("OpenAPI_snssai_convertToJSON() failed [Snssai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "sst", snssai->sst) == NULL) {
        ogs_error("OpenAPI_snssai_convertToJSON() failed [sst]");
        goto end;
    }

    if (snssai->sd) {
    if (cJSON_AddStringToObject(item, "sd", snssai->sd) == NULL) {
        ogs_error("OpenAPI_snssai_convertToJSON() failed [sd]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_snssai_t *OpenAPI_snssai_parseFromJSON(cJSON *snssaiJSON)
{
    OpenAPI_snssai_t *snssai_local_var = NULL;
    cJSON *sst = cJSON_GetObjectItemCaseSensitive(snssaiJSON, "sst");
    if (!sst) {
        ogs_error("OpenAPI_snssai_parseFromJSON() failed [sst]");
        goto end;
    }

    if (!cJSON_IsNumber(sst)) {
        ogs_error("OpenAPI_snssai_parseFromJSON() failed [sst]");
        goto end;
    }

    cJSON *sd = cJSON_GetObjectItemCaseSensitive(snssaiJSON, "sd");

    if (sd) {
    if (!cJSON_IsString(sd)) {
        ogs_error("OpenAPI_snssai_parseFromJSON() failed [sd]");
        goto end;
    }
    }

    snssai_local_var = OpenAPI_snssai_create (
        
        sst->valuedouble,
        sd ? ogs_strdup(sd->valuestring) : NULL
    );

    return snssai_local_var;
end:
    return NULL;
}

OpenAPI_snssai_t *OpenAPI_snssai_copy(OpenAPI_snssai_t *dst, OpenAPI_snssai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_snssai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_snssai_convertToJSON() failed");
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

    OpenAPI_snssai_free(dst);
    dst = OpenAPI_snssai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

