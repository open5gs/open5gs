
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "send_mt_sms_200_response.h"

OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_create(
    OpenAPI_sms_delivery_data_t *json_data,
    OpenAPI_binary_t* binary_payload
)
{
    OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response_local_var = ogs_malloc(sizeof(OpenAPI_send_mt_sms_200_response_t));
    ogs_assert(send_mt_sms_200_response_local_var);

    send_mt_sms_200_response_local_var->json_data = json_data;
    send_mt_sms_200_response_local_var->binary_payload = binary_payload;

    return send_mt_sms_200_response_local_var;
}

void OpenAPI_send_mt_sms_200_response_free(OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == send_mt_sms_200_response) {
        return;
    }
    if (send_mt_sms_200_response->json_data) {
        OpenAPI_sms_delivery_data_free(send_mt_sms_200_response->json_data);
        send_mt_sms_200_response->json_data = NULL;
    }
    if (send_mt_sms_200_response->binary_payload) {
        ogs_free(send_mt_sms_200_response->binary_payload->data);
        send_mt_sms_200_response->binary_payload = NULL;
    }
    ogs_free(send_mt_sms_200_response);
}

cJSON *OpenAPI_send_mt_sms_200_response_convertToJSON(OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (send_mt_sms_200_response == NULL) {
        ogs_error("OpenAPI_send_mt_sms_200_response_convertToJSON() failed [SendMtSMS_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (send_mt_sms_200_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_sms_delivery_data_convertToJSON(send_mt_sms_200_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_send_mt_sms_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_send_mt_sms_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (send_mt_sms_200_response->binary_payload) {
    char* encoded_str_binary_payload = OpenAPI_base64encode(send_mt_sms_200_response->binary_payload->data,send_mt_sms_200_response->binary_payload->len);
    if (cJSON_AddStringToObject(item, "binaryPayload", encoded_str_binary_payload) == NULL) {
        ogs_error("OpenAPI_send_mt_sms_200_response_convertToJSON() failed [binary_payload]");
        goto end;
    }
    ogs_free(encoded_str_binary_payload);
    }

end:
    return item;
}

OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_parseFromJSON(cJSON *send_mt_sms_200_responseJSON)
{
    OpenAPI_send_mt_sms_200_response_t *send_mt_sms_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_sms_delivery_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_payload = NULL;
    OpenAPI_binary_t *decoded_str_binary_payload = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(send_mt_sms_200_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_sms_delivery_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_sms_delivery_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_payload = cJSON_GetObjectItemCaseSensitive(send_mt_sms_200_responseJSON, "binaryPayload");
    if (binary_payload) {
    decoded_str_binary_payload = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_payload);
    if (!cJSON_IsString(binary_payload)) {
        ogs_error("OpenAPI_send_mt_sms_200_response_parseFromJSON() failed [binary_payload]");
        goto end;
    }
    decoded_str_binary_payload->data = OpenAPI_base64decode(binary_payload->valuestring, strlen(binary_payload->valuestring), &decoded_str_binary_payload->len);
    if (!decoded_str_binary_payload->data) {
        ogs_error("OpenAPI_send_mt_sms_200_response_parseFromJSON() failed [binary_payload]");
        goto end;
    }
    }

    send_mt_sms_200_response_local_var = OpenAPI_send_mt_sms_200_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_payload ? decoded_str_binary_payload : NULL
    );

    return send_mt_sms_200_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_sms_delivery_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_send_mt_sms_200_response_t *OpenAPI_send_mt_sms_200_response_copy(OpenAPI_send_mt_sms_200_response_t *dst, OpenAPI_send_mt_sms_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_send_mt_sms_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_send_mt_sms_200_response_convertToJSON() failed");
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

    OpenAPI_send_mt_sms_200_response_free(dst);
    dst = OpenAPI_send_mt_sms_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

