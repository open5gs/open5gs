
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_sustainability_info.h"

OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_create(
    OpenAPI_network_area_info_t *area_info,
    OpenAPI_list_t *fine_area_infos,
    char *start_ts,
    char *end_ts,
    OpenAPI_retainability_threshold_t *qos_flow_ret_thd,
    char *ran_ue_throu_thd,
    bool is_e2e_delay_thd,
    int e2e_delay_thd,
    OpenAPI_snssai_t *snssai,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_qos_sustainability_info_t *qos_sustainability_info_local_var = ogs_malloc(sizeof(OpenAPI_qos_sustainability_info_t));
    ogs_assert(qos_sustainability_info_local_var);

    qos_sustainability_info_local_var->area_info = area_info;
    qos_sustainability_info_local_var->fine_area_infos = fine_area_infos;
    qos_sustainability_info_local_var->start_ts = start_ts;
    qos_sustainability_info_local_var->end_ts = end_ts;
    qos_sustainability_info_local_var->qos_flow_ret_thd = qos_flow_ret_thd;
    qos_sustainability_info_local_var->ran_ue_throu_thd = ran_ue_throu_thd;
    qos_sustainability_info_local_var->is_e2e_delay_thd = is_e2e_delay_thd;
    qos_sustainability_info_local_var->e2e_delay_thd = e2e_delay_thd;
    qos_sustainability_info_local_var->snssai = snssai;
    qos_sustainability_info_local_var->is_confidence = is_confidence;
    qos_sustainability_info_local_var->confidence = confidence;

    return qos_sustainability_info_local_var;
}

void OpenAPI_qos_sustainability_info_free(OpenAPI_qos_sustainability_info_t *qos_sustainability_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_sustainability_info) {
        return;
    }
    if (qos_sustainability_info->area_info) {
        OpenAPI_network_area_info_free(qos_sustainability_info->area_info);
        qos_sustainability_info->area_info = NULL;
    }
    if (qos_sustainability_info->fine_area_infos) {
        OpenAPI_list_for_each(qos_sustainability_info->fine_area_infos, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(qos_sustainability_info->fine_area_infos);
        qos_sustainability_info->fine_area_infos = NULL;
    }
    if (qos_sustainability_info->start_ts) {
        ogs_free(qos_sustainability_info->start_ts);
        qos_sustainability_info->start_ts = NULL;
    }
    if (qos_sustainability_info->end_ts) {
        ogs_free(qos_sustainability_info->end_ts);
        qos_sustainability_info->end_ts = NULL;
    }
    if (qos_sustainability_info->qos_flow_ret_thd) {
        OpenAPI_retainability_threshold_free(qos_sustainability_info->qos_flow_ret_thd);
        qos_sustainability_info->qos_flow_ret_thd = NULL;
    }
    if (qos_sustainability_info->ran_ue_throu_thd) {
        ogs_free(qos_sustainability_info->ran_ue_throu_thd);
        qos_sustainability_info->ran_ue_throu_thd = NULL;
    }
    if (qos_sustainability_info->snssai) {
        OpenAPI_snssai_free(qos_sustainability_info->snssai);
        qos_sustainability_info->snssai = NULL;
    }
    ogs_free(qos_sustainability_info);
}

cJSON *OpenAPI_qos_sustainability_info_convertToJSON(OpenAPI_qos_sustainability_info_t *qos_sustainability_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_sustainability_info == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [QosSustainabilityInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_sustainability_info->area_info) {
    cJSON *area_info_local_JSON = OpenAPI_network_area_info_convertToJSON(qos_sustainability_info->area_info);
    if (area_info_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [area_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaInfo", area_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [area_info]");
        goto end;
    }
    }

    if (qos_sustainability_info->fine_area_infos) {
    cJSON *fine_area_infosList = cJSON_AddArrayToObject(item, "fineAreaInfos");
    if (fine_area_infosList == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [fine_area_infos]");
        goto end;
    }
    OpenAPI_list_for_each(qos_sustainability_info->fine_area_infos, node) {
        cJSON *itemLocal = OpenAPI_geographical_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [fine_area_infos]");
            goto end;
        }
        cJSON_AddItemToArray(fine_area_infosList, itemLocal);
    }
    }

    if (qos_sustainability_info->start_ts) {
    if (cJSON_AddStringToObject(item, "startTs", qos_sustainability_info->start_ts) == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [start_ts]");
        goto end;
    }
    }

    if (qos_sustainability_info->end_ts) {
    if (cJSON_AddStringToObject(item, "endTs", qos_sustainability_info->end_ts) == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [end_ts]");
        goto end;
    }
    }

    if (qos_sustainability_info->qos_flow_ret_thd) {
    cJSON *qos_flow_ret_thd_local_JSON = OpenAPI_retainability_threshold_convertToJSON(qos_sustainability_info->qos_flow_ret_thd);
    if (qos_flow_ret_thd_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [qos_flow_ret_thd]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosFlowRetThd", qos_flow_ret_thd_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [qos_flow_ret_thd]");
        goto end;
    }
    }

    if (qos_sustainability_info->ran_ue_throu_thd) {
    if (cJSON_AddStringToObject(item, "ranUeThrouThd", qos_sustainability_info->ran_ue_throu_thd) == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [ran_ue_throu_thd]");
        goto end;
    }
    }

    if (qos_sustainability_info->is_e2e_delay_thd) {
    if (cJSON_AddNumberToObject(item, "e2eDelayThd", qos_sustainability_info->e2e_delay_thd) == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [e2e_delay_thd]");
        goto end;
    }
    }

    if (qos_sustainability_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(qos_sustainability_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (qos_sustainability_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", qos_sustainability_info->confidence) == NULL) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_parseFromJSON(cJSON *qos_sustainability_infoJSON)
{
    OpenAPI_qos_sustainability_info_t *qos_sustainability_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *area_info = NULL;
    OpenAPI_network_area_info_t *area_info_local_nonprim = NULL;
    cJSON *fine_area_infos = NULL;
    OpenAPI_list_t *fine_area_infosList = NULL;
    cJSON *start_ts = NULL;
    cJSON *end_ts = NULL;
    cJSON *qos_flow_ret_thd = NULL;
    OpenAPI_retainability_threshold_t *qos_flow_ret_thd_local_nonprim = NULL;
    cJSON *ran_ue_throu_thd = NULL;
    cJSON *e2e_delay_thd = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *confidence = NULL;
    area_info = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "areaInfo");
    if (area_info) {
    area_info_local_nonprim = OpenAPI_network_area_info_parseFromJSON(area_info);
    if (!area_info_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [area_info]");
        goto end;
    }
    }

    fine_area_infos = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "fineAreaInfos");
    if (fine_area_infos) {
        cJSON *fine_area_infos_local = NULL;
        if (!cJSON_IsArray(fine_area_infos)) {
            ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [fine_area_infos]");
            goto end;
        }

        fine_area_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(fine_area_infos_local, fine_area_infos) {
            if (!cJSON_IsObject(fine_area_infos_local)) {
                ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [fine_area_infos]");
                goto end;
            }
            OpenAPI_geographical_area_t *fine_area_infosItem = OpenAPI_geographical_area_parseFromJSON(fine_area_infos_local);
            if (!fine_area_infosItem) {
                ogs_error("No fine_area_infosItem");
                goto end;
            }
            OpenAPI_list_add(fine_area_infosList, fine_area_infosItem);
        }
    }

    start_ts = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "startTs");
    if (start_ts) {
    if (!cJSON_IsString(start_ts) && !cJSON_IsNull(start_ts)) {
        ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [start_ts]");
        goto end;
    }
    }

    end_ts = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "endTs");
    if (end_ts) {
    if (!cJSON_IsString(end_ts) && !cJSON_IsNull(end_ts)) {
        ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [end_ts]");
        goto end;
    }
    }

    qos_flow_ret_thd = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "qosFlowRetThd");
    if (qos_flow_ret_thd) {
    qos_flow_ret_thd_local_nonprim = OpenAPI_retainability_threshold_parseFromJSON(qos_flow_ret_thd);
    if (!qos_flow_ret_thd_local_nonprim) {
        ogs_error("OpenAPI_retainability_threshold_parseFromJSON failed [qos_flow_ret_thd]");
        goto end;
    }
    }

    ran_ue_throu_thd = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "ranUeThrouThd");
    if (ran_ue_throu_thd) {
    if (!cJSON_IsString(ran_ue_throu_thd) && !cJSON_IsNull(ran_ue_throu_thd)) {
        ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [ran_ue_throu_thd]");
        goto end;
    }
    }

    e2e_delay_thd = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "e2eDelayThd");
    if (e2e_delay_thd) {
    if (!cJSON_IsNumber(e2e_delay_thd)) {
        ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [e2e_delay_thd]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(qos_sustainability_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_qos_sustainability_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    qos_sustainability_info_local_var = OpenAPI_qos_sustainability_info_create (
        area_info ? area_info_local_nonprim : NULL,
        fine_area_infos ? fine_area_infosList : NULL,
        start_ts && !cJSON_IsNull(start_ts) ? ogs_strdup(start_ts->valuestring) : NULL,
        end_ts && !cJSON_IsNull(end_ts) ? ogs_strdup(end_ts->valuestring) : NULL,
        qos_flow_ret_thd ? qos_flow_ret_thd_local_nonprim : NULL,
        ran_ue_throu_thd && !cJSON_IsNull(ran_ue_throu_thd) ? ogs_strdup(ran_ue_throu_thd->valuestring) : NULL,
        e2e_delay_thd ? true : false,
        e2e_delay_thd ? e2e_delay_thd->valuedouble : 0,
        snssai ? snssai_local_nonprim : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return qos_sustainability_info_local_var;
end:
    if (area_info_local_nonprim) {
        OpenAPI_network_area_info_free(area_info_local_nonprim);
        area_info_local_nonprim = NULL;
    }
    if (fine_area_infosList) {
        OpenAPI_list_for_each(fine_area_infosList, node) {
            OpenAPI_geographical_area_free(node->data);
        }
        OpenAPI_list_free(fine_area_infosList);
        fine_area_infosList = NULL;
    }
    if (qos_flow_ret_thd_local_nonprim) {
        OpenAPI_retainability_threshold_free(qos_flow_ret_thd_local_nonprim);
        qos_flow_ret_thd_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_qos_sustainability_info_t *OpenAPI_qos_sustainability_info_copy(OpenAPI_qos_sustainability_info_t *dst, OpenAPI_qos_sustainability_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_sustainability_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_sustainability_info_convertToJSON() failed");
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

    OpenAPI_qos_sustainability_info_free(dst);
    dst = OpenAPI_qos_sustainability_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

