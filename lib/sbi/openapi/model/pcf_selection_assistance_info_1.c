
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pcf_selection_assistance_info_1.h"

OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_create(
    char *dnn,
    OpenAPI_snssai_t *single_nssai
)
{
    OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1_local_var = ogs_malloc(sizeof(OpenAPI_pcf_selection_assistance_info_1_t));
    ogs_assert(pcf_selection_assistance_info_1_local_var);

    pcf_selection_assistance_info_1_local_var->dnn = dnn;
    pcf_selection_assistance_info_1_local_var->single_nssai = single_nssai;

    return pcf_selection_assistance_info_1_local_var;
}

void OpenAPI_pcf_selection_assistance_info_1_free(OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pcf_selection_assistance_info_1) {
        return;
    }
    if (pcf_selection_assistance_info_1->dnn) {
        ogs_free(pcf_selection_assistance_info_1->dnn);
        pcf_selection_assistance_info_1->dnn = NULL;
    }
    if (pcf_selection_assistance_info_1->single_nssai) {
        OpenAPI_snssai_free(pcf_selection_assistance_info_1->single_nssai);
        pcf_selection_assistance_info_1->single_nssai = NULL;
    }
    ogs_free(pcf_selection_assistance_info_1);
}

cJSON *OpenAPI_pcf_selection_assistance_info_1_convertToJSON(OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pcf_selection_assistance_info_1 == NULL) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [PcfSelectionAssistanceInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pcf_selection_assistance_info_1->dnn) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pcf_selection_assistance_info_1->dnn) == NULL) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!pcf_selection_assistance_info_1->single_nssai) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [single_nssai]");
        return NULL;
    }
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(pcf_selection_assistance_info_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed [single_nssai]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_parseFromJSON(cJSON *pcf_selection_assistance_info_1JSON)
{
    OpenAPI_pcf_selection_assistance_info_1_t *pcf_selection_assistance_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(pcf_selection_assistance_info_1JSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(pcf_selection_assistance_info_1JSON, "singleNssai");
    if (!single_nssai) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_parseFromJSON() failed [single_nssai]");
        goto end;
    }
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }

    pcf_selection_assistance_info_1_local_var = OpenAPI_pcf_selection_assistance_info_1_create (
        ogs_strdup(dnn->valuestring),
        single_nssai_local_nonprim
    );

    return pcf_selection_assistance_info_1_local_var;
end:
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pcf_selection_assistance_info_1_t *OpenAPI_pcf_selection_assistance_info_1_copy(OpenAPI_pcf_selection_assistance_info_1_t *dst, OpenAPI_pcf_selection_assistance_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pcf_selection_assistance_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pcf_selection_assistance_info_1_convertToJSON() failed");
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

    OpenAPI_pcf_selection_assistance_info_1_free(dst);
    dst = OpenAPI_pcf_selection_assistance_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

