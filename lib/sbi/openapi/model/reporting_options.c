
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reporting_options.h"

OpenAPI_reporting_options_t *OpenAPI_reporting_options_create(
    OpenAPI_event_report_mode_t *report_mode,
    int max_num_of_reports,
    char *expiry,
    int sampling_ratio,
    int guard_time,
    int report_period
    )
{
    OpenAPI_reporting_options_t *reporting_options_local_var = OpenAPI_malloc(sizeof(OpenAPI_reporting_options_t));
    if (!reporting_options_local_var) {
        return NULL;
    }
    reporting_options_local_var->report_mode = report_mode;
    reporting_options_local_var->max_num_of_reports = max_num_of_reports;
    reporting_options_local_var->expiry = expiry;
    reporting_options_local_var->sampling_ratio = sampling_ratio;
    reporting_options_local_var->guard_time = guard_time;
    reporting_options_local_var->report_period = report_period;

    return reporting_options_local_var;
}

void OpenAPI_reporting_options_free(OpenAPI_reporting_options_t *reporting_options)
{
    if (NULL == reporting_options) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_event_report_mode_free(reporting_options->report_mode);
    ogs_free(reporting_options->expiry);
    ogs_free(reporting_options);
}

cJSON *OpenAPI_reporting_options_convertToJSON(OpenAPI_reporting_options_t *reporting_options)
{
    cJSON *item = NULL;

    if (reporting_options == NULL) {
        ogs_error("OpenAPI_reporting_options_convertToJSON() failed [ReportingOptions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (reporting_options->report_mode) {
        cJSON *report_mode_local_JSON = OpenAPI_event_report_mode_convertToJSON(reporting_options->report_mode);
        if (report_mode_local_JSON == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [report_mode]");
            goto end;
        }
        cJSON_AddItemToObject(item, "reportMode", report_mode_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [report_mode]");
            goto end;
        }
    }

    if (reporting_options->max_num_of_reports) {
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

    if (reporting_options->sampling_ratio) {
        if (cJSON_AddNumberToObject(item, "samplingRatio", reporting_options->sampling_ratio) == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [sampling_ratio]");
            goto end;
        }
    }

    if (reporting_options->guard_time) {
        if (cJSON_AddNumberToObject(item, "guardTime", reporting_options->guard_time) == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [guard_time]");
            goto end;
        }
    }

    if (reporting_options->report_period) {
        if (cJSON_AddNumberToObject(item, "reportPeriod", reporting_options->report_period) == NULL) {
            ogs_error("OpenAPI_reporting_options_convertToJSON() failed [report_period]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_reporting_options_t *OpenAPI_reporting_options_parseFromJSON(cJSON *reporting_optionsJSON)
{
    OpenAPI_reporting_options_t *reporting_options_local_var = NULL;
    cJSON *report_mode = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "reportMode");

    OpenAPI_event_report_mode_t *report_mode_local_nonprim = NULL;
    if (report_mode) {
        report_mode_local_nonprim = OpenAPI_event_report_mode_parseFromJSON(report_mode);
    }

    cJSON *max_num_of_reports = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "maxNumOfReports");

    if (max_num_of_reports) {
        if (!cJSON_IsNumber(max_num_of_reports)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [max_num_of_reports]");
            goto end;
        }
    }

    cJSON *expiry = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "expiry");

    if (expiry) {
        if (!cJSON_IsString(expiry)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [expiry]");
            goto end;
        }
    }

    cJSON *sampling_ratio = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "samplingRatio");

    if (sampling_ratio) {
        if (!cJSON_IsNumber(sampling_ratio)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [sampling_ratio]");
            goto end;
        }
    }

    cJSON *guard_time = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "guardTime");

    if (guard_time) {
        if (!cJSON_IsNumber(guard_time)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [guard_time]");
            goto end;
        }
    }

    cJSON *report_period = cJSON_GetObjectItemCaseSensitive(reporting_optionsJSON, "reportPeriod");

    if (report_period) {
        if (!cJSON_IsNumber(report_period)) {
            ogs_error("OpenAPI_reporting_options_parseFromJSON() failed [report_period]");
            goto end;
        }
    }

    reporting_options_local_var = OpenAPI_reporting_options_create (
        report_mode ? report_mode_local_nonprim : NULL,
        max_num_of_reports ? max_num_of_reports->valuedouble : 0,
        expiry ? ogs_strdup(expiry->valuestring) : NULL,
        sampling_ratio ? sampling_ratio->valuedouble : 0,
        guard_time ? guard_time->valuedouble : 0,
        report_period ? report_period->valuedouble : 0
        );

    return reporting_options_local_var;
end:
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

