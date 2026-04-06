
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "odb_exempted_conditions_1.h"

OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_create(
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond,
    char *validity_time
)
{
    OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1_local_var = ogs_malloc(sizeof(OpenAPI_odb_exempted_conditions_1_t));
    ogs_assert(odb_exempted_conditions_1_local_var);

    odb_exempted_conditions_1_local_var->spatial_validity_cond = spatial_validity_cond;
    odb_exempted_conditions_1_local_var->validity_time = validity_time;

    return odb_exempted_conditions_1_local_var;
}

void OpenAPI_odb_exempted_conditions_1_free(OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == odb_exempted_conditions_1) {
        return;
    }
    if (odb_exempted_conditions_1->spatial_validity_cond) {
        OpenAPI_spatial_validity_cond_1_free(odb_exempted_conditions_1->spatial_validity_cond);
        odb_exempted_conditions_1->spatial_validity_cond = NULL;
    }
    if (odb_exempted_conditions_1->validity_time) {
        ogs_free(odb_exempted_conditions_1->validity_time);
        odb_exempted_conditions_1->validity_time = NULL;
    }
    ogs_free(odb_exempted_conditions_1);
}

cJSON *OpenAPI_odb_exempted_conditions_1_convertToJSON(OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (odb_exempted_conditions_1 == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_convertToJSON() failed [OdbExemptedConditions_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (odb_exempted_conditions_1->spatial_validity_cond) {
    cJSON *spatial_validity_cond_local_JSON = OpenAPI_spatial_validity_cond_1_convertToJSON(odb_exempted_conditions_1->spatial_validity_cond);
    if (spatial_validity_cond_local_JSON == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    cJSON_AddItemToObject(item, "spatialValidityCond", spatial_validity_cond_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_convertToJSON() failed [spatial_validity_cond]");
        goto end;
    }
    }

    if (odb_exempted_conditions_1->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", odb_exempted_conditions_1->validity_time) == NULL) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_parseFromJSON(cJSON *odb_exempted_conditions_1JSON)
{
    OpenAPI_odb_exempted_conditions_1_t *odb_exempted_conditions_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *spatial_validity_cond = NULL;
    OpenAPI_spatial_validity_cond_1_t *spatial_validity_cond_local_nonprim = NULL;
    cJSON *validity_time = NULL;
    spatial_validity_cond = cJSON_GetObjectItemCaseSensitive(odb_exempted_conditions_1JSON, "spatialValidityCond");
    if (spatial_validity_cond) {
    spatial_validity_cond_local_nonprim = OpenAPI_spatial_validity_cond_1_parseFromJSON(spatial_validity_cond);
    if (!spatial_validity_cond_local_nonprim) {
        ogs_error("OpenAPI_spatial_validity_cond_1_parseFromJSON failed [spatial_validity_cond]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(odb_exempted_conditions_1JSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    odb_exempted_conditions_1_local_var = OpenAPI_odb_exempted_conditions_1_create (
        spatial_validity_cond ? spatial_validity_cond_local_nonprim : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL
    );

    return odb_exempted_conditions_1_local_var;
end:
    if (spatial_validity_cond_local_nonprim) {
        OpenAPI_spatial_validity_cond_1_free(spatial_validity_cond_local_nonprim);
        spatial_validity_cond_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_odb_exempted_conditions_1_t *OpenAPI_odb_exempted_conditions_1_copy(OpenAPI_odb_exempted_conditions_1_t *dst, OpenAPI_odb_exempted_conditions_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_odb_exempted_conditions_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_odb_exempted_conditions_1_convertToJSON() failed");
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

    OpenAPI_odb_exempted_conditions_1_free(dst);
    dst = OpenAPI_odb_exempted_conditions_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

