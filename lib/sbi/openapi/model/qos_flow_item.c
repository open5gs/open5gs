
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_item.h"

OpenAPI_qos_flow_item_t *OpenAPI_qos_flow_item_create(
    int qfi,
    OpenAPI_cause_e cause,
    bool is_current_qos_profile_index,
    int current_qos_profile_index,
    bool is_null_qo_s_profile_index,
    int null_qo_s_profile_index,
    OpenAPI_ng_ap_cause_t *ng_ap_cause
)
{
    OpenAPI_qos_flow_item_t *qos_flow_item_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_item_t));
    ogs_assert(qos_flow_item_local_var);

    qos_flow_item_local_var->qfi = qfi;
    qos_flow_item_local_var->cause = cause;
    qos_flow_item_local_var->is_current_qos_profile_index = is_current_qos_profile_index;
    qos_flow_item_local_var->current_qos_profile_index = current_qos_profile_index;
    qos_flow_item_local_var->is_null_qo_s_profile_index = is_null_qo_s_profile_index;
    qos_flow_item_local_var->null_qo_s_profile_index = null_qo_s_profile_index;
    qos_flow_item_local_var->ng_ap_cause = ng_ap_cause;

    return qos_flow_item_local_var;
}

void OpenAPI_qos_flow_item_free(OpenAPI_qos_flow_item_t *qos_flow_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_flow_item) {
        return;
    }
    if (qos_flow_item->ng_ap_cause) {
        OpenAPI_ng_ap_cause_free(qos_flow_item->ng_ap_cause);
        qos_flow_item->ng_ap_cause = NULL;
    }
    ogs_free(qos_flow_item);
}

cJSON *OpenAPI_qos_flow_item_convertToJSON(OpenAPI_qos_flow_item_t *qos_flow_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_flow_item == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [QosFlowItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_item->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [qfi]");
        goto end;
    }

    if (qos_flow_item->cause != OpenAPI_cause_NULL) {
    if (cJSON_AddStringToObject(item, "cause", OpenAPI_cause_ToString(qos_flow_item->cause)) == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (qos_flow_item->is_current_qos_profile_index) {
    if (cJSON_AddNumberToObject(item, "currentQosProfileIndex", qos_flow_item->current_qos_profile_index) == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [current_qos_profile_index]");
        goto end;
    }
    }

    if (qos_flow_item->is_null_qo_s_profile_index) {
    if (cJSON_AddBoolToObject(item, "nullQoSProfileIndex", qos_flow_item->null_qo_s_profile_index) == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [null_qo_s_profile_index]");
        goto end;
    }
    }

    if (qos_flow_item->ng_ap_cause) {
    cJSON *ng_ap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(qos_flow_item->ng_ap_cause);
    if (ng_ap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngApCause", ng_ap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_item_convertToJSON() failed [ng_ap_cause]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flow_item_t *OpenAPI_qos_flow_item_parseFromJSON(cJSON *qos_flow_itemJSON)
{
    OpenAPI_qos_flow_item_t *qos_flow_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qfi = NULL;
    cJSON *cause = NULL;
    OpenAPI_cause_e causeVariable = 0;
    cJSON *current_qos_profile_index = NULL;
    cJSON *null_qo_s_profile_index = NULL;
    cJSON *ng_ap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ng_ap_cause_local_nonprim = NULL;
    qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [qfi]");
        goto end;
    }
    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    cause = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause)) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [cause]");
        goto end;
    }
    causeVariable = OpenAPI_cause_FromString(cause->valuestring);
    }

    current_qos_profile_index = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "currentQosProfileIndex");
    if (current_qos_profile_index) {
    if (!cJSON_IsNumber(current_qos_profile_index)) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [current_qos_profile_index]");
        goto end;
    }
    }

    null_qo_s_profile_index = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "nullQoSProfileIndex");
    if (null_qo_s_profile_index) {
    if (!cJSON_IsBool(null_qo_s_profile_index)) {
        ogs_error("OpenAPI_qos_flow_item_parseFromJSON() failed [null_qo_s_profile_index]");
        goto end;
    }
    }

    ng_ap_cause = cJSON_GetObjectItemCaseSensitive(qos_flow_itemJSON, "ngApCause");
    if (ng_ap_cause) {
    ng_ap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ng_ap_cause);
    if (!ng_ap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ng_ap_cause]");
        goto end;
    }
    }

    qos_flow_item_local_var = OpenAPI_qos_flow_item_create (
        
        qfi->valuedouble,
        cause ? causeVariable : 0,
        current_qos_profile_index ? true : false,
        current_qos_profile_index ? current_qos_profile_index->valuedouble : 0,
        null_qo_s_profile_index ? true : false,
        null_qo_s_profile_index ? null_qo_s_profile_index->valueint : 0,
        ng_ap_cause ? ng_ap_cause_local_nonprim : NULL
    );

    return qos_flow_item_local_var;
end:
    if (ng_ap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ng_ap_cause_local_nonprim);
        ng_ap_cause_local_nonprim = NULL;
    }
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

