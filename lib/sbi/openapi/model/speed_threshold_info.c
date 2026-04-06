
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "speed_threshold_info.h"

OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_create(
    bool is_speed_thr,
    float speed_thr,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_ratio,
    int ratio
)
{
    OpenAPI_speed_threshold_info_t *speed_threshold_info_local_var = ogs_malloc(sizeof(OpenAPI_speed_threshold_info_t));
    ogs_assert(speed_threshold_info_local_var);

    speed_threshold_info_local_var->is_speed_thr = is_speed_thr;
    speed_threshold_info_local_var->speed_thr = speed_thr;
    speed_threshold_info_local_var->is_num_of_ue = is_num_of_ue;
    speed_threshold_info_local_var->num_of_ue = num_of_ue;
    speed_threshold_info_local_var->is_ratio = is_ratio;
    speed_threshold_info_local_var->ratio = ratio;

    return speed_threshold_info_local_var;
}

void OpenAPI_speed_threshold_info_free(OpenAPI_speed_threshold_info_t *speed_threshold_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == speed_threshold_info) {
        return;
    }
    ogs_free(speed_threshold_info);
}

cJSON *OpenAPI_speed_threshold_info_convertToJSON(OpenAPI_speed_threshold_info_t *speed_threshold_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (speed_threshold_info == NULL) {
        ogs_error("OpenAPI_speed_threshold_info_convertToJSON() failed [SpeedThresholdInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (speed_threshold_info->is_speed_thr) {
    if (cJSON_AddNumberToObject(item, "speedThr", speed_threshold_info->speed_thr) == NULL) {
        ogs_error("OpenAPI_speed_threshold_info_convertToJSON() failed [speed_thr]");
        goto end;
    }
    }

    if (speed_threshold_info->is_num_of_ue) {
    if (cJSON_AddNumberToObject(item, "numOfUe", speed_threshold_info->num_of_ue) == NULL) {
        ogs_error("OpenAPI_speed_threshold_info_convertToJSON() failed [num_of_ue]");
        goto end;
    }
    }

    if (speed_threshold_info->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", speed_threshold_info->ratio) == NULL) {
        ogs_error("OpenAPI_speed_threshold_info_convertToJSON() failed [ratio]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_parseFromJSON(cJSON *speed_threshold_infoJSON)
{
    OpenAPI_speed_threshold_info_t *speed_threshold_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *speed_thr = NULL;
    cJSON *num_of_ue = NULL;
    cJSON *ratio = NULL;
    speed_thr = cJSON_GetObjectItemCaseSensitive(speed_threshold_infoJSON, "speedThr");
    if (speed_thr) {
    if (!cJSON_IsNumber(speed_thr)) {
        ogs_error("OpenAPI_speed_threshold_info_parseFromJSON() failed [speed_thr]");
        goto end;
    }
    }

    num_of_ue = cJSON_GetObjectItemCaseSensitive(speed_threshold_infoJSON, "numOfUe");
    if (num_of_ue) {
    if (!cJSON_IsNumber(num_of_ue)) {
        ogs_error("OpenAPI_speed_threshold_info_parseFromJSON() failed [num_of_ue]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(speed_threshold_infoJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_speed_threshold_info_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    speed_threshold_info_local_var = OpenAPI_speed_threshold_info_create (
        speed_thr ? true : false,
        speed_thr ? speed_thr->valuedouble : 0,
        num_of_ue ? true : false,
        num_of_ue ? num_of_ue->valuedouble : 0,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0
    );

    return speed_threshold_info_local_var;
end:
    return NULL;
}

OpenAPI_speed_threshold_info_t *OpenAPI_speed_threshold_info_copy(OpenAPI_speed_threshold_info_t *dst, OpenAPI_speed_threshold_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_speed_threshold_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_speed_threshold_info_convertToJSON() failed");
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

    OpenAPI_speed_threshold_info_free(dst);
    dst = OpenAPI_speed_threshold_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

