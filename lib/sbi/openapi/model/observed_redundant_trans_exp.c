
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "observed_redundant_trans_exp.h"

OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_create(
    bool is_avg_pkt_drop_rate_ul,
    int avg_pkt_drop_rate_ul,
    bool is_var_pkt_drop_rate_ul,
    float var_pkt_drop_rate_ul,
    bool is_avg_pkt_drop_rate_dl,
    int avg_pkt_drop_rate_dl,
    bool is_var_pkt_drop_rate_dl,
    float var_pkt_drop_rate_dl,
    bool is_avg_pkt_delay_ul,
    int avg_pkt_delay_ul,
    bool is_var_pkt_delay_ul,
    float var_pkt_delay_ul,
    bool is_avg_pkt_delay_dl,
    int avg_pkt_delay_dl,
    bool is_var_pkt_delay_dl,
    float var_pkt_delay_dl
)
{
    OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp_local_var = ogs_malloc(sizeof(OpenAPI_observed_redundant_trans_exp_t));
    ogs_assert(observed_redundant_trans_exp_local_var);

    observed_redundant_trans_exp_local_var->is_avg_pkt_drop_rate_ul = is_avg_pkt_drop_rate_ul;
    observed_redundant_trans_exp_local_var->avg_pkt_drop_rate_ul = avg_pkt_drop_rate_ul;
    observed_redundant_trans_exp_local_var->is_var_pkt_drop_rate_ul = is_var_pkt_drop_rate_ul;
    observed_redundant_trans_exp_local_var->var_pkt_drop_rate_ul = var_pkt_drop_rate_ul;
    observed_redundant_trans_exp_local_var->is_avg_pkt_drop_rate_dl = is_avg_pkt_drop_rate_dl;
    observed_redundant_trans_exp_local_var->avg_pkt_drop_rate_dl = avg_pkt_drop_rate_dl;
    observed_redundant_trans_exp_local_var->is_var_pkt_drop_rate_dl = is_var_pkt_drop_rate_dl;
    observed_redundant_trans_exp_local_var->var_pkt_drop_rate_dl = var_pkt_drop_rate_dl;
    observed_redundant_trans_exp_local_var->is_avg_pkt_delay_ul = is_avg_pkt_delay_ul;
    observed_redundant_trans_exp_local_var->avg_pkt_delay_ul = avg_pkt_delay_ul;
    observed_redundant_trans_exp_local_var->is_var_pkt_delay_ul = is_var_pkt_delay_ul;
    observed_redundant_trans_exp_local_var->var_pkt_delay_ul = var_pkt_delay_ul;
    observed_redundant_trans_exp_local_var->is_avg_pkt_delay_dl = is_avg_pkt_delay_dl;
    observed_redundant_trans_exp_local_var->avg_pkt_delay_dl = avg_pkt_delay_dl;
    observed_redundant_trans_exp_local_var->is_var_pkt_delay_dl = is_var_pkt_delay_dl;
    observed_redundant_trans_exp_local_var->var_pkt_delay_dl = var_pkt_delay_dl;

    return observed_redundant_trans_exp_local_var;
}

void OpenAPI_observed_redundant_trans_exp_free(OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == observed_redundant_trans_exp) {
        return;
    }
    ogs_free(observed_redundant_trans_exp);
}

cJSON *OpenAPI_observed_redundant_trans_exp_convertToJSON(OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (observed_redundant_trans_exp == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [ObservedRedundantTransExp]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (observed_redundant_trans_exp->is_avg_pkt_drop_rate_ul) {
    if (cJSON_AddNumberToObject(item, "avgPktDropRateUl", observed_redundant_trans_exp->avg_pkt_drop_rate_ul) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [avg_pkt_drop_rate_ul]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_var_pkt_drop_rate_ul) {
    if (cJSON_AddNumberToObject(item, "varPktDropRateUl", observed_redundant_trans_exp->var_pkt_drop_rate_ul) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [var_pkt_drop_rate_ul]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_avg_pkt_drop_rate_dl) {
    if (cJSON_AddNumberToObject(item, "avgPktDropRateDl", observed_redundant_trans_exp->avg_pkt_drop_rate_dl) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [avg_pkt_drop_rate_dl]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_var_pkt_drop_rate_dl) {
    if (cJSON_AddNumberToObject(item, "varPktDropRateDl", observed_redundant_trans_exp->var_pkt_drop_rate_dl) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [var_pkt_drop_rate_dl]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_avg_pkt_delay_ul) {
    if (cJSON_AddNumberToObject(item, "avgPktDelayUl", observed_redundant_trans_exp->avg_pkt_delay_ul) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [avg_pkt_delay_ul]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_var_pkt_delay_ul) {
    if (cJSON_AddNumberToObject(item, "varPktDelayUl", observed_redundant_trans_exp->var_pkt_delay_ul) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [var_pkt_delay_ul]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_avg_pkt_delay_dl) {
    if (cJSON_AddNumberToObject(item, "avgPktDelayDl", observed_redundant_trans_exp->avg_pkt_delay_dl) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [avg_pkt_delay_dl]");
        goto end;
    }
    }

    if (observed_redundant_trans_exp->is_var_pkt_delay_dl) {
    if (cJSON_AddNumberToObject(item, "varPktDelayDl", observed_redundant_trans_exp->var_pkt_delay_dl) == NULL) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed [var_pkt_delay_dl]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_parseFromJSON(cJSON *observed_redundant_trans_expJSON)
{
    OpenAPI_observed_redundant_trans_exp_t *observed_redundant_trans_exp_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *avg_pkt_drop_rate_ul = NULL;
    cJSON *var_pkt_drop_rate_ul = NULL;
    cJSON *avg_pkt_drop_rate_dl = NULL;
    cJSON *var_pkt_drop_rate_dl = NULL;
    cJSON *avg_pkt_delay_ul = NULL;
    cJSON *var_pkt_delay_ul = NULL;
    cJSON *avg_pkt_delay_dl = NULL;
    cJSON *var_pkt_delay_dl = NULL;
    avg_pkt_drop_rate_ul = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "avgPktDropRateUl");
    if (avg_pkt_drop_rate_ul) {
    if (!cJSON_IsNumber(avg_pkt_drop_rate_ul)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [avg_pkt_drop_rate_ul]");
        goto end;
    }
    }

    var_pkt_drop_rate_ul = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "varPktDropRateUl");
    if (var_pkt_drop_rate_ul) {
    if (!cJSON_IsNumber(var_pkt_drop_rate_ul)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [var_pkt_drop_rate_ul]");
        goto end;
    }
    }

    avg_pkt_drop_rate_dl = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "avgPktDropRateDl");
    if (avg_pkt_drop_rate_dl) {
    if (!cJSON_IsNumber(avg_pkt_drop_rate_dl)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [avg_pkt_drop_rate_dl]");
        goto end;
    }
    }

    var_pkt_drop_rate_dl = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "varPktDropRateDl");
    if (var_pkt_drop_rate_dl) {
    if (!cJSON_IsNumber(var_pkt_drop_rate_dl)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [var_pkt_drop_rate_dl]");
        goto end;
    }
    }

    avg_pkt_delay_ul = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "avgPktDelayUl");
    if (avg_pkt_delay_ul) {
    if (!cJSON_IsNumber(avg_pkt_delay_ul)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [avg_pkt_delay_ul]");
        goto end;
    }
    }

    var_pkt_delay_ul = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "varPktDelayUl");
    if (var_pkt_delay_ul) {
    if (!cJSON_IsNumber(var_pkt_delay_ul)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [var_pkt_delay_ul]");
        goto end;
    }
    }

    avg_pkt_delay_dl = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "avgPktDelayDl");
    if (avg_pkt_delay_dl) {
    if (!cJSON_IsNumber(avg_pkt_delay_dl)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [avg_pkt_delay_dl]");
        goto end;
    }
    }

    var_pkt_delay_dl = cJSON_GetObjectItemCaseSensitive(observed_redundant_trans_expJSON, "varPktDelayDl");
    if (var_pkt_delay_dl) {
    if (!cJSON_IsNumber(var_pkt_delay_dl)) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_parseFromJSON() failed [var_pkt_delay_dl]");
        goto end;
    }
    }

    observed_redundant_trans_exp_local_var = OpenAPI_observed_redundant_trans_exp_create (
        avg_pkt_drop_rate_ul ? true : false,
        avg_pkt_drop_rate_ul ? avg_pkt_drop_rate_ul->valuedouble : 0,
        var_pkt_drop_rate_ul ? true : false,
        var_pkt_drop_rate_ul ? var_pkt_drop_rate_ul->valuedouble : 0,
        avg_pkt_drop_rate_dl ? true : false,
        avg_pkt_drop_rate_dl ? avg_pkt_drop_rate_dl->valuedouble : 0,
        var_pkt_drop_rate_dl ? true : false,
        var_pkt_drop_rate_dl ? var_pkt_drop_rate_dl->valuedouble : 0,
        avg_pkt_delay_ul ? true : false,
        avg_pkt_delay_ul ? avg_pkt_delay_ul->valuedouble : 0,
        var_pkt_delay_ul ? true : false,
        var_pkt_delay_ul ? var_pkt_delay_ul->valuedouble : 0,
        avg_pkt_delay_dl ? true : false,
        avg_pkt_delay_dl ? avg_pkt_delay_dl->valuedouble : 0,
        var_pkt_delay_dl ? true : false,
        var_pkt_delay_dl ? var_pkt_delay_dl->valuedouble : 0
    );

    return observed_redundant_trans_exp_local_var;
end:
    return NULL;
}

OpenAPI_observed_redundant_trans_exp_t *OpenAPI_observed_redundant_trans_exp_copy(OpenAPI_observed_redundant_trans_exp_t *dst, OpenAPI_observed_redundant_trans_exp_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_observed_redundant_trans_exp_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_observed_redundant_trans_exp_convertToJSON() failed");
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

    OpenAPI_observed_redundant_trans_exp_free(dst);
    dst = OpenAPI_observed_redundant_trans_exp_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

