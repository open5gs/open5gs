
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_status_cfg.h"

OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_create(
    char *dnn
)
{
    OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_status_cfg_t));
    ogs_assert(pdu_session_status_cfg_local_var);

    pdu_session_status_cfg_local_var->dnn = dnn;

    return pdu_session_status_cfg_local_var;
}

void OpenAPI_pdu_session_status_cfg_free(OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg)
{
    if (NULL == pdu_session_status_cfg) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_status_cfg->dnn);
    ogs_free(pdu_session_status_cfg);
}

cJSON *OpenAPI_pdu_session_status_cfg_convertToJSON(OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg)
{
    cJSON *item = NULL;

    if (pdu_session_status_cfg == NULL) {
        ogs_error("OpenAPI_pdu_session_status_cfg_convertToJSON() failed [PduSessionStatusCfg]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_status_cfg->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_status_cfg->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_status_cfg_convertToJSON() failed [dnn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_parseFromJSON(cJSON *pdu_session_status_cfgJSON)
{
    OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_status_cfgJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_status_cfg_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    pdu_session_status_cfg_local_var = OpenAPI_pdu_session_status_cfg_create (
        dnn ? ogs_strdup(dnn->valuestring) : NULL
    );

    return pdu_session_status_cfg_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_status_cfg_t *OpenAPI_pdu_session_status_cfg_copy(OpenAPI_pdu_session_status_cfg_t *dst, OpenAPI_pdu_session_status_cfg_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_status_cfg_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_status_cfg_convertToJSON() failed");
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

    OpenAPI_pdu_session_status_cfg_free(dst);
    dst = OpenAPI_pdu_session_status_cfg_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

