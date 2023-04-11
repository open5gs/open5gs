
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nf_load_level_information.h"

OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_create(
    OpenAPI_nf_type_e nf_type,
    char *nf_instance_id,
    char *nf_set_id,
    OpenAPI_nnwdaf_nf_status_t *nf_status,
    bool is_nf_cpu_usage,
    int nf_cpu_usage,
    bool is_nf_memory_usage,
    int nf_memory_usage,
    bool is_nf_storage_usage,
    int nf_storage_usage,
    bool is_nf_load_level_average,
    int nf_load_level_average,
    bool is_nf_load_levelpeak,
    int nf_load_levelpeak,
    bool is_nf_load_avg_in_aoi,
    int nf_load_avg_in_aoi,
    OpenAPI_snssai_t *snssai,
    bool is_confidence,
    int confidence
)
{
    OpenAPI_nf_load_level_information_t *nf_load_level_information_local_var = ogs_malloc(sizeof(OpenAPI_nf_load_level_information_t));
    ogs_assert(nf_load_level_information_local_var);

    nf_load_level_information_local_var->nf_type = nf_type;
    nf_load_level_information_local_var->nf_instance_id = nf_instance_id;
    nf_load_level_information_local_var->nf_set_id = nf_set_id;
    nf_load_level_information_local_var->nf_status = nf_status;
    nf_load_level_information_local_var->is_nf_cpu_usage = is_nf_cpu_usage;
    nf_load_level_information_local_var->nf_cpu_usage = nf_cpu_usage;
    nf_load_level_information_local_var->is_nf_memory_usage = is_nf_memory_usage;
    nf_load_level_information_local_var->nf_memory_usage = nf_memory_usage;
    nf_load_level_information_local_var->is_nf_storage_usage = is_nf_storage_usage;
    nf_load_level_information_local_var->nf_storage_usage = nf_storage_usage;
    nf_load_level_information_local_var->is_nf_load_level_average = is_nf_load_level_average;
    nf_load_level_information_local_var->nf_load_level_average = nf_load_level_average;
    nf_load_level_information_local_var->is_nf_load_levelpeak = is_nf_load_levelpeak;
    nf_load_level_information_local_var->nf_load_levelpeak = nf_load_levelpeak;
    nf_load_level_information_local_var->is_nf_load_avg_in_aoi = is_nf_load_avg_in_aoi;
    nf_load_level_information_local_var->nf_load_avg_in_aoi = nf_load_avg_in_aoi;
    nf_load_level_information_local_var->snssai = snssai;
    nf_load_level_information_local_var->is_confidence = is_confidence;
    nf_load_level_information_local_var->confidence = confidence;

    return nf_load_level_information_local_var;
}

void OpenAPI_nf_load_level_information_free(OpenAPI_nf_load_level_information_t *nf_load_level_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nf_load_level_information) {
        return;
    }
    if (nf_load_level_information->nf_instance_id) {
        ogs_free(nf_load_level_information->nf_instance_id);
        nf_load_level_information->nf_instance_id = NULL;
    }
    if (nf_load_level_information->nf_set_id) {
        ogs_free(nf_load_level_information->nf_set_id);
        nf_load_level_information->nf_set_id = NULL;
    }
    if (nf_load_level_information->nf_status) {
        OpenAPI_nnwdaf_nf_status_free(nf_load_level_information->nf_status);
        nf_load_level_information->nf_status = NULL;
    }
    if (nf_load_level_information->snssai) {
        OpenAPI_snssai_free(nf_load_level_information->snssai);
        nf_load_level_information->snssai = NULL;
    }
    ogs_free(nf_load_level_information);
}

cJSON *OpenAPI_nf_load_level_information_convertToJSON(OpenAPI_nf_load_level_information_t *nf_load_level_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nf_load_level_information == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [NfLoadLevelInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nf_load_level_information->nf_type != OpenAPI_nf_type_NULL) {
    if (cJSON_AddStringToObject(item, "nfType", OpenAPI_nf_type_ToString(nf_load_level_information->nf_type)) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_type]");
        goto end;
    }
    }

    if (nf_load_level_information->nf_instance_id) {
    if (cJSON_AddStringToObject(item, "nfInstanceId", nf_load_level_information->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    if (nf_load_level_information->nf_set_id) {
    if (cJSON_AddStringToObject(item, "nfSetId", nf_load_level_information->nf_set_id) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_set_id]");
        goto end;
    }
    }

    if (nf_load_level_information->nf_status) {
    cJSON *nf_status_local_JSON = OpenAPI_nnwdaf_nf_status_convertToJSON(nf_load_level_information->nf_status);
    if (nf_status_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nfStatus", nf_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_status]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_cpu_usage) {
    if (cJSON_AddNumberToObject(item, "nfCpuUsage", nf_load_level_information->nf_cpu_usage) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_cpu_usage]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_memory_usage) {
    if (cJSON_AddNumberToObject(item, "nfMemoryUsage", nf_load_level_information->nf_memory_usage) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_memory_usage]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_storage_usage) {
    if (cJSON_AddNumberToObject(item, "nfStorageUsage", nf_load_level_information->nf_storage_usage) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_storage_usage]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_load_level_average) {
    if (cJSON_AddNumberToObject(item, "nfLoadLevelAverage", nf_load_level_information->nf_load_level_average) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_load_level_average]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_load_levelpeak) {
    if (cJSON_AddNumberToObject(item, "nfLoadLevelpeak", nf_load_level_information->nf_load_levelpeak) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_load_levelpeak]");
        goto end;
    }
    }

    if (nf_load_level_information->is_nf_load_avg_in_aoi) {
    if (cJSON_AddNumberToObject(item, "nfLoadAvgInAoi", nf_load_level_information->nf_load_avg_in_aoi) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [nf_load_avg_in_aoi]");
        goto end;
    }
    }

    if (nf_load_level_information->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(nf_load_level_information->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (nf_load_level_information->is_confidence) {
    if (cJSON_AddNumberToObject(item, "confidence", nf_load_level_information->confidence) == NULL) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed [confidence]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_parseFromJSON(cJSON *nf_load_level_informationJSON)
{
    OpenAPI_nf_load_level_information_t *nf_load_level_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_type = NULL;
    OpenAPI_nf_type_e nf_typeVariable = 0;
    cJSON *nf_instance_id = NULL;
    cJSON *nf_set_id = NULL;
    cJSON *nf_status = NULL;
    OpenAPI_nnwdaf_nf_status_t *nf_status_local_nonprim = NULL;
    cJSON *nf_cpu_usage = NULL;
    cJSON *nf_memory_usage = NULL;
    cJSON *nf_storage_usage = NULL;
    cJSON *nf_load_level_average = NULL;
    cJSON *nf_load_levelpeak = NULL;
    cJSON *nf_load_avg_in_aoi = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *confidence = NULL;
    nf_type = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfType");
    if (nf_type) {
    if (!cJSON_IsString(nf_type)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_type]");
        goto end;
    }
    nf_typeVariable = OpenAPI_nf_type_FromString(nf_type->valuestring);
    }

    nf_instance_id = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfInstanceId");
    if (nf_instance_id) {
    if (!cJSON_IsString(nf_instance_id) && !cJSON_IsNull(nf_instance_id)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    }

    nf_set_id = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfSetId");
    if (nf_set_id) {
    if (!cJSON_IsString(nf_set_id) && !cJSON_IsNull(nf_set_id)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_set_id]");
        goto end;
    }
    }

    nf_status = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfStatus");
    if (nf_status) {
    nf_status_local_nonprim = OpenAPI_nnwdaf_nf_status_parseFromJSON(nf_status);
    if (!nf_status_local_nonprim) {
        ogs_error("OpenAPI_nnwdaf_nf_status_parseFromJSON failed [nf_status]");
        goto end;
    }
    }

    nf_cpu_usage = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfCpuUsage");
    if (nf_cpu_usage) {
    if (!cJSON_IsNumber(nf_cpu_usage)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_cpu_usage]");
        goto end;
    }
    }

    nf_memory_usage = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfMemoryUsage");
    if (nf_memory_usage) {
    if (!cJSON_IsNumber(nf_memory_usage)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_memory_usage]");
        goto end;
    }
    }

    nf_storage_usage = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfStorageUsage");
    if (nf_storage_usage) {
    if (!cJSON_IsNumber(nf_storage_usage)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_storage_usage]");
        goto end;
    }
    }

    nf_load_level_average = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfLoadLevelAverage");
    if (nf_load_level_average) {
    if (!cJSON_IsNumber(nf_load_level_average)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_load_level_average]");
        goto end;
    }
    }

    nf_load_levelpeak = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfLoadLevelpeak");
    if (nf_load_levelpeak) {
    if (!cJSON_IsNumber(nf_load_levelpeak)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_load_levelpeak]");
        goto end;
    }
    }

    nf_load_avg_in_aoi = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "nfLoadAvgInAoi");
    if (nf_load_avg_in_aoi) {
    if (!cJSON_IsNumber(nf_load_avg_in_aoi)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [nf_load_avg_in_aoi]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    confidence = cJSON_GetObjectItemCaseSensitive(nf_load_level_informationJSON, "confidence");
    if (confidence) {
    if (!cJSON_IsNumber(confidence)) {
        ogs_error("OpenAPI_nf_load_level_information_parseFromJSON() failed [confidence]");
        goto end;
    }
    }

    nf_load_level_information_local_var = OpenAPI_nf_load_level_information_create (
        nf_type ? nf_typeVariable : 0,
        nf_instance_id && !cJSON_IsNull(nf_instance_id) ? ogs_strdup(nf_instance_id->valuestring) : NULL,
        nf_set_id && !cJSON_IsNull(nf_set_id) ? ogs_strdup(nf_set_id->valuestring) : NULL,
        nf_status ? nf_status_local_nonprim : NULL,
        nf_cpu_usage ? true : false,
        nf_cpu_usage ? nf_cpu_usage->valuedouble : 0,
        nf_memory_usage ? true : false,
        nf_memory_usage ? nf_memory_usage->valuedouble : 0,
        nf_storage_usage ? true : false,
        nf_storage_usage ? nf_storage_usage->valuedouble : 0,
        nf_load_level_average ? true : false,
        nf_load_level_average ? nf_load_level_average->valuedouble : 0,
        nf_load_levelpeak ? true : false,
        nf_load_levelpeak ? nf_load_levelpeak->valuedouble : 0,
        nf_load_avg_in_aoi ? true : false,
        nf_load_avg_in_aoi ? nf_load_avg_in_aoi->valuedouble : 0,
        snssai ? snssai_local_nonprim : NULL,
        confidence ? true : false,
        confidence ? confidence->valuedouble : 0
    );

    return nf_load_level_information_local_var;
end:
    if (nf_status_local_nonprim) {
        OpenAPI_nnwdaf_nf_status_free(nf_status_local_nonprim);
        nf_status_local_nonprim = NULL;
    }
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nf_load_level_information_t *OpenAPI_nf_load_level_information_copy(OpenAPI_nf_load_level_information_t *dst, OpenAPI_nf_load_level_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nf_load_level_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nf_load_level_information_convertToJSON() failed");
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

    OpenAPI_nf_load_level_information_free(dst);
    dst = OpenAPI_nf_load_level_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

