
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "create_ue_context_403_response.h"

OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_create(
    OpenAPI_ue_context_create_error_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information
)
{
    OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response_local_var = ogs_malloc(sizeof(OpenAPI_create_ue_context_403_response_t));
    ogs_assert(create_ue_context_403_response_local_var);

    create_ue_context_403_response_local_var->json_data = json_data;
    create_ue_context_403_response_local_var->binary_data_n2_information = binary_data_n2_information;

    return create_ue_context_403_response_local_var;
}

void OpenAPI_create_ue_context_403_response_free(OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == create_ue_context_403_response) {
        return;
    }
    if (create_ue_context_403_response->json_data) {
        OpenAPI_ue_context_create_error_free(create_ue_context_403_response->json_data);
        create_ue_context_403_response->json_data = NULL;
    }
    if (create_ue_context_403_response->binary_data_n2_information) {
        ogs_free(create_ue_context_403_response->binary_data_n2_information->data);
        create_ue_context_403_response->binary_data_n2_information = NULL;
    }
    ogs_free(create_ue_context_403_response);
}

cJSON *OpenAPI_create_ue_context_403_response_convertToJSON(OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (create_ue_context_403_response == NULL) {
        ogs_error("OpenAPI_create_ue_context_403_response_convertToJSON() failed [CreateUEContext_403_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (create_ue_context_403_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_ue_context_create_error_convertToJSON(create_ue_context_403_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_create_ue_context_403_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_create_ue_context_403_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (create_ue_context_403_response->binary_data_n2_information) {
    char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(create_ue_context_403_response->binary_data_n2_information->data,create_ue_context_403_response->binary_data_n2_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
        ogs_error("OpenAPI_create_ue_context_403_response_convertToJSON() failed [binary_data_n2_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information);
    }

end:
    return item;
}

OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_parseFromJSON(cJSON *create_ue_context_403_responseJSON)
{
    OpenAPI_create_ue_context_403_response_t *create_ue_context_403_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_ue_context_create_error_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n2_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(create_ue_context_403_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_ue_context_create_error_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_create_error_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(create_ue_context_403_responseJSON, "binaryDataN2Information");
    if (binary_data_n2_information) {
    decoded_str_binary_data_n2_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information);
    if (!cJSON_IsString(binary_data_n2_information)) {
        ogs_error("OpenAPI_create_ue_context_403_response_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
    if (!decoded_str_binary_data_n2_information->data) {
        ogs_error("OpenAPI_create_ue_context_403_response_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    }

    create_ue_context_403_response_local_var = OpenAPI_create_ue_context_403_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n2_information ? decoded_str_binary_data_n2_information : NULL
    );

    return create_ue_context_403_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_ue_context_create_error_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_create_ue_context_403_response_t *OpenAPI_create_ue_context_403_response_copy(OpenAPI_create_ue_context_403_response_t *dst, OpenAPI_create_ue_context_403_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_create_ue_context_403_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_create_ue_context_403_response_convertToJSON() failed");
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

    OpenAPI_create_ue_context_403_response_free(dst);
    dst = OpenAPI_create_ue_context_403_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

