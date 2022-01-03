
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_profile.h"

OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_create(
    int _5qi,
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi,
    OpenAPI_dynamic5_qi_t *dynamic5_qi,
    OpenAPI_arp_t *arp,
    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_info,
    OpenAPI_reflective_qo_s_attribute_e rqa,
    OpenAPI_additional_qos_flow_info_e additional_qos_flow_info,
    OpenAPI_qos_monitoring_req_e qos_monitoring_req,
    bool is_qos_rep_period,
    int qos_rep_period
)
{
    OpenAPI_qos_flow_profile_t *qos_flow_profile_local_var = ogs_malloc(sizeof(OpenAPI_qos_flow_profile_t));
    ogs_assert(qos_flow_profile_local_var);

    qos_flow_profile_local_var->_5qi = _5qi;
    qos_flow_profile_local_var->non_dynamic5_qi = non_dynamic5_qi;
    qos_flow_profile_local_var->dynamic5_qi = dynamic5_qi;
    qos_flow_profile_local_var->arp = arp;
    qos_flow_profile_local_var->gbr_qos_flow_info = gbr_qos_flow_info;
    qos_flow_profile_local_var->rqa = rqa;
    qos_flow_profile_local_var->additional_qos_flow_info = additional_qos_flow_info;
    qos_flow_profile_local_var->qos_monitoring_req = qos_monitoring_req;
    qos_flow_profile_local_var->is_qos_rep_period = is_qos_rep_period;
    qos_flow_profile_local_var->qos_rep_period = qos_rep_period;

    return qos_flow_profile_local_var;
}

void OpenAPI_qos_flow_profile_free(OpenAPI_qos_flow_profile_t *qos_flow_profile)
{
    if (NULL == qos_flow_profile) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_non_dynamic5_qi_free(qos_flow_profile->non_dynamic5_qi);
    OpenAPI_dynamic5_qi_free(qos_flow_profile->dynamic5_qi);
    OpenAPI_arp_free(qos_flow_profile->arp);
    OpenAPI_gbr_qos_flow_information_free(qos_flow_profile->gbr_qos_flow_info);
    ogs_free(qos_flow_profile);
}

cJSON *OpenAPI_qos_flow_profile_convertToJSON(OpenAPI_qos_flow_profile_t *qos_flow_profile)
{
    cJSON *item = NULL;

    if (qos_flow_profile == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [QosFlowProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "5qi", qos_flow_profile->_5qi) == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [_5qi]");
        goto end;
    }

    if (qos_flow_profile->non_dynamic5_qi) {
    cJSON *non_dynamic5_qi_local_JSON = OpenAPI_non_dynamic5_qi_convertToJSON(qos_flow_profile->non_dynamic5_qi);
    if (non_dynamic5_qi_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [non_dynamic5_qi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nonDynamic5Qi", non_dynamic5_qi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [non_dynamic5_qi]");
        goto end;
    }
    }

    if (qos_flow_profile->dynamic5_qi) {
    cJSON *dynamic5_qi_local_JSON = OpenAPI_dynamic5_qi_convertToJSON(qos_flow_profile->dynamic5_qi);
    if (dynamic5_qi_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [dynamic5_qi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dynamic5Qi", dynamic5_qi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [dynamic5_qi]");
        goto end;
    }
    }

    if (qos_flow_profile->arp) {
    cJSON *arp_local_JSON = OpenAPI_arp_convertToJSON(qos_flow_profile->arp);
    if (arp_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [arp]");
        goto end;
    }
    cJSON_AddItemToObject(item, "arp", arp_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [arp]");
        goto end;
    }
    }

    if (qos_flow_profile->gbr_qos_flow_info) {
    cJSON *gbr_qos_flow_info_local_JSON = OpenAPI_gbr_qos_flow_information_convertToJSON(qos_flow_profile->gbr_qos_flow_info);
    if (gbr_qos_flow_info_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [gbr_qos_flow_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "gbrQosFlowInfo", gbr_qos_flow_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [gbr_qos_flow_info]");
        goto end;
    }
    }

    if (qos_flow_profile->rqa) {
    if (cJSON_AddStringToObject(item, "rqa", OpenAPI_reflective_qo_s_attribute_ToString(qos_flow_profile->rqa)) == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [rqa]");
        goto end;
    }
    }

    if (qos_flow_profile->additional_qos_flow_info) {
    if (cJSON_AddStringToObject(item, "additionalQosFlowInfo", OpenAPI_additional_qos_flow_info_ToString(qos_flow_profile->additional_qos_flow_info)) == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [additional_qos_flow_info]");
        goto end;
    }
    }

    if (qos_flow_profile->qos_monitoring_req) {
    if (cJSON_AddStringToObject(item, "qosMonitoringReq", OpenAPI_qos_monitoring_req_ToString(qos_flow_profile->qos_monitoring_req)) == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [qos_monitoring_req]");
        goto end;
    }
    }

    if (qos_flow_profile->is_qos_rep_period) {
    if (cJSON_AddNumberToObject(item, "qosRepPeriod", qos_flow_profile->qos_rep_period) == NULL) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed [qos_rep_period]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_parseFromJSON(cJSON *qos_flow_profileJSON)
{
    OpenAPI_qos_flow_profile_t *qos_flow_profile_local_var = NULL;
    cJSON *_5qi = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "5qi");
    if (!_5qi) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [_5qi]");
        goto end;
    }

    if (!cJSON_IsNumber(_5qi)) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [_5qi]");
        goto end;
    }

    cJSON *non_dynamic5_qi = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "nonDynamic5Qi");

    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi_local_nonprim = NULL;
    if (non_dynamic5_qi) {
    non_dynamic5_qi_local_nonprim = OpenAPI_non_dynamic5_qi_parseFromJSON(non_dynamic5_qi);
    }

    cJSON *dynamic5_qi = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "dynamic5Qi");

    OpenAPI_dynamic5_qi_t *dynamic5_qi_local_nonprim = NULL;
    if (dynamic5_qi) {
    dynamic5_qi_local_nonprim = OpenAPI_dynamic5_qi_parseFromJSON(dynamic5_qi);
    }

    cJSON *arp = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "arp");

    OpenAPI_arp_t *arp_local_nonprim = NULL;
    if (arp) {
    arp_local_nonprim = OpenAPI_arp_parseFromJSON(arp);
    }

    cJSON *gbr_qos_flow_info = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "gbrQosFlowInfo");

    OpenAPI_gbr_qos_flow_information_t *gbr_qos_flow_info_local_nonprim = NULL;
    if (gbr_qos_flow_info) {
    gbr_qos_flow_info_local_nonprim = OpenAPI_gbr_qos_flow_information_parseFromJSON(gbr_qos_flow_info);
    }

    cJSON *rqa = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "rqa");

    OpenAPI_reflective_qo_s_attribute_e rqaVariable;
    if (rqa) {
    if (!cJSON_IsString(rqa)) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [rqa]");
        goto end;
    }
    rqaVariable = OpenAPI_reflective_qo_s_attribute_FromString(rqa->valuestring);
    }

    cJSON *additional_qos_flow_info = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "additionalQosFlowInfo");

    OpenAPI_additional_qos_flow_info_e additional_qos_flow_infoVariable;
    if (additional_qos_flow_info) {
    if (!cJSON_IsString(additional_qos_flow_info)) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [additional_qos_flow_info]");
        goto end;
    }
    additional_qos_flow_infoVariable = OpenAPI_additional_qos_flow_info_FromString(additional_qos_flow_info->valuestring);
    }

    cJSON *qos_monitoring_req = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "qosMonitoringReq");

    OpenAPI_qos_monitoring_req_e qos_monitoring_reqVariable;
    if (qos_monitoring_req) {
    if (!cJSON_IsString(qos_monitoring_req)) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [qos_monitoring_req]");
        goto end;
    }
    qos_monitoring_reqVariable = OpenAPI_qos_monitoring_req_FromString(qos_monitoring_req->valuestring);
    }

    cJSON *qos_rep_period = cJSON_GetObjectItemCaseSensitive(qos_flow_profileJSON, "qosRepPeriod");

    if (qos_rep_period) {
    if (!cJSON_IsNumber(qos_rep_period)) {
        ogs_error("OpenAPI_qos_flow_profile_parseFromJSON() failed [qos_rep_period]");
        goto end;
    }
    }

    qos_flow_profile_local_var = OpenAPI_qos_flow_profile_create (
        
        _5qi->valuedouble,
        non_dynamic5_qi ? non_dynamic5_qi_local_nonprim : NULL,
        dynamic5_qi ? dynamic5_qi_local_nonprim : NULL,
        arp ? arp_local_nonprim : NULL,
        gbr_qos_flow_info ? gbr_qos_flow_info_local_nonprim : NULL,
        rqa ? rqaVariable : 0,
        additional_qos_flow_info ? additional_qos_flow_infoVariable : 0,
        qos_monitoring_req ? qos_monitoring_reqVariable : 0,
        qos_rep_period ? true : false,
        qos_rep_period ? qos_rep_period->valuedouble : 0
    );

    return qos_flow_profile_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_profile_t *OpenAPI_qos_flow_profile_copy(OpenAPI_qos_flow_profile_t *dst, OpenAPI_qos_flow_profile_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_profile_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_profile_convertToJSON() failed");
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

    OpenAPI_qos_flow_profile_free(dst);
    dst = OpenAPI_qos_flow_profile_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

