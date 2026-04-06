
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "accuracy_info.h"

OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_create(
    int accuracy_val,
    bool is_accu_sample_nbr,
    int accu_sample_nbr,
    OpenAPI_analytics_accuracy_indication_e ana_accu_ind
)
{
    OpenAPI_accuracy_info_t *accuracy_info_local_var = ogs_malloc(sizeof(OpenAPI_accuracy_info_t));
    ogs_assert(accuracy_info_local_var);

    accuracy_info_local_var->accuracy_val = accuracy_val;
    accuracy_info_local_var->is_accu_sample_nbr = is_accu_sample_nbr;
    accuracy_info_local_var->accu_sample_nbr = accu_sample_nbr;
    accuracy_info_local_var->ana_accu_ind = ana_accu_ind;

    return accuracy_info_local_var;
}

void OpenAPI_accuracy_info_free(OpenAPI_accuracy_info_t *accuracy_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == accuracy_info) {
        return;
    }
    ogs_free(accuracy_info);
}

cJSON *OpenAPI_accuracy_info_convertToJSON(OpenAPI_accuracy_info_t *accuracy_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (accuracy_info == NULL) {
        ogs_error("OpenAPI_accuracy_info_convertToJSON() failed [AccuracyInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "accuracyVal", accuracy_info->accuracy_val) == NULL) {
        ogs_error("OpenAPI_accuracy_info_convertToJSON() failed [accuracy_val]");
        goto end;
    }

    if (accuracy_info->is_accu_sample_nbr) {
    if (cJSON_AddNumberToObject(item, "accuSampleNbr", accuracy_info->accu_sample_nbr) == NULL) {
        ogs_error("OpenAPI_accuracy_info_convertToJSON() failed [accu_sample_nbr]");
        goto end;
    }
    }

    if (accuracy_info->ana_accu_ind != OpenAPI_analytics_accuracy_indication_NULL) {
    if (cJSON_AddStringToObject(item, "anaAccuInd", OpenAPI_analytics_accuracy_indication_ToString(accuracy_info->ana_accu_ind)) == NULL) {
        ogs_error("OpenAPI_accuracy_info_convertToJSON() failed [ana_accu_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_parseFromJSON(cJSON *accuracy_infoJSON)
{
    OpenAPI_accuracy_info_t *accuracy_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *accuracy_val = NULL;
    cJSON *accu_sample_nbr = NULL;
    cJSON *ana_accu_ind = NULL;
    OpenAPI_analytics_accuracy_indication_e ana_accu_indVariable = 0;
    accuracy_val = cJSON_GetObjectItemCaseSensitive(accuracy_infoJSON, "accuracyVal");
    if (!accuracy_val) {
        ogs_error("OpenAPI_accuracy_info_parseFromJSON() failed [accuracy_val]");
        goto end;
    }
    if (!cJSON_IsNumber(accuracy_val)) {
        ogs_error("OpenAPI_accuracy_info_parseFromJSON() failed [accuracy_val]");
        goto end;
    }

    accu_sample_nbr = cJSON_GetObjectItemCaseSensitive(accuracy_infoJSON, "accuSampleNbr");
    if (accu_sample_nbr) {
    if (!cJSON_IsNumber(accu_sample_nbr)) {
        ogs_error("OpenAPI_accuracy_info_parseFromJSON() failed [accu_sample_nbr]");
        goto end;
    }
    }

    ana_accu_ind = cJSON_GetObjectItemCaseSensitive(accuracy_infoJSON, "anaAccuInd");
    if (ana_accu_ind) {
    if (!cJSON_IsString(ana_accu_ind)) {
        ogs_error("OpenAPI_accuracy_info_parseFromJSON() failed [ana_accu_ind]");
        goto end;
    }
    ana_accu_indVariable = OpenAPI_analytics_accuracy_indication_FromString(ana_accu_ind->valuestring);
    }

    accuracy_info_local_var = OpenAPI_accuracy_info_create (
        
        accuracy_val->valuedouble,
        accu_sample_nbr ? true : false,
        accu_sample_nbr ? accu_sample_nbr->valuedouble : 0,
        ana_accu_ind ? ana_accu_indVariable : 0
    );

    return accuracy_info_local_var;
end:
    return NULL;
}

OpenAPI_accuracy_info_t *OpenAPI_accuracy_info_copy(OpenAPI_accuracy_info_t *dst, OpenAPI_accuracy_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_accuracy_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_accuracy_info_convertToJSON() failed");
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

    OpenAPI_accuracy_info_free(dst);
    dst = OpenAPI_accuracy_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

