
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "monitoring_configuration.h"

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_create(
    OpenAPI_event_type_t *event_type,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration,
    OpenAPI_association_type_t *association_type,
    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg
    )
{
    OpenAPI_monitoring_configuration_t *monitoring_configuration_local_var = OpenAPI_malloc(sizeof(OpenAPI_monitoring_configuration_t));
    if (!monitoring_configuration_local_var) {
        return NULL;
    }
    monitoring_configuration_local_var->event_type = event_type;
    monitoring_configuration_local_var->immediate_flag = immediate_flag;
    monitoring_configuration_local_var->location_reporting_configuration = location_reporting_configuration;
    monitoring_configuration_local_var->association_type = association_type;
    monitoring_configuration_local_var->datalink_report_cfg = datalink_report_cfg;

    return monitoring_configuration_local_var;
}

void OpenAPI_monitoring_configuration_free(OpenAPI_monitoring_configuration_t *monitoring_configuration)
{
    if (NULL == monitoring_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_event_type_free(monitoring_configuration->event_type);
    OpenAPI_location_reporting_configuration_free(monitoring_configuration->location_reporting_configuration);
    OpenAPI_association_type_free(monitoring_configuration->association_type);
    OpenAPI_datalink_reporting_configuration_free(monitoring_configuration->datalink_report_cfg);
    ogs_free(monitoring_configuration);
}

cJSON *OpenAPI_monitoring_configuration_convertToJSON(OpenAPI_monitoring_configuration_t *monitoring_configuration)
{
    cJSON *item = NULL;

    if (monitoring_configuration == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [MonitoringConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!monitoring_configuration->event_type) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [event_type]");
        goto end;
    }
    cJSON *event_type_local_JSON = OpenAPI_event_type_convertToJSON(monitoring_configuration->event_type);
    if (event_type_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [event_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventType", event_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [event_type]");
        goto end;
    }

    if (monitoring_configuration->immediate_flag) {
        if (cJSON_AddBoolToObject(item, "immediateFlag", monitoring_configuration->immediate_flag) == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [immediate_flag]");
            goto end;
        }
    }

    if (monitoring_configuration->location_reporting_configuration) {
        cJSON *location_reporting_configuration_local_JSON = OpenAPI_location_reporting_configuration_convertToJSON(monitoring_configuration->location_reporting_configuration);
        if (location_reporting_configuration_local_JSON == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [location_reporting_configuration]");
            goto end;
        }
        cJSON_AddItemToObject(item, "locationReportingConfiguration", location_reporting_configuration_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [location_reporting_configuration]");
            goto end;
        }
    }

    if (monitoring_configuration->association_type) {
        cJSON *association_type_local_JSON = OpenAPI_association_type_convertToJSON(monitoring_configuration->association_type);
        if (association_type_local_JSON == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [association_type]");
            goto end;
        }
        cJSON_AddItemToObject(item, "associationType", association_type_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [association_type]");
            goto end;
        }
    }

    if (monitoring_configuration->datalink_report_cfg) {
        cJSON *datalink_report_cfg_local_JSON = OpenAPI_datalink_reporting_configuration_convertToJSON(monitoring_configuration->datalink_report_cfg);
        if (datalink_report_cfg_local_JSON == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [datalink_report_cfg]");
            goto end;
        }
        cJSON_AddItemToObject(item, "datalinkReportCfg", datalink_report_cfg_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [datalink_report_cfg]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_parseFromJSON(cJSON *monitoring_configurationJSON)
{
    OpenAPI_monitoring_configuration_t *monitoring_configuration_local_var = NULL;
    cJSON *event_type = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "eventType");
    if (!event_type) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [event_type]");
        goto end;
    }

    OpenAPI_event_type_t *event_type_local_nonprim = NULL;

    event_type_local_nonprim = OpenAPI_event_type_parseFromJSON(event_type);

    cJSON *immediate_flag = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "immediateFlag");

    if (immediate_flag) {
        if (!cJSON_IsBool(immediate_flag)) {
            ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [immediate_flag]");
            goto end;
        }
    }

    cJSON *location_reporting_configuration = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "locationReportingConfiguration");

    OpenAPI_location_reporting_configuration_t *location_reporting_configuration_local_nonprim = NULL;
    if (location_reporting_configuration) {
        location_reporting_configuration_local_nonprim = OpenAPI_location_reporting_configuration_parseFromJSON(location_reporting_configuration);
    }

    cJSON *association_type = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "associationType");

    OpenAPI_association_type_t *association_type_local_nonprim = NULL;
    if (association_type) {
        association_type_local_nonprim = OpenAPI_association_type_parseFromJSON(association_type);
    }

    cJSON *datalink_report_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "datalinkReportCfg");

    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg_local_nonprim = NULL;
    if (datalink_report_cfg) {
        datalink_report_cfg_local_nonprim = OpenAPI_datalink_reporting_configuration_parseFromJSON(datalink_report_cfg);
    }

    monitoring_configuration_local_var = OpenAPI_monitoring_configuration_create (
        event_type_local_nonprim,
        immediate_flag ? immediate_flag->valueint : 0,
        location_reporting_configuration ? location_reporting_configuration_local_nonprim : NULL,
        association_type ? association_type_local_nonprim : NULL,
        datalink_report_cfg ? datalink_report_cfg_local_nonprim : NULL
        );

    return monitoring_configuration_local_var;
end:
    return NULL;
}

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_copy(OpenAPI_monitoring_configuration_t *dst, OpenAPI_monitoring_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_monitoring_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed");
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

    OpenAPI_monitoring_configuration_free(dst);
    dst = OpenAPI_monitoring_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

