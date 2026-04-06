
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_to_collision_info.h"

OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_create(
    char *ttc,
    bool is_accuracy,
    int accuracy,
    bool is_confidence,
    int confidence,
    OpenAPI_point_altitude_uncertainty_t *collision_space,
    bool is_col_spc_confidence,
    int col_spc_confidence,
    OpenAPI_range_direction_t *colli_direction,
    bool is_col_dir_confidence,
    int col_dir_confidence
)
{
    OpenAPI_time_to_collision_info_t *time_to_collision_info_local_var = ogs_malloc(sizeof(OpenAPI_time_to_collision_info_t));
    ogs_assert(time_to_collision_info_local_var);

    time_to_collision_info_local_var->ttc = ttc;
    time_to_collision_info_local_var->is_accuracy = is_accuracy;
    time_to_collision_info_local_var->accuracy = accuracy;
    time_to_collision_info_local_var->is_confidence = is_confidence;
    time_to_collision_info_local_var->confidence = confidence;
    time_to_collision_info_local_var->collision_space = collision_space;
    time_to_collision_info_local_var->is_col_spc_confidence = is_col_spc_confidence;
    time_to_collision_info_local_var->col_spc_confidence = col_spc_confidence;
    time_to_collision_info_local_var->colli_direction = colli_direction;
    time_to_collision_info_local_var->is_col_dir_confidence = is_col_dir_confidence;
    time_to_collision_info_local_var->col_dir_confidence = col_dir_confidence;

    return time_to_collision_info_local_var;
}

void OpenAPI_time_to_collision_info_free(OpenAPI_time_to_collision_info_t *time_to_collision_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_to_collision_info) {
        return;
    }
    if (time_to_collision_info->ttc) {
        ogs_free(time_to_collision_info->ttc);
        time_to_collision_info->ttc = NULL;
    }
    if (time_to_collision_info->collision_space) {
        OpenAPI_point_altitude_uncertainty_free(time_to_collision_info->collision_space);
        time_to_collision_info->collision_space = NULL;
    }
    if (time_to_collision_info->colli_direction) {
        OpenAPI_range_direction_free(time_to_collision_info->colli_direction);
        time_to_collision_info->colli_direction = NULL;
    }
    ogs_free(time_to_collision_info);
}

cJSON *OpenAPI_time_to_collision_info_convertToJSON(OpenAPI_time_to_collision_info_t *time_to_collision_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_to_collision_info == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [TimeToCollisionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (time_to_collision_info->ttc) {
    if (cJSON_AddStringToObject(item, "ttc", time_to_collision_info->ttc) == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [ttc]");
        goto end;
    }
    }

    if (time_to_collision_info->is_accuracy) {
    if (cJSON_AddNumberToObject(item, "accuracy", time_to_collision_info->accuracy) == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [accuracy]");
        goto end;
    }
    }

    if (time_to_collision_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", time_to_collision_info->confidence) == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (time_to_collision_info->collision_space) {
    cJSON *collision_space_local_JSON = OpenAPI_point_altitude_uncertainty_convertToJSON(time_to_collision_info->collision_space);
    if (collision_space_local_JSON == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [collision_space]");
        goto end;
    }
    cJSON_AddItemToObject(item, "collisionSpace", collision_space_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [collision_space]");
        goto end;
    }
    }

    if (time_to_collision_info->is_col_spc_confidence) {
    if (cJSON_AddNumberToObject(item, "colSpcConfidence", time_to_collision_info->col_spc_confidence) == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [col_spc_confidence]");
        goto end;
    }
    }

    if (time_to_collision_info->colli_direction) {
    cJSON *colli_direction_local_JSON = OpenAPI_range_direction_convertToJSON(time_to_collision_info->colli_direction);
    if (colli_direction_local_JSON == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [colli_direction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "colliDirection", colli_direction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [colli_direction]");
        goto end;
    }
    }

    if (time_to_collision_info->is_col_dir_confidence) {
    if (cJSON_AddNumberToObject(item, "colDirConfidence", time_to_collision_info->col_dir_confidence) == NULL) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed [col_dir_confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_parseFromJSON(cJSON *time_to_collision_infoJSON)
{
    OpenAPI_time_to_collision_info_t *time_to_collision_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ttc = NULL;
    cJSON *accuracy = NULL;
    cJSON *confidence = NULL;
    cJSON *collision_space = NULL;
    OpenAPI_point_altitude_uncertainty_t *collision_space_local_nonprim = NULL;
    cJSON *col_spc_confidence = NULL;
    cJSON *colli_direction = NULL;
    OpenAPI_range_direction_t *colli_direction_local_nonprim = NULL;
    cJSON *col_dir_confidence = NULL;
    ttc = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "ttc");
    if (ttc) {
    if (!cJSON_IsString(ttc) && !cJSON_IsNull(ttc)) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON() failed [ttc]");
        goto end;
    }
    }

    accuracy = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "accuracy");
    if (accuracy) {
    if (!cJSON_IsNumber(accuracy)) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON() failed [accuracy]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    collision_space = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "collisionSpace");
    if (collision_space) {
    collision_space_local_nonprim = OpenAPI_point_altitude_uncertainty_parseFromJSON(collision_space);
    if (!collision_space_local_nonprim) {
        ogs_error("OpenAPI_point_altitude_uncertainty_parseFromJSON failed [collision_space]");
        goto end;
    }
    }

    col_spc_confidence = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "colSpcConfidence");
    if (col_spc_confidence) {
    if (!cJSON_IsNumber(col_spc_confidence)) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON() failed [col_spc_confidence]");
        goto end;
    }
    }

    colli_direction = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "colliDirection");
    if (colli_direction) {
    colli_direction_local_nonprim = OpenAPI_range_direction_parseFromJSON(colli_direction);
    if (!colli_direction_local_nonprim) {
        ogs_error("OpenAPI_range_direction_parseFromJSON failed [colli_direction]");
        goto end;
    }
    }

    col_dir_confidence = cJSON_GetObjectItemCaseSensitive(time_to_collision_infoJSON, "colDirConfidence");
    if (col_dir_confidence) {
    if (!cJSON_IsNumber(col_dir_confidence)) {
        ogs_error("OpenAPI_time_to_collision_info_parseFromJSON() failed [col_dir_confidence]");
        goto end;
    }
    }

    time_to_collision_info_local_var = OpenAPI_time_to_collision_info_create (
        ttc && !cJSON_IsNull(ttc) ? ogs_strdup(ttc->valuestring) : NULL,
        accuracy ? true : false,
        accuracy ? accuracy->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        collision_space ? collision_space_local_nonprim : NULL,
        col_spc_confidence ? true : false,
        col_spc_confidence ? col_spc_confidence->valuedouble : 0,
        colli_direction ? colli_direction_local_nonprim : NULL,
        col_dir_confidence ? true : false,
        col_dir_confidence ? col_dir_confidence->valuedouble : 0
    );

    return time_to_collision_info_local_var;
end:
    if (collision_space_local_nonprim) {
        OpenAPI_point_altitude_uncertainty_free(collision_space_local_nonprim);
        collision_space_local_nonprim = NULL;
    }
    if (colli_direction_local_nonprim) {
        OpenAPI_range_direction_free(colli_direction_local_nonprim);
        colli_direction_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_time_to_collision_info_t *OpenAPI_time_to_collision_info_copy(OpenAPI_time_to_collision_info_t *dst, OpenAPI_time_to_collision_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_to_collision_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_to_collision_info_convertToJSON() failed");
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

    OpenAPI_time_to_collision_info_free(dst);
    dst = OpenAPI_time_to_collision_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

