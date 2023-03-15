
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_transfer_request.h"

OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_create(
    OpenAPI_ue_context_transfer_req_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message
)
{
    OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_transfer_request_t));
    ogs_assert(ue_context_transfer_request_local_var);

    ue_context_transfer_request_local_var->json_data = json_data;
    ue_context_transfer_request_local_var->binary_data_n1_message = binary_data_n1_message;

    return ue_context_transfer_request_local_var;
}

void OpenAPI_ue_context_transfer_request_free(OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_transfer_request) {
        return;
    }
    if (ue_context_transfer_request->json_data) {
        OpenAPI_ue_context_transfer_req_data_free(ue_context_transfer_request->json_data);
        ue_context_transfer_request->json_data = NULL;
    }
    if (ue_context_transfer_request->binary_data_n1_message) {
        ogs_free(ue_context_transfer_request->binary_data_n1_message->data);
        ue_context_transfer_request->binary_data_n1_message = NULL;
    }
    ogs_free(ue_context_transfer_request);
}

cJSON *OpenAPI_ue_context_transfer_request_convertToJSON(OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_transfer_request == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_request_convertToJSON() failed [UEContextTransfer_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_transfer_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_ue_context_transfer_req_data_convertToJSON(ue_context_transfer_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (ue_context_transfer_request->binary_data_n1_message) {
    char* encoded_str_binary_data_n1_message = OpenAPI_base64encode(ue_context_transfer_request->binary_data_n1_message->data,ue_context_transfer_request->binary_data_n1_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1Message", encoded_str_binary_data_n1_message) == NULL) {
        ogs_error("OpenAPI_ue_context_transfer_request_convertToJSON() failed [binary_data_n1_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_message);
    }

end:
    return item;
}

OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_parseFromJSON(cJSON *ue_context_transfer_requestJSON)
{
    OpenAPI_ue_context_transfer_request_t *ue_context_transfer_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_ue_context_transfer_req_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_message = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_ue_context_transfer_req_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_transfer_req_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_message = cJSON_GetObjectItemCaseSensitive(ue_context_transfer_requestJSON, "binaryDataN1Message");
    if (binary_data_n1_message) {
    decoded_str_binary_data_n1_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_message);
    if (!cJSON_IsString(binary_data_n1_message)) {
        ogs_error("OpenAPI_ue_context_transfer_request_parseFromJSON() failed [binary_data_n1_message]");
        goto end;
    }
    decoded_str_binary_data_n1_message->data = OpenAPI_base64decode(binary_data_n1_message->valuestring, strlen(binary_data_n1_message->valuestring), &decoded_str_binary_data_n1_message->len);
    if (!decoded_str_binary_data_n1_message->data) {
        ogs_error("OpenAPI_ue_context_transfer_request_parseFromJSON() failed [binary_data_n1_message]");
        goto end;
    }
    }

    ue_context_transfer_request_local_var = OpenAPI_ue_context_transfer_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_message ? decoded_str_binary_data_n1_message : NULL
    );

    return ue_context_transfer_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_ue_context_transfer_req_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_transfer_request_t *OpenAPI_ue_context_transfer_request_copy(OpenAPI_ue_context_transfer_request_t *dst, OpenAPI_ue_context_transfer_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_transfer_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_transfer_request_convertToJSON() failed");
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

    OpenAPI_ue_context_transfer_request_free(dst);
    dst = OpenAPI_ue_context_transfer_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

