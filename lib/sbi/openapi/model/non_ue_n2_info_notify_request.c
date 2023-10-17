
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "non_ue_n2_info_notify_request.h"

OpenAPI_non_ue_n2_info_notify_request_t *OpenAPI_non_ue_n2_info_notify_request_create(
    OpenAPI_n2_information_notification_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information
)
{
    OpenAPI_non_ue_n2_info_notify_request_t *non_ue_n2_info_notify_request_local_var = ogs_malloc(sizeof(OpenAPI_non_ue_n2_info_notify_request_t));
    ogs_assert(non_ue_n2_info_notify_request_local_var);

    non_ue_n2_info_notify_request_local_var->json_data = json_data;
    non_ue_n2_info_notify_request_local_var->binary_data_n2_information = binary_data_n2_information;

    return non_ue_n2_info_notify_request_local_var;
}

void OpenAPI_non_ue_n2_info_notify_request_free(OpenAPI_non_ue_n2_info_notify_request_t *non_ue_n2_info_notify_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == non_ue_n2_info_notify_request) {
        return;
    }
    if (non_ue_n2_info_notify_request->json_data) {
        OpenAPI_n2_information_notification_free(non_ue_n2_info_notify_request->json_data);
        non_ue_n2_info_notify_request->json_data = NULL;
    }
    if (non_ue_n2_info_notify_request->binary_data_n2_information) {
        ogs_free(non_ue_n2_info_notify_request->binary_data_n2_information->data);
        non_ue_n2_info_notify_request->binary_data_n2_information = NULL;
    }
    ogs_free(non_ue_n2_info_notify_request);
}

cJSON *OpenAPI_non_ue_n2_info_notify_request_convertToJSON(OpenAPI_non_ue_n2_info_notify_request_t *non_ue_n2_info_notify_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (non_ue_n2_info_notify_request == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_convertToJSON() failed [NonUeN2InfoNotify_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (non_ue_n2_info_notify_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_n2_information_notification_convertToJSON(non_ue_n2_info_notify_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (non_ue_n2_info_notify_request->binary_data_n2_information) {
    char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(non_ue_n2_info_notify_request->binary_data_n2_information->data,non_ue_n2_info_notify_request->binary_data_n2_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_convertToJSON() failed [binary_data_n2_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information);
    }

end:
    return item;
}

OpenAPI_non_ue_n2_info_notify_request_t *OpenAPI_non_ue_n2_info_notify_request_parseFromJSON(cJSON *non_ue_n2_info_notify_requestJSON)
{
    OpenAPI_non_ue_n2_info_notify_request_t *non_ue_n2_info_notify_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_n2_information_notification_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n2_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_notify_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_n2_information_notification_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_n2_information_notification_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(non_ue_n2_info_notify_requestJSON, "binaryDataN2Information");
    if (binary_data_n2_information) {
    decoded_str_binary_data_n2_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information);
    if (!cJSON_IsString(binary_data_n2_information)) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
    if (!decoded_str_binary_data_n2_information->data) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    }

    non_ue_n2_info_notify_request_local_var = OpenAPI_non_ue_n2_info_notify_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n2_information ? decoded_str_binary_data_n2_information : NULL
    );

    return non_ue_n2_info_notify_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_n2_information_notification_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_non_ue_n2_info_notify_request_t *OpenAPI_non_ue_n2_info_notify_request_copy(OpenAPI_non_ue_n2_info_notify_request_t *dst, OpenAPI_non_ue_n2_info_notify_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_non_ue_n2_info_notify_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_non_ue_n2_info_notify_request_convertToJSON() failed");
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

    OpenAPI_non_ue_n2_info_notify_request_free(dst);
    dst = OpenAPI_non_ue_n2_info_notify_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

