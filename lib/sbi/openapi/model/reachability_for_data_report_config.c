
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_for_data_report_config.h"

OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_create(
)
{
    OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config_local_var = ogs_malloc(sizeof(OpenAPI_reachability_for_data_report_config_t));
    ogs_assert(reachability_for_data_report_config_local_var);


    return reachability_for_data_report_config_local_var;
}

void OpenAPI_reachability_for_data_report_config_free(OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == reachability_for_data_report_config) {
        return;
    }
    ogs_free(reachability_for_data_report_config);
}

cJSON *OpenAPI_reachability_for_data_report_config_convertToJSON(OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (reachability_for_data_report_config == NULL) {
        ogs_error("OpenAPI_reachability_for_data_report_config_convertToJSON() failed [ReachabilityForDataReportConfig]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_parseFromJSON(cJSON *reachability_for_data_report_configJSON)
{
    OpenAPI_reachability_for_data_report_config_t *reachability_for_data_report_config_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    reachability_for_data_report_config_local_var = OpenAPI_reachability_for_data_report_config_create (
    );

    return reachability_for_data_report_config_local_var;
end:
    return NULL;
}

OpenAPI_reachability_for_data_report_config_t *OpenAPI_reachability_for_data_report_config_copy(OpenAPI_reachability_for_data_report_config_t *dst, OpenAPI_reachability_for_data_report_config_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reachability_for_data_report_config_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reachability_for_data_report_config_convertToJSON() failed");
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

    OpenAPI_reachability_for_data_report_config_free(dst);
    dst = OpenAPI_reachability_for_data_report_config_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

