
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "odb_exempted_conditions.h"

OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_create(
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond,
    char *validity_time
)
{
    OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions_local_var = ogs_malloc(sizeof(OpenAPI_odb_exempted_conditions_t));
    ogs_assert(odb_exempted_conditions_local_var);

    odb_exempted_conditions_local_var->spatial_validity_cond = spatial_validity_cond;
    odb_exempted_conditions_local_var->validity_time = validity_time;

    return odb_exempted_conditions_local_var;
}

void OpenAPI_odb_exempted_conditions_free(OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == odb_exempted_conditions) {
        return;
    }
    if (odb_exempted_conditions->spatial_validity_cond) {
        OpenAPI_spatial_validity_cond_free(odb_exempted_conditions->spatial_validity_cond);
        odb_exempted_conditions->spatial_validity_cond = NULL;
    }
    if (odb_exempted_conditions->validity_time) {
        ogs_free(odb_exempted_conditions->validity_time);
        odb_exempted_conditions->validity_time = NULL;
    }
    ogs_free(odb_exempted_conditions);
}

cJSON *OpenAPI_odb_exempted_conditions_convertToJSON(OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (odb_exempted_conditions == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_convertToJSON() failed [OdbExemptedConditions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (odb_exempted_conditions->spatial_validity_cond) {
    cJSON *spatial_validity_cond_local_JSON = OpenAPI_spatial_validity_cond_convertToJSON(odb_exempted_conditions->spatial_validity_cond);
    if (spatial_validity_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidityCond", spatial_validity_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    }

    if (odb_exempted_conditions->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", odb_exempted_conditions->validity_time) == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_parseFromJSON(cJSON *odb_exempted_conditionsJSON)
{
    OpenAPI_odb_exempted_conditions_t *odb_exempted_conditions_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *spatial_validity_cond = NULL;
    OpenAPI_spatial_validity_cond_t *spatial_validity_cond_local_nonprim = NULL;
    cJSON *validity_time = NULL;
    spatial_validity_cond = cJSON_GetObjectItemCaseSensitive(odb_exempted_conditionsJSON, "spatialValidityCond");
    if (spatial_validity_cond) {
    spatial_validity_cond_local_nonprim = OpenAPI_spatial_validity_cond_parseFromJSON(spatial_validity_cond);
    if (!spatial_validity_cond_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_cond_parseFromJSON failed [spatial_validity_cond]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(odb_exempted_conditionsJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_odb_exempted_conditions_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    odb_exempted_conditions_local_var = OpenAPI_odb_exempted_conditions_create (
        spatial_validity_cond ? spatial_validity_cond_local_nonprim : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL
    );

    return odb_exempted_conditions_local_var;
end:
    if (spatial_validity_cond_local_nonprim) {
        OpenAPI_spatial_validity_cond_free(spatial_validity_cond_local_nonprim);
        spatial_validity_cond_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_odb_exempted_conditions_t *OpenAPI_odb_exempted_conditions_copy(OpenAPI_odb_exempted_conditions_t *dst, OpenAPI_odb_exempted_conditions_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_odb_exempted_conditions_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_odb_exempted_conditions_convertToJSON() failed");
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

    OpenAPI_odb_exempted_conditions_free(dst);
    dst = OpenAPI_odb_exempted_conditions_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

