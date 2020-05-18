
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_type.h"

OpenAPI_pdu_session_type_t *OpenAPI_pdu_session_type_create(
    )
{
    OpenAPI_pdu_session_type_t *pdu_session_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_pdu_session_type_t));
    if (!pdu_session_type_local_var) {
        return NULL;
    }

    return pdu_session_type_local_var;
}

void OpenAPI_pdu_session_type_free(OpenAPI_pdu_session_type_t *pdu_session_type)
{
    if (NULL == pdu_session_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pdu_session_type);
}

cJSON *OpenAPI_pdu_session_type_convertToJSON(OpenAPI_pdu_session_type_t *pdu_session_type)
{
    cJSON *item = NULL;

    if (pdu_session_type == NULL) {
        ogs_error("OpenAPI_pdu_session_type_convertToJSON() failed [PduSessionType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_pdu_session_type_t *OpenAPI_pdu_session_type_parseFromJSON(cJSON *pdu_session_typeJSON)
{
    OpenAPI_pdu_session_type_t *pdu_session_type_local_var = NULL;
    pdu_session_type_local_var = OpenAPI_pdu_session_type_create (
        );

    return pdu_session_type_local_var;
end:
    return NULL;
}

