
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mov_behav.h"

OpenAPI_mov_behav_t *OpenAPI_mov_behav_create(
    char *ts_start,
    int ts_duration,
    bool is_num_of_ue,
    int num_of_ue,
    bool is_ratio,
    int ratio,
    bool is_avr_speed,
    float avr_speed,
    OpenAPI_list_t *speed_thresd_infos,
    OpenAPI_list_t *direction_ue_infos
)
{
    OpenAPI_mov_behav_t *mov_behav_local_var = ogs_malloc(sizeof(OpenAPI_mov_behav_t));
    ogs_assert(mov_behav_local_var);

    mov_behav_local_var->ts_start = ts_start;
    mov_behav_local_var->ts_duration = ts_duration;
    mov_behav_local_var->is_num_of_ue = is_num_of_ue;
    mov_behav_local_var->num_of_ue = num_of_ue;
    mov_behav_local_var->is_ratio = is_ratio;
    mov_behav_local_var->ratio = ratio;
    mov_behav_local_var->is_avr_speed = is_avr_speed;
    mov_behav_local_var->avr_speed = avr_speed;
    mov_behav_local_var->speed_thresd_infos = speed_thresd_infos;
    mov_behav_local_var->direction_ue_infos = direction_ue_infos;

    return mov_behav_local_var;
}

void OpenAPI_mov_behav_free(OpenAPI_mov_behav_t *mov_behav)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mov_behav) {
        return;
    }
    if (mov_behav->ts_start) {
        ogs_free(mov_behav->ts_start);
        mov_behav->ts_start = NULL;
    }
    if (mov_behav->speed_thresd_infos) {
        OpenAPI_list_for_each(mov_behav->speed_thresd_infos, node) {
            OpenAPI_speed_threshold_info_free(node->data);
        }
        OpenAPI_list_free(mov_behav->speed_thresd_infos);
        mov_behav->speed_thresd_infos = NULL;
    }
    if (mov_behav->direction_ue_infos) {
        OpenAPI_list_for_each(mov_behav->direction_ue_infos, node) {
            OpenAPI_direction_info_free(node->data);
        }
        OpenAPI_list_free(mov_behav->direction_ue_infos);
        mov_behav->direction_ue_infos = NULL;
    }
    ogs_free(mov_behav);
}

cJSON *OpenAPI_mov_behav_convertToJSON(OpenAPI_mov_behav_t *mov_behav)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mov_behav == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [MovBehav]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!mov_behav->ts_start) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [ts_start]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "tsStart", mov_behav->ts_start) == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [ts_start]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "tsDuration", mov_behav->ts_duration) == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [ts_duration]");
        goto end;
    }

    if (mov_behav->is_num_of_ue) {
    if (cJSON_AddNumberToObject(item, "numOfUe", mov_behav->num_of_ue) == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [num_of_ue]");
        goto end;
    }
    }

    if (mov_behav->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", mov_behav->ratio) == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (mov_behav->is_avr_speed) {
    if (cJSON_AddNumberToObject(item, "avrSpeed", mov_behav->avr_speed) == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [avr_speed]");
        goto end;
    }
    }

    if (mov_behav->speed_thresd_infos) {
    cJSON *speed_thresd_infosList = cJSON_AddArrayToObject(item, "speedThresdInfos");
    if (speed_thresd_infosList == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [speed_thresd_infos]");
        goto end;
    }
    OpenAPI_list_for_each(mov_behav->speed_thresd_infos, node) {
        cJSON *itemLocal = OpenAPI_speed_threshold_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mov_behav_convertToJSON() failed [speed_thresd_infos]");
            goto end;
        }
        cJSON_AddItemToArray(speed_thresd_infosList, itemLocal);
    }
    }

    if (mov_behav->direction_ue_infos) {
    cJSON *direction_ue_infosList = cJSON_AddArrayToObject(item, "directionUeInfos");
    if (direction_ue_infosList == NULL) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed [direction_ue_infos]");
        goto end;
    }
    OpenAPI_list_for_each(mov_behav->direction_ue_infos, node) {
        cJSON *itemLocal = OpenAPI_direction_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mov_behav_convertToJSON() failed [direction_ue_infos]");
            goto end;
        }
        cJSON_AddItemToArray(direction_ue_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mov_behav_t *OpenAPI_mov_behav_parseFromJSON(cJSON *mov_behavJSON)
{
    OpenAPI_mov_behav_t *mov_behav_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts_start = NULL;
    cJSON *ts_duration = NULL;
    cJSON *num_of_ue = NULL;
    cJSON *ratio = NULL;
    cJSON *avr_speed = NULL;
    cJSON *speed_thresd_infos = NULL;
    OpenAPI_list_t *speed_thresd_infosList = NULL;
    cJSON *direction_ue_infos = NULL;
    OpenAPI_list_t *direction_ue_infosList = NULL;
    ts_start = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "tsStart");
    if (!ts_start) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [ts_start]");
        goto end;
    }
    if (!cJSON_IsString(ts_start) && !cJSON_IsNull(ts_start)) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [ts_start]");
        goto end;
    }

    ts_duration = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "tsDuration");
    if (!ts_duration) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [ts_duration]");
        goto end;
    }
    if (!cJSON_IsNumber(ts_duration)) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [ts_duration]");
        goto end;
    }

    num_of_ue = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "numOfUe");
    if (num_of_ue) {
    if (!cJSON_IsNumber(num_of_ue)) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [num_of_ue]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    avr_speed = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "avrSpeed");
    if (avr_speed) {
    if (!cJSON_IsNumber(avr_speed)) {
        ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [avr_speed]");
        goto end;
    }
    }

    speed_thresd_infos = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "speedThresdInfos");
    if (speed_thresd_infos) {
        cJSON *speed_thresd_infos_local = NULL;
        if (!cJSON_IsArray(speed_thresd_infos)) {
            ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [speed_thresd_infos]");
            goto end;
        }

        speed_thresd_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(speed_thresd_infos_local, speed_thresd_infos) {
            if (!cJSON_IsObject(speed_thresd_infos_local)) {
                ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [speed_thresd_infos]");
                goto end;
            }
            OpenAPI_speed_threshold_info_t *speed_thresd_infosItem = OpenAPI_speed_threshold_info_parseFromJSON(speed_thresd_infos_local);
            if (!speed_thresd_infosItem) {
                ogs_error("No speed_thresd_infosItem");
                goto end;
            }
            OpenAPI_list_add(speed_thresd_infosList, speed_thresd_infosItem);
        }
    }

    direction_ue_infos = cJSON_GetObjectItemCaseSensitive(mov_behavJSON, "directionUeInfos");
    if (direction_ue_infos) {
        cJSON *direction_ue_infos_local = NULL;
        if (!cJSON_IsArray(direction_ue_infos)) {
            ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [direction_ue_infos]");
            goto end;
        }

        direction_ue_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(direction_ue_infos_local, direction_ue_infos) {
            if (!cJSON_IsObject(direction_ue_infos_local)) {
                ogs_error("OpenAPI_mov_behav_parseFromJSON() failed [direction_ue_infos]");
                goto end;
            }
            OpenAPI_direction_info_t *direction_ue_infosItem = OpenAPI_direction_info_parseFromJSON(direction_ue_infos_local);
            if (!direction_ue_infosItem) {
                ogs_error("No direction_ue_infosItem");
                goto end;
            }
            OpenAPI_list_add(direction_ue_infosList, direction_ue_infosItem);
        }
    }

    mov_behav_local_var = OpenAPI_mov_behav_create (
        ogs_strdup(ts_start->valuestring),
        
        ts_duration->valuedouble,
        num_of_ue ? true : false,
        num_of_ue ? num_of_ue->valuedouble : 0,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        avr_speed ? true : false,
        avr_speed ? avr_speed->valuedouble : 0,
        speed_thresd_infos ? speed_thresd_infosList : NULL,
        direction_ue_infos ? direction_ue_infosList : NULL
    );

    return mov_behav_local_var;
end:
    if (speed_thresd_infosList) {
        OpenAPI_list_for_each(speed_thresd_infosList, node) {
            OpenAPI_speed_threshold_info_free(node->data);
        }
        OpenAPI_list_free(speed_thresd_infosList);
        speed_thresd_infosList = NULL;
    }
    if (direction_ue_infosList) {
        OpenAPI_list_for_each(direction_ue_infosList, node) {
            OpenAPI_direction_info_free(node->data);
        }
        OpenAPI_list_free(direction_ue_infosList);
        direction_ue_infosList = NULL;
    }
    return NULL;
}

OpenAPI_mov_behav_t *OpenAPI_mov_behav_copy(OpenAPI_mov_behav_t *dst, OpenAPI_mov_behav_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mov_behav_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mov_behav_convertToJSON() failed");
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

    OpenAPI_mov_behav_free(dst);
    dst = OpenAPI_mov_behav_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

