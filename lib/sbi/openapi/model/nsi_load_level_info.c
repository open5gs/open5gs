
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nsi_load_level_info.h"

OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_create(
    int load_level_information,
    OpenAPI_snssai_t *snssai,
    char *nsi_id,
    OpenAPI_resource_usage_t *res_usage,
    bool is_num_of_exceed_load_level_thr,
    int num_of_exceed_load_level_thr,
    bool is_exceed_load_level_thr_ind,
    int exceed_load_level_thr_ind,
    OpenAPI_network_area_info_t *network_area,
    OpenAPI_time_window_t *time_period,
    OpenAPI_list_t *res_usg_thr_cross_time_period,
    OpenAPI_number_average_t *num_of_ues,
    OpenAPI_number_average_t *num_of_pdu_sess,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_nsi_load_level_info_t *nsi_load_level_info_local_var = ogs_malloc(sizeof(OpenAPI_nsi_load_level_info_t));
    ogs_assert(nsi_load_level_info_local_var);

    nsi_load_level_info_local_var->load_level_information = load_level_information;
    nsi_load_level_info_local_var->snssai = snssai;
    nsi_load_level_info_local_var->nsi_id = nsi_id;
    nsi_load_level_info_local_var->res_usage = res_usage;
    nsi_load_level_info_local_var->is_num_of_exceed_load_level_thr = is_num_of_exceed_load_level_thr;
    nsi_load_level_info_local_var->num_of_exceed_load_level_thr = num_of_exceed_load_level_thr;
    nsi_load_level_info_local_var->is_exceed_load_level_thr_ind = is_exceed_load_level_thr_ind;
    nsi_load_level_info_local_var->exceed_load_level_thr_ind = exceed_load_level_thr_ind;
    nsi_load_level_info_local_var->network_area = network_area;
    nsi_load_level_info_local_var->time_period = time_period;
    nsi_load_level_info_local_var->res_usg_thr_cross_time_period = res_usg_thr_cross_time_period;
    nsi_load_level_info_local_var->num_of_ues = num_of_ues;
    nsi_load_level_info_local_var->num_of_pdu_sess = num_of_pdu_sess;
    nsi_load_level_info_local_var->is_confidence = is_confidence;
    nsi_load_level_info_local_var->confidence = confidence;

    return nsi_load_level_info_local_var;
}

void OpenAPI_nsi_load_level_info_free(OpenAPI_nsi_load_level_info_t *nsi_load_level_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nsi_load_level_info) {
        return;
    }
    if (nsi_load_level_info->snssai) {
        OpenAPI_snssai_free(nsi_load_level_info->snssai);
        nsi_load_level_info->snssai = NULL;
    }
    if (nsi_load_level_info->nsi_id) {
        ogs_free(nsi_load_level_info->nsi_id);
        nsi_load_level_info->nsi_id = NULL;
    }
    if (nsi_load_level_info->res_usage) {
        OpenAPI_resource_usage_free(nsi_load_level_info->res_usage);
        nsi_load_level_info->res_usage = NULL;
    }
    if (nsi_load_level_info->network_area) {
        OpenAPI_network_area_info_free(nsi_load_level_info->network_area);
        nsi_load_level_info->network_area = NULL;
    }
    if (nsi_load_level_info->time_period) {
        OpenAPI_time_window_free(nsi_load_level_info->time_period);
        nsi_load_level_info->time_period = NULL;
    }
    if (nsi_load_level_info->res_usg_thr_cross_time_period) {
        OpenAPI_list_for_each(nsi_load_level_info->res_usg_thr_cross_time_period, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(nsi_load_level_info->res_usg_thr_cross_time_period);
        nsi_load_level_info->res_usg_thr_cross_time_period = NULL;
    }
    if (nsi_load_level_info->num_of_ues) {
        OpenAPI_number_average_free(nsi_load_level_info->num_of_ues);
        nsi_load_level_info->num_of_ues = NULL;
    }
    if (nsi_load_level_info->num_of_pdu_sess) {
        OpenAPI_number_average_free(nsi_load_level_info->num_of_pdu_sess);
        nsi_load_level_info->num_of_pdu_sess = NULL;
    }
    ogs_free(nsi_load_level_info);
}

cJSON *OpenAPI_nsi_load_level_info_convertToJSON(OpenAPI_nsi_load_level_info_t *nsi_load_level_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nsi_load_level_info == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [NsiLoadLevelInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "loadLevelInformation", nsi_load_level_info->load_level_information) == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [load_level_information]");
        goto end;
    }

    if (!nsi_load_level_info->snssai) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(nsi_load_level_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (nsi_load_level_info->nsi_id) {
    if (cJSON_AddStringToObject(item, "nsiId", nsi_load_level_info->nsi_id) == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [nsi_id]");
        goto end;
    }
    }

    if (nsi_load_level_info->res_usage) {
    cJSON *res_usage_local_JSON = OpenAPI_resource_usage_convertToJSON(nsi_load_level_info->res_usage);
    if (res_usage_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [res_usage]");
        goto end;
    }
    cJSON_AddItemToObject(item, "resUsage", res_usage_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [res_usage]");
        goto end;
    }
    }

    if (nsi_load_level_info->is_num_of_exceed_load_level_thr) {
    if (cJSON_AddNumberToObject(item, "numOfExceedLoadLevelThr", nsi_load_level_info->num_of_exceed_load_level_thr) == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [num_of_exceed_load_level_thr]");
        goto end;
    }
    }

    if (nsi_load_level_info->is_exceed_load_level_thr_ind) {
    if (cJSON_AddBoolToObject(item, "exceedLoadLevelThrInd", nsi_load_level_info->exceed_load_level_thr_ind) == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [exceed_load_level_thr_ind]");
        goto end;
    }
    }

    if (nsi_load_level_info->network_area) {
    cJSON *network_area_local_JSON = OpenAPI_network_area_info_convertToJSON(nsi_load_level_info->network_area);
    if (network_area_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [network_area]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkArea", network_area_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [network_area]");
        goto end;
    }
    }

    if (nsi_load_level_info->time_period) {
    cJSON *time_period_local_JSON = OpenAPI_time_window_convertToJSON(nsi_load_level_info->time_period);
    if (time_period_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [time_period]");
        goto end;
    }
    cJSON_AddItemToObject(item, "timePeriod", time_period_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [time_period]");
        goto end;
    }
    }

    if (nsi_load_level_info->res_usg_thr_cross_time_period) {
    cJSON *res_usg_thr_cross_time_periodList = cJSON_AddArrayToObject(item, "resUsgThrCrossTimePeriod");
    if (res_usg_thr_cross_time_periodList == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [res_usg_thr_cross_time_period]");
        goto end;
    }
    OpenAPI_list_for_each(nsi_load_level_info->res_usg_thr_cross_time_period, node) {
        cJSON *itemLocal = OpenAPI_time_window_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [res_usg_thr_cross_time_period]");
            goto end;
        }
        cJSON_AddItemToArray(res_usg_thr_cross_time_periodList, itemLocal);
    }
    }

    if (nsi_load_level_info->num_of_ues) {
    cJSON *num_of_ues_local_JSON = OpenAPI_number_average_convertToJSON(nsi_load_level_info->num_of_ues);
    if (num_of_ues_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [num_of_ues]");
        goto end;
    }
    cJSON_AddItemToObject(item, "numOfUes", num_of_ues_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [num_of_ues]");
        goto end;
    }
    }

    if (nsi_load_level_info->num_of_pdu_sess) {
    cJSON *num_of_pdu_sess_local_JSON = OpenAPI_number_average_convertToJSON(nsi_load_level_info->num_of_pdu_sess);
    if (num_of_pdu_sess_local_JSON == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [num_of_pdu_sess]");
        goto end;
    }
    cJSON_AddItemToObject(item, "numOfPduSess", num_of_pdu_sess_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [num_of_pdu_sess]");
        goto end;
    }
    }

    if (nsi_load_level_info->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", nsi_load_level_info->confidence) == NULL) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_parseFromJSON(cJSON *nsi_load_level_infoJSON)
{
    OpenAPI_nsi_load_level_info_t *nsi_load_level_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *load_level_information = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *nsi_id = NULL;
    cJSON *res_usage = NULL;
    OpenAPI_resource_usage_t *res_usage_local_nonprim = NULL;
    cJSON *num_of_exceed_load_level_thr = NULL;
    cJSON *exceed_load_level_thr_ind = NULL;
    cJSON *network_area = NULL;
    OpenAPI_network_area_info_t *network_area_local_nonprim = NULL;
    cJSON *time_period = NULL;
    OpenAPI_time_window_t *time_period_local_nonprim = NULL;
    cJSON *res_usg_thr_cross_time_period = NULL;
    OpenAPI_list_t *res_usg_thr_cross_time_periodList = NULL;
    cJSON *num_of_ues = NULL;
    OpenAPI_number_average_t *num_of_ues_local_nonprim = NULL;
    cJSON *num_of_pdu_sess = NULL;
    OpenAPI_number_average_t *num_of_pdu_sess_local_nonprim = NULL;
    cJSON *confidence = NULL;
    load_level_information = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "loadLevelInformation");
    if (!load_level_information) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [load_level_information]");
        goto end;
    }
    if (!cJSON_IsNumber(load_level_information)) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [load_level_information]");
        goto end;
    }

    snssai = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    nsi_id = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "nsiId");
    if (nsi_id) {
    if (!cJSON_IsString(nsi_id) && !cJSON_IsNull(nsi_id)) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [nsi_id]");
        goto end;
    }
    }

    res_usage = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "resUsage");
    if (res_usage) {
    res_usage_local_nonprim = OpenAPI_resource_usage_parseFromJSON(res_usage);
    if (!res_usage_local_nonprim) {
        ogs_error("OpenAPI_resource_usage_parseFromJSON failed [res_usage]");
        goto end;
    }
    }

    num_of_exceed_load_level_thr = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "numOfExceedLoadLevelThr");
    if (num_of_exceed_load_level_thr) {
    if (!cJSON_IsNumber(num_of_exceed_load_level_thr)) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [num_of_exceed_load_level_thr]");
        goto end;
    }
    }

    exceed_load_level_thr_ind = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "exceedLoadLevelThrInd");
    if (exceed_load_level_thr_ind) {
    if (!cJSON_IsBool(exceed_load_level_thr_ind)) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [exceed_load_level_thr_ind]");
        goto end;
    }
    }

    network_area = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "networkArea");
    if (network_area) {
    network_area_local_nonprim = OpenAPI_network_area_info_parseFromJSON(network_area);
    if (!network_area_local_nonprim) {
        ogs_error("OpenAPI_network_area_info_parseFromJSON failed [network_area]");
        goto end;
    }
    }

    time_period = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "timePeriod");
    if (time_period) {
    time_period_local_nonprim = OpenAPI_time_window_parseFromJSON(time_period);
    if (!time_period_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [time_period]");
        goto end;
    }
    }

    res_usg_thr_cross_time_period = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "resUsgThrCrossTimePeriod");
    if (res_usg_thr_cross_time_period) {
        cJSON *res_usg_thr_cross_time_period_local = NULL;
        if (!cJSON_IsArray(res_usg_thr_cross_time_period)) {
            ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [res_usg_thr_cross_time_period]");
            goto end;
        }

        res_usg_thr_cross_time_periodList = OpenAPI_list_create();

        cJSON_ArrayForEach(res_usg_thr_cross_time_period_local, res_usg_thr_cross_time_period) {
            if (!cJSON_IsObject(res_usg_thr_cross_time_period_local)) {
                ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [res_usg_thr_cross_time_period]");
                goto end;
            }
            OpenAPI_time_window_t *res_usg_thr_cross_time_periodItem = OpenAPI_time_window_parseFromJSON(res_usg_thr_cross_time_period_local);
            if (!res_usg_thr_cross_time_periodItem) {
                ogs_error("No res_usg_thr_cross_time_periodItem");
                goto end;
            }
            OpenAPI_list_add(res_usg_thr_cross_time_periodList, res_usg_thr_cross_time_periodItem);
        }
    }

    num_of_ues = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "numOfUes");
    if (num_of_ues) {
    num_of_ues_local_nonprim = OpenAPI_number_average_parseFromJSON(num_of_ues);
    if (!num_of_ues_local_nonprim) {
        ogs_error("OpenAPI_number_average_parseFromJSON failed [num_of_ues]");
        goto end;
    }
    }

    num_of_pdu_sess = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "numOfPduSess");
    if (num_of_pdu_sess) {
    num_of_pdu_sess_local_nonprim = OpenAPI_number_average_parseFromJSON(num_of_pdu_sess);
    if (!num_of_pdu_sess_local_nonprim) {
        ogs_error("OpenAPI_number_average_parseFromJSON failed [num_of_pdu_sess]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(nsi_load_level_infoJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    nsi_load_level_info_local_var = OpenAPI_nsi_load_level_info_create (
        
        load_level_information->valuedouble,
        snssai_local_nonprim,
        nsi_id && !cJSON_IsNull(nsi_id) ? ogs_strdup(nsi_id->valuestring) : NULL,
        res_usage ? res_usage_local_nonprim : NULL,
        num_of_exceed_load_level_thr ? true : false,
        num_of_exceed_load_level_thr ? num_of_exceed_load_level_thr->valuedouble : 0,
        exceed_load_level_thr_ind ? true : false,
        exceed_load_level_thr_ind ? exceed_load_level_thr_ind->valueint : 0,
        network_area ? network_area_local_nonprim : NULL,
        time_period ? time_period_local_nonprim : NULL,
        res_usg_thr_cross_time_period ? res_usg_thr_cross_time_periodList : NULL,
        num_of_ues ? num_of_ues_local_nonprim : NULL,
        num_of_pdu_sess ? num_of_pdu_sess_local_nonprim : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return nsi_load_level_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (res_usage_local_nonprim) {
        OpenAPI_resource_usage_free(res_usage_local_nonprim);
        res_usage_local_nonprim = NULL;
    }
    if (network_area_local_nonprim) {
        OpenAPI_network_area_info_free(network_area_local_nonprim);
        network_area_local_nonprim = NULL;
    }
    if (time_period_local_nonprim) {
        OpenAPI_time_window_free(time_period_local_nonprim);
        time_period_local_nonprim = NULL;
    }
    if (res_usg_thr_cross_time_periodList) {
        OpenAPI_list_for_each(res_usg_thr_cross_time_periodList, node) {
            OpenAPI_time_window_free(node->data);
        }
        OpenAPI_list_free(res_usg_thr_cross_time_periodList);
        res_usg_thr_cross_time_periodList = NULL;
    }
    if (num_of_ues_local_nonprim) {
        OpenAPI_number_average_free(num_of_ues_local_nonprim);
        num_of_ues_local_nonprim = NULL;
    }
    if (num_of_pdu_sess_local_nonprim) {
        OpenAPI_number_average_free(num_of_pdu_sess_local_nonprim);
        num_of_pdu_sess_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nsi_load_level_info_t *OpenAPI_nsi_load_level_info_copy(OpenAPI_nsi_load_level_info_t *dst, OpenAPI_nsi_load_level_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nsi_load_level_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nsi_load_level_info_convertToJSON() failed");
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

    OpenAPI_nsi_load_level_info_free(dst);
    dst = OpenAPI_nsi_load_level_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

