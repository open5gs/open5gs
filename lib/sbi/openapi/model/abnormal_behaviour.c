
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "abnormal_behaviour.h"

OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_create(
    OpenAPI_list_t *supis,
    OpenAPI_exception_t *excep,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    bool is_ratio,
    int ratio,
    bool is_confidence,
    int confidence,
    OpenAPI_additional_measurement_t *addt_meas_info
)
{
    OpenAPI_abnormal_behaviour_t *abnormal_behaviour_local_var = ogs_malloc(sizeof(OpenAPI_abnormal_behaviour_t));
    ogs_assert(abnormal_behaviour_local_var);

    abnormal_behaviour_local_var->supis = supis;
    abnormal_behaviour_local_var->excep = excep;
    abnormal_behaviour_local_var->dnn = dnn;
    abnormal_behaviour_local_var->snssai = snssai;
    abnormal_behaviour_local_var->is_ratio = is_ratio;
    abnormal_behaviour_local_var->ratio = ratio;
    abnormal_behaviour_local_var->is_confidence = is_confidence;
    abnormal_behaviour_local_var->confidence = confidence;
    abnormal_behaviour_local_var->addt_meas_info = addt_meas_info;

    return abnormal_behaviour_local_var;
}

void OpenAPI_abnormal_behaviour_free(OpenAPI_abnormal_behaviour_t *abnormal_behaviour)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == abnormal_behaviour) {
        return;
    }
    if (abnormal_behaviour->supis) {
        OpenAPI_list_for_each(abnormal_behaviour->supis, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(abnormal_behaviour->supis);
        abnormal_behaviour->supis = NULL;
    }
    if (abnormal_behaviour->excep) {
        OpenAPI_exception_free(abnormal_behaviour->excep);
        abnormal_behaviour->excep = NULL;
    }
    if (abnormal_behaviour->dnn) {
        ogs_free(abnormal_behaviour->dnn);
        abnormal_behaviour->dnn = NULL;
    }
    if (abnormal_behaviour->snssai) {
        OpenAPI_snssai_free(abnormal_behaviour->snssai);
        abnormal_behaviour->snssai = NULL;
    }
    if (abnormal_behaviour->addt_meas_info) {
        OpenAPI_additional_measurement_free(abnormal_behaviour->addt_meas_info);
        abnormal_behaviour->addt_meas_info = NULL;
    }
    ogs_free(abnormal_behaviour);
}

cJSON *OpenAPI_abnormal_behaviour_convertToJSON(OpenAPI_abnormal_behaviour_t *abnormal_behaviour)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (abnormal_behaviour == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [AbnormalBehaviour]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (abnormal_behaviour->supis) {
    cJSON *supisList = cJSON_AddArrayToObject(item, "supis");
    if (supisList == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [supis]");
        goto end;
    }
    OpenAPI_list_for_each(abnormal_behaviour->supis, node) {
        if (cJSON_AddStringToObject(supisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [supis]");
            goto end;
        }
    }
    }

    if (!abnormal_behaviour->excep) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [excep]");
        return NULL;
    }
    cJSON *excep_local_JSON = OpenAPI_exception_convertToJSON(abnormal_behaviour->excep);
    if (excep_local_JSON == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [excep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "excep", excep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [excep]");
        goto end;
    }

    if (abnormal_behaviour->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", abnormal_behaviour->dnn) == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (abnormal_behaviour->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(abnormal_behaviour->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (abnormal_behaviour->is_ratio) {
    if (cJSON_AddNumberToObject(item, "ratio", abnormal_behaviour->ratio) == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [ratio]");
        goto end;
    }
    }

    if (abnormal_behaviour->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", abnormal_behaviour->confidence) == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [confidence]");
        goto end;
    }
    }

    if (abnormal_behaviour->addt_meas_info) {
    cJSON *addt_meas_info_local_JSON = OpenAPI_additional_measurement_convertToJSON(abnormal_behaviour->addt_meas_info);
    if (addt_meas_info_local_JSON == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [addt_meas_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "addtMeasInfo", addt_meas_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed [addt_meas_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_parseFromJSON(cJSON *abnormal_behaviourJSON)
{
    OpenAPI_abnormal_behaviour_t *abnormal_behaviour_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *supis = NULL;
    OpenAPI_list_t *supisList = NULL;
    cJSON *excep = NULL;
    OpenAPI_exception_t *excep_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *ratio = NULL;
    cJSON *confidence = NULL;
    cJSON *addt_meas_info = NULL;
    OpenAPI_additional_measurement_t *addt_meas_info_local_nonprim = NULL;
    supis = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "supis");
    if (supis) {
        cJSON *supis_local = NULL;
        if (!cJSON_IsArray(supis)) {
            ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [supis]");
            goto end;
        }

        supisList = OpenAPI_list_create();

        cJSON_ArrayForEach(supis_local, supis) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supis_local)) {
                ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [supis]");
                goto end;
            }
            OpenAPI_list_add(supisList, ogs_strdup(supis_local->valuestring));
        }
    }

    excep = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "excep");
    if (!excep) {
        ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [excep]");
        goto end;
    }
    excep_local_nonprim = OpenAPI_exception_parseFromJSON(excep);
    if (!excep_local_nonprim) {
        ogs_error("OpenAPI_exception_parseFromJSON failed [excep]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    ratio = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "ratio");
    if (ratio) {
    if (!cJSON_IsNumber(ratio)) {
        ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [ratio]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_abnormal_behaviour_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    addt_meas_info = cJSON_GetObjectItemCaseSensitive(abnormal_behaviourJSON, "addtMeasInfo");
    if (addt_meas_info) {
    addt_meas_info_local_nonprim = OpenAPI_additional_measurement_parseFromJSON(addt_meas_info);
    if (!addt_meas_info_local_nonprim) {
        ogs_error("OpenAPI_additional_measurement_parseFromJSON failed [addt_meas_info]");
        goto end;
    }
    }

    abnormal_behaviour_local_var = OpenAPI_abnormal_behaviour_create (
        supis ? supisList : NULL,
        excep_local_nonprim,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        ratio ? true : false,
        ratio ? ratio->valuedouble : 0,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0,
        addt_meas_info ? addt_meas_info_local_nonprim : NULL
    );

    return abnormal_behaviour_local_var;
end:
    if (supisList) {
        OpenAPI_list_for_each(supisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supisList);
        supisList = NULL;
    }
    if (excep_local_nonprim) {
        OpenAPI_exception_free(excep_local_nonprim);
        excep_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (addt_meas_info_local_nonprim) {
        OpenAPI_additional_measurement_free(addt_meas_info_local_nonprim);
        addt_meas_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_abnormal_behaviour_t *OpenAPI_abnormal_behaviour_copy(OpenAPI_abnormal_behaviour_t *dst, OpenAPI_abnormal_behaviour_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_abnormal_behaviour_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_abnormal_behaviour_convertToJSON() failed");
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

    OpenAPI_abnormal_behaviour_free(dst);
    dst = OpenAPI_abnormal_behaviour_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

