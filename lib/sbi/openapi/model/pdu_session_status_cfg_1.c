
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_status_cfg_1.h"

OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_create(
    char *dnn
)
{
    OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_status_cfg_1_t));
    ogs_assert(pdu_session_status_cfg_1_local_var);

    pdu_session_status_cfg_1_local_var->dnn = dnn;

    return pdu_session_status_cfg_1_local_var;
}

void OpenAPI_pdu_session_status_cfg_1_free(OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_status_cfg_1) {
        return;
    }
    if (pdu_session_status_cfg_1->dnn) {
        ogs_free(pdu_session_status_cfg_1->dnn);
        pdu_session_status_cfg_1->dnn = NULL;
    }
    ogs_free(pdu_session_status_cfg_1);
}

cJSON *OpenAPI_pdu_session_status_cfg_1_convertToJSON(OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_status_cfg_1 == NULL) {
        ogs_error("OpenAPI_pdu_session_status_cfg_1_convertToJSON() failed [PduSessionStatusCfg_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_status_cfg_1->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_status_cfg_1->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_status_cfg_1_convertToJSON() failed [dnn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_parseFromJSON(cJSON *pdu_session_status_cfg_1JSON)
{
    OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_status_cfg_1JSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_pdu_session_status_cfg_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    pdu_session_status_cfg_1_local_var = OpenAPI_pdu_session_status_cfg_1_create (
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL
    );

    return pdu_session_status_cfg_1_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_status_cfg_1_t *OpenAPI_pdu_session_status_cfg_1_copy(OpenAPI_pdu_session_status_cfg_1_t *dst, OpenAPI_pdu_session_status_cfg_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_status_cfg_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_status_cfg_1_convertToJSON() failed");
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

    OpenAPI_pdu_session_status_cfg_1_free(dst);
    dst = OpenAPI_pdu_session_status_cfg_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

