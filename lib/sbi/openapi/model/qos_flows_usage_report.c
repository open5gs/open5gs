
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flows_usage_report.h"

OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_create(
    bool is_q_fi,
    int q_fi,
    char *start_timestamp,
    char *end_timestamp,
    bool is_uplink_volume,
    long uplink_volume,
    bool is_downlink_volume,
    long downlink_volume
)
{
    OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report_local_var = ogs_malloc(sizeof(OpenAPI_qos_flows_usage_report_t));
    ogs_assert(qos_flows_usage_report_local_var);

    qos_flows_usage_report_local_var->is_q_fi = is_q_fi;
    qos_flows_usage_report_local_var->q_fi = q_fi;
    qos_flows_usage_report_local_var->start_timestamp = start_timestamp;
    qos_flows_usage_report_local_var->end_timestamp = end_timestamp;
    qos_flows_usage_report_local_var->is_uplink_volume = is_uplink_volume;
    qos_flows_usage_report_local_var->uplink_volume = uplink_volume;
    qos_flows_usage_report_local_var->is_downlink_volume = is_downlink_volume;
    qos_flows_usage_report_local_var->downlink_volume = downlink_volume;

    return qos_flows_usage_report_local_var;
}

void OpenAPI_qos_flows_usage_report_free(OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_flows_usage_report) {
        return;
    }
    if (qos_flows_usage_report->start_timestamp) {
        ogs_free(qos_flows_usage_report->start_timestamp);
        qos_flows_usage_report->start_timestamp = NULL;
    }
    if (qos_flows_usage_report->end_timestamp) {
        ogs_free(qos_flows_usage_report->end_timestamp);
        qos_flows_usage_report->end_timestamp = NULL;
    }
    ogs_free(qos_flows_usage_report);
}

cJSON *OpenAPI_qos_flows_usage_report_convertToJSON(OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_flows_usage_report == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [QosFlowsUsageReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_flows_usage_report->is_q_fi) {
    if (cJSON_AddNumberToObject(item, "qFI", qos_flows_usage_report->q_fi) == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [q_fi]");
        goto end;
    }
    }

    if (qos_flows_usage_report->start_timestamp) {
    if (cJSON_AddStringToObject(item, "startTimestamp", qos_flows_usage_report->start_timestamp) == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [start_timestamp]");
        goto end;
    }
    }

    if (qos_flows_usage_report->end_timestamp) {
    if (cJSON_AddStringToObject(item, "endTimestamp", qos_flows_usage_report->end_timestamp) == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [end_timestamp]");
        goto end;
    }
    }

    if (qos_flows_usage_report->is_uplink_volume) {
    if (cJSON_AddNumberToObject(item, "uplinkVolume", qos_flows_usage_report->uplink_volume) == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [uplink_volume]");
        goto end;
    }
    }

    if (qos_flows_usage_report->is_downlink_volume) {
    if (cJSON_AddNumberToObject(item, "downlinkVolume", qos_flows_usage_report->downlink_volume) == NULL) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed [downlink_volume]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_parseFromJSON(cJSON *qos_flows_usage_reportJSON)
{
    OpenAPI_qos_flows_usage_report_t *qos_flows_usage_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *q_fi = NULL;
    cJSON *start_timestamp = NULL;
    cJSON *end_timestamp = NULL;
    cJSON *uplink_volume = NULL;
    cJSON *downlink_volume = NULL;
    q_fi = cJSON_GetObjectItemCaseSensitive(qos_flows_usage_reportJSON, "qFI");
    if (q_fi) {
    if (!cJSON_IsNumber(q_fi)) {
        ogs_error("OpenAPI_qos_flows_usage_report_parseFromJSON() failed [q_fi]");
        goto end;
    }
    }

    start_timestamp = cJSON_GetObjectItemCaseSensitive(qos_flows_usage_reportJSON, "startTimestamp");
    if (start_timestamp) {
    if (!cJSON_IsString(start_timestamp) && !cJSON_IsNull(start_timestamp)) {
        ogs_error("OpenAPI_qos_flows_usage_report_parseFromJSON() failed [start_timestamp]");
        goto end;
    }
    }

    end_timestamp = cJSON_GetObjectItemCaseSensitive(qos_flows_usage_reportJSON, "endTimestamp");
    if (end_timestamp) {
    if (!cJSON_IsString(end_timestamp) && !cJSON_IsNull(end_timestamp)) {
        ogs_error("OpenAPI_qos_flows_usage_report_parseFromJSON() failed [end_timestamp]");
        goto end;
    }
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(qos_flows_usage_reportJSON, "uplinkVolume");
    if (uplink_volume) {
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_qos_flows_usage_report_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(qos_flows_usage_reportJSON, "downlinkVolume");
    if (downlink_volume) {
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_qos_flows_usage_report_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    }

    qos_flows_usage_report_local_var = OpenAPI_qos_flows_usage_report_create (
        q_fi ? true : false,
        q_fi ? q_fi->valuedouble : 0,
        start_timestamp && !cJSON_IsNull(start_timestamp) ? ogs_strdup(start_timestamp->valuestring) : NULL,
        end_timestamp && !cJSON_IsNull(end_timestamp) ? ogs_strdup(end_timestamp->valuestring) : NULL,
        uplink_volume ? true : false,
        uplink_volume ? uplink_volume->valuedouble : 0,
        downlink_volume ? true : false,
        downlink_volume ? downlink_volume->valuedouble : 0
    );

    return qos_flows_usage_report_local_var;
end:
    return NULL;
}

OpenAPI_qos_flows_usage_report_t *OpenAPI_qos_flows_usage_report_copy(OpenAPI_qos_flows_usage_report_t *dst, OpenAPI_qos_flows_usage_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flows_usage_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flows_usage_report_convertToJSON() failed");
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

    OpenAPI_qos_flows_usage_report_free(dst);
    dst = OpenAPI_qos_flows_usage_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

