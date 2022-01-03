
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_1.h"

OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_create(
    char *dnn,
    char *smf_instance_id,
    OpenAPI_plmn_id_1_t *plmn_id,
    OpenAPI_snssai_t *single_nssai
)
{
    OpenAPI_pdu_session_1_t *pdu_session_1_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_1_t));
    ogs_assert(pdu_session_1_local_var);

    pdu_session_1_local_var->dnn = dnn;
    pdu_session_1_local_var->smf_instance_id = smf_instance_id;
    pdu_session_1_local_var->plmn_id = plmn_id;
    pdu_session_1_local_var->single_nssai = single_nssai;

    return pdu_session_1_local_var;
}

void OpenAPI_pdu_session_1_free(OpenAPI_pdu_session_1_t *pdu_session_1)
{
    if (NULL == pdu_session_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_1->dnn);
    ogs_free(pdu_session_1->smf_instance_id);
    OpenAPI_plmn_id_1_free(pdu_session_1->plmn_id);
    OpenAPI_snssai_free(pdu_session_1->single_nssai);
    ogs_free(pdu_session_1);
}

cJSON *OpenAPI_pdu_session_1_convertToJSON(OpenAPI_pdu_session_1_t *pdu_session_1)
{
    cJSON *item = NULL;

    if (pdu_session_1 == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [PduSession_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "dnn", pdu_session_1->dnn) == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [dnn]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "smfInstanceId", pdu_session_1->smf_instance_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [smf_instance_id]");
        goto end;
    }

    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_1_convertToJSON(pdu_session_1->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [plmn_id]");
        goto end;
    }

    if (pdu_session_1->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(pdu_session_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_parseFromJSON(cJSON *pdu_session_1JSON)
{
    OpenAPI_pdu_session_1_t *pdu_session_1_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(pdu_session_1JSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_pdu_session_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_pdu_session_1_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *smf_instance_id = cJSON_GetObjectItemCaseSensitive(pdu_session_1JSON, "smfInstanceId");
    if (!smf_instance_id) {
        ogs_error("OpenAPI_pdu_session_1_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    if (!cJSON_IsString(smf_instance_id)) {
        ogs_error("OpenAPI_pdu_session_1_parseFromJSON() failed [smf_instance_id]");
        goto end;
    }

    cJSON *plmn_id = cJSON_GetObjectItemCaseSensitive(pdu_session_1JSON, "plmnId");
    if (!plmn_id) {
        ogs_error("OpenAPI_pdu_session_1_parseFromJSON() failed [plmn_id]");
        goto end;
    }

    OpenAPI_plmn_id_1_t *plmn_id_local_nonprim = NULL;
    plmn_id_local_nonprim = OpenAPI_plmn_id_1_parseFromJSON(plmn_id);

    cJSON *single_nssai = cJSON_GetObjectItemCaseSensitive(pdu_session_1JSON, "singleNssai");

    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    }

    pdu_session_1_local_var = OpenAPI_pdu_session_1_create (
        ogs_strdup(dnn->valuestring),
        ogs_strdup(smf_instance_id->valuestring),
        plmn_id_local_nonprim,
        single_nssai ? single_nssai_local_nonprim : NULL
    );

    return pdu_session_1_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_1_t *OpenAPI_pdu_session_1_copy(OpenAPI_pdu_session_1_t *dst, OpenAPI_pdu_session_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_1_convertToJSON() failed");
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

    OpenAPI_pdu_session_1_free(dst);
    dst = OpenAPI_pdu_session_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

