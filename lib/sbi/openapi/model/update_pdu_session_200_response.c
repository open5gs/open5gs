
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "update_pdu_session_200_response.h"

OpenAPI_update_pdu_session_200_response_t *OpenAPI_update_pdu_session_200_response_create(
    OpenAPI_hsmf_updated_data_t *json_data,
    OpenAPI_binary_t* binary_data_n1_sm_info_to_ue,
    OpenAPI_binary_t* binary_data_n4_information,
    OpenAPI_binary_t* binary_data_n4_information_ext1,
    OpenAPI_binary_t* binary_data_n4_information_ext2
)
{
    OpenAPI_update_pdu_session_200_response_t *update_pdu_session_200_response_local_var = ogs_malloc(sizeof(OpenAPI_update_pdu_session_200_response_t));
    ogs_assert(update_pdu_session_200_response_local_var);

    update_pdu_session_200_response_local_var->json_data = json_data;
    update_pdu_session_200_response_local_var->binary_data_n1_sm_info_to_ue = binary_data_n1_sm_info_to_ue;
    update_pdu_session_200_response_local_var->binary_data_n4_information = binary_data_n4_information;
    update_pdu_session_200_response_local_var->binary_data_n4_information_ext1 = binary_data_n4_information_ext1;
    update_pdu_session_200_response_local_var->binary_data_n4_information_ext2 = binary_data_n4_information_ext2;

    return update_pdu_session_200_response_local_var;
}

void OpenAPI_update_pdu_session_200_response_free(OpenAPI_update_pdu_session_200_response_t *update_pdu_session_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == update_pdu_session_200_response) {
        return;
    }
    if (update_pdu_session_200_response->json_data) {
        OpenAPI_hsmf_updated_data_free(update_pdu_session_200_response->json_data);
        update_pdu_session_200_response->json_data = NULL;
    }
    if (update_pdu_session_200_response->binary_data_n1_sm_info_to_ue) {
        ogs_free(update_pdu_session_200_response->binary_data_n1_sm_info_to_ue->data);
        update_pdu_session_200_response->binary_data_n1_sm_info_to_ue = NULL;
    }
    if (update_pdu_session_200_response->binary_data_n4_information) {
        ogs_free(update_pdu_session_200_response->binary_data_n4_information->data);
        update_pdu_session_200_response->binary_data_n4_information = NULL;
    }
    if (update_pdu_session_200_response->binary_data_n4_information_ext1) {
        ogs_free(update_pdu_session_200_response->binary_data_n4_information_ext1->data);
        update_pdu_session_200_response->binary_data_n4_information_ext1 = NULL;
    }
    if (update_pdu_session_200_response->binary_data_n4_information_ext2) {
        ogs_free(update_pdu_session_200_response->binary_data_n4_information_ext2->data);
        update_pdu_session_200_response->binary_data_n4_information_ext2 = NULL;
    }
    ogs_free(update_pdu_session_200_response);
}

cJSON *OpenAPI_update_pdu_session_200_response_convertToJSON(OpenAPI_update_pdu_session_200_response_t *update_pdu_session_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (update_pdu_session_200_response == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [UpdatePduSession_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (update_pdu_session_200_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_hsmf_updated_data_convertToJSON(update_pdu_session_200_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (update_pdu_session_200_response->binary_data_n1_sm_info_to_ue) {
    char* encoded_str_binary_data_n1_sm_info_to_ue = OpenAPI_base64encode(update_pdu_session_200_response->binary_data_n1_sm_info_to_ue->data,update_pdu_session_200_response->binary_data_n1_sm_info_to_ue->len);
    if (cJSON_AddStringToObject(item, "binaryDataN1SmInfoToUe", encoded_str_binary_data_n1_sm_info_to_ue) == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n1_sm_info_to_ue);
    }

    if (update_pdu_session_200_response->binary_data_n4_information) {
    char* encoded_str_binary_data_n4_information = OpenAPI_base64encode(update_pdu_session_200_response->binary_data_n4_information->data,update_pdu_session_200_response->binary_data_n4_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN4Information", encoded_str_binary_data_n4_information) == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [binary_data_n4_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n4_information);
    }

    if (update_pdu_session_200_response->binary_data_n4_information_ext1) {
    char* encoded_str_binary_data_n4_information_ext1 = OpenAPI_base64encode(update_pdu_session_200_response->binary_data_n4_information_ext1->data,update_pdu_session_200_response->binary_data_n4_information_ext1->len);
    if (cJSON_AddStringToObject(item, "binaryDataN4InformationExt1", encoded_str_binary_data_n4_information_ext1) == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [binary_data_n4_information_ext1]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n4_information_ext1);
    }

    if (update_pdu_session_200_response->binary_data_n4_information_ext2) {
    char* encoded_str_binary_data_n4_information_ext2 = OpenAPI_base64encode(update_pdu_session_200_response->binary_data_n4_information_ext2->data,update_pdu_session_200_response->binary_data_n4_information_ext2->len);
    if (cJSON_AddStringToObject(item, "binaryDataN4InformationExt2", encoded_str_binary_data_n4_information_ext2) == NULL) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed [binary_data_n4_information_ext2]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n4_information_ext2);
    }

end:
    return item;
}

OpenAPI_update_pdu_session_200_response_t *OpenAPI_update_pdu_session_200_response_parseFromJSON(cJSON *update_pdu_session_200_responseJSON)
{
    OpenAPI_update_pdu_session_200_response_t *update_pdu_session_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_hsmf_updated_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n1_sm_info_to_ue = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n1_sm_info_to_ue = NULL;
    cJSON *binary_data_n4_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n4_information = NULL;
    cJSON *binary_data_n4_information_ext1 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n4_information_ext1 = NULL;
    cJSON *binary_data_n4_information_ext2 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n4_information_ext2 = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(update_pdu_session_200_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_hsmf_updated_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_hsmf_updated_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n1_sm_info_to_ue = cJSON_GetObjectItemCaseSensitive(update_pdu_session_200_responseJSON, "binaryDataN1SmInfoToUe");
    if (binary_data_n1_sm_info_to_ue) {
    decoded_str_binary_data_n1_sm_info_to_ue = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n1_sm_info_to_ue);
    if (!cJSON_IsString(binary_data_n1_sm_info_to_ue)) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    decoded_str_binary_data_n1_sm_info_to_ue->data = OpenAPI_base64decode(binary_data_n1_sm_info_to_ue->valuestring, strlen(binary_data_n1_sm_info_to_ue->valuestring), &decoded_str_binary_data_n1_sm_info_to_ue->len);
    if (!decoded_str_binary_data_n1_sm_info_to_ue->data) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n1_sm_info_to_ue]");
        goto end;
    }
    }

    binary_data_n4_information = cJSON_GetObjectItemCaseSensitive(update_pdu_session_200_responseJSON, "binaryDataN4Information");
    if (binary_data_n4_information) {
    decoded_str_binary_data_n4_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n4_information);
    if (!cJSON_IsString(binary_data_n4_information)) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information]");
        goto end;
    }
    decoded_str_binary_data_n4_information->data = OpenAPI_base64decode(binary_data_n4_information->valuestring, strlen(binary_data_n4_information->valuestring), &decoded_str_binary_data_n4_information->len);
    if (!decoded_str_binary_data_n4_information->data) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information]");
        goto end;
    }
    }

    binary_data_n4_information_ext1 = cJSON_GetObjectItemCaseSensitive(update_pdu_session_200_responseJSON, "binaryDataN4InformationExt1");
    if (binary_data_n4_information_ext1) {
    decoded_str_binary_data_n4_information_ext1 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n4_information_ext1);
    if (!cJSON_IsString(binary_data_n4_information_ext1)) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information_ext1]");
        goto end;
    }
    decoded_str_binary_data_n4_information_ext1->data = OpenAPI_base64decode(binary_data_n4_information_ext1->valuestring, strlen(binary_data_n4_information_ext1->valuestring), &decoded_str_binary_data_n4_information_ext1->len);
    if (!decoded_str_binary_data_n4_information_ext1->data) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information_ext1]");
        goto end;
    }
    }

    binary_data_n4_information_ext2 = cJSON_GetObjectItemCaseSensitive(update_pdu_session_200_responseJSON, "binaryDataN4InformationExt2");
    if (binary_data_n4_information_ext2) {
    decoded_str_binary_data_n4_information_ext2 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n4_information_ext2);
    if (!cJSON_IsString(binary_data_n4_information_ext2)) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information_ext2]");
        goto end;
    }
    decoded_str_binary_data_n4_information_ext2->data = OpenAPI_base64decode(binary_data_n4_information_ext2->valuestring, strlen(binary_data_n4_information_ext2->valuestring), &decoded_str_binary_data_n4_information_ext2->len);
    if (!decoded_str_binary_data_n4_information_ext2->data) {
        ogs_error("OpenAPI_update_pdu_session_200_response_parseFromJSON() failed [binary_data_n4_information_ext2]");
        goto end;
    }
    }

    update_pdu_session_200_response_local_var = OpenAPI_update_pdu_session_200_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n1_sm_info_to_ue ? decoded_str_binary_data_n1_sm_info_to_ue : NULL,
        binary_data_n4_information ? decoded_str_binary_data_n4_information : NULL,
        binary_data_n4_information_ext1 ? decoded_str_binary_data_n4_information_ext1 : NULL,
        binary_data_n4_information_ext2 ? decoded_str_binary_data_n4_information_ext2 : NULL
    );

    return update_pdu_session_200_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_hsmf_updated_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_update_pdu_session_200_response_t *OpenAPI_update_pdu_session_200_response_copy(OpenAPI_update_pdu_session_200_response_t *dst, OpenAPI_update_pdu_session_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_update_pdu_session_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_update_pdu_session_200_response_convertToJSON() failed");
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

    OpenAPI_update_pdu_session_200_response_free(dst);
    dst = OpenAPI_update_pdu_session_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

