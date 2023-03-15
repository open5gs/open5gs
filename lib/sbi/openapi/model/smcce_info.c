
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smcce_info.h"

OpenAPI_smcce_info_t *OpenAPI_smcce_info_create(
    char *dnn,
    OpenAPI_snssai_t *snssai,
    OpenAPI_smcce_ue_list_t *smcce_ue_list
)
{
    OpenAPI_smcce_info_t *smcce_info_local_var = ogs_malloc(sizeof(OpenAPI_smcce_info_t));
    ogs_assert(smcce_info_local_var);

    smcce_info_local_var->dnn = dnn;
    smcce_info_local_var->snssai = snssai;
    smcce_info_local_var->smcce_ue_list = smcce_ue_list;

    return smcce_info_local_var;
}

void OpenAPI_smcce_info_free(OpenAPI_smcce_info_t *smcce_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == smcce_info) {
        return;
    }
    if (smcce_info->dnn) {
        ogs_free(smcce_info->dnn);
        smcce_info->dnn = NULL;
    }
    if (smcce_info->snssai) {
        OpenAPI_snssai_free(smcce_info->snssai);
        smcce_info->snssai = NULL;
    }
    if (smcce_info->smcce_ue_list) {
        OpenAPI_smcce_ue_list_free(smcce_info->smcce_ue_list);
        smcce_info->smcce_ue_list = NULL;
    }
    ogs_free(smcce_info);
}

cJSON *OpenAPI_smcce_info_convertToJSON(OpenAPI_smcce_info_t *smcce_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (smcce_info == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [SmcceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (smcce_info->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", smcce_info->dnn) == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (smcce_info->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(smcce_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (!smcce_info->smcce_ue_list) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [smcce_ue_list]");
        return NULL;
    }
    cJSON *smcce_ue_list_local_JSON = OpenAPI_smcce_ue_list_convertToJSON(smcce_info->smcce_ue_list);
    if (smcce_ue_list_local_JSON == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [smcce_ue_list]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smcceUeList", smcce_ue_list_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed [smcce_ue_list]");
        goto end;
    }

end:
    return item;
}

OpenAPI_smcce_info_t *OpenAPI_smcce_info_parseFromJSON(cJSON *smcce_infoJSON)
{
    OpenAPI_smcce_info_t *smcce_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *smcce_ue_list = NULL;
    OpenAPI_smcce_ue_list_t *smcce_ue_list_local_nonprim = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(smcce_infoJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_smcce_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(smcce_infoJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    smcce_ue_list = cJSON_GetObjectItemCaseSensitive(smcce_infoJSON, "smcceUeList");
    if (!smcce_ue_list) {
        ogs_error("OpenAPI_smcce_info_parseFromJSON() failed [smcce_ue_list]");
        goto end;
    }
    smcce_ue_list_local_nonprim = OpenAPI_smcce_ue_list_parseFromJSON(smcce_ue_list);
    if (!smcce_ue_list_local_nonprim) {
        ogs_error("OpenAPI_smcce_ue_list_parseFromJSON failed [smcce_ue_list]");
        goto end;
    }

    smcce_info_local_var = OpenAPI_smcce_info_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        smcce_ue_list_local_nonprim
    );

    return smcce_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (smcce_ue_list_local_nonprim) {
        OpenAPI_smcce_ue_list_free(smcce_ue_list_local_nonprim);
        smcce_ue_list_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_smcce_info_t *OpenAPI_smcce_info_copy(OpenAPI_smcce_info_t *dst, OpenAPI_smcce_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smcce_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smcce_info_convertToJSON() failed");
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

    OpenAPI_smcce_info_free(dst);
    dst = OpenAPI_smcce_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

