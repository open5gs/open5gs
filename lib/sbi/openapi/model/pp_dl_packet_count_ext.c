
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_dl_packet_count_ext.h"

OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_create(
    char *af_instance_id,
    int reference_id,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    char *validity_time,
    char *mtc_provider_information
)
{
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext_local_var = ogs_malloc(sizeof(OpenAPI_pp_dl_packet_count_ext_t));
    ogs_assert(pp_dl_packet_count_ext_local_var);

    pp_dl_packet_count_ext_local_var->af_instance_id = af_instance_id;
    pp_dl_packet_count_ext_local_var->reference_id = reference_id;
    pp_dl_packet_count_ext_local_var->dnn = dnn;
    pp_dl_packet_count_ext_local_var->single_nssai = single_nssai;
    pp_dl_packet_count_ext_local_var->validity_time = validity_time;
    pp_dl_packet_count_ext_local_var->mtc_provider_information = mtc_provider_information;

    return pp_dl_packet_count_ext_local_var;
}

void OpenAPI_pp_dl_packet_count_ext_free(OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_dl_packet_count_ext) {
        return;
    }
    if (pp_dl_packet_count_ext->af_instance_id) {
        ogs_free(pp_dl_packet_count_ext->af_instance_id);
        pp_dl_packet_count_ext->af_instance_id = NULL;
    }
    if (pp_dl_packet_count_ext->dnn) {
        ogs_free(pp_dl_packet_count_ext->dnn);
        pp_dl_packet_count_ext->dnn = NULL;
    }
    if (pp_dl_packet_count_ext->single_nssai) {
        OpenAPI_snssai_free(pp_dl_packet_count_ext->single_nssai);
        pp_dl_packet_count_ext->single_nssai = NULL;
    }
    if (pp_dl_packet_count_ext->validity_time) {
        ogs_free(pp_dl_packet_count_ext->validity_time);
        pp_dl_packet_count_ext->validity_time = NULL;
    }
    if (pp_dl_packet_count_ext->mtc_provider_information) {
        ogs_free(pp_dl_packet_count_ext->mtc_provider_information);
        pp_dl_packet_count_ext->mtc_provider_information = NULL;
    }
    ogs_free(pp_dl_packet_count_ext);
}

cJSON *OpenAPI_pp_dl_packet_count_ext_convertToJSON(OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_dl_packet_count_ext == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [PpDlPacketCountExt]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pp_dl_packet_count_ext->af_instance_id) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [af_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "afInstanceId", pp_dl_packet_count_ext->af_instance_id) == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [af_instance_id]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "referenceId", pp_dl_packet_count_ext->reference_id) == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [reference_id]");
        goto end;
    }

    if (pp_dl_packet_count_ext->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pp_dl_packet_count_ext->dnn) == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (pp_dl_packet_count_ext->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(pp_dl_packet_count_ext->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (pp_dl_packet_count_ext->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", pp_dl_packet_count_ext->validity_time) == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (pp_dl_packet_count_ext->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", pp_dl_packet_count_ext->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_parseFromJSON(cJSON *pp_dl_packet_count_extJSON)
{
    OpenAPI_pp_dl_packet_count_ext_t *pp_dl_packet_count_ext_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *reference_id = NULL;
    cJSON *dnn = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *validity_time = NULL;
    cJSON *mtc_provider_information = NULL;
    af_instance_id = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "afInstanceId");
    if (!af_instance_id) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(af_instance_id)) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [af_instance_id]");
        goto end;
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "referenceId");
    if (!reference_id) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [reference_id]");
        goto end;
    }
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [reference_id]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(pp_dl_packet_count_extJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    pp_dl_packet_count_ext_local_var = OpenAPI_pp_dl_packet_count_ext_create (
        ogs_strdup(af_instance_id->valuestring),
        
        reference_id->valuedouble,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return pp_dl_packet_count_ext_local_var;
end:
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pp_dl_packet_count_ext_t *OpenAPI_pp_dl_packet_count_ext_copy(OpenAPI_pp_dl_packet_count_ext_t *dst, OpenAPI_pp_dl_packet_count_ext_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_dl_packet_count_ext_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_dl_packet_count_ext_convertToJSON() failed");
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

    OpenAPI_pp_dl_packet_count_ext_free(dst);
    dst = OpenAPI_pp_dl_packet_count_ext_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

