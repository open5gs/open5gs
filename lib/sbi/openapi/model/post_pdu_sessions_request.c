
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "post_pdu_sessions_request.h"

OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_create(
    OpenAPI_pdu_session_create_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_from_ue,
    OpenAPI_binary_t* binary_data_unknown_n1_sm_info
)
{
    OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request_local_var = ogs_malloc(sizeof(OpenAPI_post_pdu_sessions_request_t));
    ogs_assert(post_pdu_sessions_request_local_var);

    post_pdu_sessions_request_local_var->json_data = json_data;
    post_pdu_sessions_request_local_var->binary_data_n1_sm_info_from_ue = binary_data_n1_sm_info_from_ue;
    post_pdu_sessions_request_local_var->binary_data_unknown_n1_sm_info = binary_data_unknown_n1_sm_info;

    return post_pdu_sessions_request_local_var;
}

void OpenAPI_post_pdu_sessions_request_free(OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == post_pdu_sessions_request) {
        return;
    }
    if (post_pdu_sessions_request->json_data) {
        OpenAPI_pdu_session_create_data_free(post_pdu_sessions_request->json_data);
        post_pdu_sessions_request->json_data = NULL;
    }
    if (post_pdu_sessions_request->binary_data_n1_sm_info_from_ue) {
        ogs_free(post_pdu_sessions_request->binary_data_n1_sm_info_from_ue->data);
        post_pdu_sessions_request->binary_data_n1_sm_info_from_ue = NULL;
    }
    if (post_pdu_sessions_request->binary_data_unknown_n1_sm_info) {
        ogs_free(post_pdu_sessions_request->binary_data_unknown_n1_sm_info->data);
        post_pdu_sessions_request->binary_data_unknown_n1_sm_info = NULL;
    }
    ogs_free(post_pdu_sessions_request);
}

cJSON *OpenAPI_post_pdu_sessions_request_convertToJSON(OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (post_pdu_sessions_request == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed [PostPduSessions_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (post_pdu_sessions_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_pdu_session_create_data_convertToJSON(post_pdu_sessions_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (post_pdu_sessions_request->binary_data_n1_sm_info_from_ue) {
    char* encoded_str_binary_data_n1_sm_info_from_ue = OpenAPI_base64encode(post_pdu_sessions_request->binary_data_n1_sm_info_from_ue->data,post_pdu_sessions_request->binary_data_n1_sm_info_from_ue->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1SmInfoFromUe", encoded_str_binary_data_n1_sm_info_from_ue) == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed [binary_data_n1_sm_info_from_ue]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_sm_info_from_ue);
    }

    if (post_pdu_sessions_request->binary_data_unknown_n1_sm_info) {
    char* encoded_str_binary_data_unknown_n1_sm_info = OpenAPI_base64encode(post_pdu_sessions_request->binary_data_unknown_n1_sm_info->data,post_pdu_sessions_request->binary_data_unknown_n1_sm_info->len);
    if (cJSON_AddStringToObject(item, "binaryDataUnknownN1SmInfo", encoded_str_binary_data_unknown_n1_sm_info) == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed [binary_data_unknown_n1_sm_info]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_unknown_n1_sm_info);
    }

end:
    return item;
}

OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_parseFromJSON(cJSON *post_pdu_sessions_requestJSON)
{
    OpenAPI_post_pdu_sessions_request_t *post_pdu_sessions_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_pdu_session_create_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_sm_info_from_ue = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_sm_info_from_ue = NULL;
    cJSON *binary_data_unknown_n1_sm_info = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_unknown_n1_sm_info = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(post_pdu_sessions_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_pdu_session_create_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_create_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_sm_info_from_ue = cJSON_GetObjectItemCaseSensitive(post_pdu_sessions_requestJSON, "binaryDataN1SmInfoFromUe");
    if (binary_data_n1_sm_info_from_ue) {
    decoded_str_binary_data_n1_sm_info_from_ue = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_sm_info_from_ue);
    if (!cJSON_IsString(binary_data_n1_sm_info_from_ue)) {
        ogs_error("OpenAPI_post_pdu_sessions_request_parseFromJSON() failed [binary_data_n1_sm_info_from_ue]");
        goto end;
    }
    decoded_str_binary_data_n1_sm_info_from_ue->data = OpenAPI_base64decode(binary_data_n1_sm_info_from_ue->valuestring, strlen(binary_data_n1_sm_info_from_ue->valuestring), &decoded_str_binary_data_n1_sm_info_from_ue->len);
    if (!decoded_str_binary_data_n1_sm_info_from_ue->data) {
        ogs_error("OpenAPI_post_pdu_sessions_request_parseFromJSON() failed [binary_data_n1_sm_info_from_ue]");
        goto end;
    }
    }

    binary_data_unknown_n1_sm_info = cJSON_GetObjectItemCaseSensitive(post_pdu_sessions_requestJSON, "binaryDataUnknownN1SmInfo");
    if (binary_data_unknown_n1_sm_info) {
    decoded_str_binary_data_unknown_n1_sm_info = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_unknown_n1_sm_info);
    if (!cJSON_IsString(binary_data_unknown_n1_sm_info)) {
        ogs_error("OpenAPI_post_pdu_sessions_request_parseFromJSON() failed [binary_data_unknown_n1_sm_info]");
        goto end;
    }
    decoded_str_binary_data_unknown_n1_sm_info->data = OpenAPI_base64decode(binary_data_unknown_n1_sm_info->valuestring, strlen(binary_data_unknown_n1_sm_info->valuestring), &decoded_str_binary_data_unknown_n1_sm_info->len);
    if (!decoded_str_binary_data_unknown_n1_sm_info->data) {
        ogs_error("OpenAPI_post_pdu_sessions_request_parseFromJSON() failed [binary_data_unknown_n1_sm_info]");
        goto end;
    }
    }

    post_pdu_sessions_request_local_var = OpenAPI_post_pdu_sessions_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_sm_info_from_ue ? decoded_str_binary_data_n1_sm_info_from_ue : NULL,
        binary_data_unknown_n1_sm_info ? decoded_str_binary_data_unknown_n1_sm_info : NULL
    );

    return post_pdu_sessions_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_pdu_session_create_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_post_pdu_sessions_request_t *OpenAPI_post_pdu_sessions_request_copy(OpenAPI_post_pdu_sessions_request_t *dst, OpenAPI_post_pdu_sessions_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_post_pdu_sessions_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_post_pdu_sessions_request_convertToJSON() failed");
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

    OpenAPI_post_pdu_sessions_request_free(dst);
    dst = OpenAPI_post_pdu_sessions_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

