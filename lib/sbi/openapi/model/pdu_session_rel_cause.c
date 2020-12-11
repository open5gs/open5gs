
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_rel_cause.h"

OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_create(
    )
{
    OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause_local_var = OpenAPI_malloc(sizeof(OpenAPI_pdu_session_rel_cause_t));
    if (!pdu_session_rel_cause_local_var) {
        return NULL;
    }

    return pdu_session_rel_cause_local_var;
}

void OpenAPI_pdu_session_rel_cause_free(OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause)
{
    if (NULL == pdu_session_rel_cause) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_rel_cause);
}

cJSON *OpenAPI_pdu_session_rel_cause_convertToJSON(OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause)
{
    cJSON *item = NULL;

    if (pdu_session_rel_cause == NULL) {
        ogs_error("OpenAPI_pdu_session_rel_cause_convertToJSON() failed [PduSessionRelCause]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_parseFromJSON(cJSON *pdu_session_rel_causeJSON)
{
    OpenAPI_pdu_session_rel_cause_t *pdu_session_rel_cause_local_var = NULL;
    pdu_session_rel_cause_local_var = OpenAPI_pdu_session_rel_cause_create (
        );

    return pdu_session_rel_cause_local_var;
end:
    return NULL;
}

OpenAPI_pdu_session_rel_cause_t *OpenAPI_pdu_session_rel_cause_copy(OpenAPI_pdu_session_rel_cause_t *dst, OpenAPI_pdu_session_rel_cause_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_rel_cause_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_rel_cause_convertToJSON() failed");
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

    OpenAPI_pdu_session_rel_cause_free(dst);
    dst = OpenAPI_pdu_session_rel_cause_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

