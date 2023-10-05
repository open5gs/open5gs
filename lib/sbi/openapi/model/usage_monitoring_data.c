
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "usage_monitoring_data.h"

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_create(
    char *um_id,
    bool is_volume_threshold_null,
    bool is_volume_threshold,
    long volume_threshold,
    bool is_volume_threshold_uplink_null,
    bool is_volume_threshold_uplink,
    long volume_threshold_uplink,
    bool is_volume_threshold_downlink_null,
    bool is_volume_threshold_downlink,
    long volume_threshold_downlink,
    bool is_time_threshold_null,
    bool is_time_threshold,
    int time_threshold,
    bool is_monitoring_time_null,
    char *monitoring_time,
    bool is_next_vol_threshold_null,
    bool is_next_vol_threshold,
    long next_vol_threshold,
    bool is_next_vol_threshold_uplink_null,
    bool is_next_vol_threshold_uplink,
    long next_vol_threshold_uplink,
    bool is_next_vol_threshold_downlink_null,
    bool is_next_vol_threshold_downlink,
    long next_vol_threshold_downlink,
    bool is_next_time_threshold_null,
    bool is_next_time_threshold,
    int next_time_threshold,
    bool is_inactivity_time_null,
    bool is_inactivity_time,
    int inactivity_time,
    bool is_ex_usage_pcc_rule_ids_null,
    OpenAPI_list_t *ex_usage_pcc_rule_ids
)
{
    OpenAPI_usage_monitoring_data_t *usage_monitoring_data_local_var = ogs_malloc(sizeof(OpenAPI_usage_monitoring_data_t));
    ogs_assert(usage_monitoring_data_local_var);

    usage_monitoring_data_local_var->um_id = um_id;
    usage_monitoring_data_local_var->is_volume_threshold_null = is_volume_threshold_null;
    usage_monitoring_data_local_var->is_volume_threshold = is_volume_threshold;
    usage_monitoring_data_local_var->volume_threshold = volume_threshold;
    usage_monitoring_data_local_var->is_volume_threshold_uplink_null = is_volume_threshold_uplink_null;
    usage_monitoring_data_local_var->is_volume_threshold_uplink = is_volume_threshold_uplink;
    usage_monitoring_data_local_var->volume_threshold_uplink = volume_threshold_uplink;
    usage_monitoring_data_local_var->is_volume_threshold_downlink_null = is_volume_threshold_downlink_null;
    usage_monitoring_data_local_var->is_volume_threshold_downlink = is_volume_threshold_downlink;
    usage_monitoring_data_local_var->volume_threshold_downlink = volume_threshold_downlink;
    usage_monitoring_data_local_var->is_time_threshold_null = is_time_threshold_null;
    usage_monitoring_data_local_var->is_time_threshold = is_time_threshold;
    usage_monitoring_data_local_var->time_threshold = time_threshold;
    usage_monitoring_data_local_var->is_monitoring_time_null = is_monitoring_time_null;
    usage_monitoring_data_local_var->monitoring_time = monitoring_time;
    usage_monitoring_data_local_var->is_next_vol_threshold_null = is_next_vol_threshold_null;
    usage_monitoring_data_local_var->is_next_vol_threshold = is_next_vol_threshold;
    usage_monitoring_data_local_var->next_vol_threshold = next_vol_threshold;
    usage_monitoring_data_local_var->is_next_vol_threshold_uplink_null = is_next_vol_threshold_uplink_null;
    usage_monitoring_data_local_var->is_next_vol_threshold_uplink = is_next_vol_threshold_uplink;
    usage_monitoring_data_local_var->next_vol_threshold_uplink = next_vol_threshold_uplink;
    usage_monitoring_data_local_var->is_next_vol_threshold_downlink_null = is_next_vol_threshold_downlink_null;
    usage_monitoring_data_local_var->is_next_vol_threshold_downlink = is_next_vol_threshold_downlink;
    usage_monitoring_data_local_var->next_vol_threshold_downlink = next_vol_threshold_downlink;
    usage_monitoring_data_local_var->is_next_time_threshold_null = is_next_time_threshold_null;
    usage_monitoring_data_local_var->is_next_time_threshold = is_next_time_threshold;
    usage_monitoring_data_local_var->next_time_threshold = next_time_threshold;
    usage_monitoring_data_local_var->is_inactivity_time_null = is_inactivity_time_null;
    usage_monitoring_data_local_var->is_inactivity_time = is_inactivity_time;
    usage_monitoring_data_local_var->inactivity_time = inactivity_time;
    usage_monitoring_data_local_var->is_ex_usage_pcc_rule_ids_null = is_ex_usage_pcc_rule_ids_null;
    usage_monitoring_data_local_var->ex_usage_pcc_rule_ids = ex_usage_pcc_rule_ids;

    return usage_monitoring_data_local_var;
}

void OpenAPI_usage_monitoring_data_free(OpenAPI_usage_monitoring_data_t *usage_monitoring_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == usage_monitoring_data) {
        return;
    }
    if (usage_monitoring_data->um_id) {
        ogs_free(usage_monitoring_data->um_id);
        usage_monitoring_data->um_id = NULL;
    }
    if (usage_monitoring_data->monitoring_time) {
        ogs_free(usage_monitoring_data->monitoring_time);
        usage_monitoring_data->monitoring_time = NULL;
    }
    if (usage_monitoring_data->ex_usage_pcc_rule_ids) {
        OpenAPI_list_for_each(usage_monitoring_data->ex_usage_pcc_rule_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(usage_monitoring_data->ex_usage_pcc_rule_ids);
        usage_monitoring_data->ex_usage_pcc_rule_ids = NULL;
    }
    ogs_free(usage_monitoring_data);
}

cJSON *OpenAPI_usage_monitoring_data_convertToJSON(OpenAPI_usage_monitoring_data_t *usage_monitoring_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (usage_monitoring_data == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [UsageMonitoringData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!usage_monitoring_data->um_id) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [um_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "umId", usage_monitoring_data->um_id) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [um_id]");
        goto end;
    }

    if (usage_monitoring_data->is_volume_threshold) {
    if (cJSON_AddNumberToObject(item, "volumeThreshold", usage_monitoring_data->volume_threshold) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold]");
        goto end;
    }
    } else if (usage_monitoring_data->is_volume_threshold_null) {
        if (cJSON_AddNullToObject(item, "volumeThreshold") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_volume_threshold_uplink) {
    if (cJSON_AddNumberToObject(item, "volumeThresholdUplink", usage_monitoring_data->volume_threshold_uplink) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_uplink]");
        goto end;
    }
    } else if (usage_monitoring_data->is_volume_threshold_uplink_null) {
        if (cJSON_AddNullToObject(item, "volumeThresholdUplink") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_uplink]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_volume_threshold_downlink) {
    if (cJSON_AddNumberToObject(item, "volumeThresholdDownlink", usage_monitoring_data->volume_threshold_downlink) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_downlink]");
        goto end;
    }
    } else if (usage_monitoring_data->is_volume_threshold_downlink_null) {
        if (cJSON_AddNullToObject(item, "volumeThresholdDownlink") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [volume_threshold_downlink]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_time_threshold) {
    if (cJSON_AddNumberToObject(item, "timeThreshold", usage_monitoring_data->time_threshold) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [time_threshold]");
        goto end;
    }
    } else if (usage_monitoring_data->is_time_threshold_null) {
        if (cJSON_AddNullToObject(item, "timeThreshold") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [time_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->monitoring_time) {
    if (cJSON_AddStringToObject(item, "monitoringTime", usage_monitoring_data->monitoring_time) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [monitoring_time]");
        goto end;
    }
    } else if (usage_monitoring_data->is_monitoring_time_null) {
        if (cJSON_AddNullToObject(item, "monitoringTime") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [monitoring_time]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_next_vol_threshold) {
    if (cJSON_AddNumberToObject(item, "nextVolThreshold", usage_monitoring_data->next_vol_threshold) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold]");
        goto end;
    }
    } else if (usage_monitoring_data->is_next_vol_threshold_null) {
        if (cJSON_AddNullToObject(item, "nextVolThreshold") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_next_vol_threshold_uplink) {
    if (cJSON_AddNumberToObject(item, "nextVolThresholdUplink", usage_monitoring_data->next_vol_threshold_uplink) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_uplink]");
        goto end;
    }
    } else if (usage_monitoring_data->is_next_vol_threshold_uplink_null) {
        if (cJSON_AddNullToObject(item, "nextVolThresholdUplink") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_uplink]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_next_vol_threshold_downlink) {
    if (cJSON_AddNumberToObject(item, "nextVolThresholdDownlink", usage_monitoring_data->next_vol_threshold_downlink) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_downlink]");
        goto end;
    }
    } else if (usage_monitoring_data->is_next_vol_threshold_downlink_null) {
        if (cJSON_AddNullToObject(item, "nextVolThresholdDownlink") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_vol_threshold_downlink]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_next_time_threshold) {
    if (cJSON_AddNumberToObject(item, "nextTimeThreshold", usage_monitoring_data->next_time_threshold) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_time_threshold]");
        goto end;
    }
    } else if (usage_monitoring_data->is_next_time_threshold_null) {
        if (cJSON_AddNullToObject(item, "nextTimeThreshold") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [next_time_threshold]");
            goto end;
        }
    }

    if (usage_monitoring_data->is_inactivity_time) {
    if (cJSON_AddNumberToObject(item, "inactivityTime", usage_monitoring_data->inactivity_time) == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [inactivity_time]");
        goto end;
    }
    } else if (usage_monitoring_data->is_inactivity_time_null) {
        if (cJSON_AddNullToObject(item, "inactivityTime") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [inactivity_time]");
            goto end;
        }
    }

    if (usage_monitoring_data->ex_usage_pcc_rule_ids) {
    cJSON *ex_usage_pcc_rule_idsList = cJSON_AddArrayToObject(item, "exUsagePccRuleIds");
    if (ex_usage_pcc_rule_idsList == NULL) {
        ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [ex_usage_pcc_rule_ids]");
        goto end;
    }
    OpenAPI_list_for_each(usage_monitoring_data->ex_usage_pcc_rule_ids, node) {
        if (cJSON_AddStringToObject(ex_usage_pcc_rule_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [ex_usage_pcc_rule_ids]");
            goto end;
        }
    }
    } else if (usage_monitoring_data->is_ex_usage_pcc_rule_ids_null) {
        if (cJSON_AddNullToObject(item, "exUsagePccRuleIds") == NULL) {
            ogs_error("OpenAPI_usage_monitoring_data_convertToJSON() failed [ex_usage_pcc_rule_ids]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_usage_monitoring_data_t *OpenAPI_usage_monitoring_data_parseFromJSON(cJSON *usage_monitoring_dataJSON)
{
    OpenAPI_usage_monitoring_data_t *usage_monitoring_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *um_id = NULL;
    cJSON *volume_threshold = NULL;
    cJSON *volume_threshold_uplink = NULL;
    cJSON *volume_threshold_downlink = NULL;
    cJSON *time_threshold = NULL;
    cJSON *monitoring_time = NULL;
    cJSON *next_vol_threshold = NULL;
    cJSON *next_vol_threshold_uplink = NULL;
    cJSON *next_vol_threshold_downlink = NULL;
    cJSON *next_time_threshold = NULL;
    cJSON *inactivity_time = NULL;
    cJSON *ex_usage_pcc_rule_ids = NULL;
    OpenAPI_list_t *ex_usage_pcc_rule_idsList = NULL;
    um_id = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "umId");
    if (!um_id) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [um_id]");
        goto end;
    }
    if (!cJSON_IsString(um_id)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [um_id]");
        goto end;
    }

    volume_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThreshold");
    if (volume_threshold) {
    if (!cJSON_IsNull(volume_threshold)) {
    if (!cJSON_IsNumber(volume_threshold)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold]");
        goto end;
    }
    }
    }

    volume_threshold_uplink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThresholdUplink");
    if (volume_threshold_uplink) {
    if (!cJSON_IsNull(volume_threshold_uplink)) {
    if (!cJSON_IsNumber(volume_threshold_uplink)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold_uplink]");
        goto end;
    }
    }
    }

    volume_threshold_downlink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "volumeThresholdDownlink");
    if (volume_threshold_downlink) {
    if (!cJSON_IsNull(volume_threshold_downlink)) {
    if (!cJSON_IsNumber(volume_threshold_downlink)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [volume_threshold_downlink]");
        goto end;
    }
    }
    }

    time_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "timeThreshold");
    if (time_threshold) {
    if (!cJSON_IsNull(time_threshold)) {
    if (!cJSON_IsNumber(time_threshold)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [time_threshold]");
        goto end;
    }
    }
    }

    monitoring_time = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "monitoringTime");
    if (monitoring_time) {
    if (!cJSON_IsNull(monitoring_time)) {
    if (!cJSON_IsString(monitoring_time) && !cJSON_IsNull(monitoring_time)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [monitoring_time]");
        goto end;
    }
    }
    }

    next_vol_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThreshold");
    if (next_vol_threshold) {
    if (!cJSON_IsNull(next_vol_threshold)) {
    if (!cJSON_IsNumber(next_vol_threshold)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold]");
        goto end;
    }
    }
    }

    next_vol_threshold_uplink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThresholdUplink");
    if (next_vol_threshold_uplink) {
    if (!cJSON_IsNull(next_vol_threshold_uplink)) {
    if (!cJSON_IsNumber(next_vol_threshold_uplink)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold_uplink]");
        goto end;
    }
    }
    }

    next_vol_threshold_downlink = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextVolThresholdDownlink");
    if (next_vol_threshold_downlink) {
    if (!cJSON_IsNull(next_vol_threshold_downlink)) {
    if (!cJSON_IsNumber(next_vol_threshold_downlink)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_vol_threshold_downlink]");
        goto end;
    }
    }
    }

    next_time_threshold = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "nextTimeThreshold");
    if (next_time_threshold) {
    if (!cJSON_IsNull(next_time_threshold)) {
    if (!cJSON_IsNumber(next_time_threshold)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [next_time_threshold]");
        goto end;
    }
    }
    }

    inactivity_time = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "inactivityTime");
    if (inactivity_time) {
    if (!cJSON_IsNull(inactivity_time)) {
    if (!cJSON_IsNumber(inactivity_time)) {
        ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [inactivity_time]");
        goto end;
    }
    }
    }

    ex_usage_pcc_rule_ids = cJSON_GetObjectItemCaseSensitive(usage_monitoring_dataJSON, "exUsagePccRuleIds");
    if (ex_usage_pcc_rule_ids) {
    if (!cJSON_IsNull(ex_usage_pcc_rule_ids)) {
        cJSON *ex_usage_pcc_rule_ids_local = NULL;
        if (!cJSON_IsArray(ex_usage_pcc_rule_ids)) {
            ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [ex_usage_pcc_rule_ids]");
            goto end;
        }

        ex_usage_pcc_rule_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ex_usage_pcc_rule_ids_local, ex_usage_pcc_rule_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(ex_usage_pcc_rule_ids_local)) {
                ogs_error("OpenAPI_usage_monitoring_data_parseFromJSON() failed [ex_usage_pcc_rule_ids]");
                goto end;
            }
            OpenAPI_list_add(ex_usage_pcc_rule_idsList, ogs_strdup(ex_usage_pcc_rule_ids_local->valuestring));
        }
    }
    }

    usage_monitoring_data_local_var = OpenAPI_usage_monitoring_data_create (
        ogs_strdup(um_id->valuestring),
        volume_threshold && cJSON_IsNull(volume_threshold) ? true : false,
        volume_threshold ? true : false,
        volume_threshold ? volume_threshold->valuedouble : 0,
        volume_threshold_uplink && cJSON_IsNull(volume_threshold_uplink) ? true : false,
        volume_threshold_uplink ? true : false,
        volume_threshold_uplink ? volume_threshold_uplink->valuedouble : 0,
        volume_threshold_downlink && cJSON_IsNull(volume_threshold_downlink) ? true : false,
        volume_threshold_downlink ? true : false,
        volume_threshold_downlink ? volume_threshold_downlink->valuedouble : 0,
        time_threshold && cJSON_IsNull(time_threshold) ? true : false,
        time_threshold ? true : false,
        time_threshold ? time_threshold->valuedouble : 0,
        monitoring_time && cJSON_IsNull(monitoring_time) ? true : false,
        monitoring_time && !cJSON_IsNull(monitoring_time) ? ogs_strdup(monitoring_time->valuestring) : NULL,
        next_vol_threshold && cJSON_IsNull(next_vol_threshold) ? true : false,
        next_vol_threshold ? true : false,
        next_vol_threshold ? next_vol_threshold->valuedouble : 0,
        next_vol_threshold_uplink && cJSON_IsNull(next_vol_threshold_uplink) ? true : false,
        next_vol_threshold_uplink ? true : false,
        next_vol_threshold_uplink ? next_vol_threshold_uplink->valuedouble : 0,
        next_vol_threshold_downlink && cJSON_IsNull(next_vol_threshold_downlink) ? true : false,
        next_vol_threshold_downlink ? true : false,
        next_vol_threshold_downlink ? next_vol_threshold_downlink->valuedouble : 0,
        next_time_threshold && cJSON_IsNull(next_time_threshold) ? true : false,
        next_time_threshold ? true : false,
        next_time_threshold ? next_time_threshold->valuedouble : 0,
        inactivity_time && cJSON_IsNull(inactivity_time) ? true : false,
        inactivity_time ? true : false,
        inactivity_time ? inactivity_time->valuedouble : 0,
        ex_usage_pcc_rule_ids && cJSON_IsNull(ex_usage_pcc_rule_ids) ? true : false,
        ex_usage_pcc_rule_ids ? ex_usage_pcc_rule_idsList : NULL
    );

    return usage_monitoring_data_local_var;
end:
    if (ex_usage_pcc_rule_idsList) {
        OpenAPI_list_for_each(ex_usage_pcc_rule_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ex_usage_pcc_rule_idsList);
        ex_usage_pcc_rule_idsList = NULL;
    }
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

