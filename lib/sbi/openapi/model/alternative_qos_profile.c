
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "alternative_qos_profile.h"

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_create(
    int index,
    char *gua_fbr_dl,
    char *gua_fbr_ul,
    bool is_packet_delay_budget,
    int packet_delay_budget,
    char *packet_err_rate,
    bool is_max_data_burst_vol,
    int max_data_burst_vol,
    bool is_pdu_set_qos_dl_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_dl,
    bool is_pdu_set_qos_ul_null,
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_ul
)
{
    OpenAPI_alternative_qos_profile_t *alternative_qos_profile_local_var = ogs_malloc(sizeof(OpenAPI_alternative_qos_profile_t));
    ogs_assert(alternative_qos_profile_local_var);

    alternative_qos_profile_local_var->index = index;
    alternative_qos_profile_local_var->gua_fbr_dl = gua_fbr_dl;
    alternative_qos_profile_local_var->gua_fbr_ul = gua_fbr_ul;
    alternative_qos_profile_local_var->is_packet_delay_budget = is_packet_delay_budget;
    alternative_qos_profile_local_var->packet_delay_budget = packet_delay_budget;
    alternative_qos_profile_local_var->packet_err_rate = packet_err_rate;
    alternative_qos_profile_local_var->is_max_data_burst_vol = is_max_data_burst_vol;
    alternative_qos_profile_local_var->max_data_burst_vol = max_data_burst_vol;
    alternative_qos_profile_local_var->is_pdu_set_qos_dl_null = is_pdu_set_qos_dl_null;
    alternative_qos_profile_local_var->pdu_set_qos_dl = pdu_set_qos_dl;
    alternative_qos_profile_local_var->is_pdu_set_qos_ul_null = is_pdu_set_qos_ul_null;
    alternative_qos_profile_local_var->pdu_set_qos_ul = pdu_set_qos_ul;

    return alternative_qos_profile_local_var;
}

void OpenAPI_alternative_qos_profile_free(OpenAPI_alternative_qos_profile_t *alternative_qos_profile)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == alternative_qos_profile) {
        return;
    }
    if (alternative_qos_profile->gua_fbr_dl) {
        ogs_free(alternative_qos_profile->gua_fbr_dl);
        alternative_qos_profile->gua_fbr_dl = NULL;
    }
    if (alternative_qos_profile->gua_fbr_ul) {
        ogs_free(alternative_qos_profile->gua_fbr_ul);
        alternative_qos_profile->gua_fbr_ul = NULL;
    }
    if (alternative_qos_profile->packet_err_rate) {
        ogs_free(alternative_qos_profile->packet_err_rate);
        alternative_qos_profile->packet_err_rate = NULL;
    }
    if (alternative_qos_profile->pdu_set_qos_dl) {
        OpenAPI_pdu_set_qos_para_free(alternative_qos_profile->pdu_set_qos_dl);
        alternative_qos_profile->pdu_set_qos_dl = NULL;
    }
    if (alternative_qos_profile->pdu_set_qos_ul) {
        OpenAPI_pdu_set_qos_para_free(alternative_qos_profile->pdu_set_qos_ul);
        alternative_qos_profile->pdu_set_qos_ul = NULL;
    }
    ogs_free(alternative_qos_profile);
}

cJSON *OpenAPI_alternative_qos_profile_convertToJSON(OpenAPI_alternative_qos_profile_t *alternative_qos_profile)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (alternative_qos_profile == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [AlternativeQosProfile]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "index", alternative_qos_profile->index) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [index]");
        goto end;
    }

    if (alternative_qos_profile->gua_fbr_dl) {
    if (cJSON_AddStringToObject(item, "guaFbrDl", alternative_qos_profile->gua_fbr_dl) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    if (alternative_qos_profile->gua_fbr_ul) {
    if (cJSON_AddStringToObject(item, "guaFbrUl", alternative_qos_profile->gua_fbr_ul) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    if (alternative_qos_profile->is_packet_delay_budget) {
    if (cJSON_AddNumberToObject(item, "packetDelayBudget", alternative_qos_profile->packet_delay_budget) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    if (alternative_qos_profile->packet_err_rate) {
    if (cJSON_AddStringToObject(item, "packetErrRate", alternative_qos_profile->packet_err_rate) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [packet_err_rate]");
        goto end;
    }
    }

    if (alternative_qos_profile->is_max_data_burst_vol) {
    if (cJSON_AddNumberToObject(item, "maxDataBurstVol", alternative_qos_profile->max_data_burst_vol) == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    if (alternative_qos_profile->pdu_set_qos_dl) {
    cJSON *pdu_set_qos_dl_local_JSON = OpenAPI_pdu_set_qos_para_convertToJSON(alternative_qos_profile->pdu_set_qos_dl);
    if (pdu_set_qos_dl_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_dl]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSetQosDl", pdu_set_qos_dl_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_dl]");
        goto end;
    }
    } else if (alternative_qos_profile->is_pdu_set_qos_dl_null) {
        if (cJSON_AddNullToObject(item, "pduSetQosDl") == NULL) {
            ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_dl]");
            goto end;
        }
    }

    if (alternative_qos_profile->pdu_set_qos_ul) {
    cJSON *pdu_set_qos_ul_local_JSON = OpenAPI_pdu_set_qos_para_convertToJSON(alternative_qos_profile->pdu_set_qos_ul);
    if (pdu_set_qos_ul_local_JSON == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_ul]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSetQosUl", pdu_set_qos_ul_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_ul]");
        goto end;
    }
    } else if (alternative_qos_profile->is_pdu_set_qos_ul_null) {
        if (cJSON_AddNullToObject(item, "pduSetQosUl") == NULL) {
            ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed [pdu_set_qos_ul]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_parseFromJSON(cJSON *alternative_qos_profileJSON)
{
    OpenAPI_alternative_qos_profile_t *alternative_qos_profile_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *index = NULL;
    cJSON *gua_fbr_dl = NULL;
    cJSON *gua_fbr_ul = NULL;
    cJSON *packet_delay_budget = NULL;
    cJSON *packet_err_rate = NULL;
    cJSON *max_data_burst_vol = NULL;
    cJSON *pdu_set_qos_dl = NULL;
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_dl_local_nonprim = NULL;
    cJSON *pdu_set_qos_ul = NULL;
    OpenAPI_pdu_set_qos_para_t *pdu_set_qos_ul_local_nonprim = NULL;
    index = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "index");
    if (!index) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [index]");
        goto end;
    }
    if (!cJSON_IsNumber(index)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [index]");
        goto end;
    }

    gua_fbr_dl = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "guaFbrDl");
    if (gua_fbr_dl) {
    if (!cJSON_IsString(gua_fbr_dl) && !cJSON_IsNull(gua_fbr_dl)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [gua_fbr_dl]");
        goto end;
    }
    }

    gua_fbr_ul = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "guaFbrUl");
    if (gua_fbr_ul) {
    if (!cJSON_IsString(gua_fbr_ul) && !cJSON_IsNull(gua_fbr_ul)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [gua_fbr_ul]");
        goto end;
    }
    }

    packet_delay_budget = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "packetDelayBudget");
    if (packet_delay_budget) {
    if (!cJSON_IsNumber(packet_delay_budget)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [packet_delay_budget]");
        goto end;
    }
    }

    packet_err_rate = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "packetErrRate");
    if (packet_err_rate) {
    if (!cJSON_IsString(packet_err_rate) && !cJSON_IsNull(packet_err_rate)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [packet_err_rate]");
        goto end;
    }
    }

    max_data_burst_vol = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "maxDataBurstVol");
    if (max_data_burst_vol) {
    if (!cJSON_IsNumber(max_data_burst_vol)) {
        ogs_error("OpenAPI_alternative_qos_profile_parseFromJSON() failed [max_data_burst_vol]");
        goto end;
    }
    }

    pdu_set_qos_dl = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "pduSetQosDl");
    if (pdu_set_qos_dl) {
    if (!cJSON_IsNull(pdu_set_qos_dl)) {
    pdu_set_qos_dl_local_nonprim = OpenAPI_pdu_set_qos_para_parseFromJSON(pdu_set_qos_dl);
    if (!pdu_set_qos_dl_local_nonprim) {
        ogs_error("OpenAPI_pdu_set_qos_para_parseFromJSON failed [pdu_set_qos_dl]");
        goto end;
    }
    }
    }

    pdu_set_qos_ul = cJSON_GetObjectItemCaseSensitive(alternative_qos_profileJSON, "pduSetQosUl");
    if (pdu_set_qos_ul) {
    if (!cJSON_IsNull(pdu_set_qos_ul)) {
    pdu_set_qos_ul_local_nonprim = OpenAPI_pdu_set_qos_para_parseFromJSON(pdu_set_qos_ul);
    if (!pdu_set_qos_ul_local_nonprim) {
        ogs_error("OpenAPI_pdu_set_qos_para_parseFromJSON failed [pdu_set_qos_ul]");
        goto end;
    }
    }
    }

    alternative_qos_profile_local_var = OpenAPI_alternative_qos_profile_create (
        
        index->valuedouble,
        gua_fbr_dl && !cJSON_IsNull(gua_fbr_dl) ? ogs_strdup(gua_fbr_dl->valuestring) : NULL,
        gua_fbr_ul && !cJSON_IsNull(gua_fbr_ul) ? ogs_strdup(gua_fbr_ul->valuestring) : NULL,
        packet_delay_budget ? true : false,
        packet_delay_budget ? packet_delay_budget->valuedouble : 0,
        packet_err_rate && !cJSON_IsNull(packet_err_rate) ? ogs_strdup(packet_err_rate->valuestring) : NULL,
        max_data_burst_vol ? true : false,
        max_data_burst_vol ? max_data_burst_vol->valuedouble : 0,
        pdu_set_qos_dl && cJSON_IsNull(pdu_set_qos_dl) ? true : false,
        pdu_set_qos_dl ? pdu_set_qos_dl_local_nonprim : NULL,
        pdu_set_qos_ul && cJSON_IsNull(pdu_set_qos_ul) ? true : false,
        pdu_set_qos_ul ? pdu_set_qos_ul_local_nonprim : NULL
    );

    return alternative_qos_profile_local_var;
end:
    if (pdu_set_qos_dl_local_nonprim) {
        OpenAPI_pdu_set_qos_para_free(pdu_set_qos_dl_local_nonprim);
        pdu_set_qos_dl_local_nonprim = NULL;
    }
    if (pdu_set_qos_ul_local_nonprim) {
        OpenAPI_pdu_set_qos_para_free(pdu_set_qos_ul_local_nonprim);
        pdu_set_qos_ul_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_alternative_qos_profile_t *OpenAPI_alternative_qos_profile_copy(OpenAPI_alternative_qos_profile_t *dst, OpenAPI_alternative_qos_profile_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_alternative_qos_profile_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_alternative_qos_profile_convertToJSON() failed");
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

    OpenAPI_alternative_qos_profile_free(dst);
    dst = OpenAPI_alternative_qos_profile_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

