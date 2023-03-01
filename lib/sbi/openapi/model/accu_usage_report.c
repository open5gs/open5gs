
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accu_usage_report.h"

OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_create(
    char *ref_um_ids,
    bool is_vol_usage,
    long vol_usage,
    bool is_vol_usage_uplink,
    long vol_usage_uplink,
    bool is_vol_usage_downlink,
    long vol_usage_downlink,
    bool is_time_usage,
    int time_usage,
    bool is_next_vol_usage,
    long next_vol_usage,
    bool is_next_vol_usage_uplink,
    long next_vol_usage_uplink,
    bool is_next_vol_usage_downlink,
    long next_vol_usage_downlink,
    bool is_next_time_usage,
    int next_time_usage
)
{
    OpenAPI_accu_usage_report_t *accu_usage_report_local_var = ogs_malloc(sizeof(OpenAPI_accu_usage_report_t));
    ogs_assert(accu_usage_report_local_var);

    accu_usage_report_local_var->ref_um_ids = ref_um_ids;
    accu_usage_report_local_var->is_vol_usage = is_vol_usage;
    accu_usage_report_local_var->vol_usage = vol_usage;
    accu_usage_report_local_var->is_vol_usage_uplink = is_vol_usage_uplink;
    accu_usage_report_local_var->vol_usage_uplink = vol_usage_uplink;
    accu_usage_report_local_var->is_vol_usage_downlink = is_vol_usage_downlink;
    accu_usage_report_local_var->vol_usage_downlink = vol_usage_downlink;
    accu_usage_report_local_var->is_time_usage = is_time_usage;
    accu_usage_report_local_var->time_usage = time_usage;
    accu_usage_report_local_var->is_next_vol_usage = is_next_vol_usage;
    accu_usage_report_local_var->next_vol_usage = next_vol_usage;
    accu_usage_report_local_var->is_next_vol_usage_uplink = is_next_vol_usage_uplink;
    accu_usage_report_local_var->next_vol_usage_uplink = next_vol_usage_uplink;
    accu_usage_report_local_var->is_next_vol_usage_downlink = is_next_vol_usage_downlink;
    accu_usage_report_local_var->next_vol_usage_downlink = next_vol_usage_downlink;
    accu_usage_report_local_var->is_next_time_usage = is_next_time_usage;
    accu_usage_report_local_var->next_time_usage = next_time_usage;

    return accu_usage_report_local_var;
}

void OpenAPI_accu_usage_report_free(OpenAPI_accu_usage_report_t *accu_usage_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == accu_usage_report) {
        return;
    }
    if (accu_usage_report->ref_um_ids) {
        ogs_free(accu_usage_report->ref_um_ids);
        accu_usage_report->ref_um_ids = NULL;
    }
    ogs_free(accu_usage_report);
}

cJSON *OpenAPI_accu_usage_report_convertToJSON(OpenAPI_accu_usage_report_t *accu_usage_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (accu_usage_report == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [AccuUsageReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!accu_usage_report->ref_um_ids) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [ref_um_ids]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "refUmIds", accu_usage_report->ref_um_ids) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [ref_um_ids]");
        goto end;
    }

    if (accu_usage_report->is_vol_usage) {
    if (cJSON_AddNumberToObject(item, "volUsage", accu_usage_report->vol_usage) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [vol_usage]");
        goto end;
    }
    }

    if (accu_usage_report->is_vol_usage_uplink) {
    if (cJSON_AddNumberToObject(item, "volUsageUplink", accu_usage_report->vol_usage_uplink) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [vol_usage_uplink]");
        goto end;
    }
    }

    if (accu_usage_report->is_vol_usage_downlink) {
    if (cJSON_AddNumberToObject(item, "volUsageDownlink", accu_usage_report->vol_usage_downlink) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [vol_usage_downlink]");
        goto end;
    }
    }

    if (accu_usage_report->is_time_usage) {
    if (cJSON_AddNumberToObject(item, "timeUsage", accu_usage_report->time_usage) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [time_usage]");
        goto end;
    }
    }

    if (accu_usage_report->is_next_vol_usage) {
    if (cJSON_AddNumberToObject(item, "nextVolUsage", accu_usage_report->next_vol_usage) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [next_vol_usage]");
        goto end;
    }
    }

    if (accu_usage_report->is_next_vol_usage_uplink) {
    if (cJSON_AddNumberToObject(item, "nextVolUsageUplink", accu_usage_report->next_vol_usage_uplink) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [next_vol_usage_uplink]");
        goto end;
    }
    }

    if (accu_usage_report->is_next_vol_usage_downlink) {
    if (cJSON_AddNumberToObject(item, "nextVolUsageDownlink", accu_usage_report->next_vol_usage_downlink) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [next_vol_usage_downlink]");
        goto end;
    }
    }

    if (accu_usage_report->is_next_time_usage) {
    if (cJSON_AddNumberToObject(item, "nextTimeUsage", accu_usage_report->next_time_usage) == NULL) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed [next_time_usage]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_parseFromJSON(cJSON *accu_usage_reportJSON)
{
    OpenAPI_accu_usage_report_t *accu_usage_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ref_um_ids = NULL;
    cJSON *vol_usage = NULL;
    cJSON *vol_usage_uplink = NULL;
    cJSON *vol_usage_downlink = NULL;
    cJSON *time_usage = NULL;
    cJSON *next_vol_usage = NULL;
    cJSON *next_vol_usage_uplink = NULL;
    cJSON *next_vol_usage_downlink = NULL;
    cJSON *next_time_usage = NULL;
    ref_um_ids = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "refUmIds");
    if (!ref_um_ids) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [ref_um_ids]");
        goto end;
    }
    if (!cJSON_IsString(ref_um_ids)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [ref_um_ids]");
        goto end;
    }

    vol_usage = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "volUsage");
    if (vol_usage) {
    if (!cJSON_IsNumber(vol_usage)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [vol_usage]");
        goto end;
    }
    }

    vol_usage_uplink = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "volUsageUplink");
    if (vol_usage_uplink) {
    if (!cJSON_IsNumber(vol_usage_uplink)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [vol_usage_uplink]");
        goto end;
    }
    }

    vol_usage_downlink = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "volUsageDownlink");
    if (vol_usage_downlink) {
    if (!cJSON_IsNumber(vol_usage_downlink)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [vol_usage_downlink]");
        goto end;
    }
    }

    time_usage = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "timeUsage");
    if (time_usage) {
    if (!cJSON_IsNumber(time_usage)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [time_usage]");
        goto end;
    }
    }

    next_vol_usage = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "nextVolUsage");
    if (next_vol_usage) {
    if (!cJSON_IsNumber(next_vol_usage)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [next_vol_usage]");
        goto end;
    }
    }

    next_vol_usage_uplink = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "nextVolUsageUplink");
    if (next_vol_usage_uplink) {
    if (!cJSON_IsNumber(next_vol_usage_uplink)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [next_vol_usage_uplink]");
        goto end;
    }
    }

    next_vol_usage_downlink = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "nextVolUsageDownlink");
    if (next_vol_usage_downlink) {
    if (!cJSON_IsNumber(next_vol_usage_downlink)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [next_vol_usage_downlink]");
        goto end;
    }
    }

    next_time_usage = cJSON_GetObjectItemCaseSensitive(accu_usage_reportJSON, "nextTimeUsage");
    if (next_time_usage) {
    if (!cJSON_IsNumber(next_time_usage)) {
        ogs_error("OpenAPI_accu_usage_report_parseFromJSON() failed [next_time_usage]");
        goto end;
    }
    }

    accu_usage_report_local_var = OpenAPI_accu_usage_report_create (
        ogs_strdup(ref_um_ids->valuestring),
        vol_usage ? true : false,
        vol_usage ? vol_usage->valuedouble : 0,
        vol_usage_uplink ? true : false,
        vol_usage_uplink ? vol_usage_uplink->valuedouble : 0,
        vol_usage_downlink ? true : false,
        vol_usage_downlink ? vol_usage_downlink->valuedouble : 0,
        time_usage ? true : false,
        time_usage ? time_usage->valuedouble : 0,
        next_vol_usage ? true : false,
        next_vol_usage ? next_vol_usage->valuedouble : 0,
        next_vol_usage_uplink ? true : false,
        next_vol_usage_uplink ? next_vol_usage_uplink->valuedouble : 0,
        next_vol_usage_downlink ? true : false,
        next_vol_usage_downlink ? next_vol_usage_downlink->valuedouble : 0,
        next_time_usage ? true : false,
        next_time_usage ? next_time_usage->valuedouble : 0
    );

    return accu_usage_report_local_var;
end:
    return NULL;
}

OpenAPI_accu_usage_report_t *OpenAPI_accu_usage_report_copy(OpenAPI_accu_usage_report_t *dst, OpenAPI_accu_usage_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accu_usage_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accu_usage_report_convertToJSON() failed");
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

    OpenAPI_accu_usage_report_free(dst);
    dst = OpenAPI_accu_usage_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

