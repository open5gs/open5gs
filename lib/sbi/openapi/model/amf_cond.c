
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "amf_cond.h"

OpenAPI_amf_cond_t *OpenAPI_amf_cond_create(
    char *amf_set_id,
    char *amf_region_id
)
{
    OpenAPI_amf_cond_t *amf_cond_local_var = ogs_malloc(sizeof(OpenAPI_amf_cond_t));
    ogs_assert(amf_cond_local_var);

    amf_cond_local_var->amf_set_id = amf_set_id;
    amf_cond_local_var->amf_region_id = amf_region_id;

    return amf_cond_local_var;
}

void OpenAPI_amf_cond_free(OpenAPI_amf_cond_t *amf_cond)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == amf_cond) {
        return;
    }
    if (amf_cond->amf_set_id) {
        ogs_free(amf_cond->amf_set_id);
        amf_cond->amf_set_id = NULL;
    }
    if (amf_cond->amf_region_id) {
        ogs_free(amf_cond->amf_region_id);
        amf_cond->amf_region_id = NULL;
    }
    ogs_free(amf_cond);
}

cJSON *OpenAPI_amf_cond_convertToJSON(OpenAPI_amf_cond_t *amf_cond)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (amf_cond == NULL) {
        ogs_error("OpenAPI_amf_cond_convertToJSON() failed [AmfCond]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (amf_cond->amf_set_id) {
    if (cJSON_AddStringToObject(item, "amfSetId", amf_cond->amf_set_id) == NULL) {
        ogs_error("OpenAPI_amf_cond_convertToJSON() failed [amf_set_id]");
        goto end;
    }
    }

    if (amf_cond->amf_region_id) {
    if (cJSON_AddStringToObject(item, "amfRegionId", amf_cond->amf_region_id) == NULL) {
        ogs_error("OpenAPI_amf_cond_convertToJSON() failed [amf_region_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_amf_cond_t *OpenAPI_amf_cond_parseFromJSON(cJSON *amf_condJSON)
{
    OpenAPI_amf_cond_t *amf_cond_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *amf_set_id = NULL;
    cJSON *amf_region_id = NULL;
    amf_set_id = cJSON_GetObjectItemCaseSensitive(amf_condJSON, "amfSetId");
    if (amf_set_id) {
    if (!cJSON_IsString(amf_set_id) && !cJSON_IsNull(amf_set_id)) {
        ogs_error("OpenAPI_amf_cond_parseFromJSON() failed [amf_set_id]");
        goto end;
    }
    }

    amf_region_id = cJSON_GetObjectItemCaseSensitive(amf_condJSON, "amfRegionId");
    if (amf_region_id) {
    if (!cJSON_IsString(amf_region_id) && !cJSON_IsNull(amf_region_id)) {
        ogs_error("OpenAPI_amf_cond_parseFromJSON() failed [amf_region_id]");
        goto end;
    }
    }

    amf_cond_local_var = OpenAPI_amf_cond_create (
        amf_set_id && !cJSON_IsNull(amf_set_id) ? ogs_strdup(amf_set_id->valuestring) : NULL,
        amf_region_id && !cJSON_IsNull(amf_region_id) ? ogs_strdup(amf_region_id->valuestring) : NULL
    );

    return amf_cond_local_var;
end:
    return NULL;
}

OpenAPI_amf_cond_t *OpenAPI_amf_cond_copy(OpenAPI_amf_cond_t *dst, OpenAPI_amf_cond_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_amf_cond_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_amf_cond_convertToJSON() failed");
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

    OpenAPI_amf_cond_free(dst);
    dst = OpenAPI_amf_cond_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

