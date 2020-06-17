
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inline_object_2.h"

OpenAPI_inline_object_2_t *OpenAPI_inline_object_2_create(
    OpenAPI_n2_information_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n1_message,
    OpenAPI_binary_t* binary_data_n2_information
    )
{
    OpenAPI_inline_object_2_t *inline_object_2_local_var = OpenAPI_malloc(sizeof(OpenAPI_inline_object_2_t));
    if (!inline_object_2_local_var) {
        return NULL;
    }
    inline_object_2_local_var->json_data = json_data;
    inline_object_2_local_var->binary_data_n1_message = binary_data_n1_message;
    inline_object_2_local_var->binary_data_n2_information = binary_data_n2_information;

    return inline_object_2_local_var;
}

void OpenAPI_inline_object_2_free(OpenAPI_inline_object_2_t *inline_object_2)
{
    if (NULL == inline_object_2) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_n2_information_notification_free(inline_object_2->json_data);
    ogs_free(inline_object_2->binary_data_n1_message->data);
    ogs_free(inline_object_2->binary_data_n2_information->data);
    ogs_free(inline_object_2);
}

cJSON *OpenAPI_inline_object_2_convertToJSON(OpenAPI_inline_object_2_t *inline_object_2)
{
    cJSON *item = NULL;

    if (inline_object_2 == NULL) {
        ogs_error("OpenAPI_inline_object_2_convertToJSON() failed [inline_object_2]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (inline_object_2->json_data) {
        cJSON *json_data_local_JSON = OpenAPI_n2_information_notification_convertToJSON(inline_object_2->json_data);
        if (json_data_local_JSON == NULL) {
            ogs_error("OpenAPI_inline_object_2_convertToJSON() failed [json_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_inline_object_2_convertToJSON() failed [json_data]");
            goto end;
        }
    }

    if (inline_object_2->binary_data_n1_message) {
        char* encoded_str_binary_data_n1_message = OpenAPI_base64encode(inline_object_2->binary_data_n1_message->data,inline_object_2->binary_data_n1_message->len);
        if (cJSON_AddStringToObject(item, "binaryDataN1Message", encoded_str_binary_data_n1_message) == NULL) {
            ogs_error("OpenAPI_inline_object_2_convertToJSON() failed [binary_data_n1_message]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n1_message);
    }

    if (inline_object_2->binary_data_n2_information) {
        char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(inline_object_2->binary_data_n2_information->data,inline_object_2->binary_data_n2_information->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
            ogs_error("OpenAPI_inline_object_2_convertToJSON() failed [binary_data_n2_information]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information);
    }

end:
    return item;
}

OpenAPI_inline_object_2_t *OpenAPI_inline_object_2_parseFromJSON(cJSON *inline_object_2JSON)
{
    OpenAPI_inline_object_2_t *inline_object_2_local_var = NULL;
    cJSON *json_data = cJSON_GetObjectItemCaseSensitive(inline_object_2JSON, "jsonData");

    OpenAPI_n2_information_notification_t *json_data_local_nonprim = NULL;
    if (json_data) {
        json_data_local_nonprim = OpenAPI_n2_information_notification_parseFromJSON(json_data);
    }

    cJSON *binary_data_n1_message = cJSON_GetObjectItemCaseSensitive(inline_object_2JSON, "binaryDataN1Message");

    OpenAPI_binary_t* decoded_str_binary_data_n1_message = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n1_message) {
        if (!cJSON_IsString(binary_data_n1_message)) {
            ogs_error("OpenAPI_inline_object_2_parseFromJSON() failed [binary_data_n1_message]");
            goto end;
        }
        decoded_str_binary_data_n1_message->data = OpenAPI_base64decode(binary_data_n1_message->valuestring, strlen(binary_data_n1_message->valuestring), &decoded_str_binary_data_n1_message->len);
        if (!decoded_str_binary_data_n1_message->data) {
            ogs_error("OpenAPI_inline_object_2_parseFromJSON() failed [binary_data_n1_message]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(inline_object_2JSON, "binaryDataN2Information");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information) {
        if (!cJSON_IsString(binary_data_n2_information)) {
            ogs_error("OpenAPI_inline_object_2_parseFromJSON() failed [binary_data_n2_information]");
            goto end;
        }
        decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
        if (!decoded_str_binary_data_n2_information->data) {
            ogs_error("OpenAPI_inline_object_2_parseFromJSON() failed [binary_data_n2_information]");
            goto end;
        }
    }

    inline_object_2_local_var = OpenAPI_inline_object_2_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_message ? decoded_str_binary_data_n1_message : NULL,
        binary_data_n2_information ? decoded_str_binary_data_n2_information : NULL
        );

    return inline_object_2_local_var;
end:
    return NULL;
}

OpenAPI_inline_object_2_t *OpenAPI_inline_object_2_copy(OpenAPI_inline_object_2_t *dst, OpenAPI_inline_object_2_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inline_object_2_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inline_object_2_convertToJSON() failed");
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

    OpenAPI_inline_object_2_free(dst);
    dst = OpenAPI_inline_object_2_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

