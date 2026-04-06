
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "post_pdu_sessions_201_response.h"

OpenAPI_post_pdu_sessions_201_response_t *OpenAPI_post_pdu_sessions_201_response_create(
    OpenAPI_pdu_session_created_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue
)
{
    OpenAPI_post_pdu_sessions_201_response_t *post_pdu_sessions_201_response_local_var = ogs_malloc(sizeof(OpenAPI_post_pdu_sessions_201_response_t));
    ogs_assert(post_pdu_sessions_201_response_local_var);

    post_pdu_sessions_201_response_local_var->json_data = json_data;
    post_pdu_sessions_201_response_local_var->binary_data_n1_sm_info_to_ue = binary_data_n1_sm_info_to_ue;

    return post_pdu_sessions_201_response_local_var;
}

void OpenAPI_post_pdu_sessions_201_response_free(OpenAPI_post_pdu_sessions_201_response_t *post_pdu_sessions_201_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == post_pdu_sessions_201_response) {
        return;
    }
    if (post_pdu_sessions_201_response->json_data) {
        OpenAPI_pdu_session_created_data_free(post_pdu_sessions_201_response->json_data);
        post_pdu_sessions_201_response->json_data = NULL;
    }
    if (post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue) {
        ogs_free(post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue->data);
        post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue = NULL;
    }
    ogs_free(post_pdu_sessions_201_response);
}

cJSON *OpenAPI_post_pdu_sessions_201_response_convertToJSON(OpenAPI_post_pdu_sessions_201_response_t *post_pdu_sessions_201_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (post_pdu_sessions_201_response == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_convertToJSON() failed [PostPduSessions_201_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (post_pdu_sessions_201_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_pdu_session_created_data_convertToJSON(post_pdu_sessions_201_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue) {
    char* encoded_str_binary_data_n1_sm_info_to_ue = OpenAPI_base64encode(post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue->data,post_pdu_sessions_201_response->binary_data_n1_sm_info_to_ue->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1SmInfoToUe", encoded_str_binary_data_n1_sm_info_to_ue) == NULL) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_convertToJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_sm_info_to_ue);
    }

end:
    return item;
}

OpenAPI_post_pdu_sessions_201_response_t *OpenAPI_post_pdu_sessions_201_response_parseFromJSON(cJSON *post_pdu_sessions_201_responseJSON)
{
    OpenAPI_post_pdu_sessions_201_response_t *post_pdu_sessions_201_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_pdu_session_created_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_sm_info_to_ue = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_sm_info_to_ue = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(post_pdu_sessions_201_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_pdu_session_created_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_created_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_sm_info_to_ue = cJSON_GetObjectItemCaseSensitive(post_pdu_sessions_201_responseJSON, "binaryDataN1SmInfoToUe");
    if (binary_data_n1_sm_info_to_ue) {
    decoded_str_binary_data_n1_sm_info_to_ue = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_sm_info_to_ue);
    if (!cJSON_IsString(binary_data_n1_sm_info_to_ue)) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_parseFromJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    decoded_str_binary_data_n1_sm_info_to_ue->data = OpenAPI_base64decode(binary_data_n1_sm_info_to_ue->valuestring, strlen(binary_data_n1_sm_info_to_ue->valuestring), &decoded_str_binary_data_n1_sm_info_to_ue->len);
    if (!decoded_str_binary_data_n1_sm_info_to_ue->data) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_parseFromJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    }

    post_pdu_sessions_201_response_local_var = OpenAPI_post_pdu_sessions_201_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_sm_info_to_ue ? decoded_str_binary_data_n1_sm_info_to_ue : NULL
    );

    return post_pdu_sessions_201_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_pdu_session_created_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_post_pdu_sessions_201_response_t *OpenAPI_post_pdu_sessions_201_response_copy(OpenAPI_post_pdu_sessions_201_response_t *dst, OpenAPI_post_pdu_sessions_201_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_post_pdu_sessions_201_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_post_pdu_sessions_201_response_convertToJSON() failed");
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

    OpenAPI_post_pdu_sessions_201_response_free(dst);
    dst = OpenAPI_post_pdu_sessions_201_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

