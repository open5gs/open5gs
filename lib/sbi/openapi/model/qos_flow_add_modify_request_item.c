
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_add_modify_request_item.h"

OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_create(
    int qfi,
    bool is_ebi,
    int ebi,
    char *qos_rules,
    char *protoc_desc,
    char *qos_flow_description,
    OpenAPI_qos_flow_profile_t *qos_flow_profile,
    OpenAPI_qos_flow_access_type_e associated_an_type,
    OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congest_info_req,
    OpenAPI_tsc_assistance_information_t *tscai_ul,
    OpenAPI_tsc_assistance_information_t *tscai_dl,
    bool is_transp_level_mark_ind,
    int transp_level_mark_ind
)
{
    OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_add_modify_request_item_t));
    ogs_assert(qos_flow_add_modify_request_item_local_var);

    qos_flow_add_modify_request_item_local_var->qfi = qfi;
    qos_flow_add_modify_request_item_local_var->is_ebi = is_ebi;
    qos_flow_add_modify_request_item_local_var->ebi = ebi;
    qos_flow_add_modify_request_item_local_var->qos_rules = qos_rules;
    qos_flow_add_modify_request_item_local_var->protoc_desc = protoc_desc;
    qos_flow_add_modify_request_item_local_var->qos_flow_description = qos_flow_description;
    qos_flow_add_modify_request_item_local_var->qos_flow_profile = qos_flow_profile;
    qos_flow_add_modify_request_item_local_var->associated_an_type = associated_an_type;
    qos_flow_add_modify_request_item_local_var->ecn_marking_congest_info_req = ecn_marking_congest_info_req;
    qos_flow_add_modify_request_item_local_var->tscai_ul = tscai_ul;
    qos_flow_add_modify_request_item_local_var->tscai_dl = tscai_dl;
    qos_flow_add_modify_request_item_local_var->is_transp_level_mark_ind = is_transp_level_mark_ind;
    qos_flow_add_modify_request_item_local_var->transp_level_mark_ind = transp_level_mark_ind;

    return qos_flow_add_modify_request_item_local_var;
}

void OpenAPI_qos_flow_add_modify_request_item_free(OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_flow_add_modify_request_item) {
        return;
    }
    if (qos_flow_add_modify_request_item->qos_rules) {
        ogs_free(qos_flow_add_modify_request_item->qos_rules);
        qos_flow_add_modify_request_item->qos_rules = NULL;
    }
    if (qos_flow_add_modify_request_item->protoc_desc) {
        ogs_free(qos_flow_add_modify_request_item->protoc_desc);
        qos_flow_add_modify_request_item->protoc_desc = NULL;
    }
    if (qos_flow_add_modify_request_item->qos_flow_description) {
        ogs_free(qos_flow_add_modify_request_item->qos_flow_description);
        qos_flow_add_modify_request_item->qos_flow_description = NULL;
    }
    if (qos_flow_add_modify_request_item->qos_flow_profile) {
        OpenAPI_qos_flow_profile_free(qos_flow_add_modify_request_item->qos_flow_profile);
        qos_flow_add_modify_request_item->qos_flow_profile = NULL;
    }
    if (qos_flow_add_modify_request_item->ecn_marking_congest_info_req) {
        OpenAPI_ecn_marking_congestion_info_req_free(qos_flow_add_modify_request_item->ecn_marking_congest_info_req);
        qos_flow_add_modify_request_item->ecn_marking_congest_info_req = NULL;
    }
    if (qos_flow_add_modify_request_item->tscai_ul) {
        OpenAPI_tsc_assistance_information_free(qos_flow_add_modify_request_item->tscai_ul);
        qos_flow_add_modify_request_item->tscai_ul = NULL;
    }
    if (qos_flow_add_modify_request_item->tscai_dl) {
        OpenAPI_tsc_assistance_information_free(qos_flow_add_modify_request_item->tscai_dl);
        qos_flow_add_modify_request_item->tscai_dl = NULL;
    }
    ogs_free(qos_flow_add_modify_request_item);
}

cJSON *OpenAPI_qos_flow_add_modify_request_item_convertToJSON(OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_flow_add_modify_request_item == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [QosFlowAddModifyRequestItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "qfi", qos_flow_add_modify_request_item->qfi) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [qfi]");
        goto end;
    }

    if (qos_flow_add_modify_request_item->is_ebi) {
    if (cJSON_AddNumberToObject(item, "ebi", qos_flow_add_modify_request_item->ebi) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [ebi]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->qos_rules) {
    if (cJSON_AddStringToObject(item, "qosRules", qos_flow_add_modify_request_item->qos_rules) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [qos_rules]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->protoc_desc) {
    if (cJSON_AddStringToObject(item, "protocDesc", qos_flow_add_modify_request_item->protoc_desc) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [protoc_desc]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->qos_flow_description) {
    if (cJSON_AddStringToObject(item, "qosFlowDescription", qos_flow_add_modify_request_item->qos_flow_description) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [qos_flow_description]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->qos_flow_profile) {
    cJSON *qos_flow_profile_local_JSON = OpenAPI_qos_flow_profile_convertToJSON(qos_flow_add_modify_request_item->qos_flow_profile);
    if (qos_flow_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [qos_flow_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosFlowProfile", qos_flow_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [qos_flow_profile]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->associated_an_type != OpenAPI_qos_flow_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "associatedAnType", OpenAPI_qos_flow_access_type_ToString(qos_flow_add_modify_request_item->associated_an_type)) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [associated_an_type]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->ecn_marking_congest_info_req) {
    cJSON *ecn_marking_congest_info_req_local_JSON = OpenAPI_ecn_marking_congestion_info_req_convertToJSON(qos_flow_add_modify_request_item->ecn_marking_congest_info_req);
    if (ecn_marking_congest_info_req_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [ecn_marking_congest_info_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecnMarkingCongestInfoReq", ecn_marking_congest_info_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [ecn_marking_congest_info_req]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->tscai_ul) {
    cJSON *tscai_ul_local_JSON = OpenAPI_tsc_assistance_information_convertToJSON(qos_flow_add_modify_request_item->tscai_ul);
    if (tscai_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [tscai_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiUl", tscai_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [tscai_ul]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->tscai_dl) {
    cJSON *tscai_dl_local_JSON = OpenAPI_tsc_assistance_information_convertToJSON(qos_flow_add_modify_request_item->tscai_dl);
    if (tscai_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [tscai_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tscaiDl", tscai_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [tscai_dl]");
        goto end;
    }
    }

    if (qos_flow_add_modify_request_item->is_transp_level_mark_ind) {
    if (cJSON_AddBoolToObject(item, "transpLevelMarkInd", qos_flow_add_modify_request_item->transp_level_mark_ind) == NULL) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed [transp_level_mark_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_parseFromJSON(cJSON *qos_flow_add_modify_request_itemJSON)
{
    OpenAPI_qos_flow_add_modify_request_item_t *qos_flow_add_modify_request_item_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qfi = NULL;
    cJSON *ebi = NULL;
    cJSON *qos_rules = NULL;
    cJSON *protoc_desc = NULL;
    cJSON *qos_flow_description = NULL;
    cJSON *qos_flow_profile = NULL;
    OpenAPI_qos_flow_profile_t *qos_flow_profile_local_nonprim = NULL;
    cJSON *associated_an_type = NULL;
    OpenAPI_qos_flow_access_type_e associated_an_typeVariable = 0;
    cJSON *ecn_marking_congest_info_req = NULL;
    OpenAPI_ecn_marking_congestion_info_req_t *ecn_marking_congest_info_req_local_nonprim = NULL;
    cJSON *tscai_ul = NULL;
    OpenAPI_tsc_assistance_information_t *tscai_ul_local_nonprim = NULL;
    cJSON *tscai_dl = NULL;
    OpenAPI_tsc_assistance_information_t *tscai_dl_local_nonprim = NULL;
    cJSON *transp_level_mark_ind = NULL;
    qfi = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "qfi");
    if (!qfi) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [qfi]");
        goto end;
    }
    if (!cJSON_IsNumber(qfi)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [qfi]");
        goto end;
    }

    ebi = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "ebi");
    if (ebi) {
    if (!cJSON_IsNumber(ebi)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [ebi]");
        goto end;
    }
    }

    qos_rules = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "qosRules");
    if (qos_rules) {
    if (!cJSON_IsString(qos_rules) && !cJSON_IsNull(qos_rules)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [qos_rules]");
        goto end;
    }
    }

    protoc_desc = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "protocDesc");
    if (protoc_desc) {
    if (!cJSON_IsString(protoc_desc) && !cJSON_IsNull(protoc_desc)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [protoc_desc]");
        goto end;
    }
    }

    qos_flow_description = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "qosFlowDescription");
    if (qos_flow_description) {
    if (!cJSON_IsString(qos_flow_description) && !cJSON_IsNull(qos_flow_description)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [qos_flow_description]");
        goto end;
    }
    }

    qos_flow_profile = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "qosFlowProfile");
    if (qos_flow_profile) {
    qos_flow_profile_local_nonprim = OpenAPI_qos_flow_profile_parseFromJSON(qos_flow_profile);
    if (!qos_flow_profile_local_nonprim) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON failed [qos_flow_profile]");
        goto end;
    }
    }

    associated_an_type = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "associatedAnType");
    if (associated_an_type) {
    if (!cJSON_IsString(associated_an_type)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [associated_an_type]");
        goto end;
    }
    associated_an_typeVariable = OpenAPI_qos_flow_access_type_FromString(associated_an_type->valuestring);
    }

    ecn_marking_congest_info_req = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "ecnMarkingCongestInfoReq");
    if (ecn_marking_congest_info_req) {
    ecn_marking_congest_info_req_local_nonprim = OpenAPI_ecn_marking_congestion_info_req_parseFromJSON(ecn_marking_congest_info_req);
    if (!ecn_marking_congest_info_req_local_nonprim) {
        ogs_error("OpenAPI_ecn_marking_congestion_info_req_parseFromJSON failed [ecn_marking_congest_info_req]");
        goto end;
    }
    }

    tscai_ul = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "tscaiUl");
    if (tscai_ul) {
    tscai_ul_local_nonprim = OpenAPI_tsc_assistance_information_parseFromJSON(tscai_ul);
    if (!tscai_ul_local_nonprim) {
        ogs_error("OpenAPI_tsc_assistance_information_parseFromJSON failed [tscai_ul]");
        goto end;
    }
    }

    tscai_dl = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "tscaiDl");
    if (tscai_dl) {
    tscai_dl_local_nonprim = OpenAPI_tsc_assistance_information_parseFromJSON(tscai_dl);
    if (!tscai_dl_local_nonprim) {
        ogs_error("OpenAPI_tsc_assistance_information_parseFromJSON failed [tscai_dl]");
        goto end;
    }
    }

    transp_level_mark_ind = cJSON_GetObjectItemCaseSensitive(qos_flow_add_modify_request_itemJSON, "transpLevelMarkInd");
    if (transp_level_mark_ind) {
    if (!cJSON_IsBool(transp_level_mark_ind)) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_parseFromJSON() failed [transp_level_mark_ind]");
        goto end;
    }
    }

    qos_flow_add_modify_request_item_local_var = OpenAPI_qos_flow_add_modify_request_item_create (
        
        qfi->valuedouble,
        ebi ? true : false,
        ebi ? ebi->valuedouble : 0,
        qos_rules && !cJSON_IsNull(qos_rules) ? ogs_strdup(qos_rules->valuestring) : NULL,
        protoc_desc && !cJSON_IsNull(protoc_desc) ? ogs_strdup(protoc_desc->valuestring) : NULL,
        qos_flow_description && !cJSON_IsNull(qos_flow_description) ? ogs_strdup(qos_flow_description->valuestring) : NULL,
        qos_flow_profile ? qos_flow_profile_local_nonprim : NULL,
        associated_an_type ? associated_an_typeVariable : 0,
        ecn_marking_congest_info_req ? ecn_marking_congest_info_req_local_nonprim : NULL,
        tscai_ul ? tscai_ul_local_nonprim : NULL,
        tscai_dl ? tscai_dl_local_nonprim : NULL,
        transp_level_mark_ind ? true : false,
        transp_level_mark_ind ? transp_level_mark_ind->valueint : 0
    );

    return qos_flow_add_modify_request_item_local_var;
end:
    if (qos_flow_profile_local_nonprim) {
        OpenAPI_qos_flow_profile_free(qos_flow_profile_local_nonprim);
        qos_flow_profile_local_nonprim = NULL;
    }
    if (ecn_marking_congest_info_req_local_nonprim) {
        OpenAPI_ecn_marking_congestion_info_req_free(ecn_marking_congest_info_req_local_nonprim);
        ecn_marking_congest_info_req_local_nonprim = NULL;
    }
    if (tscai_ul_local_nonprim) {
        OpenAPI_tsc_assistance_information_free(tscai_ul_local_nonprim);
        tscai_ul_local_nonprim = NULL;
    }
    if (tscai_dl_local_nonprim) {
        OpenAPI_tsc_assistance_information_free(tscai_dl_local_nonprim);
        tscai_dl_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_qos_flow_add_modify_request_item_t *OpenAPI_qos_flow_add_modify_request_item_copy(OpenAPI_qos_flow_add_modify_request_item_t *dst, OpenAPI_qos_flow_add_modify_request_item_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_add_modify_request_item_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_add_modify_request_item_convertToJSON() failed");
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

    OpenAPI_qos_flow_add_modify_request_item_free(dst);
    dst = OpenAPI_qos_flow_add_modify_request_item_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

