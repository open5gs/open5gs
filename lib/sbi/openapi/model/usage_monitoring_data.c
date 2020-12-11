
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_monitoring_data.h"

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_create(
    char *um_id,
    long volume_threshold,
    long volume_threshold_uplink,
    long volume_threshold_downlink,
    int time_threshold,
    char *monitoring_time,
    long next_vol_threshold,
    long next_vol_threshold_uplink,
    long next_vol_threshold_downlink,
    int next_time_threshold,
    int inactivity_time,
    OpenAPI_list_t *ex_usage_pcc_rule_ids
    )
{
    OpenAPI_usage_monitoring_data_t *usage_monitoring_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_usage_monitoring_data_t));
    if (!usage_monitoring_data_local_var) {
        return NULL;
    }
    usage_monitoring_data_local_var->um_id = um_id;
    usage_monitoring_data_local_var->volume_threshold = volume_threshold;
    usage_monitoring_data_local_var->volume_threshold_uplink = volume_threshold_uplink;
    usage_monitoring_data_local_var->volume_threshold_downlink = volume_threshold_downlink;
    usage_monitoring_data_local_var->time_threshold = time_threshold;
    usage_monitoring_data_local_var->monitoring_time = monitoring_time;
    usage_monitoring_data_local_var->next_vol_threshold = next_vol_threshold;
    usage_monitoring_data_local_var->next_vol_threshold_uplink = next_vol_threshold_uplink;
    usage_monitoring_data_local_var->next_vol_threshold_downlink = next_vol_threshold_downlink;
    usage_monitoring_data_local_var->next_time_threshold = next_time_threshold;
    usage_monitoring_data_local_var->inactivity_time = inactivity_time;
    usage_monitoring_data_local_var->ex_usage_pcc_rule_ids = ex_usage_pcc_rule_ids;

    return usage_monitoring_data_local_var;
}

void OpenAPI_usage_monitoring_data_free(OpenAPI_usage_monitoring_data_t *usage_monitoring_data)
{
    if (NULL == usage_monitoring_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(usage_monitoring_data->um_id);
    ogs_free(usage_monitoring_data->monitoring_time);
    OpenAPI_list_for_each(usage_monitoring_data->ex_usage_pcc_rule_ids, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(usage_monitoring_data->ex_usage_pcc_rule_ids);
    ogs_free(usage_monitoring_data);
}

cJSON *OpenAPI_usage_monitoring_data_convertToJSON(OpenAPI_usage_monitoring_data_t *usage_monitoring_data)
{
    cJSON *item = NULL;

    if (usage_monitoring_data == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [UsageMonitoringData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_monitoring_data->um_id) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [um_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "umId", usage_monitoring_data->um_id) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [um_id]");
        goto end;
    }

    if (usage_monitoring_data->volume_threshold) {
        if (cJSON_AddNumberToObject(item, "volumeThreshold", usage_monitoring_data->volume_threshold) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->volume_threshold_uplink) {
        if (cJSON_AddNumberToObject(item, "volumeThresholdUplink", usage_monitoring_data->volume_threshold_uplink) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_uplink]");
            goto end;
        }
    }

    if (usage_monitoring_data->volume_threshold_downlink) {
        if (cJSON_AddNumberToObject(item, "volumeThresholdDownlink", usage_monitoring_data->volume_threshold_downlink) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_downlink]");
            goto end;
        }
    }

    if (usage_monitoring_data->time_threshold) {
        if (cJSON_AddNumberToObject(item, "timeThreshold", usage_monitoring_data->time_threshold) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [time_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->monitoring_time) {
        if (cJSON_AddStringToObject(item, "monitoringTime", usage_monitoring_data->monitoring_time) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [monitoring_time]");
            goto end;
        }
    }

    if (usage_monitoring_data->next_vol_threshold) {
        if (cJSON_AddNumberToObject(item, "nextVolThreshold", usage_monitoring_data->next_vol_threshold) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->next_vol_threshold_uplink) {
        if (cJSON_AddNumberToObject(item, "nextVolThresholdUplink", usage_monitoring_data->next_vol_threshold_uplink) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_uplink]");
            goto end;
        }
    }

    if (usage_monitoring_data->next_vol_threshold_downlink) {
        if (cJSON_AddNumberToObject(item, "nextVolThresholdDownlink", usage_monitoring_data->next_vol_threshold_downlink) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_downlink]");
            goto end;
        }
    }

    if (usage_monitoring_data->next_time_threshold) {
        if (cJSON_AddNumberToObject(item, "nextTimeThreshold", usage_monitoring_data->next_time_threshold) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_time_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->inactivity_time) {
        if (cJSON_AddNumberToObject(item, "inactivityTime", usage_monitoring_data->inactivity_time) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [inactivity_time]");
            goto end;
        }
    }

    if (usage_monitoring_data->ex_usage_pcc_rule_ids) {
        cJSON *ex_usage_pcc_rule_ids = cJSON_AddArrayToObject(item, "exUsagePccRuleIds");
        if (ex_usage_pcc_rule_ids == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [ex_usage_pcc_rule_ids]");
            goto end;
        }

        OpenAPI_lnode_t *ex_usage_pcc_rule_ids_node;
        OpenAPI_list_for_each(usage_monitoring_data->ex_usage_pcc_rule_ids, ex_usage_pcc_rule_ids_node)  {
            if (cJSON_AddStringToObject(ex_usage_pcc_rule_ids, "", (char*)ex_usage_pcc_rule_ids_node->data) == NULL) {
                ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [ex_usage_pcc_rule_ids]");
                goto end;
            }
        }
    }

end:
    return item;
}

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_parseFromJSON(cJSON *usage_monitoring_dataJSON)
{
    OpenAPI_usage_monitoring_data_t *usage_monitoring_data_local_var = NULL;
    cJSON *um_id = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "umId");
    if (!um_id) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [um_id]");
        goto end;
    }


    if (!cJSON_IsString(um_id)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [um_id]");
        goto end;
    }

    cJSON *volume_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThreshold");

    if (volume_threshold) {
        if (!cJSON_IsNumber(volume_threshold)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold]");
            goto end;
        }
    }

    cJSON *volume_threshold_uplink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThresholdUplink");

    if (volume_threshold_uplink) {
        if (!cJSON_IsNumber(volume_threshold_uplink)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold_uplink]");
            goto end;
        }
    }

    cJSON *volume_threshold_downlink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThresholdDownlink");

    if (volume_threshold_downlink) {
        if (!cJSON_IsNumber(volume_threshold_downlink)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold_downlink]");
            goto end;
        }
    }

    cJSON *time_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "timeThreshold");

    if (time_threshold) {
        if (!cJSON_IsNumber(time_threshold)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [time_threshold]");
            goto end;
        }
    }

    cJSON *monitoring_time = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "monitoringTime");

    if (monitoring_time) {
        if (!cJSON_IsString(monitoring_time)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [monitoring_time]");
            goto end;
        }
    }

    cJSON *next_vol_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThreshold");

    if (next_vol_threshold) {
        if (!cJSON_IsNumber(next_vol_threshold)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold]");
            goto end;
        }
    }

    cJSON *next_vol_threshold_uplink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThresholdUplink");

    if (next_vol_threshold_uplink) {
        if (!cJSON_IsNumber(next_vol_threshold_uplink)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold_uplink]");
            goto end;
        }
    }

    cJSON *next_vol_threshold_downlink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThresholdDownlink");

    if (next_vol_threshold_downlink) {
        if (!cJSON_IsNumber(next_vol_threshold_downlink)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold_downlink]");
            goto end;
        }
    }

    cJSON *next_time_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextTimeThreshold");

    if (next_time_threshold) {
        if (!cJSON_IsNumber(next_time_threshold)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_time_threshold]");
            goto end;
        }
    }

    cJSON *inactivity_time = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "inactivityTime");

    if (inactivity_time) {
        if (!cJSON_IsNumber(inactivity_time)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [inactivity_time]");
            goto end;
        }
    }

    cJSON *ex_usage_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "exUsagePccRuleIds");

    OpenAPI_list_t *ex_usage_pcc_rule_idsList;
    if (ex_usage_pcc_rule_ids) {
        cJSON *ex_usage_pcc_rule_ids_local;
        if (!cJSON_IsArray(ex_usage_pcc_rule_ids)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [ex_usage_pcc_rule_ids]");
            goto end;
        }
        ex_usage_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ex_usage_pcc_rule_ids_local, ex_usage_pcc_rule_ids) {
            if (!cJSON_IsString(ex_usage_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [ex_usage_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ex_usage_pcc_rule_idsList, ogs_strdup(ex_usage_pcc_rule_ids_local->valuestring));
        }
    }

    usage_monitoring_data_local_var = OpenAPI_usage_monitoring_data_create (
        ogs_strdup(um_id->valuestring),
        volume_threshold ? volume_threshold->valuedouble : 0,
        volume_threshold_uplink ? volume_threshold_uplink->valuedouble : 0,
        volume_threshold_downlink ? volume_threshold_downlink->valuedouble : 0,
        time_threshold ? time_threshold->valuedouble : 0,
        monitoring_time ? ogs_strdup(monitoring_time->valuestring) : NULL,
        next_vol_threshold ? next_vol_threshold->valuedouble : 0,
        next_vol_threshold_uplink ? next_vol_threshold_uplink->valuedouble : 0,
        next_vol_threshold_downlink ? next_vol_threshold_downlink->valuedouble : 0,
        next_time_threshold ? next_time_threshold->valuedouble : 0,
        inactivity_time ? inactivity_time->valuedouble : 0,
        ex_usage_pcc_rule_ids ? ex_usage_pcc_rule_idsList : NULL
        );

    return usage_monitoring_data_local_var;
end:
    return NULL;
}

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_copy(OpenAPI_usage_monitoring_data_t *dst, OpenAPI_usage_monitoring_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_usage_monitoring_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed");
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

    OpenAPI_usage_monitoring_data_free(dst);
    dst = OpenAPI_usage_monitoring_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

