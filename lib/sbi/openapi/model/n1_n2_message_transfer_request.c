
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n1_n2_message_transfer_request.h"

OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_create(
    OpenAPI_n1_n2_message_transfer_req_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message,
    OpenAPI_binary_t* binary_data_n2_information,
    OpenAPI_binary_t* binary_mt_data
)
{
    OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request_local_var = ogs_malloc(sizeof(OpenAPI_n1_n2_message_transfer_request_t));
    ogs_assert(n1_n2_message_transfer_request_local_var);

    n1_n2_message_transfer_request_local_var->json_data = json_data;
    n1_n2_message_transfer_request_local_var->binary_data_n1_message = binary_data_n1_message;
    n1_n2_message_transfer_request_local_var->binary_data_n2_information = binary_data_n2_information;
    n1_n2_message_transfer_request_local_var->binary_mt_data = binary_mt_data;

    return n1_n2_message_transfer_request_local_var;
}

void OpenAPI_n1_n2_message_transfer_request_free(OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n1_n2_message_transfer_request) {
        return;
    }
    if (n1_n2_message_transfer_request->json_data) {
        OpenAPI_n1_n2_message_transfer_req_data_free(n1_n2_message_transfer_request->json_data);
        n1_n2_message_transfer_request->json_data = NULL;
    }
    if (n1_n2_message_transfer_request->binary_data_n1_message) {
        ogs_free(n1_n2_message_transfer_request->binary_data_n1_message->data);
        n1_n2_message_transfer_request->binary_data_n1_message = NULL;
    }
    if (n1_n2_message_transfer_request->binary_data_n2_information) {
        ogs_free(n1_n2_message_transfer_request->binary_data_n2_information->data);
        n1_n2_message_transfer_request->binary_data_n2_information = NULL;
    }
    if (n1_n2_message_transfer_request->binary_mt_data) {
        ogs_free(n1_n2_message_transfer_request->binary_mt_data->data);
        n1_n2_message_transfer_request->binary_mt_data = NULL;
    }
    ogs_free(n1_n2_message_transfer_request);
}

cJSON *OpenAPI_n1_n2_message_transfer_request_convertToJSON(OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n1_n2_message_transfer_request == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [N1N2MessageTransfer_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n1_n2_message_transfer_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(n1_n2_message_transfer_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (n1_n2_message_transfer_request->binary_data_n1_message) {
    char* encoded_str_binary_data_n1_message = OpenAPI_base64encode(n1_n2_message_transfer_request->binary_data_n1_message->data,n1_n2_message_transfer_request->binary_data_n1_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1Message", encoded_str_binary_data_n1_message) == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [binary_data_n1_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_message);
    }

    if (n1_n2_message_transfer_request->binary_data_n2_information) {
    char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(n1_n2_message_transfer_request->binary_data_n2_information->data,n1_n2_message_transfer_request->binary_data_n2_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [binary_data_n2_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information);
    }

    if (n1_n2_message_transfer_request->binary_mt_data) {
    char* encoded_str_binary_mt_data = OpenAPI_base64encode(n1_n2_message_transfer_request->binary_mt_data->data,n1_n2_message_transfer_request->binary_mt_data->len);
    if (cJSON_AddStringToObject(item, "binaryMtData", encoded_str_binary_mt_data) == NULL) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed [binary_mt_data]");
        goto end;
    }
    ogs_free(encoded_str_binary_mt_data);
    }

end:
    return item;
}

OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_parseFromJSON(cJSON *n1_n2_message_transfer_requestJSON)
{
    OpenAPI_n1_n2_message_transfer_request_t *n1_n2_message_transfer_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_n1_n2_message_transfer_req_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_message = NULL;
    cJSON *binary_data_n2_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information = NULL;
    cJSON *binary_mt_data = NULL;
    OpenAPI_binary_t *decoded_str_binary_mt_data = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_message = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_requestJSON, "binaryDataN1Message");
    if (binary_data_n1_message) {
    decoded_str_binary_data_n1_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_message);
    if (!cJSON_IsString(binary_data_n1_message)) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_data_n1_message]");
        goto end;
    }
    decoded_str_binary_data_n1_message->data = OpenAPI_base64decode(binary_data_n1_message->valuestring, strlen(binary_data_n1_message->valuestring), &decoded_str_binary_data_n1_message->len);
    if (!decoded_str_binary_data_n1_message->data) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_data_n1_message]");
        goto end;
    }
    }

    binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_requestJSON, "binaryDataN2Information");
    if (binary_data_n2_information) {
    decoded_str_binary_data_n2_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information);
    if (!cJSON_IsString(binary_data_n2_information)) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
    if (!decoded_str_binary_data_n2_information->data) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    }

    binary_mt_data = cJSON_GetObjectItemCaseSensitive(n1_n2_message_transfer_requestJSON, "binaryMtData");
    if (binary_mt_data) {
    decoded_str_binary_mt_data = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_mt_data);
    if (!cJSON_IsString(binary_mt_data)) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_mt_data]");
        goto end;
    }
    decoded_str_binary_mt_data->data = OpenAPI_base64decode(binary_mt_data->valuestring, strlen(binary_mt_data->valuestring), &decoded_str_binary_mt_data->len);
    if (!decoded_str_binary_mt_data->data) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_parseFromJSON() failed [binary_mt_data]");
        goto end;
    }
    }

    n1_n2_message_transfer_request_local_var = OpenAPI_n1_n2_message_transfer_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_message ? decoded_str_binary_data_n1_message : NULL,
        binary_data_n2_information ? decoded_str_binary_data_n2_information : NULL,
        binary_mt_data ? decoded_str_binary_mt_data : NULL
    );

    return n1_n2_message_transfer_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_n1_n2_message_transfer_req_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n1_n2_message_transfer_request_t *OpenAPI_n1_n2_message_transfer_request_copy(OpenAPI_n1_n2_message_transfer_request_t *dst, OpenAPI_n1_n2_message_transfer_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n1_n2_message_transfer_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n1_n2_message_transfer_request_convertToJSON() failed");
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

    OpenAPI_n1_n2_message_transfer_request_free(dst);
    dst = OpenAPI_n1_n2_message_transfer_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

