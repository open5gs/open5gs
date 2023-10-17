
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_mobility.h"

OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_create(
    char *ts,
    OpenAPI_scheduled_communication_time_1_t *recurring_time,
    bool is_duration,
    int duration,
    bool is_duration_variance,
    float duration_variance,
    OpenAPI_list_t *loc_infos
)
{
    OpenAPI_ue_mobility_t *ue_mobility_local_var = ogs_malloc(sizeof(OpenAPI_ue_mobility_t));
    ogs_assert(ue_mobility_local_var);

    ue_mobility_local_var->ts = ts;
    ue_mobility_local_var->recurring_time = recurring_time;
    ue_mobility_local_var->is_duration = is_duration;
    ue_mobility_local_var->duration = duration;
    ue_mobility_local_var->is_duration_variance = is_duration_variance;
    ue_mobility_local_var->duration_variance = duration_variance;
    ue_mobility_local_var->loc_infos = loc_infos;

    return ue_mobility_local_var;
}

void OpenAPI_ue_mobility_free(OpenAPI_ue_mobility_t *ue_mobility)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_mobility) {
        return;
    }
    if (ue_mobility->ts) {
        ogs_free(ue_mobility->ts);
        ue_mobility->ts = NULL;
    }
    if (ue_mobility->recurring_time) {
        OpenAPI_scheduled_communication_time_1_free(ue_mobility->recurring_time);
        ue_mobility->recurring_time = NULL;
    }
    if (ue_mobility->loc_infos) {
        OpenAPI_list_for_each(ue_mobility->loc_infos, node) {
            OpenAPI_location_info_free(node->data);
        }
        OpenAPI_list_free(ue_mobility->loc_infos);
        ue_mobility->loc_infos = NULL;
    }
    ogs_free(ue_mobility);
}

cJSON *OpenAPI_ue_mobility_convertToJSON(OpenAPI_ue_mobility_t *ue_mobility)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_mobility == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [UeMobility]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_mobility->ts) {
    if (cJSON_AddStringToObject(item, "ts", ue_mobility->ts) == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [ts]");
        goto end;
    }
    }

    if (ue_mobility->recurring_time) {
    cJSON *recurring_time_local_JSON = OpenAPI_scheduled_communication_time_1_convertToJSON(ue_mobility->recurring_time);
    if (recurring_time_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [recurring_time]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recurringTime", recurring_time_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [recurring_time]");
        goto end;
    }
    }

    if (ue_mobility->is_duration) {
    if (cJSON_AddNumberToObject(item, "duration", ue_mobility->duration) == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [duration]");
        goto end;
    }
    }

    if (ue_mobility->is_duration_variance) {
    if (cJSON_AddNumberToObject(item, "durationVariance", ue_mobility->duration_variance) == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [duration_variance]");
        goto end;
    }
    }

    if (ue_mobility->loc_infos) {
    cJSON *loc_infosList = cJSON_AddArrayToObject(item, "locInfos");
    if (loc_infosList == NULL) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [loc_infos]");
        goto end;
    }
    OpenAPI_list_for_each(ue_mobility->loc_infos, node) {
        cJSON *itemLocal = OpenAPI_location_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_mobility_convertToJSON() failed [loc_infos]");
            goto end;
        }
        cJSON_AddItemToArray(loc_infosList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_parseFromJSON(cJSON *ue_mobilityJSON)
{
    OpenAPI_ue_mobility_t *ue_mobility_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ts = NULL;
    cJSON *recurring_time = NULL;
    OpenAPI_scheduled_communication_time_1_t *recurring_time_local_nonprim = NULL;
    cJSON *duration = NULL;
    cJSON *duration_variance = NULL;
    cJSON *loc_infos = NULL;
    OpenAPI_list_t *loc_infosList = NULL;
    ts = cJSON_GetObjectItemCaseSensitive(ue_mobilityJSON, "ts");
    if (ts) {
    if (!cJSON_IsString(ts) && !cJSON_IsNull(ts)) {
        ogs_error("OpenAPI_ue_mobility_parseFromJSON() failed [ts]");
        goto end;
    }
    }

    recurring_time = cJSON_GetObjectItemCaseSensitive(ue_mobilityJSON, "recurringTime");
    if (recurring_time) {
    recurring_time_local_nonprim = OpenAPI_scheduled_communication_time_1_parseFromJSON(recurring_time);
    if (!recurring_time_local_nonprim) {
        ogs_error("OpenAPI_scheduled_communication_time_1_parseFromJSON failed [recurring_time]");
        goto end;
    }
    }

    duration = cJSON_GetObjectItemCaseSensitive(ue_mobilityJSON, "duration");
    if (duration) {
    if (!cJSON_IsNumber(duration)) {
        ogs_error("OpenAPI_ue_mobility_parseFromJSON() failed [duration]");
        goto end;
    }
    }

    duration_variance = cJSON_GetObjectItemCaseSensitive(ue_mobilityJSON, "durationVariance");
    if (duration_variance) {
    if (!cJSON_IsNumber(duration_variance)) {
        ogs_error("OpenAPI_ue_mobility_parseFromJSON() failed [duration_variance]");
        goto end;
    }
    }

    loc_infos = cJSON_GetObjectItemCaseSensitive(ue_mobilityJSON, "locInfos");
    if (loc_infos) {
        cJSON *loc_infos_local = NULL;
        if (!cJSON_IsArray(loc_infos)) {
            ogs_error("OpenAPI_ue_mobility_parseFromJSON() failed [loc_infos]");
            goto end;
        }

        loc_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(loc_infos_local, loc_infos) {
            if (!cJSON_IsObject(loc_infos_local)) {
                ogs_error("OpenAPI_ue_mobility_parseFromJSON() failed [loc_infos]");
                goto end;
            }
            OpenAPI_location_info_t *loc_infosItem = OpenAPI_location_info_parseFromJSON(loc_infos_local);
            if (!loc_infosItem) {
                ogs_error("No loc_infosItem");
                goto end;
            }
            OpenAPI_list_add(loc_infosList, loc_infosItem);
        }
    }

    ue_mobility_local_var = OpenAPI_ue_mobility_create (
        ts && !cJSON_IsNull(ts) ? ogs_strdup(ts->valuestring) : NULL,
        recurring_time ? recurring_time_local_nonprim : NULL,
        duration ? true : false,
        duration ? duration->valuedouble : 0,
        duration_variance ? true : false,
        duration_variance ? duration_variance->valuedouble : 0,
        loc_infos ? loc_infosList : NULL
    );

    return ue_mobility_local_var;
end:
    if (recurring_time_local_nonprim) {
        OpenAPI_scheduled_communication_time_1_free(recurring_time_local_nonprim);
        recurring_time_local_nonprim = NULL;
    }
    if (loc_infosList) {
        OpenAPI_list_for_each(loc_infosList, node) {
            OpenAPI_location_info_free(node->data);
        }
        OpenAPI_list_free(loc_infosList);
        loc_infosList = NULL;
    }
    return NULL;
}

OpenAPI_ue_mobility_t *OpenAPI_ue_mobility_copy(OpenAPI_ue_mobility_t *dst, OpenAPI_ue_mobility_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_mobility_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_mobility_convertToJSON() failed");
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

    OpenAPI_ue_mobility_free(dst);
    dst = OpenAPI_ue_mobility_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

