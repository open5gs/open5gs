
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_options.h"

OpenAPI_reporting_options_t *OpenAPI_reporting_options_create(
    OpenAPI_event_report_mode_e report_mode,
    bool is_max_num_of_reports,
    int max_num_of_reports,
    char *expiry,
    bool is_sampling_ratio,
    int sampling_ratio,
    bool is_guard_time,
    int guard_time,
    bool is_report_period,
    int report_period,
    OpenAPI_notification_flag_e notif_flag,
    OpenAPI_muting_exception_instructions_t *muting_exc_instructions,
    OpenAPI_muting_notifications_settings_t *muting_not_settings,
    OpenAPI_list_t *var_rep_period_info
)
{
    OpenAPI_reporting_options_t *reporting_options_local_var = ogs_malloc(sizeof(OpenAPI_reporting_options_t));
    ogs_assert(reporting_options_local_var);

    reporting_options_local_var->report_mode = report_mode;
    reporting_options_local_var->is_max_num_of_reports = is_max_num_of_reports;
    reporting_options_local_var->max_num_of_reports = max_num_of_reports;
    reporting_options_local_var->expiry = expiry;
    reporting_options_local_var->is_sampling_ratio = is_sampling_ratio;
    reporting_options_local_var->sampling_ratio = sampling_ratio;
    reporting_options_local_var->is_guard_time = is_guard_time;
    reporting_options_local_var->guard_time = guard_time;
    reporting_options_local_var->is_report_period = is_report_period;
    reporting_options_local_var->report_period = report_period;
    reporting_options_local_var->notif_flag = notif_flag;
    reporting_options_local_var->muting_exc_instructions = muting_exc_instructions;
    reporting_options_local_var->muting_not_settings = muting_not_settings;
    reporting_options_local_var->var_rep_period_info = var_rep_period_info;

    return reporting_options_local_var;
}

void OpenAPI_reporting_options_free(OpenAPI_reporting_options_t *reporting_options)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reporting_options) {
        return;
    }
    if (reporting_options->expiry) {
        ogs_free(reporting_options->expiry);
        reporting_options->expiry = NULL;
    }
    if (reporting_options->muting_exc_instructions) {
        OpenAPI_muting_exception_instructions_free(reporting_options->muting_exc_instructions);
        reporting_options->muting_exc_instructions = NULL;
    }
    if (reporting_options->muting_not_settings) {
        OpenAPI_muting_notifications_settings_free(reporting_options->muting_not_settings);
        reporting_options->muting_not_settings = NULL;
    }
    if (reporting_options->var_rep_period_info) {
        OpenAPI_list_for_each(reporting_options->var_rep_period_info, node) {
            OpenAPI_var_rep_period_free(node->data);
        }
        OpenAPI_list_free(reporting_options->var_rep_period_info);
        reporting_options->var_rep_period_info = NULL;
    }
    ogs_free(reporting_options);
}

cJSON *OpenAPI_reporting_options_convertToJSON(OpenAPI_reporting_options_t *reporting_options)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reporting_options == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [ReportingOptions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reporting_options->report_mode != OpenAPI_event_report_mode_NULL) {
    if (cJSON_AddStringToObject(item, "reportMode", OpenAPI_event_report_mode_ToString(reporting_options->report_mode)) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [report_mode]");
        goto end;
    }
    }

    if (reporting_options->is_max_num_of_reports) {
    if (cJSON_AddNumberToObject(item, "maxNumOfReports", reporting_options->max_num_of_reports) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [max_num_of_reports]");
        goto end;
    }
    }

    if (reporting_options->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", reporting_options->expiry) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (reporting_options->is_sampling_ratio) {
    if (cJSON_AddNumberToObject(item, "samplingRatio", reporting_options->sampling_ratio) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [sampling_ratio]");
        goto end;
    }
    }

    if (reporting_options->is_guard_time) {
    if (cJSON_AddNumberToObject(item, "guardTime", reporting_options->guard_time) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [guard_time]");
        goto end;
    }
    }

    if (reporting_options->is_report_period) {
    if (cJSON_AddNumberToObject(item, "reportPeriod", reporting_options->report_period) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [report_period]");
        goto end;
    }
    }

    if (reporting_options->notif_flag != OpenAPI_notification_flag_NULL) {
    if (cJSON_AddStringToObject(item, "notifFlag", OpenAPI_notification_flag_ToString(reporting_options->notif_flag)) == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [notif_flag]");
        goto end;
    }
    }

    if (reporting_options->muting_exc_instructions) {
    cJSON *muting_exc_instructions_local_JSON = OpenAPI_muting_exception_instructions_convertToJSON(reporting_options->muting_exc_instructions);
    if (muting_exc_instructions_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [muting_exc_instructions]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mutingExcInstructions", muting_exc_instructions_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [muting_exc_instructions]");
        goto end;
    }
    }

    if (reporting_options->muting_not_settings) {
    cJSON *muting_not_settings_local_JSON = OpenAPI_muting_notifications_settings_convertToJSON(reporting_options->muting_not_settings);
    if (muting_not_settings_local_JSON == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [muting_not_settings]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mutingNotSettings", muting_not_settings_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [muting_not_settings]");
        goto end;
    }
    }

    if (reporting_options->var_rep_period_info) {
    cJSON *var_rep_period_infoList = cJSON_AddArrayToObject(item, "varRepPeriodInfo");
    if (var_rep_period_infoList == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [var_rep_period_info]");
        goto end;
    }
    OpenAPI_list_for_each(reporting_options->var_rep_period_info, node) {
        cJSON *itemLocal = OpenAPI_var_rep_period_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [var_rep_period_info]");
            goto end;
        }
        cJSON_AddItemToArray(var_rep_period_infoList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_reporting_options_t *OpenAPI_reporting_options_parseFromJSON(cJSON *reporting_optionsJSON)
{
    OpenAPI_reporting_options_t *reporting_options_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *report_mode = NULL;
    OpenAPI_event_report_mode_e report_modeVariable = 0;
    cJSON *max_num_of_reports = NULL;
    cJSON *expiry = NULL;
    cJSON *sampling_ratio = NULL;
    cJSON *guard_time = NULL;
    cJSON *report_period = NULL;
    cJSON *notif_flag = NULL;
    OpenAPI_notification_flag_e notif_flagVariable = 0;
    cJSON *muting_exc_instructions = NULL;
    OpenAPI_muting_exception_instructions_t *muting_exc_instructions_local_nonprim = NULL;
    cJSON *muting_not_settings = NULL;
    OpenAPI_muting_notifications_settings_t *muting_not_settings_local_nonprim = NULL;
    cJSON *var_rep_period_info = NULL;
    OpenAPI_list_t *var_rep_period_infoList = NULL;
    report_mode = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "reportMode");
    if (report_mode) {
    if (!cJSON_IsString(report_mode)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [report_mode]");
        goto end;
    }
    report_modeVariable = OpenAPI_event_report_mode_FromString(report_mode->valuestring);
    }

    max_num_of_reports = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "maxNumOfReports");
    if (max_num_of_reports) {
    if (!cJSON_IsNumber(max_num_of_reports)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [max_num_of_reports]");
        goto end;
    }
    }

    expiry = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    sampling_ratio = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "samplingRatio");
    if (sampling_ratio) {
    if (!cJSON_IsNumber(sampling_ratio)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [sampling_ratio]");
        goto end;
    }
    }

    guard_time = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "guardTime");
    if (guard_time) {
    if (!cJSON_IsNumber(guard_time)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [guard_time]");
        goto end;
    }
    }

    report_period = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "reportPeriod");
    if (report_period) {
    if (!cJSON_IsNumber(report_period)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [report_period]");
        goto end;
    }
    }

    notif_flag = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "notifFlag");
    if (notif_flag) {
    if (!cJSON_IsString(notif_flag)) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [notif_flag]");
        goto end;
    }
    notif_flagVariable = OpenAPI_notification_flag_FromString(notif_flag->valuestring);
    }

    muting_exc_instructions = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "mutingExcInstructions");
    if (muting_exc_instructions) {
    muting_exc_instructions_local_nonprim = OpenAPI_muting_exception_instructions_parseFromJSON(muting_exc_instructions);
    if (!muting_exc_instructions_local_nonprim) {
        ogs_error("OpenAPI_muting_exception_instructions_parseFromJSON failed [muting_exc_instructions]");
        goto end;
    }
    }

    muting_not_settings = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "mutingNotSettings");
    if (muting_not_settings) {
    muting_not_settings_local_nonprim = OpenAPI_muting_notifications_settings_parseFromJSON(muting_not_settings);
    if (!muting_not_settings_local_nonprim) {
        ogs_error("OpenAPI_muting_notifications_settings_parseFromJSON failed [muting_not_settings]");
        goto end;
    }
    }

    var_rep_period_info = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "varRepPeriodInfo");
    if (var_rep_period_info) {
        cJSON *var_rep_period_info_local = NULL;
        if (!cJSON_IsArray(var_rep_period_info)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [var_rep_period_info]");
            goto end;
        }

        var_rep_period_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(var_rep_period_info_local, var_rep_period_info) {
            if (!cJSON_IsObject(var_rep_period_info_local)) {
                ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [var_rep_period_info]");
                goto end;
            }
            OpenAPI_var_rep_period_t *var_rep_period_infoItem = OpenAPI_var_rep_period_parseFromJSON(var_rep_period_info_local);
            if (!var_rep_period_infoItem) {
                ogs_error("No var_rep_period_infoItem");
                goto end;
            }
            OpenAPI_list_add(var_rep_period_infoList, var_rep_period_infoItem);
        }
    }

    reporting_options_local_var = OpenAPI_reporting_options_create (
        report_mode ? report_modeVariable : 0,
        max_num_of_reports ? true : false,
        max_num_of_reports ? max_num_of_reports->valuedouble : 0,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        sampling_ratio ? true : false,
        sampling_ratio ? sampling_ratio->valuedouble : 0,
        guard_time ? true : false,
        guard_time ? guard_time->valuedouble : 0,
        report_period ? true : false,
        report_period ? report_period->valuedouble : 0,
        notif_flag ? notif_flagVariable : 0,
        muting_exc_instructions ? muting_exc_instructions_local_nonprim : NULL,
        muting_not_settings ? muting_not_settings_local_nonprim : NULL,
        var_rep_period_info ? var_rep_period_infoList : NULL
    );

    return reporting_options_local_var;
end:
    if (muting_exc_instructions_local_nonprim) {
        OpenAPI_muting_exception_instructions_free(muting_exc_instructions_local_nonprim);
        muting_exc_instructions_local_nonprim = NULL;
    }
    if (muting_not_settings_local_nonprim) {
        OpenAPI_muting_notifications_settings_free(muting_not_settings_local_nonprim);
        muting_not_settings_local_nonprim = NULL;
    }
    if (var_rep_period_infoList) {
        OpenAPI_list_for_each(var_rep_period_infoList, node) {
            OpenAPI_var_rep_period_free(node->data);
        }
        OpenAPI_list_free(var_rep_period_infoList);
        var_rep_period_infoList = NULL;
    }
    return NULL;
}

OpenAPI_reporting_options_t *OpenAPI_reporting_options_copy(OpenAPI_reporting_options_t *dst, OpenAPI_reporting_options_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reporting_options_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed");
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

    OpenAPI_reporting_options_free(dst);
    dst = OpenAPI_reporting_options_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

