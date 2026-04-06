
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "post_sm_contexts_400_response.h"

OpenAPI_post_sm_contexts_400_response_t *OpenAPI_post_sm_contexts_400_response_create(
    OpenAPI_sm_context_create_error_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_message,
    OpenAPI_binary_t* binary_data_n2_sm_message
)
{
    OpenAPI_post_sm_contexts_400_response_t *post_sm_contexts_400_response_local_var = ogs_malloc(sizeof(OpenAPI_post_sm_contexts_400_response_t));
    ogs_assert(post_sm_contexts_400_response_local_var);

    post_sm_contexts_400_response_local_var->json_data = json_data;
    post_sm_contexts_400_response_local_var->binary_data_n1_sm_message = binary_data_n1_sm_message;
    post_sm_contexts_400_response_local_var->binary_data_n2_sm_message = binary_data_n2_sm_message;

    return post_sm_contexts_400_response_local_var;
}

void OpenAPI_post_sm_contexts_400_response_free(OpenAPI_post_sm_contexts_400_response_t *post_sm_contexts_400_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == post_sm_contexts_400_response) {
        return;
    }
    if (post_sm_contexts_400_response->json_data) {
        OpenAPI_sm_context_create_error_free(post_sm_contexts_400_response->json_data);
        post_sm_contexts_400_response->json_data = NULL;
    }
    if (post_sm_contexts_400_response->binary_data_n1_sm_message) {
        ogs_free(post_sm_contexts_400_response->binary_data_n1_sm_message->data);
        post_sm_contexts_400_response->binary_data_n1_sm_message = NULL;
    }
    if (post_sm_contexts_400_response->binary_data_n2_sm_message) {
        ogs_free(post_sm_contexts_400_response->binary_data_n2_sm_message->data);
        post_sm_contexts_400_response->binary_data_n2_sm_message = NULL;
    }
    ogs_free(post_sm_contexts_400_response);
}

cJSON *OpenAPI_post_sm_contexts_400_response_convertToJSON(OpenAPI_post_sm_contexts_400_response_t *post_sm_contexts_400_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (post_sm_contexts_400_response == NULL) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed [PostSmContexts_400_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (post_sm_contexts_400_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_sm_context_create_error_convertToJSON(post_sm_contexts_400_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (post_sm_contexts_400_response->binary_data_n1_sm_message) {
    char* encoded_str_binary_data_n1_sm_message = OpenAPI_base64encode(post_sm_contexts_400_response->binary_data_n1_sm_message->data,post_sm_contexts_400_response->binary_data_n1_sm_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1SmMessage", encoded_str_binary_data_n1_sm_message) == NULL) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_sm_message);
    }

    if (post_sm_contexts_400_response->binary_data_n2_sm_message) {
    char* encoded_str_binary_data_n2_sm_message = OpenAPI_base64encode(post_sm_contexts_400_response->binary_data_n2_sm_message->data,post_sm_contexts_400_response->binary_data_n2_sm_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2SmMessage", encoded_str_binary_data_n2_sm_message) == NULL) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed [binary_data_n2_sm_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_sm_message);
    }

end:
    return item;
}

OpenAPI_post_sm_contexts_400_response_t *OpenAPI_post_sm_contexts_400_response_parseFromJSON(cJSON *post_sm_contexts_400_responseJSON)
{
    OpenAPI_post_sm_contexts_400_response_t *post_sm_contexts_400_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_sm_context_create_error_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_sm_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_sm_message = NULL;
    cJSON *binary_data_n2_sm_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_sm_message = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(post_sm_contexts_400_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_sm_context_create_error_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_sm_context_create_error_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_sm_message = cJSON_GetObjectItemCaseSensitive(post_sm_contexts_400_responseJSON, "binaryDataN1SmMessage");
    if (binary_data_n1_sm_message) {
    decoded_str_binary_data_n1_sm_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_sm_message);
    if (!cJSON_IsString(binary_data_n1_sm_message)) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_parseFromJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    decoded_str_binary_data_n1_sm_message->data = OpenAPI_base64decode(binary_data_n1_sm_message->valuestring, strlen(binary_data_n1_sm_message->valuestring), &decoded_str_binary_data_n1_sm_message->len);
    if (!decoded_str_binary_data_n1_sm_message->data) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_parseFromJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    }

    binary_data_n2_sm_message = cJSON_GetObjectItemCaseSensitive(post_sm_contexts_400_responseJSON, "binaryDataN2SmMessage");
    if (binary_data_n2_sm_message) {
    decoded_str_binary_data_n2_sm_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_sm_message);
    if (!cJSON_IsString(binary_data_n2_sm_message)) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_parseFromJSON() failed [binary_data_n2_sm_message]");
        goto end;
    }
    decoded_str_binary_data_n2_sm_message->data = OpenAPI_base64decode(binary_data_n2_sm_message->valuestring, strlen(binary_data_n2_sm_message->valuestring), &decoded_str_binary_data_n2_sm_message->len);
    if (!decoded_str_binary_data_n2_sm_message->data) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_parseFromJSON() failed [binary_data_n2_sm_message]");
        goto end;
    }
    }

    post_sm_contexts_400_response_local_var = OpenAPI_post_sm_contexts_400_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_sm_message ? decoded_str_binary_data_n1_sm_message : NULL,
        binary_data_n2_sm_message ? decoded_str_binary_data_n2_sm_message : NULL
    );

    return post_sm_contexts_400_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_sm_context_create_error_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_post_sm_contexts_400_response_t *OpenAPI_post_sm_contexts_400_response_copy(OpenAPI_post_sm_contexts_400_response_t *dst, OpenAPI_post_sm_contexts_400_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_post_sm_contexts_400_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_post_sm_contexts_400_response_convertToJSON() failed");
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

    OpenAPI_post_sm_contexts_400_response_free(dst);
    dst = OpenAPI_post_sm_contexts_400_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

