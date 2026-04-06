
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "uav_altitude_reporting_config.h"

OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_create(
    double rpt_threshold_min,
    double rpt_threshold_max,
    bool is_alt_report_period,
    int alt_report_period,
    OpenAPI_list_t *alt_report_event_trigger
)
{
    OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config_local_var = ogs_malloc(sizeof(OpenAPI_uav_altitude_reporting_config_t));
    ogs_assert(uav_altitude_reporting_config_local_var);

    uav_altitude_reporting_config_local_var->rpt_threshold_min = rpt_threshold_min;
    uav_altitude_reporting_config_local_var->rpt_threshold_max = rpt_threshold_max;
    uav_altitude_reporting_config_local_var->is_alt_report_period = is_alt_report_period;
    uav_altitude_reporting_config_local_var->alt_report_period = alt_report_period;
    uav_altitude_reporting_config_local_var->alt_report_event_trigger = alt_report_event_trigger;

    return uav_altitude_reporting_config_local_var;
}

void OpenAPI_uav_altitude_reporting_config_free(OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == uav_altitude_reporting_config) {
        return;
    }
    if (uav_altitude_reporting_config->alt_report_event_trigger) {
        OpenAPI_list_free(uav_altitude_reporting_config->alt_report_event_trigger);
        uav_altitude_reporting_config->alt_report_event_trigger = NULL;
    }
    ogs_free(uav_altitude_reporting_config);
}

cJSON *OpenAPI_uav_altitude_reporting_config_convertToJSON(OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (uav_altitude_reporting_config == NULL) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [UavAltitudeReportingConfig]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "rptThresholdMin", uav_altitude_reporting_config->rpt_threshold_min) == NULL) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [rpt_threshold_min]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "rptThresholdMax", uav_altitude_reporting_config->rpt_threshold_max) == NULL) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [rpt_threshold_max]");
        goto end;
    }

    if (uav_altitude_reporting_config->is_alt_report_period) {
    if (cJSON_AddNumberToObject(item, "altReportPeriod", uav_altitude_reporting_config->alt_report_period) == NULL) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [alt_report_period]");
        goto end;
    }
    }

    if (uav_altitude_reporting_config->alt_report_event_trigger != OpenAPI_alt_report_event_trigger_NULL) {
    cJSON *alt_report_event_triggerList = cJSON_AddArrayToObject(item, "altReportEventTrigger");
    if (alt_report_event_triggerList == NULL) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [alt_report_event_trigger]");
        goto end;
    }
    OpenAPI_list_for_each(uav_altitude_reporting_config->alt_report_event_trigger, node) {
        if (cJSON_AddStringToObject(alt_report_event_triggerList, "", OpenAPI_alt_report_event_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed [alt_report_event_trigger]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_parseFromJSON(cJSON *uav_altitude_reporting_configJSON)
{
    OpenAPI_uav_altitude_reporting_config_t *uav_altitude_reporting_config_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rpt_threshold_min = NULL;
    cJSON *rpt_threshold_max = NULL;
    cJSON *alt_report_period = NULL;
    cJSON *alt_report_event_trigger = NULL;
    OpenAPI_list_t *alt_report_event_triggerList = NULL;
    rpt_threshold_min = cJSON_GetObjectItemCaseSensitive(uav_altitude_reporting_configJSON, "rptThresholdMin");
    if (!rpt_threshold_min) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [rpt_threshold_min]");
        goto end;
    }
    if (!cJSON_IsNumber(rpt_threshold_min)) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [rpt_threshold_min]");
        goto end;
    }

    rpt_threshold_max = cJSON_GetObjectItemCaseSensitive(uav_altitude_reporting_configJSON, "rptThresholdMax");
    if (!rpt_threshold_max) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [rpt_threshold_max]");
        goto end;
    }
    if (!cJSON_IsNumber(rpt_threshold_max)) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [rpt_threshold_max]");
        goto end;
    }

    alt_report_period = cJSON_GetObjectItemCaseSensitive(uav_altitude_reporting_configJSON, "altReportPeriod");
    if (alt_report_period) {
    if (!cJSON_IsNumber(alt_report_period)) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [alt_report_period]");
        goto end;
    }
    }

    alt_report_event_trigger = cJSON_GetObjectItemCaseSensitive(uav_altitude_reporting_configJSON, "altReportEventTrigger");
    if (alt_report_event_trigger) {
        cJSON *alt_report_event_trigger_local = NULL;
        if (!cJSON_IsArray(alt_report_event_trigger)) {
            ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [alt_report_event_trigger]");
            goto end;
        }

        alt_report_event_triggerList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_report_event_trigger_local, alt_report_event_trigger) {
            OpenAPI_alt_report_event_trigger_e localEnum = OpenAPI_alt_report_event_trigger_NULL;
            if (!cJSON_IsString(alt_report_event_trigger_local)) {
                ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed [alt_report_event_trigger]");
                goto end;
            }
            localEnum = OpenAPI_alt_report_event_trigger_FromString(alt_report_event_trigger_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"alt_report_event_trigger\" is not supported. Ignoring it ...",
                         alt_report_event_trigger_local->valuestring);
            } else {
                OpenAPI_list_add(alt_report_event_triggerList, (void *)localEnum);
            }
        }
        if (alt_report_event_triggerList->count == 0) {
            ogs_error("OpenAPI_uav_altitude_reporting_config_parseFromJSON() failed: Expected alt_report_event_triggerList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    uav_altitude_reporting_config_local_var = OpenAPI_uav_altitude_reporting_config_create (
        
        rpt_threshold_min->valuedouble,
        
        rpt_threshold_max->valuedouble,
        alt_report_period ? true : false,
        alt_report_period ? alt_report_period->valuedouble : 0,
        alt_report_event_trigger ? alt_report_event_triggerList : NULL
    );

    return uav_altitude_reporting_config_local_var;
end:
    if (alt_report_event_triggerList) {
        OpenAPI_list_free(alt_report_event_triggerList);
        alt_report_event_triggerList = NULL;
    }
    return NULL;
}

OpenAPI_uav_altitude_reporting_config_t *OpenAPI_uav_altitude_reporting_config_copy(OpenAPI_uav_altitude_reporting_config_t *dst, OpenAPI_uav_altitude_reporting_config_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_uav_altitude_reporting_config_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_uav_altitude_reporting_config_convertToJSON() failed");
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

    OpenAPI_uav_altitude_reporting_config_free(dst);
    dst = OpenAPI_uav_altitude_reporting_config_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

