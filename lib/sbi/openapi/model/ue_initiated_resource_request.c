
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_initiated_resource_request.h"

OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_create(
    char *pcc_rule_id,
    OpenAPI_rule_operation_e rule_op,
    bool is_precedence,
    int precedence,
    OpenAPI_list_t *pack_filt_info,
    OpenAPI_requested_qos_t *req_qos
)
{
    OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request_local_var = ogs_malloc(sizeof(OpenAPI_ue_initiated_resource_request_t));
    ogs_assert(ue_initiated_resource_request_local_var);

    ue_initiated_resource_request_local_var->pcc_rule_id = pcc_rule_id;
    ue_initiated_resource_request_local_var->rule_op = rule_op;
    ue_initiated_resource_request_local_var->is_precedence = is_precedence;
    ue_initiated_resource_request_local_var->precedence = precedence;
    ue_initiated_resource_request_local_var->pack_filt_info = pack_filt_info;
    ue_initiated_resource_request_local_var->req_qos = req_qos;

    return ue_initiated_resource_request_local_var;
}

void OpenAPI_ue_initiated_resource_request_free(OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_initiated_resource_request) {
        return;
    }
    if (ue_initiated_resource_request->pcc_rule_id) {
        ogs_free(ue_initiated_resource_request->pcc_rule_id);
        ue_initiated_resource_request->pcc_rule_id = NULL;
    }
    if (ue_initiated_resource_request->pack_filt_info) {
        OpenAPI_list_for_each(ue_initiated_resource_request->pack_filt_info, node) {
            OpenAPI_packet_filter_info_free(node->data);
        }
        OpenAPI_list_free(ue_initiated_resource_request->pack_filt_info);
        ue_initiated_resource_request->pack_filt_info = NULL;
    }
    if (ue_initiated_resource_request->req_qos) {
        OpenAPI_requested_qos_free(ue_initiated_resource_request->req_qos);
        ue_initiated_resource_request->req_qos = NULL;
    }
    ogs_free(ue_initiated_resource_request);
}

cJSON *OpenAPI_ue_initiated_resource_request_convertToJSON(OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_initiated_resource_request == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [UeInitiatedResourceRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_initiated_resource_request->pcc_rule_id) {
    if (cJSON_AddStringToObject(item, "pccRuleId", ue_initiated_resource_request->pcc_rule_id) == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [pcc_rule_id]");
        goto end;
    }
    }

    if (ue_initiated_resource_request->rule_op == OpenAPI_rule_operation_NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [rule_op]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ruleOp", OpenAPI_rule_operation_ToString(ue_initiated_resource_request->rule_op)) == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [rule_op]");
        goto end;
    }

    if (ue_initiated_resource_request->is_precedence) {
    if (cJSON_AddNumberToObject(item, "precedence", ue_initiated_resource_request->precedence) == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [precedence]");
        goto end;
    }
    }

    if (!ue_initiated_resource_request->pack_filt_info) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [pack_filt_info]");
        return NULL;
    }
    cJSON *pack_filt_infoList = cJSON_AddArrayToObject(item, "packFiltInfo");
    if (pack_filt_infoList == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [pack_filt_info]");
        goto end;
    }
    OpenAPI_list_for_each(ue_initiated_resource_request->pack_filt_info, node) {
        cJSON *itemLocal = OpenAPI_packet_filter_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [pack_filt_info]");
            goto end;
        }
        cJSON_AddItemToArray(pack_filt_infoList, itemLocal);
    }

    if (ue_initiated_resource_request->req_qos) {
    cJSON *req_qos_local_JSON = OpenAPI_requested_qos_convertToJSON(ue_initiated_resource_request->req_qos);
    if (req_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [req_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reqQos", req_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed [req_qos]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_parseFromJSON(cJSON *ue_initiated_resource_requestJSON)
{
    OpenAPI_ue_initiated_resource_request_t *ue_initiated_resource_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pcc_rule_id = NULL;
    cJSON *rule_op = NULL;
    OpenAPI_rule_operation_e rule_opVariable = 0;
    cJSON *precedence = NULL;
    cJSON *pack_filt_info = NULL;
    OpenAPI_list_t *pack_filt_infoList = NULL;
    cJSON *req_qos = NULL;
    OpenAPI_requested_qos_t *req_qos_local_nonprim = NULL;
    pcc_rule_id = cJSON_GetObjectItemCaseSensitive(ue_initiated_resource_requestJSON, "pccRuleId");
    if (pcc_rule_id) {
    if (!cJSON_IsString(pcc_rule_id) && !cJSON_IsNull(pcc_rule_id)) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [pcc_rule_id]");
        goto end;
    }
    }

    rule_op = cJSON_GetObjectItemCaseSensitive(ue_initiated_resource_requestJSON, "ruleOp");
    if (!rule_op) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [rule_op]");
        goto end;
    }
    if (!cJSON_IsString(rule_op)) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [rule_op]");
        goto end;
    }
    rule_opVariable = OpenAPI_rule_operation_FromString(rule_op->valuestring);

    precedence = cJSON_GetObjectItemCaseSensitive(ue_initiated_resource_requestJSON, "precedence");
    if (precedence) {
    if (!cJSON_IsNumber(precedence)) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [precedence]");
        goto end;
    }
    }

    pack_filt_info = cJSON_GetObjectItemCaseSensitive(ue_initiated_resource_requestJSON, "packFiltInfo");
    if (!pack_filt_info) {
        ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [pack_filt_info]");
        goto end;
    }
        cJSON *pack_filt_info_local = NULL;
        if (!cJSON_IsArray(pack_filt_info)) {
            ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [pack_filt_info]");
            goto end;
        }

        pack_filt_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pack_filt_info_local, pack_filt_info) {
            if (!cJSON_IsObject(pack_filt_info_local)) {
                ogs_error("OpenAPI_ue_initiated_resource_request_parseFromJSON() failed [pack_filt_info]");
                goto end;
            }
            OpenAPI_packet_filter_info_t *pack_filt_infoItem = OpenAPI_packet_filter_info_parseFromJSON(pack_filt_info_local);
            if (!pack_filt_infoItem) {
                ogs_error("No pack_filt_infoItem");
                goto end;
            }
            OpenAPI_list_add(pack_filt_infoList, pack_filt_infoItem);
        }

    req_qos = cJSON_GetObjectItemCaseSensitive(ue_initiated_resource_requestJSON, "reqQos");
    if (req_qos) {
    req_qos_local_nonprim = OpenAPI_requested_qos_parseFromJSON(req_qos);
    if (!req_qos_local_nonprim) {
        ogs_error("OpenAPI_requested_qos_parseFromJSON failed [req_qos]");
        goto end;
    }
    }

    ue_initiated_resource_request_local_var = OpenAPI_ue_initiated_resource_request_create (
        pcc_rule_id && !cJSON_IsNull(pcc_rule_id) ? ogs_strdup(pcc_rule_id->valuestring) : NULL,
        rule_opVariable,
        precedence ? true : false,
        precedence ? precedence->valuedouble : 0,
        pack_filt_infoList,
        req_qos ? req_qos_local_nonprim : NULL
    );

    return ue_initiated_resource_request_local_var;
end:
    if (pack_filt_infoList) {
        OpenAPI_list_for_each(pack_filt_infoList, node) {
            OpenAPI_packet_filter_info_free(node->data);
        }
        OpenAPI_list_free(pack_filt_infoList);
        pack_filt_infoList = NULL;
    }
    if (req_qos_local_nonprim) {
        OpenAPI_requested_qos_free(req_qos_local_nonprim);
        req_qos_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_initiated_resource_request_t *OpenAPI_ue_initiated_resource_request_copy(OpenAPI_ue_initiated_resource_request_t *dst, OpenAPI_ue_initiated_resource_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_initiated_resource_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_initiated_resource_request_convertToJSON() failed");
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

    OpenAPI_ue_initiated_resource_request_free(dst);
    dst = OpenAPI_ue_initiated_resource_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

