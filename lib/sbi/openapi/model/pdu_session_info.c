
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_info.h"

OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_create(
    OpenAPI_snssai_t *snssai,
    char *dnn
)
{
    OpenAPI_pdu_session_info_t *pdu_session_info_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_info_t));
    ogs_assert(pdu_session_info_local_var);

    pdu_session_info_local_var->snssai = snssai;
    pdu_session_info_local_var->dnn = dnn;

    return pdu_session_info_local_var;
}

void OpenAPI_pdu_session_info_free(OpenAPI_pdu_session_info_t *pdu_session_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_info) {
        return;
    }
    if (pdu_session_info->snssai) {
        OpenAPI_snssai_free(pdu_session_info->snssai);
        pdu_session_info->snssai = NULL;
    }
    if (pdu_session_info->dnn) {
        ogs_free(pdu_session_info->dnn);
        pdu_session_info->dnn = NULL;
    }
    ogs_free(pdu_session_info);
}

cJSON *OpenAPI_pdu_session_info_convertToJSON(OpenAPI_pdu_session_info_t *pdu_session_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_info == NULL) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [PduSessionInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session_info->snssai) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_info->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [snssai]");
        goto end;
    }

    if (!pdu_session_info->dnn) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_info->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed [dnn]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_parseFromJSON(cJSON *pdu_session_infoJSON)
{
    OpenAPI_pdu_session_info_t *pdu_session_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(pdu_session_infoJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_pdu_session_info_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_infoJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_info_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_info_parseFromJSON() failed [dnn]");
        goto end;
    }

    pdu_session_info_local_var = OpenAPI_pdu_session_info_create (
        snssai_local_nonprim,
        ogs_strdup(dnn->valuestring)
    );

    return pdu_session_info_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_info_t *OpenAPI_pdu_session_info_copy(OpenAPI_pdu_session_info_t *dst, OpenAPI_pdu_session_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_info_convertToJSON() failed");
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

    OpenAPI_pdu_session_info_free(dst);
    dst = OpenAPI_pdu_session_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

