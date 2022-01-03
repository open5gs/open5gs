
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "redundant_pdu_session_information.h"

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_create(
    OpenAPI_rsn_e rsn
)
{
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information_local_var = ogs_malloc(sizeof(OpenAPI_redundant_pdu_session_information_t));
    ogs_assert(redundant_pdu_session_information_local_var);

    redundant_pdu_session_information_local_var->rsn = rsn;

    return redundant_pdu_session_information_local_var;
}

void OpenAPI_redundant_pdu_session_information_free(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information)
{
    if (NULL == redundant_pdu_session_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(redundant_pdu_session_information);
}

cJSON *OpenAPI_redundant_pdu_session_information_convertToJSON(OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information)
{
    cJSON *item = NULL;

    if (redundant_pdu_session_information == NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [RedundantPduSessionInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "rsn", OpenAPI_rsn_ToString(redundant_pdu_session_information->rsn)) == NULL) {
        ogs_error("OpenAPI_redundant_pdu_session_information_convertToJSON() failed [rsn]");
        goto end;
    }

end:
    return item;
}

OpenAPI_redundant_pdu_session_information_t *OpenAPI_redundant_pdu_session_information_parseFromJSON(cJSON *redundant_pdu_session_informationJSON)
{
    OpenAPI_redundant_pdu_session_information_t *redundant_pdu_session_information_local_var = NULL;
    cJSON *rsn = cJSON_GetObjectItemCaseSensitive(redundant_pdu_session_informationJSON, "rsn");
    if (!rsn) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON() failed [rsn]");
        goto end;
    }

    OpenAPI_rsn_e rsnVariable;
    if (!cJSON_IsString(rsn)) {
        ogs_error("OpenAPI_redundant_pdu_session_information_parseFromJSON() failed [rsn]");
        goto end;
    }
    rsnVariable = OpenAPI_rsn_FromString(rsn->valuestring);

    redundant_pdu_session_information_local_var = OpenAPI_redundant_pdu_session_information_create (
        rsnVariable
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

