
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "retrieve_data.h"

OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_create(
    bool is_small_data_rate_status_req,
    int small_data_rate_status_req,
    OpenAPI_pdu_session_context_type_e pdu_session_context_type
)
{
    OpenAPI_retrieve_data_t *retrieve_data_local_var = ogs_malloc(sizeof(OpenAPI_retrieve_data_t));
    ogs_assert(retrieve_data_local_var);

    retrieve_data_local_var->is_small_data_rate_status_req = is_small_data_rate_status_req;
    retrieve_data_local_var->small_data_rate_status_req = small_data_rate_status_req;
    retrieve_data_local_var->pdu_session_context_type = pdu_session_context_type;

    return retrieve_data_local_var;
}

void OpenAPI_retrieve_data_free(OpenAPI_retrieve_data_t *retrieve_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == retrieve_data) {
        return;
    }
    ogs_free(retrieve_data);
}

cJSON *OpenAPI_retrieve_data_convertToJSON(OpenAPI_retrieve_data_t *retrieve_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (retrieve_data == NULL) {
        ogs_error("OpenAPI_retrieve_data_convertToJSON() failed [RetrieveData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (retrieve_data->is_small_data_rate_status_req) {
    if (cJSON_AddBoolToObject(item, "smallDataRateStatusReq", retrieve_data->small_data_rate_status_req) == NULL) {
        ogs_error("OpenAPI_retrieve_data_convertToJSON() failed [small_data_rate_status_req]");
        goto end;
    }
    }

    if (retrieve_data->pdu_session_context_type != OpenAPI_pdu_session_context_type_NULL) {
    if (cJSON_AddStringToObject(item, "pduSessionContextType", OpenAPI_pdu_session_context_type_ToString(retrieve_data->pdu_session_context_type)) == NULL) {
        ogs_error("OpenAPI_retrieve_data_convertToJSON() failed [pdu_session_context_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_parseFromJSON(cJSON *retrieve_dataJSON)
{
    OpenAPI_retrieve_data_t *retrieve_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *small_data_rate_status_req = NULL;
    cJSON *pdu_session_context_type = NULL;
    OpenAPI_pdu_session_context_type_e pdu_session_context_typeVariable = 0;
    small_data_rate_status_req = cJSON_GetObjectItemCaseSensitive(retrieve_dataJSON, "smallDataRateStatusReq");
    if (small_data_rate_status_req) {
    if (!cJSON_IsBool(small_data_rate_status_req)) {
        ogs_error("OpenAPI_retrieve_data_parseFromJSON() failed [small_data_rate_status_req]");
        goto end;
    }
    }

    pdu_session_context_type = cJSON_GetObjectItemCaseSensitive(retrieve_dataJSON, "pduSessionContextType");
    if (pdu_session_context_type) {
    if (!cJSON_IsString(pdu_session_context_type)) {
        ogs_error("OpenAPI_retrieve_data_parseFromJSON() failed [pdu_session_context_type]");
        goto end;
    }
    pdu_session_context_typeVariable = OpenAPI_pdu_session_context_type_FromString(pdu_session_context_type->valuestring);
    }

    retrieve_data_local_var = OpenAPI_retrieve_data_create (
        small_data_rate_status_req ? true : false,
        small_data_rate_status_req ? small_data_rate_status_req->valueint : 0,
        pdu_session_context_type ? pdu_session_context_typeVariable : 0
    );

    return retrieve_data_local_var;
end:
    return NULL;
}

OpenAPI_retrieve_data_t *OpenAPI_retrieve_data_copy(OpenAPI_retrieve_data_t *dst, OpenAPI_retrieve_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_retrieve_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_retrieve_data_convertToJSON() failed");
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

    OpenAPI_retrieve_data_free(dst);
    dst = OpenAPI_retrieve_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

