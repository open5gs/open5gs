
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "svc_experience.h"

OpenAPI_svc_experience_t *OpenAPI_svc_experience_create(
    bool is_mos,
    float mos,
    bool is_upper_range,
    float upper_range,
    bool is_lower_range,
    float lower_range
)
{
    OpenAPI_svc_experience_t *svc_experience_local_var = ogs_malloc(sizeof(OpenAPI_svc_experience_t));
    ogs_assert(svc_experience_local_var);

    svc_experience_local_var->is_mos = is_mos;
    svc_experience_local_var->mos = mos;
    svc_experience_local_var->is_upper_range = is_upper_range;
    svc_experience_local_var->upper_range = upper_range;
    svc_experience_local_var->is_lower_range = is_lower_range;
    svc_experience_local_var->lower_range = lower_range;

    return svc_experience_local_var;
}

void OpenAPI_svc_experience_free(OpenAPI_svc_experience_t *svc_experience)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == svc_experience) {
        return;
    }
    ogs_free(svc_experience);
}

cJSON *OpenAPI_svc_experience_convertToJSON(OpenAPI_svc_experience_t *svc_experience)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (svc_experience == NULL) {
        ogs_error("OpenAPI_svc_experience_convertToJSON() failed [SvcExperience]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (svc_experience->is_mos) {
    if (cJSON_AddNumberToObject(item, "mos", svc_experience->mos) == NULL) {
        ogs_error("OpenAPI_svc_experience_convertToJSON() failed [mos]");
        goto end;
    }
    }

    if (svc_experience->is_upper_range) {
    if (cJSON_AddNumberToObject(item, "upperRange", svc_experience->upper_range) == NULL) {
        ogs_error("OpenAPI_svc_experience_convertToJSON() failed [upper_range]");
        goto end;
    }
    }

    if (svc_experience->is_lower_range) {
    if (cJSON_AddNumberToObject(item, "lowerRange", svc_experience->lower_range) == NULL) {
        ogs_error("OpenAPI_svc_experience_convertToJSON() failed [lower_range]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_svc_experience_t *OpenAPI_svc_experience_parseFromJSON(cJSON *svc_experienceJSON)
{
    OpenAPI_svc_experience_t *svc_experience_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mos = NULL;
    cJSON *upper_range = NULL;
    cJSON *lower_range = NULL;
    mos = cJSON_GetObjectItemCaseSensitive(svc_experienceJSON, "mos");
    if (mos) {
    if (!cJSON_IsNumber(mos)) {
        ogs_error("OpenAPI_svc_experience_parseFromJSON() failed [mos]");
        goto end;
    }
    }

    upper_range = cJSON_GetObjectItemCaseSensitive(svc_experienceJSON, "upperRange");
    if (upper_range) {
    if (!cJSON_IsNumber(upper_range)) {
        ogs_error("OpenAPI_svc_experience_parseFromJSON() failed [upper_range]");
        goto end;
    }
    }

    lower_range = cJSON_GetObjectItemCaseSensitive(svc_experienceJSON, "lowerRange");
    if (lower_range) {
    if (!cJSON_IsNumber(lower_range)) {
        ogs_error("OpenAPI_svc_experience_parseFromJSON() failed [lower_range]");
        goto end;
    }
    }

    svc_experience_local_var = OpenAPI_svc_experience_create (
        mos ? true : false,
        mos ? mos->valuedouble : 0,
        upper_range ? true : false,
        upper_range ? upper_range->valuedouble : 0,
        lower_range ? true : false,
        lower_range ? lower_range->valuedouble : 0
    );

    return svc_experience_local_var;
end:
    return NULL;
}

OpenAPI_svc_experience_t *OpenAPI_svc_experience_copy(OpenAPI_svc_experience_t *dst, OpenAPI_svc_experience_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_svc_experience_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_svc_experience_convertToJSON() failed");
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

    OpenAPI_svc_experience_free(dst);
    dst = OpenAPI_svc_experience_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

