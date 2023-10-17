
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_dynamic5_qi.h"

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_create(
    bool is_priority_level,
    int priority_level,
    bool is_aver_window,
    int aver_window,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_ext_max_data_burst_vol,
    int ext_max_data_burst_vol,
    bool is_cn_packet_delay_budget_dl,
    int cn_packet_delay_budget_dl,
    bool is_cn_packet_delay_budget_ul,
    int cn_packet_delay_budget_ul
)
{
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi_local_var = ogs_malloc(sizeof(OpenAPI_non_dynamic5_qi_t));
    ogs_assert(non_dynamic5_qi_local_var);

    non_dynamic5_qi_local_var->is_priority_level = is_priority_level;
    non_dynamic5_qi_local_var->priority_level = priority_level;
    non_dynamic5_qi_local_var->is_aver_window = is_aver_window;
    non_dynamic5_qi_local_var->aver_window = aver_window;
    non_dynamic5_qi_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    non_dynamic5_qi_local_var->max_data_burst_vol = max_data_burst_vol;
    non_dynamic5_qi_local_var->is_ext_max_data_burst_vol = is_ext_max_data_burst_vol;
    non_dynamic5_qi_local_var->ext_max_data_burst_vol = ext_max_data_burst_vol;
    non_dynamic5_qi_local_var->is_cn_packet_delay_budget_dl = is_cn_packet_delay_budget_dl;
    non_dynamic5_qi_local_var->cn_packet_delay_budget_dl = cn_packet_delay_budget_dl;
    non_dynamic5_qi_local_var->is_cn_packet_delay_budget_ul = is_cn_packet_delay_budget_ul;
    non_dynamic5_qi_local_var->cn_packet_delay_budget_ul = cn_packet_delay_budget_ul;

    return non_dynamic5_qi_local_var;
}

void OpenAPI_non_dynamic5_qi_free(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non_dynamic5_qi) {
        return;
    }
    ogs_free(non_dynamic5_qi);
}

cJSON *OpenAPI_non_dynamic5_qi_convertToJSON(OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (non_dynamic5_qi == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [NonDynamic5Qi]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (non_dynamic5_qi->is_priority_level) {
    if (cJSON_AddNumberToObject(item, "priorityLevel", non_dynamic5_qi->priority_level) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [priority_level]");
        goto end;
    }
    }

    if (non_dynamic5_qi->is_aver_window) {
    if (cJSON_AddNumberToObject(item, "averWindow", non_dynamic5_qi->aver_window) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [aver_window]");
        goto end;
    }
    }

    if (non_dynamic5_qi->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", non_dynamic5_qi->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    if (non_dynamic5_qi->is_ext_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "extMaxDataBurstVol", non_dynamic5_qi->ext_max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    if (non_dynamic5_qi->is_cn_packet_delay_budget_dl) {
    if (cJSON_AddNumberToObject(item, "cnPacketDelayBudgetDl", non_dynamic5_qi->cn_packet_delay_budget_dl) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [cn_packet_delay_budget_dl]");
        goto end;
    }
    }

    if (non_dynamic5_qi->is_cn_packet_delay_budget_ul) {
    if (cJSON_AddNumberToObject(item, "cnPacketDelayBudgetUl", non_dynamic5_qi->cn_packet_delay_budget_ul) == NULL) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed [cn_packet_delay_budget_ul]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_parseFromJSON(cJSON *non_dynamic5_qiJSON)
{
    OpenAPI_non_dynamic5_qi_t *non_dynamic5_qi_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *priority_level = NULL;
    cJSON *aver_window = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *ext_max_data_burst_vol = NULL;
    cJSON *cn_packet_delay_budget_dl = NULL;
    cJSON *cn_packet_delay_budget_ul = NULL;
    priority_level = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "priorityLevel");
    if (priority_level) {
    if (!cJSON_IsNumber(priority_level)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [priority_level]");
        goto end;
    }
    }

    aver_window = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "averWindow");
    if (aver_window) {
    if (!cJSON_IsNumber(aver_window)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [aver_window]");
        goto end;
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    ext_max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "extMaxDataBurstVol");
    if (ext_max_data_burst_vol) {
    if (!cJSON_IsNumber(ext_max_data_burst_vol)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [ext_max_data_burst_vol]");
        goto end;
    }
    }

    cn_packet_delay_budget_dl = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "cnPacketDelayBudgetDl");
    if (cn_packet_delay_budget_dl) {
    if (!cJSON_IsNumber(cn_packet_delay_budget_dl)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [cn_packet_delay_budget_dl]");
        goto end;
    }
    }

    cn_packet_delay_budget_ul = cJSON_GetObjectItemCaseSensitive(non_dynamic5_qiJSON, "cnPacketDelayBudgetUl");
    if (cn_packet_delay_budget_ul) {
    if (!cJSON_IsNumber(cn_packet_delay_budget_ul)) {
        ogs_error("OpenAPI_non_dynamic5_qi_parseFromJSON() failed [cn_packet_delay_budget_ul]");
        goto end;
    }
    }

    non_dynamic5_qi_local_var = OpenAPI_non_dynamic5_qi_create (
        priority_level ? true : false,
        priority_level ? priority_level->valuedouble : 0,
        aver_window ? true : false,
        aver_window ? aver_window->valuedouble : 0,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        ext_max_data_burst_vol ? true : false,
        ext_max_data_burst_vol ? ext_max_data_burst_vol->valuedouble : 0,
        cn_packet_delay_budget_dl ? true : false,
        cn_packet_delay_budget_dl ? cn_packet_delay_budget_dl->valuedouble : 0,
        cn_packet_delay_budget_ul ? true : false,
        cn_packet_delay_budget_ul ? cn_packet_delay_budget_ul->valuedouble : 0
    );

    return non_dynamic5_qi_local_var;
end:
    return NULL;
}

OpenAPI_non_dynamic5_qi_t *OpenAPI_non_dynamic5_qi_copy(OpenAPI_non_dynamic5_qi_t *dst, OpenAPI_non_dynamic5_qi_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_dynamic5_qi_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_dynamic5_qi_convertToJSON() failed");
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

    OpenAPI_non_dynamic5_qi_free(dst);
    dst = OpenAPI_non_dynamic5_qi_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

