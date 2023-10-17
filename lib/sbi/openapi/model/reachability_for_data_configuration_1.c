
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_for_data_configuration_1.h"

OpenAPI_reachability_for_data_configuration_1_t *OpenAPI_reachability_for_data_configuration_1_create(
    OpenAPI_reachability_for_data_report_config_t *report_cfg,
    bool is_min_interval,
    int min_interval
)
{
    OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_configuration_1_local_var = ogs_malloc(sizeof(OpenAPI_reachability_for_data_configuration_1_t));
    ogs_assert(reachability_for_data_configuration_1_local_var);

    reachability_for_data_configuration_1_local_var->report_cfg = report_cfg;
    reachability_for_data_configuration_1_local_var->is_min_interval = is_min_interval;
    reachability_for_data_configuration_1_local_var->min_interval = min_interval;

    return reachability_for_data_configuration_1_local_var;
}

void OpenAPI_reachability_for_data_configuration_1_free(OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_configuration_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reachability_for_data_configuration_1) {
        return;
    }
    if (reachability_for_data_configuration_1->report_cfg) {
        OpenAPI_reachability_for_data_report_config_free(reachability_for_data_configuration_1->report_cfg);
        reachability_for_data_configuration_1->report_cfg = NULL;
    }
    ogs_free(reachability_for_data_configuration_1);
}

cJSON *OpenAPI_reachability_for_data_configuration_1_convertToJSON(OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_configuration_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reachability_for_data_configuration_1 == NULL) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed [ReachabilityForDataConfiguration_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!reachability_for_data_configuration_1->report_cfg) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed [report_cfg]");
        return NULL;
    }
    cJSON *report_cfg_local_JSON = OpenAPI_reachability_for_data_report_config_convertToJSON(reachability_for_data_configuration_1->report_cfg);
    if (report_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed [report_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reportCfg", report_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed [report_cfg]");
        goto end;
    }

    if (reachability_for_data_configuration_1->is_min_interval) {
    if (cJSON_AddNumberToObject(item, "minInterval", reachability_for_data_configuration_1->min_interval) == NULL) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed [min_interval]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_reachability_for_data_configuration_1_t *OpenAPI_reachability_for_data_configuration_1_parseFromJSON(cJSON *reachability_for_data_configuration_1JSON)
{
    OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_configuration_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *report_cfg = NULL;
    OpenAPI_reachability_for_data_report_config_t *report_cfg_local_nonprim = NULL;
    cJSON *min_interval = NULL;
    report_cfg = cJSON_GetObjectItemCaseSensitive(reachability_for_data_configuration_1JSON, "reportCfg");
    if (!report_cfg) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_parseFromJSON() failed [report_cfg]");
        goto end;
    }
    report_cfg_local_nonprim = OpenAPI_reachability_for_data_report_config_parseFromJSON(report_cfg);
    if (!report_cfg_local_nonprim) {
        ogs_error("OpenAPI_reachability_for_data_report_config_parseFromJSON failed [report_cfg]");
        goto end;
    }

    min_interval = cJSON_GetObjectItemCaseSensitive(reachability_for_data_configuration_1JSON, "minInterval");
    if (min_interval) {
    if (!cJSON_IsNumber(min_interval)) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_parseFromJSON() failed [min_interval]");
        goto end;
    }
    }

    reachability_for_data_configuration_1_local_var = OpenAPI_reachability_for_data_configuration_1_create (
        report_cfg_local_nonprim,
        min_interval ? true : false,
        min_interval ? min_interval->valuedouble : 0
    );

    return reachability_for_data_configuration_1_local_var;
end:
    if (report_cfg_local_nonprim) {
        OpenAPI_reachability_for_data_report_config_free(report_cfg_local_nonprim);
        report_cfg_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_reachability_for_data_configuration_1_t *OpenAPI_reachability_for_data_configuration_1_copy(OpenAPI_reachability_for_data_configuration_1_t *dst, OpenAPI_reachability_for_data_configuration_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reachability_for_data_configuration_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_convertToJSON() failed");
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

    OpenAPI_reachability_for_data_configuration_1_free(dst);
    dst = OpenAPI_reachability_for_data_configuration_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

