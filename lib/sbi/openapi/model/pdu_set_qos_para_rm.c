
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_set_qos_para_rm.h"

OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_create(
    bool is_pdu_set_delay_budget_null,
    bool is_pdu_set_delay_budget,
    int pdu_set_delay_budget,
    bool is_pdu_set_err_rate_null,
    char *pdu_set_err_rate,
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_info
)
{
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm_local_var = ogs_malloc(sizeof(OpenAPI_pdu_set_qos_para_rm_t));
    ogs_assert(pdu_set_qos_para_rm_local_var);

    pdu_set_qos_para_rm_local_var->is_pdu_set_delay_budget_null = is_pdu_set_delay_budget_null;
    pdu_set_qos_para_rm_local_var->is_pdu_set_delay_budget = is_pdu_set_delay_budget;
    pdu_set_qos_para_rm_local_var->pdu_set_delay_budget = pdu_set_delay_budget;
    pdu_set_qos_para_rm_local_var->is_pdu_set_err_rate_null = is_pdu_set_err_rate_null;
    pdu_set_qos_para_rm_local_var->pdu_set_err_rate = pdu_set_err_rate;
    pdu_set_qos_para_rm_local_var->pdu_set_handling_info = pdu_set_handling_info;

    return pdu_set_qos_para_rm_local_var;
}

void OpenAPI_pdu_set_qos_para_rm_free(OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_set_qos_para_rm) {
        return;
    }
    if (pdu_set_qos_para_rm->pdu_set_err_rate) {
        ogs_free(pdu_set_qos_para_rm->pdu_set_err_rate);
        pdu_set_qos_para_rm->pdu_set_err_rate = NULL;
    }
    ogs_free(pdu_set_qos_para_rm);
}

cJSON *OpenAPI_pdu_set_qos_para_rm_convertToJSON(OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_set_qos_para_rm == NULL) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [PduSetQosParaRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_set_qos_para_rm->is_pdu_set_delay_budget) {
    if (cJSON_AddNumberToObject(item, "pduSetDelayBudget", pdu_set_qos_para_rm->pdu_set_delay_budget) == NULL) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [pdu_set_delay_budget]");
        goto end;
    }
    } else if (pdu_set_qos_para_rm->is_pdu_set_delay_budget_null) {
        if (cJSON_AddNullToObject(item, "pduSetDelayBudget") == NULL) {
            ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [pdu_set_delay_budget]");
            goto end;
        }
    }

    if (pdu_set_qos_para_rm->pdu_set_err_rate) {
    if (cJSON_AddStringToObject(item, "pduSetErrRate", pdu_set_qos_para_rm->pdu_set_err_rate) == NULL) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [pdu_set_err_rate]");
        goto end;
    }
    } else if (pdu_set_qos_para_rm->is_pdu_set_err_rate_null) {
        if (cJSON_AddNullToObject(item, "pduSetErrRate") == NULL) {
            ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [pdu_set_err_rate]");
            goto end;
        }
    }

    if (pdu_set_qos_para_rm->pdu_set_handling_info != OpenAPI_pdu_set_handling_info_NULL) {
    if (cJSON_AddStringToObject(item, "pduSetHandlingInfo", OpenAPI_pdu_set_handling_info_ToString(pdu_set_qos_para_rm->pdu_set_handling_info)) == NULL) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed [pdu_set_handling_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_parseFromJSON(cJSON *pdu_set_qos_para_rmJSON)
{
    OpenAPI_pdu_set_qos_para_rm_t *pdu_set_qos_para_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_set_delay_budget = NULL;
    cJSON *pdu_set_err_rate = NULL;
    cJSON *pdu_set_handling_info = NULL;
    OpenAPI_pdu_set_handling_info_e pdu_set_handling_infoVariable = 0;
    pdu_set_delay_budget = cJSON_GetObjectItemCaseSensitive(pdu_set_qos_para_rmJSON, "pduSetDelayBudget");
    if (pdu_set_delay_budget) {
    if (!cJSON_IsNull(pdu_set_delay_budget)) {
    if (!cJSON_IsNumber(pdu_set_delay_budget)) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_parseFromJSON() failed [pdu_set_delay_budget]");
        goto end;
    }
    }
    }

    pdu_set_err_rate = cJSON_GetObjectItemCaseSensitive(pdu_set_qos_para_rmJSON, "pduSetErrRate");
    if (pdu_set_err_rate) {
    if (!cJSON_IsNull(pdu_set_err_rate)) {
    if (!cJSON_IsString(pdu_set_err_rate) && !cJSON_IsNull(pdu_set_err_rate)) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_parseFromJSON() failed [pdu_set_err_rate]");
        goto end;
    }
    }
    }

    pdu_set_handling_info = cJSON_GetObjectItemCaseSensitive(pdu_set_qos_para_rmJSON, "pduSetHandlingInfo");
    if (pdu_set_handling_info) {
    if (!cJSON_IsString(pdu_set_handling_info)) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_parseFromJSON() failed [pdu_set_handling_info]");
        goto end;
    }
    pdu_set_handling_infoVariable = OpenAPI_pdu_set_handling_info_FromString(pdu_set_handling_info->valuestring);
    }

    pdu_set_qos_para_rm_local_var = OpenAPI_pdu_set_qos_para_rm_create (
        pdu_set_delay_budget && cJSON_IsNull(pdu_set_delay_budget) ? true : false,
        pdu_set_delay_budget ? true : false,
        pdu_set_delay_budget ? pdu_set_delay_budget->valuedouble : 0,
        pdu_set_err_rate && cJSON_IsNull(pdu_set_err_rate) ? true : false,
        pdu_set_err_rate && !cJSON_IsNull(pdu_set_err_rate) ? ogs_strdup(pdu_set_err_rate->valuestring) : NULL,
        pdu_set_handling_info ? pdu_set_handling_infoVariable : 0
    );

    return pdu_set_qos_para_rm_local_var;
end:
    return NULL;
}

OpenAPI_pdu_set_qos_para_rm_t *OpenAPI_pdu_set_qos_para_rm_copy(OpenAPI_pdu_set_qos_para_rm_t *dst, OpenAPI_pdu_set_qos_para_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_set_qos_para_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_set_qos_para_rm_convertToJSON() failed");
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

    OpenAPI_pdu_set_qos_para_rm_free(dst);
    dst = OpenAPI_pdu_set_qos_para_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

