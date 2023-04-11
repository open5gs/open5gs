
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cancel_relocate_ue_context_request.h"

OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_create(
    OpenAPI_ue_context_cancel_relocate_data_t *json_data,
    OpenAPI_binary_t* binary_data_gtpc_message
)
{
    OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request_local_var = ogs_malloc(sizeof(OpenAPI_cancel_relocate_ue_context_request_t));
    ogs_assert(cancel_relocate_ue_context_request_local_var);

    cancel_relocate_ue_context_request_local_var->json_data = json_data;
    cancel_relocate_ue_context_request_local_var->binary_data_gtpc_message = binary_data_gtpc_message;

    return cancel_relocate_ue_context_request_local_var;
}

void OpenAPI_cancel_relocate_ue_context_request_free(OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == cancel_relocate_ue_context_request) {
        return;
    }
    if (cancel_relocate_ue_context_request->json_data) {
        OpenAPI_ue_context_cancel_relocate_data_free(cancel_relocate_ue_context_request->json_data);
        cancel_relocate_ue_context_request->json_data = NULL;
    }
    if (cancel_relocate_ue_context_request->binary_data_gtpc_message) {
        ogs_free(cancel_relocate_ue_context_request->binary_data_gtpc_message->data);
        cancel_relocate_ue_context_request->binary_data_gtpc_message = NULL;
    }
    ogs_free(cancel_relocate_ue_context_request);
}

cJSON *OpenAPI_cancel_relocate_ue_context_request_convertToJSON(OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (cancel_relocate_ue_context_request == NULL) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_convertToJSON() failed [CancelRelocateUEContext_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cancel_relocate_ue_context_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_ue_context_cancel_relocate_data_convertToJSON(cancel_relocate_ue_context_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (cancel_relocate_ue_context_request->binary_data_gtpc_message) {
    char* encoded_str_binary_data_gtpc_message = OpenAPI_base64encode(cancel_relocate_ue_context_request->binary_data_gtpc_message->data,cancel_relocate_ue_context_request->binary_data_gtpc_message->len);
    if (cJSON_AddStringToObject(item, "binaryDataGtpcMessage", encoded_str_binary_data_gtpc_message) == NULL) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_convertToJSON() failed [binary_data_gtpc_message]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_gtpc_message);
    }

end:
    return item;
}

OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_parseFromJSON(cJSON *cancel_relocate_ue_context_requestJSON)
{
    OpenAPI_cancel_relocate_ue_context_request_t *cancel_relocate_ue_context_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_ue_context_cancel_relocate_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_gtpc_message = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_gtpc_message = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(cancel_relocate_ue_context_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_ue_context_cancel_relocate_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_cancel_relocate_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_gtpc_message = cJSON_GetObjectItemCaseSensitive(cancel_relocate_ue_context_requestJSON, "binaryDataGtpcMessage");
    if (binary_data_gtpc_message) {
    decoded_str_binary_data_gtpc_message = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_gtpc_message);
    if (!cJSON_IsString(binary_data_gtpc_message)) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_parseFromJSON() failed [binary_data_gtpc_message]");
        goto end;
    }
    decoded_str_binary_data_gtpc_message->data = OpenAPI_base64decode(binary_data_gtpc_message->valuestring, strlen(binary_data_gtpc_message->valuestring), &decoded_str_binary_data_gtpc_message->len);
    if (!decoded_str_binary_data_gtpc_message->data) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_parseFromJSON() failed [binary_data_gtpc_message]");
        goto end;
    }
    }

    cancel_relocate_ue_context_request_local_var = OpenAPI_cancel_relocate_ue_context_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_gtpc_message ? decoded_str_binary_data_gtpc_message : NULL
    );

    return cancel_relocate_ue_context_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_ue_context_cancel_relocate_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_cancel_relocate_ue_context_request_t *OpenAPI_cancel_relocate_ue_context_request_copy(OpenAPI_cancel_relocate_ue_context_request_t *dst, OpenAPI_cancel_relocate_ue_context_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_cancel_relocate_ue_context_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_cancel_relocate_ue_context_request_convertToJSON() failed");
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

    OpenAPI_cancel_relocate_ue_context_request_free(dst);
    dst = OpenAPI_cancel_relocate_ue_context_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

