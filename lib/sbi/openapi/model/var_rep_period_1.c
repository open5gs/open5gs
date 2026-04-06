
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "var_rep_period_1.h"

OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_create(
    int rep_period,
    bool is_perc_value_nf_load,
    int perc_value_nf_load
)
{
    OpenAPI_var_rep_period_1_t *var_rep_period_1_local_var = ogs_malloc(sizeof(OpenAPI_var_rep_period_1_t));
    ogs_assert(var_rep_period_1_local_var);

    var_rep_period_1_local_var->rep_period = rep_period;
    var_rep_period_1_local_var->is_perc_value_nf_load = is_perc_value_nf_load;
    var_rep_period_1_local_var->perc_value_nf_load = perc_value_nf_load;

    return var_rep_period_1_local_var;
}

void OpenAPI_var_rep_period_1_free(OpenAPI_var_rep_period_1_t *var_rep_period_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == var_rep_period_1) {
        return;
    }
    ogs_free(var_rep_period_1);
}

cJSON *OpenAPI_var_rep_period_1_convertToJSON(OpenAPI_var_rep_period_1_t *var_rep_period_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (var_rep_period_1 == NULL) {
        ogs_error("OpenAPI_var_rep_period_1_convertToJSON() failed [VarRepPeriod_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "repPeriod", var_rep_period_1->rep_period) == NULL) {
        ogs_error("OpenAPI_var_rep_period_1_convertToJSON() failed [rep_period]");
        goto end;
    }

    if (var_rep_period_1->is_perc_value_nf_load) {
    if (cJSON_AddNumberToObject(item, "percValueNfLoad", var_rep_period_1->perc_value_nf_load) == NULL) {
        ogs_error("OpenAPI_var_rep_period_1_convertToJSON() failed [perc_value_nf_load]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_parseFromJSON(cJSON *var_rep_period_1JSON)
{
    OpenAPI_var_rep_period_1_t *var_rep_period_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rep_period = NULL;
    cJSON *perc_value_nf_load = NULL;
    rep_period = cJSON_GetObjectItemCaseSensitive(var_rep_period_1JSON, "repPeriod");
    if (!rep_period) {
        ogs_error("OpenAPI_var_rep_period_1_parseFromJSON() failed [rep_period]");
        goto end;
    }
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_var_rep_period_1_parseFromJSON() failed [rep_period]");
        goto end;
    }

    perc_value_nf_load = cJSON_GetObjectItemCaseSensitive(var_rep_period_1JSON, "percValueNfLoad");
    if (perc_value_nf_load) {
    if (!cJSON_IsNumber(perc_value_nf_load)) {
        ogs_error("OpenAPI_var_rep_period_1_parseFromJSON() failed [perc_value_nf_load]");
        goto end;
    }
    }

    var_rep_period_1_local_var = OpenAPI_var_rep_period_1_create (
        
        rep_period->valuedouble,
        perc_value_nf_load ? true : false,
        perc_value_nf_load ? perc_value_nf_load->valuedouble : 0
    );

    return var_rep_period_1_local_var;
end:
    return NULL;
}

OpenAPI_var_rep_period_1_t *OpenAPI_var_rep_period_1_copy(OpenAPI_var_rep_period_1_t *dst, OpenAPI_var_rep_period_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_var_rep_period_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_var_rep_period_1_convertToJSON() failed");
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

    OpenAPI_var_rep_period_1_free(dst);
    dst = OpenAPI_var_rep_period_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

