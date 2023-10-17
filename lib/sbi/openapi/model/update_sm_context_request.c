
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "update_sm_context_request.h"

OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_create(
    OpenAPI_sm_context_update_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_message,
    OpenAPI_binary_t* binary_data_n2_sm_information,
    OpenAPI_binary_t* binary_data_n2_sm_information_ext1
)
{
    OpenAPI_update_sm_context_request_t *update_sm_context_request_local_var = ogs_malloc(sizeof(OpenAPI_update_sm_context_request_t));
    ogs_assert(update_sm_context_request_local_var);

    update_sm_context_request_local_var->json_data = json_data;
    update_sm_context_request_local_var->binary_data_n1_sm_message = binary_data_n1_sm_message;
    update_sm_context_request_local_var->binary_data_n2_sm_information = binary_data_n2_sm_information;
    update_sm_context_request_local_var->binary_data_n2_sm_information_ext1 = binary_data_n2_sm_information_ext1;

    return update_sm_context_request_local_var;
}

void OpenAPI_update_sm_context_request_free(OpenAPI_update_sm_context_request_t *update_sm_context_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == update_sm_context_request) {
        return;
    }
    if (update_sm_context_request->json_data) {
        OpenAPI_sm_context_update_data_free(update_sm_context_request->json_data);
        update_sm_context_request->json_data = NULL;
    }
    if (update_sm_context_request->binary_data_n1_sm_message) {
        ogs_free(update_sm_context_request->binary_data_n1_sm_message->data);
        update_sm_context_request->binary_data_n1_sm_message = NULL;
    }
    if (update_sm_context_request->binary_data_n2_sm_information) {
        ogs_free(update_sm_context_request->binary_data_n2_sm_information->data);
        update_sm_context_request->binary_data_n2_sm_information = NULL;
    }
    if (update_sm_context_request->binary_data_n2_sm_information_ext1) {
        ogs_free(update_sm_context_request->binary_data_n2_sm_information_ext1->data);
        update_sm_context_request->binary_data_n2_sm_information_ext1 = NULL;
    }
    ogs_free(update_sm_context_request);
}

cJSON *OpenAPI_update_sm_context_request_convertToJSON(OpenAPI_update_sm_context_request_t *update_sm_context_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (update_sm_context_request == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [UpdateSmContext_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (update_sm_context_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_sm_context_update_data_convertToJSON(update_sm_context_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (update_sm_context_request->binary_data_n1_sm_message) {
    char* encoded_str_binary_data_n1_sm_message = OpenAPI_base64encode(update_sm_context_request->binary_data_n1_sm_message->data,update_sm_context_request->binary_data_n1_sm_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1SmMessage", encoded_str_binary_data_n1_sm_message) == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_sm_message);
    }

    if (update_sm_context_request->binary_data_n2_sm_information) {
    char* encoded_str_binary_data_n2_sm_information = OpenAPI_base64encode(update_sm_context_request->binary_data_n2_sm_information->data,update_sm_context_request->binary_data_n2_sm_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2SmInformation", encoded_str_binary_data_n2_sm_information) == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_sm_information);
    }

    if (update_sm_context_request->binary_data_n2_sm_information_ext1) {
    char* encoded_str_binary_data_n2_sm_information_ext1 = OpenAPI_base64encode(update_sm_context_request->binary_data_n2_sm_information_ext1->data,update_sm_context_request->binary_data_n2_sm_information_ext1->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2SmInformationExt1", encoded_str_binary_data_n2_sm_information_ext1) == NULL) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed [binary_data_n2_sm_information_ext1]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_sm_information_ext1);
    }

end:
    return item;
}

OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_parseFromJSON(cJSON *update_sm_context_requestJSON)
{
    OpenAPI_update_sm_context_request_t *update_sm_context_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_sm_context_update_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_sm_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_sm_message = NULL;
    cJSON *binary_data_n2_sm_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_sm_information = NULL;
    cJSON *binary_data_n2_sm_information_ext1 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_sm_information_ext1 = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(update_sm_context_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_sm_context_update_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_sm_context_update_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_sm_message = cJSON_GetObjectItemCaseSensitive(update_sm_context_requestJSON, "binaryDataN1SmMessage");
    if (binary_data_n1_sm_message) {
    decoded_str_binary_data_n1_sm_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_sm_message);
    if (!cJSON_IsString(binary_data_n1_sm_message)) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    decoded_str_binary_data_n1_sm_message->data = OpenAPI_base64decode(binary_data_n1_sm_message->valuestring, strlen(binary_data_n1_sm_message->valuestring), &decoded_str_binary_data_n1_sm_message->len);
    if (!decoded_str_binary_data_n1_sm_message->data) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n1_sm_message]");
        goto end;
    }
    }

    binary_data_n2_sm_information = cJSON_GetObjectItemCaseSensitive(update_sm_context_requestJSON, "binaryDataN2SmInformation");
    if (binary_data_n2_sm_information) {
    decoded_str_binary_data_n2_sm_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_sm_information);
    if (!cJSON_IsString(binary_data_n2_sm_information)) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    decoded_str_binary_data_n2_sm_information->data = OpenAPI_base64decode(binary_data_n2_sm_information->valuestring, strlen(binary_data_n2_sm_information->valuestring), &decoded_str_binary_data_n2_sm_information->len);
    if (!decoded_str_binary_data_n2_sm_information->data) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information]");
        goto end;
    }
    }

    binary_data_n2_sm_information_ext1 = cJSON_GetObjectItemCaseSensitive(update_sm_context_requestJSON, "binaryDataN2SmInformationExt1");
    if (binary_data_n2_sm_information_ext1) {
    decoded_str_binary_data_n2_sm_information_ext1 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_sm_information_ext1);
    if (!cJSON_IsString(binary_data_n2_sm_information_ext1)) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information_ext1]");
        goto end;
    }
    decoded_str_binary_data_n2_sm_information_ext1->data = OpenAPI_base64decode(binary_data_n2_sm_information_ext1->valuestring, strlen(binary_data_n2_sm_information_ext1->valuestring), &decoded_str_binary_data_n2_sm_information_ext1->len);
    if (!decoded_str_binary_data_n2_sm_information_ext1->data) {
        ogs_error("OpenAPI_update_sm_context_request_parseFromJSON() failed [binary_data_n2_sm_information_ext1]");
        goto end;
    }
    }

    update_sm_context_request_local_var = OpenAPI_update_sm_context_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_sm_message ? decoded_str_binary_data_n1_sm_message : NULL,
        binary_data_n2_sm_information ? decoded_str_binary_data_n2_sm_information : NULL,
        binary_data_n2_sm_information_ext1 ? decoded_str_binary_data_n2_sm_information_ext1 : NULL
    );

    return update_sm_context_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_sm_context_update_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_update_sm_context_request_t *OpenAPI_update_sm_context_request_copy(OpenAPI_update_sm_context_request_t *dst, OpenAPI_update_sm_context_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_update_sm_context_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_update_sm_context_request_convertToJSON() failed");
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

    OpenAPI_update_sm_context_request_free(dst);
    dst = OpenAPI_update_sm_context_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

