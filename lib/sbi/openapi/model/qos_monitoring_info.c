
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_monitoring_info.h"

OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_create(
    bool is_qos_monitoring_ind,
    int qos_monitoring_ind
)
{
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info_local_var = ogs_malloc(sizeof(OpenAPI_qos_monitoring_info_t));
    ogs_assert(qos_monitoring_info_local_var);

    qos_monitoring_info_local_var->is_qos_monitoring_ind = is_qos_monitoring_ind;
    qos_monitoring_info_local_var->qos_monitoring_ind = qos_monitoring_ind;

    return qos_monitoring_info_local_var;
}

void OpenAPI_qos_monitoring_info_free(OpenAPI_qos_monitoring_info_t *qos_monitoring_info)
{
    if (NULL == qos_monitoring_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(qos_monitoring_info);
}

cJSON *OpenAPI_qos_monitoring_info_convertToJSON(OpenAPI_qos_monitoring_info_t *qos_monitoring_info)
{
    cJSON *item = NULL;

    if (qos_monitoring_info == NULL) {
        ogs_error("OpenAPI_qos_monitoring_info_convertToJSON() failed [QosMonitoringInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_monitoring_info->is_qos_monitoring_ind) {
    if (cJSON_AddBoolToObject(item, "qosMonitoringInd", qos_monitoring_info->qos_monitoring_ind) == NULL) {
        ogs_error("OpenAPI_qos_monitoring_info_convertToJSON() failed [qos_monitoring_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_parseFromJSON(cJSON *qos_monitoring_infoJSON)
{
    OpenAPI_qos_monitoring_info_t *qos_monitoring_info_local_var = NULL;
    cJSON *qos_monitoring_ind = cJSON_GetObjectItemCaseSensitive(qos_monitoring_infoJSON, "qosMonitoringInd");

    if (qos_monitoring_ind) {
    if (!cJSON_IsBool(qos_monitoring_ind)) {
        ogs_error("OpenAPI_qos_monitoring_info_parseFromJSON() failed [qos_monitoring_ind]");
        goto end;
    }
    }

    qos_monitoring_info_local_var = OpenAPI_qos_monitoring_info_create (
        qos_monitoring_ind ? true : false,
        qos_monitoring_ind ? qos_monitoring_ind->valueint : 0
    );

    return qos_monitoring_info_local_var;
end:
    return NULL;
}

OpenAPI_qos_monitoring_info_t *OpenAPI_qos_monitoring_info_copy(OpenAPI_qos_monitoring_info_t *dst, OpenAPI_qos_monitoring_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_monitoring_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_monitoring_info_convertToJSON() failed");
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

    OpenAPI_qos_monitoring_info_free(dst);
    dst = OpenAPI_qos_monitoring_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

