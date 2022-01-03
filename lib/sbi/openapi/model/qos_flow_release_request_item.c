
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_release_request_item.h"

OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_create(
    int qfi,
    char qos_rules,
    char qos_flow_description
)
{
    OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_release_request_item_t));
    ogs_assert(qos_flow_release_request_item_local_var);

    qos_flow_release_request_item_local_var->qfi = qfi;
    qos_flow_release_request_item_local_var->qos_rules = qos_rules;
    qos_flow_release_request_item_local_var->qos_flow_description = qos_flow_description;

    return qos_flow_release_request_item_local_var;
}

void OpenAPI_qos_flow_release_request_item_free(OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item)
{
    if (NULL == qos_flow_release_request_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(qos_flow_release_request_item);
}

cJSON *OpenAPI_qos_flow_release_request_item_convertToJSON(OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item)
{
    cJSON *item = NULL;

    if (qos_flow_release_request_item == NULL) {
        ogs_error("OpenAPI_qos_flow_release_request_item_convertToJSON() failed [QosFlowReleaseRequestItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_release_request_item->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_release_request_item_convertToJSON() failed [qfi]");
        goto end;
    }

    if (qos_flow_release_request_item->qos_rules) {
    if (cJSON_AddNumberToObject(item, "qosRules", qos_flow_release_request_item->qos_rules) == NULL) {
        ogs_error("OpenAPI_qos_flow_release_request_item_convertToJSON() failed [qos_rules]");
        goto end;
    }
    }

    if (qos_flow_release_request_item->qos_flow_description) {
    if (cJSON_AddNumberToObject(item, "qosFlowDescription", qos_flow_release_request_item->qos_flow_description) == NULL) {
        ogs_error("OpenAPI_qos_flow_release_request_item_convertToJSON() failed [qos_flow_description]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_parseFromJSON(cJSON *qos_flow_release_request_itemJSON)
{
    OpenAPI_qos_flow_release_request_item_t *qos_flow_release_request_item_local_var = NULL;
    cJSON *qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_release_request_itemJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_release_request_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_release_request_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    cJSON *qos_rules = cJSON_GetObjectItemCaseSensitive(qos_flow_release_request_itemJSON, "qosRules");

    if (qos_rules) {
    if (!cJSON_IsNumber(qos_rules)) {
        ogs_error("OpenAPI_qos_flow_release_request_item_parseFromJSON() failed [qos_rules]");
        goto end;
    }
    }

    cJSON *qos_flow_description = cJSON_GetObjectItemCaseSensitive(qos_flow_release_request_itemJSON, "qosFlowDescription");

    if (qos_flow_description) {
    if (!cJSON_IsNumber(qos_flow_description)) {
        ogs_error("OpenAPI_qos_flow_release_request_item_parseFromJSON() failed [qos_flow_description]");
        goto end;
    }
    }

    qos_flow_release_request_item_local_var = OpenAPI_qos_flow_release_request_item_create (
        
        qfi->valuedouble,
        qos_rules ? qos_rules->valueint : 0,
        qos_flow_description ? qos_flow_description->valueint : 0
    );

    return qos_flow_release_request_item_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_release_request_item_t *OpenAPI_qos_flow_release_request_item_copy(OpenAPI_qos_flow_release_request_item_t *dst, OpenAPI_qos_flow_release_request_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_release_request_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_release_request_item_convertToJSON() failed");
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

    OpenAPI_qos_flow_release_request_item_free(dst);
    dst = OpenAPI_qos_flow_release_request_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

