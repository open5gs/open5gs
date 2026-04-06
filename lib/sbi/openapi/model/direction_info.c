
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "direction_info.h"

OpenAPI_direction_info_t *OpenAPI_direction_info_create(
    char *supi,
    char *gpsi,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_avr_speed,
    float avr_speed,
    bool is_ratio,
    int ratio,
    OpenAPI_direction_e direction
)
{
    OpenAPI_direction_info_t *direction_info_local_var = ogs_malloc(sizeof(OpenAPI_direction_info_t));
    ogs_assert(direction_info_local_var);

    direction_info_local_var->supi = supi;
    direction_info_local_var->gpsi = gpsi;
    direction_info_local_var->is_num_of_ue = is_num_of_ue;
    direction_info_local_var->num_of_ue = num_of_ue;
    direction_info_local_var->is_avr_speed = is_avr_speed;
    direction_info_local_var->avr_speed = avr_speed;
    direction_info_local_var->is_ratio = is_ratio;
    direction_info_local_var->ratio = ratio;
    direction_info_local_var->direction = direction;

    return direction_info_local_var;
}

void OpenAPI_direction_info_free(OpenAPI_direction_info_t *direction_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == direction_info) {
        return;
    }
    if (direction_info->supi) {
        ogs_free(direction_info->supi);
        direction_info->supi = NULL;
    }
    if (direction_info->gpsi) {
        ogs_free(direction_info->gpsi);
        direction_info->gpsi = NULL;
    }
    ogs_free(direction_info);
}

cJSON *OpenAPI_direction_info_convertToJSON(OpenAPI_direction_info_t *direction_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (direction_info == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [DirectionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (direction_info->supi) {
    if (cJSON_AddStringToObject(item, "supi", direction_info->supi) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (direction_info->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", direction_info->gpsi) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (direction_info->is_num_of_ue) {
    if (cJSON_AddNumberToObject(item, "numOfUe", direction_info->num_of_ue) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [num_of_ue]");
        goto end;
    }
    }

    if (direction_info->is_avr_speed) {
    if (cJSON_AddNumberToObject(item, "avrSpeed", direction_info->avr_speed) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [avr_speed]");
        goto end;
    }
    }

    if (direction_info->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", direction_info->ratio) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (direction_info->direction == OpenAPI_direction_NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [direction]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "direction", OpenAPI_direction_ToString(direction_info->direction)) == NULL) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed [direction]");
        goto end;
    }

end:
    return item;
}

OpenAPI_direction_info_t *OpenAPI_direction_info_parseFromJSON(cJSON *direction_infoJSON)
{
    OpenAPI_direction_info_t *direction_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *num_of_ue = NULL;
    cJSON *avr_speed = NULL;
    cJSON *ratio = NULL;
    cJSON *direction = NULL;
    OpenAPI_direction_e directionVariable = 0;
    supi = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    num_of_ue = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "numOfUe");
    if (num_of_ue) {
    if (!cJSON_IsNumber(num_of_ue)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [num_of_ue]");
        goto end;
    }
    }

    avr_speed = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "avrSpeed");
    if (avr_speed) {
    if (!cJSON_IsNumber(avr_speed)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [avr_speed]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    direction = cJSON_GetObjectItemCaseSensitive(direction_infoJSON, "direction");
    if (!direction) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [direction]");
        goto end;
    }
    if (!cJSON_IsString(direction)) {
        ogs_error("OpenAPI_direction_info_parseFromJSON() failed [direction]");
        goto end;
    }
    directionVariable = OpenAPI_direction_FromString(direction->valuestring);

    direction_info_local_var = OpenAPI_direction_info_create (
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        num_of_ue ? true : false,
        num_of_ue ? num_of_ue->valuedouble : 0,
        avr_speed ? true : false,
        avr_speed ? avr_speed->valuedouble : 0,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        directionVariable
    );

    return direction_info_local_var;
end:
    return NULL;
}

OpenAPI_direction_info_t *OpenAPI_direction_info_copy(OpenAPI_direction_info_t *dst, OpenAPI_direction_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_direction_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_direction_info_convertToJSON() failed");
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

    OpenAPI_direction_info_free(dst);
    dst = OpenAPI_direction_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

