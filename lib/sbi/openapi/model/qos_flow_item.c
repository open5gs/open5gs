
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_item.h"

OpenAPI_qos_flow_item_t *OpenAPI_qos_flow_item_create(
    int qfi,
    OpenAPI_cause_t *cause
    )
{
    OpenAPI_qos_flow_item_t *qos_flow_item_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_flow_item_t));
    if (!qos_flow_item_local_var) {
        return NULL;
    }
    qos_flow_item_local_var->qfi = qfi;
    qos_flow_item_local_var->cause = cause;

    return qos_flow_item_local_var;
}

void OpenAPI_qos_flow_item_free(OpenAPI_qos_flow_item_t *qos_flow_item)
{
    if (NULL == qos_flow_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_cause_free(qos_flow_item->cause);
    ogs_free(qos_flow_item);
}

cJSON *OpenAPI_qos_flow_item_convertToJSON(OpenAPI_qos_flow_item_t *qos_flow_item)
{
    cJSON *item = NULL;

    if (qos_flow_item == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [QosFlowItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!qos_flow_item->qfi) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [qfi]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_item->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [qfi]");
        goto end;
    }

    if (qos_flow_item->cause) {
        cJSON *cause_local_JSON = OpenAPI_cause_convertToJSON(qos_flow_item->cause);
        if (cause_local_JSON == NULL) {
            ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [cause]");
            goto end;
        }
        cJSON_AddItemToObject(item, "cause", cause_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [cause]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_qos_flow_item_t *OpenAPI_qos_flow_item_parseFromJSON(cJSON *qos_flow_itemJSON)
{
    OpenAPI_qos_flow_item_t *qos_flow_item_local_var = NULL;
    cJSON *qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [qfi]");
        goto end;
    }


    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "cause");

    OpenAPI_cause_t *cause_local_nonprim = NULL;
    if (cause) {
        cause_local_nonprim = OpenAPI_cause_parseFromJSON(cause);
    }

    qos_flow_item_local_var = OpenAPI_qos_flow_item_create (
        qfi->valuedouble,
        cause ? cause_local_nonprim : NULL
        );

    return qos_flow_item_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_item_t *OpenAPI_qos_flow_item_copy(OpenAPI_qos_flow_item_t *dst, OpenAPI_qos_flow_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed");
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

    OpenAPI_qos_flow_item_free(dst);
    dst = OpenAPI_qos_flow_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

