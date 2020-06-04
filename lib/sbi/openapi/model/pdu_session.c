
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session.h"

OpenAPI_pdu_session_t *OpenAPI_pdu_session_create(
    char *dnn,
    char *smf_instance_id,
    OpenAPI_plmn_id_t *plmn_id
    )
{
    OpenAPI_pdu_session_t *pdu_session_local_var = OpenAPI_malloc(sizeof(OpenAPI_pdu_session_t));
    if (!pdu_session_local_var) {
        return NULL;
    }
    pdu_session_local_var->dnn = dnn;
    pdu_session_local_var->smf_instance_id = smf_instance_id;
    pdu_session_local_var->plmn_id = plmn_id;

    return pdu_session_local_var;
}

void OpenAPI_pdu_session_free(OpenAPI_pdu_session_t *pdu_session)
{
    if (NULL == pdu_session) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session->dnn);
    ogs_free(pdu_session->smf_instance_id);
    OpenAPI_plmn_id_free(pdu_session->plmn_id);
    ogs_free(pdu_session);
}

cJSON *OpenAPI_pdu_session_convertToJSON(OpenAPI_pdu_session_t *pdu_session)
{
    cJSON *item = NULL;

    if (pdu_session == NULL) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [PduSession]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session->dnn) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", pdu_session->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!pdu_session->smf_instance_id) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [smf_instance_id]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "smfInstanceId", pdu_session->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!pdu_session->plmn_id) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(pdu_session->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_convertToJSON() failed [plmn_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pdu_session_t *OpenAPI_pdu_session_parseFromJSON(cJSON *pdu_sessionJSON)
{
    OpenAPI_pdu_session_t *pdu_session_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_sessionJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_sessionJSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_pdu_session_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }


    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_pdu_session_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(pdu_sessionJSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_pdu_session_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;

    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);

    pdu_session_local_var = OpenAPI_pdu_session_create (
        ogs_strdup(dnn->valuestring),
        ogs_strdup(smf_instance_id->valuestring),
        plmn_id_local_nonprim
        );

    return pdu_session_local_var;
end:
    return NULL;
}

