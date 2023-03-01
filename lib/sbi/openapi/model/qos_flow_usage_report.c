
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_usage_report.h"

OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_create(
    int qfi,
    char *start_time_stamp,
    char *end_time_stamp,
    long downlink_volume,
    long uplink_volume
)
{
    OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_usage_report_t));
    ogs_assert(qos_flow_usage_report_local_var);

    qos_flow_usage_report_local_var->qfi = qfi;
    qos_flow_usage_report_local_var->start_time_stamp = start_time_stamp;
    qos_flow_usage_report_local_var->end_time_stamp = end_time_stamp;
    qos_flow_usage_report_local_var->downlink_volume = downlink_volume;
    qos_flow_usage_report_local_var->uplink_volume = uplink_volume;

    return qos_flow_usage_report_local_var;
}

void OpenAPI_qos_flow_usage_report_free(OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_flow_usage_report) {
        return;
    }
    if (qos_flow_usage_report->start_time_stamp) {
        ogs_free(qos_flow_usage_report->start_time_stamp);
        qos_flow_usage_report->start_time_stamp = NULL;
    }
    if (qos_flow_usage_report->end_time_stamp) {
        ogs_free(qos_flow_usage_report->end_time_stamp);
        qos_flow_usage_report->end_time_stamp = NULL;
    }
    ogs_free(qos_flow_usage_report);
}

cJSON *OpenAPI_qos_flow_usage_report_convertToJSON(OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_flow_usage_report == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [QosFlowUsageReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_usage_report->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [qfi]");
        goto end;
    }

    if (!qos_flow_usage_report->start_time_stamp) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [start_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "startTimeStamp", qos_flow_usage_report->start_time_stamp) == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [start_time_stamp]");
        goto end;
    }

    if (!qos_flow_usage_report->end_time_stamp) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [end_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "endTimeStamp", qos_flow_usage_report->end_time_stamp) == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [end_time_stamp]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "downlinkVolume", qos_flow_usage_report->downlink_volume) == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [downlink_volume]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "uplinkVolume", qos_flow_usage_report->uplink_volume) == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed [uplink_volume]");
        goto end;
    }

end:
    return item;
}

OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_parseFromJSON(cJSON *qos_flow_usage_reportJSON)
{
    OpenAPI_qos_flow_usage_report_t *qos_flow_usage_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qfi = NULL;
    cJSON *start_time_stamp = NULL;
    cJSON *end_time_stamp = NULL;
    cJSON *downlink_volume = NULL;
    cJSON *uplink_volume = NULL;
    qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_usage_reportJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [qfi]");
        goto end;
    }
    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [qfi]");
        goto end;
    }

    start_time_stamp = cJSON_GetObjectItemCaseSensitive(qos_flow_usage_reportJSON, "startTimeStamp");
    if (!start_time_stamp) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [start_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(start_time_stamp) && !cJSON_IsNull(start_time_stamp)) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [start_time_stamp]");
        goto end;
    }

    end_time_stamp = cJSON_GetObjectItemCaseSensitive(qos_flow_usage_reportJSON, "endTimeStamp");
    if (!end_time_stamp) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [end_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(end_time_stamp) && !cJSON_IsNull(end_time_stamp)) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [end_time_stamp]");
        goto end;
    }

    downlink_volume = cJSON_GetObjectItemCaseSensitive(qos_flow_usage_reportJSON, "downlinkVolume");
    if (!downlink_volume) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [downlink_volume]");
        goto end;
    }
    if (!cJSON_IsNumber(downlink_volume)) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [downlink_volume]");
        goto end;
    }

    uplink_volume = cJSON_GetObjectItemCaseSensitive(qos_flow_usage_reportJSON, "uplinkVolume");
    if (!uplink_volume) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [uplink_volume]");
        goto end;
    }
    if (!cJSON_IsNumber(uplink_volume)) {
        ogs_error("OpenAPI_qos_flow_usage_report_parseFromJSON() failed [uplink_volume]");
        goto end;
    }

    qos_flow_usage_report_local_var = OpenAPI_qos_flow_usage_report_create (
        
        qfi->valuedouble,
        ogs_strdup(start_time_stamp->valuestring),
        ogs_strdup(end_time_stamp->valuestring),
        
        downlink_volume->valuedouble,
        
        uplink_volume->valuedouble
    );

    return qos_flow_usage_report_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_usage_report_t *OpenAPI_qos_flow_usage_report_copy(OpenAPI_qos_flow_usage_report_t *dst, OpenAPI_qos_flow_usage_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_usage_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_usage_report_convertToJSON() failed");
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

    OpenAPI_qos_flow_usage_report_free(dst);
    dst = OpenAPI_qos_flow_usage_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

