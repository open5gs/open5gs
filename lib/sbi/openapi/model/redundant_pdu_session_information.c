
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_pdu_session_information.h"

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_create(
    OpenAPI_rsn_e rsn,
    bool is_pdu_session_pair_id,
    int pdu_session_pair_id
)
{
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information_local_var = ogs_malloc(sizeof(OpenAPI_redundant_pdu_session_information_t));
    ogs_assert(redundant_pdu_session_information_local_var);

    redundant_pdu_session_information_local_var->rsn = rsn;
    redundant_pdu_session_information_local_var->is_pdu_session_pair_id = is_pdu_session_pair_id;
    redundant_pdu_session_information_local_var->pdu_session_pair_id = pdu_session_pair_id;

    return redundant_pdu_session_information_local_var;
}

void OpenAPI_redundant_pdu_session_information_free(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == redundant_pdu_session_information) {
        return;
    }
    ogs_free(redundant_pdu_session_information);
}

cJSON *OpenAPI_redundant_pdu_session_information_convertToJSON(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (redundant_pdu_session_information == NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [RedundantPduSessionInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (redundant_pdu_session_information->rsn == OpenAPI_rsn_NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [rsn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "rsn", OpenAPI_rsn_ToString(redundant_pdu_session_information->rsn)) == NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [rsn]");
        goto end;
    }

    if (redundant_pdu_session_information->is_pdu_session_pair_id) {
    if (cJSON_AddNumberToObject(item, "pduSessionPairId", redundant_pdu_session_information->pdu_session_pair_id) == NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [pdu_session_pair_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_parseFromJSON(cJSON *redundant_pdu_session_informationJSON)
{
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *rsn = NULL;
    OpenAPI_rsn_e rsnVariable = 0;
    cJSON *pdu_session_pair_id = NULL;
    rsn = cJSON_GetObjectItemCaseSensitive(redundant_pdu_session_informationJSON, "rsn");
    if (!rsn) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON() failed [rsn]");
        goto end;
    }
    if (!cJSON_IsString(rsn)) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON() failed [rsn]");
        goto end;
    }
    rsnVariable = OpenAPI_rsn_FromString(rsn->valuestring);

    pdu_session_pair_id = cJSON_GetObjectItemCaseSensitive(redundant_pdu_session_informationJSON, "pduSessionPairId");
    if (pdu_session_pair_id) {
    if (!cJSON_IsNumber(pdu_session_pair_id)) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON() failed [pdu_session_pair_id]");
        goto end;
    }
    }

    redundant_pdu_session_information_local_var = OpenAPI_redundant_pdu_session_information_create (
        rsnVariable,
        pdu_session_pair_id ? true : false,
        pdu_session_pair_id ? pdu_session_pair_id->valuedouble : 0
    );

    return redundant_pdu_session_information_local_var;
end:
    return NULL;
}

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_copy(OpenAPI_redundant_pdu_session_information_t *dst, OpenAPI_redundant_pdu_session_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_redundant_pdu_session_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed");
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

    OpenAPI_redundant_pdu_session_information_free(dst);
    dst = OpenAPI_redundant_pdu_session_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

