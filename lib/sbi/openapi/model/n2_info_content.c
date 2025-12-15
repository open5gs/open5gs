
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_content.h"

// Simple hex decode function
static uint8_t* hex_decode(const char* hex_string, int* decoded_len) {
    int len = strlen(hex_string);
    if (len % 2 != 0) {
        *decoded_len = 0;
        return NULL;
    }
    
    *decoded_len = len / 2;
    uint8_t* decoded = ogs_malloc(*decoded_len);
    if (!decoded) {
        *decoded_len = 0;
        return NULL;
    }
    
    int i;
    for (i = 0; i < *decoded_len; i++) {
        char hex_byte[3] = {hex_string[i*2], hex_string[i*2+1], '\0'};
        decoded[i] = (uint8_t)strtol(hex_byte, NULL, 16);
    }
    
    return decoded;
}

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_create(
    bool is_ngap_message_type,
    int ngap_message_type,
    OpenAPI_ngap_ie_type_e ngap_ie_type,
    OpenAPI_ref_to_binary_data_t *ngap_data
)
{
    OpenAPI_n2_info_content_t *n2_info_content_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_content_t));
    ogs_assert(n2_info_content_local_var);

    n2_info_content_local_var->is_ngap_message_type = is_ngap_message_type;
    n2_info_content_local_var->ngap_message_type = ngap_message_type;
    n2_info_content_local_var->ngap_ie_type = ngap_ie_type;
    n2_info_content_local_var->ngap_data = ngap_data;

    return n2_info_content_local_var;
}

void OpenAPI_n2_info_content_free(OpenAPI_n2_info_content_t *n2_info_content)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_info_content) {
        return;
    }
    if (n2_info_content->ngap_data) {
        OpenAPI_ref_to_binary_data_free(n2_info_content->ngap_data);
        n2_info_content->ngap_data = NULL;
    }
    ogs_free(n2_info_content);
}

cJSON *OpenAPI_n2_info_content_convertToJSON(OpenAPI_n2_info_content_t *n2_info_content)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_info_content == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [N2InfoContent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_content->is_ngap_message_type) {
    if (cJSON_AddNumberToObject(item, "ngapMessageType", n2_info_content->ngap_message_type) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_message_type]");
        goto end;
    }
    }

    if (n2_info_content->ngap_ie_type != OpenAPI_ngap_ie_type_NULL) {
    if (cJSON_AddStringToObject(item, "ngapIeType", OpenAPI_ngap_ie_type_ToString(n2_info_content->ngap_ie_type)) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_ie_type]");
        goto end;
    }
    }

    if (!n2_info_content->ngap_data) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        return NULL;
    }
    cJSON *ngap_data_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n2_info_content->ngap_data);
    if (ngap_data_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapData", ngap_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [ngap_data]");
        goto end;
    }

    // Add custom PWS fields to JSON
    if (cJSON_AddNumberToObject(item, "messageIdentifier", n2_info_content->message_identifier) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [message_identifier]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "serialNumber", n2_info_content->serial_number) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [serial_number]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "noOfTai", n2_info_content->no_of_tai) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [no_of_tai]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "repetitionPeriod", n2_info_content->repetition_period) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [repetition_period]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "numberOfBroadcast", n2_info_content->number_of_broadcast) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [number_of_broadcast]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "dataCodingScheme", n2_info_content->data_coding_scheme) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [data_coding_scheme]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "messageLength", n2_info_content->message_length) == NULL) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [message_length]");
        goto end;
    }
    
    // Convert message_contents to hex string for JSON
    if (n2_info_content->message_length > 0) {
        char *hex_content = ogs_malloc(n2_info_content->message_length * 2 + 1);
        if (hex_content) {
            uint32_t i;
            for (i = 0; i < n2_info_content->message_length; i++) {
                sprintf(hex_content + i * 2, "%02X", n2_info_content->message_contents[i]);
            }
            if (cJSON_AddStringToObject(item, "messageContents", hex_content) == NULL) {
                ogs_error("OpenAPI_n2_info_content_convertToJSON() failed [message_contents]");
                ogs_free(hex_content);
                goto end;
            }
            ogs_free(hex_content);
        }
    }

end:
    return item;
}

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_parseFromJSON(cJSON *n2_info_contentJSON)
{
    OpenAPI_n2_info_content_t *n2_info_content_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ngap_message_type = NULL;
    cJSON *ngap_ie_type = NULL;
    OpenAPI_ngap_ie_type_e ngap_ie_typeVariable = 0;
    cJSON *ngap_data = NULL;
    OpenAPI_ref_to_binary_data_t *ngap_data_local_nonprim = NULL;
    cJSON *message_identifier = NULL;
    cJSON *serial_number = NULL;
    cJSON *no_of_tai = NULL;
    cJSON *repetition_period = NULL;
    cJSON *number_of_broadcast = NULL;
    cJSON *data_coding_scheme = NULL;
    cJSON *message_length = NULL;
    cJSON *message_contents = NULL;
    
    ngap_message_type = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapMessageType");
    if (ngap_message_type) {
    if (!cJSON_IsNumber(ngap_message_type)) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_message_type]");
        goto end;
    }
    }

    ngap_ie_type = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapIeType");
    if (ngap_ie_type) {
    if (!cJSON_IsString(ngap_ie_type)) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_ie_type]");
        goto end;
    }
    ngap_ie_typeVariable = OpenAPI_ngap_ie_type_FromString(ngap_ie_type->valuestring);
    }

    ngap_data = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "ngapData");
    if (!ngap_data) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON() failed [ngap_data]");
        goto end;
    }
    ngap_data_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(ngap_data);
    if (!ngap_data_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [ngap_data]");
        goto end;
    }

    n2_info_content_local_var = OpenAPI_n2_info_content_create (
        ngap_message_type ? true : false,
        ngap_message_type ? ngap_message_type->valuedouble : 0,
        ngap_ie_type ? ngap_ie_typeVariable : 0,
        ngap_data_local_nonprim
    );

    // Parse custom PWS fields
    message_identifier = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "messageIdentifier");
    if (message_identifier && cJSON_IsNumber(message_identifier)) {
        n2_info_content_local_var->message_identifier = (uint16_t)message_identifier->valuedouble;
    }

    serial_number = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "serialNumber");
    if (serial_number && cJSON_IsNumber(serial_number)) {
        n2_info_content_local_var->serial_number = (uint16_t)serial_number->valuedouble;
    }

    no_of_tai = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "noOfTai");
    if (no_of_tai && cJSON_IsNumber(no_of_tai)) {
        n2_info_content_local_var->no_of_tai = (uint32_t)no_of_tai->valuedouble;
    }

    repetition_period = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "repetitionPeriod");
    if (repetition_period && cJSON_IsNumber(repetition_period)) {
        n2_info_content_local_var->repetition_period = (uint32_t)repetition_period->valuedouble;
    }

    number_of_broadcast = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "numberOfBroadcast");
    if (number_of_broadcast && cJSON_IsNumber(number_of_broadcast)) {
        n2_info_content_local_var->number_of_broadcast = (uint32_t)number_of_broadcast->valuedouble;
    }

    data_coding_scheme = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "dataCodingScheme");
    if (data_coding_scheme && cJSON_IsNumber(data_coding_scheme)) {
        n2_info_content_local_var->data_coding_scheme = (uint8_t)data_coding_scheme->valuedouble;
    }

    message_length = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "messageLength");
    if (message_length && cJSON_IsNumber(message_length)) {
        n2_info_content_local_var->message_length = (uint32_t)message_length->valuedouble;
    }

    message_contents = cJSON_GetObjectItemCaseSensitive(n2_info_contentJSON, "messageContents");
    if (message_contents && cJSON_IsString(message_contents)) {
        int decoded_len = 0;
        uint8_t *decoded_data = hex_decode(message_contents->valuestring, &decoded_len);
        if (decoded_len > 0 && decoded_len <= 1024) {
            n2_info_content_local_var->message_length = (uint32_t)decoded_len;
            memcpy(n2_info_content_local_var->message_contents, decoded_data, decoded_len);
            ogs_free(decoded_data);
        }
    }

    return n2_info_content_local_var;
end:
    if (ngap_data_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(ngap_data_local_nonprim);
        ngap_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_info_content_t *OpenAPI_n2_info_content_copy(OpenAPI_n2_info_content_t *dst, OpenAPI_n2_info_content_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_content_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_content_convertToJSON() failed");
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

    OpenAPI_n2_info_content_free(dst);
    dst = OpenAPI_n2_info_content_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

