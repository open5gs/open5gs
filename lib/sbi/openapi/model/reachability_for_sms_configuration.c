
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "reachability_for_sms_configuration.h"

OpenAPI_reachability_for_sms_configuration_t *OpenAPI_reachability_for_sms_configuration_create(
)
{
    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_configuration_local_var = ogs_malloc(sizeof(OpenAPI_reachability_for_sms_configuration_t));
    ogs_assert(reachability_for_sms_configuration_local_var);


    return reachability_for_sms_configuration_local_var;
}

void OpenAPI_reachability_for_sms_configuration_free(OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_configuration)
{
    if (NULL == reachability_for_sms_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(reachability_for_sms_configuration);
}

cJSON *OpenAPI_reachability_for_sms_configuration_convertToJSON(OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_configuration)
{
    cJSON *item = NULL;

    if (reachability_for_sms_configuration == NULL) {
        ogs_error("OpenAPI_reachability_for_sms_configuration_convertToJSON() failed [ReachabilityForSmsConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_reachability_for_sms_configuration_t *OpenAPI_reachability_for_sms_configuration_parseFromJSON(cJSON *reachability_for_sms_configurationJSON)
{
    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_configuration_local_var = NULL;
    reachability_for_sms_configuration_local_var = OpenAPI_reachability_for_sms_configuration_create (
    );

    return reachability_for_sms_configuration_local_var;
end:
    return NULL;
}

OpenAPI_reachability_for_sms_configuration_t *OpenAPI_reachability_for_sms_configuration_copy(OpenAPI_reachability_for_sms_configuration_t *dst, OpenAPI_reachability_for_sms_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_reachability_for_sms_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_reachability_for_sms_configuration_convertToJSON() failed");
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

    OpenAPI_reachability_for_sms_configuration_free(dst);
    dst = OpenAPI_reachability_for_sms_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

