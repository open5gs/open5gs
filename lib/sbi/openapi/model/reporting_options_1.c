
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_options_1.h"

OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_create(
    OpenAPI_event_report_mode_t *report_mode,
    bool is_max_num_of_reports,
    int max_num_of_reports,
    char *expiry,
    bool is_sampling_ratio,
    int sampling_ratio,
    bool is_guard_time,
    int guard_time,
    bool is_report_period,
    int report_period,
    OpenAPI_notification_flag_e notif_flag
)
{
    OpenAPI_reporting_options_1_t *reporting_options_1_local_var = ogs_malloc(sizeof(OpenAPI_reporting_options_1_t));
    ogs_assert(reporting_options_1_local_var);

    reporting_options_1_local_var->report_mode = report_mode;
    reporting_options_1_local_var->is_max_num_of_reports = is_max_num_of_reports;
    reporting_options_1_local_var->max_num_of_reports = max_num_of_reports;
    reporting_options_1_local_var->expiry = expiry;
    reporting_options_1_local_var->is_sampling_ratio = is_sampling_ratio;
    reporting_options_1_local_var->sampling_ratio = sampling_ratio;
    reporting_options_1_local_var->is_guard_time = is_guard_time;
    reporting_options_1_local_var->guard_time = guard_time;
    reporting_options_1_local_var->is_report_period = is_report_period;
    reporting_options_1_local_var->report_period = report_period;
    reporting_options_1_local_var->notif_flag = notif_flag;

    return reporting_options_1_local_var;
}

void OpenAPI_reporting_options_1_free(OpenAPI_reporting_options_1_t *reporting_options_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reporting_options_1) {
        return;
    }
    if (reporting_options_1->report_mode) {
        OpenAPI_event_report_mode_free(reporting_options_1->report_mode);
        reporting_options_1->report_mode = NULL;
    }
    if (reporting_options_1->expiry) {
        ogs_free(reporting_options_1->expiry);
        reporting_options_1->expiry = NULL;
    }
    ogs_free(reporting_options_1);
}

cJSON *OpenAPI_reporting_options_1_convertToJSON(OpenAPI_reporting_options_1_t *reporting_options_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reporting_options_1 == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [ReportingOptions_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reporting_options_1->report_mode) {
    cJSON *report_mode_local_JSON = OpenAPI_event_report_mode_convertToJSON(reporting_options_1->report_mode);
    if (report_mode_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [report_mode]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reportMode", report_mode_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [report_mode]");
        goto end;
    }
    }

    if (reporting_options_1->is_max_num_of_reports) {
    if (cJSON_AddNumberToObject(item, "maxNumOfReports", reporting_options_1->max_num_of_reports) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [max_num_of_reports]");
        goto end;
    }
    }

    if (reporting_options_1->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", reporting_options_1->expiry) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (reporting_options_1->is_sampling_ratio) {
    if (cJSON_AddNumberToObject(item, "samplingRatio", reporting_options_1->sampling_ratio) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [sampling_ratio]");
        goto end;
    }
    }

    if (reporting_options_1->is_guard_time) {
    if (cJSON_AddNumberToObject(item, "guardTime", reporting_options_1->guard_time) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [guard_time]");
        goto end;
    }
    }

    if (reporting_options_1->is_report_period) {
    if (cJSON_AddNumberToObject(item, "reportPeriod", reporting_options_1->report_period) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [report_period]");
        goto end;
    }
    }

    if (reporting_options_1->notif_flag != OpenAPI_notification_flag_NULL) {
    if (cJSON_AddStringToObject(item, "notifFlag", OpenAPI_notification_flag_ToString(reporting_options_1->notif_flag)) == NULL) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed [notif_flag]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_parseFromJSON(cJSON *reporting_options_1JSON)
{
    OpenAPI_reporting_options_1_t *reporting_options_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *report_mode = NULL;
    OpenAPI_event_report_mode_t *report_mode_local_nonprim = NULL;
    cJSON *max_num_of_reports = NULL;
    cJSON *expiry = NULL;
    cJSON *sampling_ratio = NULL;
    cJSON *guard_time = NULL;
    cJSON *report_period = NULL;
    cJSON *notif_flag = NULL;
    OpenAPI_notification_flag_e notif_flagVariable = 0;
    report_mode = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "reportMode");
    if (report_mode) {
    report_mode_local_nonprim = OpenAPI_event_report_mode_parseFromJSON(report_mode);
    if (!report_mode_local_nonprim) {
        ogs_error("OpenAPI_event_report_mode_parseFromJSON failed [report_mode]");
        goto end;
    }
    }

    max_num_of_reports = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "maxNumOfReports");
    if (max_num_of_reports) {
    if (!cJSON_IsNumber(max_num_of_reports)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [max_num_of_reports]");
        goto end;
    }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    sampling_ratio = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "samplingRatio");
    if (sampling_ratio) {
    if (!cJSON_IsNumber(sampling_ratio)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [sampling_ratio]");
        goto end;
    }
    }

    guard_time = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "guardTime");
    if (guard_time) {
    if (!cJSON_IsNumber(guard_time)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [guard_time]");
        goto end;
    }
    }

    report_period = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "reportPeriod");
    if (report_period) {
    if (!cJSON_IsNumber(report_period)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [report_period]");
        goto end;
    }
    }

    notif_flag = cJSON_GetObjectItemCaseSensitive(reporting_options_1JSON, "notifFlag");
    if (notif_flag) {
    if (!cJSON_IsString(notif_flag)) {
        ogs_error("OpenAPI_reporting_options_1_parseFromJSON() failed [notif_flag]");
        goto end;
    }
    notif_flagVariable = OpenAPI_notification_flag_FromString(notif_flag->valuestring);
    }

    reporting_options_1_local_var = OpenAPI_reporting_options_1_create (
        report_mode ? report_mode_local_nonprim : NULL,
        max_num_of_reports ? true : false,
        max_num_of_reports ? max_num_of_reports->valuedouble : 0,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        sampling_ratio ? true : false,
        sampling_ratio ? sampling_ratio->valuedouble : 0,
        guard_time ? true : false,
        guard_time ? guard_time->valuedouble : 0,
        report_period ? true : false,
        report_period ? report_period->valuedouble : 0,
        notif_flag ? notif_flagVariable : 0
    );

    return reporting_options_1_local_var;
end:
    if (report_mode_local_nonprim) {
        OpenAPI_event_report_mode_free(report_mode_local_nonprim);
        report_mode_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_reporting_options_1_t *OpenAPI_reporting_options_1_copy(OpenAPI_reporting_options_1_t *dst, OpenAPI_reporting_options_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_options_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_options_1_convertToJSON() failed");
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

    OpenAPI_reporting_options_1_free(dst);
    dst = OpenAPI_reporting_options_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

