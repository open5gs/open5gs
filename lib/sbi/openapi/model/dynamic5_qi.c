
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dynamic5_qi.h"

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_create(
    OpenAPI_qos_resource_type_e resource_type,
    int priority_level,
    int packet_delay_budget,
    char *packet_err_rate,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_ext_packet_del_budget,
    int ext_packet_del_budget,
    bool is_cn_packet_delay_budget_dl,
    int cn_packet_delay_budget_dl,
    bool is_cn_packet_delay_budget_ul,
    int cn_packet_delay_budget_ul
)
{
    OpenAPI_dynamic5_qi_t *dynamic5_qi_local_var = ogs_malloc(sizeof(OpenAPI_dynamic5_qi_t));
    ogs_assert(dynamic5_qi_local_var);

    dynamic5_qi_local_var->resource_type = resource_type;
    dynamic5_qi_local_var->priority_level = priority_level;
    dynamic5_qi_local_var->packet_delay_budget = packet_delay_budget;
    dynamic5_qi_local_var->packet_err_rate = packet_err_rate;
    dynamic5_qi_local_var->is_aver_window = is_aver_window;
    dynamic5_qi_local_var->aver_window = aver_window;
    dynamic5_qi_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    dynamic5_qi_local_var->max_data_burst_vol = max_data_burst_vol;
    dynamic5_qi_local_var->is_ext_max_data_burst_vol = is_ext_max_data_burst_vol;
    dynamic5_qi_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;
    dynamic5_qi_local_var->is_ext_packet_del_budget = is_ext_packet_del_budget;
    dynamic5_qi_local_var->ext_packet_del_budget = ext_packet_del_budget;
    dynamic5_qi_local_var->is_cn_packet_delay_budget_dl = is_cn_packet_delay_budget_dl;
    dynamic5_qi_local_var->cn_packet_delay_budget_dl = cn_packet_delay_budget_dl;
    dynamic5_qi_local_var->is_cn_packet_delay_budget_ul = is_cn_packet_delay_budget_ul;
    dynamic5_qi_local_var->cn_packet_delay_budget_ul = cn_packet_delay_budget_ul;

    return dynamic5_qi_local_var;
}

void OpenAPI_dynamic5_qi_free(OpenAPI_dynamic5_qi_t *dynamic5_qi)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == dynamic5_qi) {
        return;
    }
    if (dynamic5_qi->packet_err_rate) {
        ogs_free(dynamic5_qi->packet_err_rate);
        dynamic5_qi->packet_err_rate = NULL;
    }
    ogs_free(dynamic5_qi);
}

cJSON *OpenAPI_dynamic5_qi_convertToJSON(OpenAPI_dynamic5_qi_t *dynamic5_qi)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (dynamic5_qi == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [Dynamic5Qi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (dynamic5_qi->resource_type == OpenAPI_qos_resource_type_NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [resource_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "resourceType", OpenAPI_qos_resource_type_ToString(dynamic5_qi->resource_type)) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [resource_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "priorityLevel", dynamic5_qi->priority_level) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [priority_level]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "packetDelayBudget", dynamic5_qi->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }

    if (!dynamic5_qi->packet_err_rate) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_err_rate]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "packetErrRate", dynamic5_qi->packet_err_rate) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [packet_err_rate]");
        goto end;
    }

    if (dynamic5_qi->is_aver_window) {
    if (cJSON_AddNumberToObject(item, "averWindow", dynamic5_qi->aver_window) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [aver_window]");
        goto end;
    }
    }

    if (dynamic5_qi->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", dynamic5_qi->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    if (dynamic5_qi->is_ext_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", dynamic5_qi->ext_max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    if (dynamic5_qi->is_ext_packet_del_budget) {
    if (cJSON_AddNumberToObject(item, "extPacketDelBudget", dynamic5_qi->ext_packet_del_budget) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [ext_packet_del_budget]");
        goto end;
    }
    }

    if (dynamic5_qi->is_cn_packet_delay_budget_dl) {
    if (cJSON_AddNumberToObject(item, "cnPacketDelayBudgetDl", dynamic5_qi->cn_packet_delay_budget_dl) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [cn_packet_delay_budget_dl]");
        goto end;
    }
    }

    if (dynamic5_qi->is_cn_packet_delay_budget_ul) {
    if (cJSON_AddNumberToObject(item, "cnPacketDelayBudgetUl", dynamic5_qi->cn_packet_delay_budget_ul) == NULL) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed [cn_packet_delay_budget_ul]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_parseFromJSON(cJSON *dynamic5_qiJSON)
{
    OpenAPI_dynamic5_qi_t *dynamic5_qi_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *resource_type = NULL;
    OpenAPI_qos_resource_type_e resource_typeVariable = 0;
    cJSON *priority_level = NULL;
    cJSON *packet_delay_budget = NULL;
    cJSON *packet_err_rate = NULL;
    cJSON *aver_window = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *ext_max_data_burst_vol = NULL;
    cJSON *ext_packet_del_budget = NULL;
    cJSON *cn_packet_delay_budget_dl = NULL;
    cJSON *cn_packet_delay_budget_ul = NULL;
    resource_type = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "resourceType");
    if (!resource_type) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [resource_type]");
        goto end;
    }
    if (!cJSON_IsString(resource_type)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [resource_type]");
        goto end;
    }
    resource_typeVariable = OpenAPI_qos_resource_type_FromString(resource_type->valuestring);

    priority_level = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "priorityLevel");
    if (!priority_level) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [priority_level]");
        goto end;
    }
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [priority_level]");
        goto end;
    }

    packet_delay_budget = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "packetDelayBudget");
    if (!packet_delay_budget) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }
    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }

    packet_err_rate = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "packetErrRate");
    if (!packet_err_rate) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }
    if (!cJSON_IsString(packet_err_rate)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }

    aver_window = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "averWindow");
    if (aver_window) {
    if (!cJSON_IsNumber(aver_window)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [aver_window]");
        goto end;
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "extMaxDataBurstVol");
    if (ext_max_data_burst_vol) {
    if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    ext_packet_del_budget = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "extPacketDelBudget");
    if (ext_packet_del_budget) {
    if (!cJSON_IsNumber(ext_packet_del_budget)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [ext_packet_del_budget]");
        goto end;
    }
    }

    cn_packet_delay_budget_dl = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "cnPacketDelayBudgetDl");
    if (cn_packet_delay_budget_dl) {
    if (!cJSON_IsNumber(cn_packet_delay_budget_dl)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [cn_packet_delay_budget_dl]");
        goto end;
    }
    }

    cn_packet_delay_budget_ul = cJSON_GetObjectItemCaseSensitive(dynamic5_qiJSON, "cnPacketDelayBudgetUl");
    if (cn_packet_delay_budget_ul) {
    if (!cJSON_IsNumber(cn_packet_delay_budget_ul)) {
        ogs_error("OpenAPI_dynamic5_qi_parseFromJSON() failed [cn_packet_delay_budget_ul]");
        goto end;
    }
    }

    dynamic5_qi_local_var = OpenAPI_dynamic5_qi_create (
        resource_typeVariable,
        
        priority_level->valuedouble,
        
        packet_delay_budget->valuedouble,
        ogs_strdup(packet_err_rate->valuestring),
        aver_window ? true : false,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? true : false,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0,
        ext_packet_del_budget ? true : false,
        ext_packet_del_budget ? ext_packet_del_budget->valuedouble : 0,
        cn_packet_delay_budget_dl ? true : false,
        cn_packet_delay_budget_dl ? cn_packet_delay_budget_dl->valuedouble : 0,
        cn_packet_delay_budget_ul ? true : false,
        cn_packet_delay_budget_ul ? cn_packet_delay_budget_ul->valuedouble : 0
    );

    return dynamic5_qi_local_var;
end:
    return NULL;
}

OpenAPI_dynamic5_qi_t *OpenAPI_dynamic5_qi_copy(OpenAPI_dynamic5_qi_t *dst, OpenAPI_dynamic5_qi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_dynamic5_qi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_dynamic5_qi_convertToJSON() failed");
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

    OpenAPI_dynamic5_qi_free(dst);
    dst = OpenAPI_dynamic5_qi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

