
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_info_notify_handover_complete_200_response.h"

OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_create(
    OpenAPI_n2_info_notification_rsp_data_t *json_data,
    OpenAPI_binary_t* binary_data_n2_information_ext1,
    OpenAPI_binary_t* binary_data_n2_information_ext2,
    OpenAPI_binary_t* binary_data_n2_information_ext3,
    OpenAPI_binary_t* binary_data_n2_information_ext4,
    OpenAPI_binary_t* binary_data_n2_information_ext5,
    OpenAPI_binary_t* binary_data_n2_information_ext6,
    OpenAPI_binary_t* binary_data_n2_information_ext7,
    OpenAPI_binary_t* binary_data_n2_information_ext8,
    OpenAPI_binary_t* binary_data_n2_information_ext9,
    OpenAPI_binary_t* binary_data_n2_information_ext10,
    OpenAPI_binary_t* binary_data_n2_information_ext11,
    OpenAPI_binary_t* binary_data_n2_information_ext12,
    OpenAPI_binary_t* binary_data_n2_information_ext13,
    OpenAPI_binary_t* binary_data_n2_information_ext14,
    OpenAPI_binary_t* binary_data_n2_information_ext15,
    OpenAPI_binary_t* binary_data_n2_information_ext16
)
{
    OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response_local_var = ogs_malloc(sizeof(OpenAPI_n2_info_notify_handover_complete_200_response_t));
    ogs_assert(n2_info_notify_handover_complete_200_response_local_var);

    n2_info_notify_handover_complete_200_response_local_var->json_data = json_data;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext1 = binary_data_n2_information_ext1;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext2 = binary_data_n2_information_ext2;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext3 = binary_data_n2_information_ext3;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext4 = binary_data_n2_information_ext4;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext5 = binary_data_n2_information_ext5;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext6 = binary_data_n2_information_ext6;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext7 = binary_data_n2_information_ext7;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext8 = binary_data_n2_information_ext8;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext9 = binary_data_n2_information_ext9;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext10 = binary_data_n2_information_ext10;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext11 = binary_data_n2_information_ext11;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext12 = binary_data_n2_information_ext12;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext13 = binary_data_n2_information_ext13;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext14 = binary_data_n2_information_ext14;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext15 = binary_data_n2_information_ext15;
    n2_info_notify_handover_complete_200_response_local_var->binary_data_n2_information_ext16 = binary_data_n2_information_ext16;

    return n2_info_notify_handover_complete_200_response_local_var;
}

void OpenAPI_n2_info_notify_handover_complete_200_response_free(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_info_notify_handover_complete_200_response) {
        return;
    }
    if (n2_info_notify_handover_complete_200_response->json_data) {
        OpenAPI_n2_info_notification_rsp_data_free(n2_info_notify_handover_complete_200_response->json_data);
        n2_info_notify_handover_complete_200_response->json_data = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15 = NULL;
    }
    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16) {
        ogs_free(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16->data);
        n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16 = NULL;
    }
    ogs_free(n2_info_notify_handover_complete_200_response);
}

cJSON *OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON(OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_info_notify_handover_complete_200_response == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [N2InfoNotifyHandoverComplete_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (n2_info_notify_handover_complete_200_response->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_n2_info_notification_rsp_data_convertToJSON(n2_info_notify_handover_complete_200_response->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1) {
    char* encoded_str_binary_data_n2_information_ext1 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext1->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt1", encoded_str_binary_data_n2_information_ext1) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext1);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2) {
    char* encoded_str_binary_data_n2_information_ext2 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext2->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt2", encoded_str_binary_data_n2_information_ext2) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext2);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3) {
    char* encoded_str_binary_data_n2_information_ext3 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext3->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt3", encoded_str_binary_data_n2_information_ext3) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext3);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4) {
    char* encoded_str_binary_data_n2_information_ext4 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext4->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt4", encoded_str_binary_data_n2_information_ext4) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext4);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5) {
    char* encoded_str_binary_data_n2_information_ext5 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext5->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt5", encoded_str_binary_data_n2_information_ext5) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext5);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6) {
    char* encoded_str_binary_data_n2_information_ext6 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext6->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt6", encoded_str_binary_data_n2_information_ext6) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext6);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7) {
    char* encoded_str_binary_data_n2_information_ext7 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext7->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt7", encoded_str_binary_data_n2_information_ext7) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext7);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8) {
    char* encoded_str_binary_data_n2_information_ext8 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext8->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt8", encoded_str_binary_data_n2_information_ext8) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext8);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9) {
    char* encoded_str_binary_data_n2_information_ext9 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext9->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt9", encoded_str_binary_data_n2_information_ext9) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext9);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10) {
    char* encoded_str_binary_data_n2_information_ext10 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext10->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt10", encoded_str_binary_data_n2_information_ext10) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext10);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11) {
    char* encoded_str_binary_data_n2_information_ext11 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext11->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt11", encoded_str_binary_data_n2_information_ext11) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext11);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12) {
    char* encoded_str_binary_data_n2_information_ext12 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext12->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt12", encoded_str_binary_data_n2_information_ext12) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext12);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13) {
    char* encoded_str_binary_data_n2_information_ext13 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext13->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt13", encoded_str_binary_data_n2_information_ext13) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext13);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14) {
    char* encoded_str_binary_data_n2_information_ext14 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext14->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt14", encoded_str_binary_data_n2_information_ext14) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext14);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15) {
    char* encoded_str_binary_data_n2_information_ext15 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext15->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt15", encoded_str_binary_data_n2_information_ext15) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext15);
    }

    if (n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16) {
    char* encoded_str_binary_data_n2_information_ext16 = OpenAPI_base64encode(n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16->data,n2_info_notify_handover_complete_200_response->binary_data_n2_information_ext16->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt16", encoded_str_binary_data_n2_information_ext16) == NULL) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext16);
    }

end:
    return item;
}

OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON(cJSON *n2_info_notify_handover_complete_200_responseJSON)
{
    OpenAPI_n2_info_notify_handover_complete_200_response_t *n2_info_notify_handover_complete_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_n2_info_notification_rsp_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_n2_information_ext1 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext1 = NULL;
    cJSON *binary_data_n2_information_ext2 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext2 = NULL;
    cJSON *binary_data_n2_information_ext3 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext3 = NULL;
    cJSON *binary_data_n2_information_ext4 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext4 = NULL;
    cJSON *binary_data_n2_information_ext5 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext5 = NULL;
    cJSON *binary_data_n2_information_ext6 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext6 = NULL;
    cJSON *binary_data_n2_information_ext7 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext7 = NULL;
    cJSON *binary_data_n2_information_ext8 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext8 = NULL;
    cJSON *binary_data_n2_information_ext9 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext9 = NULL;
    cJSON *binary_data_n2_information_ext10 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext10 = NULL;
    cJSON *binary_data_n2_information_ext11 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext11 = NULL;
    cJSON *binary_data_n2_information_ext12 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext12 = NULL;
    cJSON *binary_data_n2_information_ext13 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext13 = NULL;
    cJSON *binary_data_n2_information_ext14 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext14 = NULL;
    cJSON *binary_data_n2_information_ext15 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext15 = NULL;
    cJSON *binary_data_n2_information_ext16 = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information_ext16 = NULL;
    json_data = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_n2_info_notification_rsp_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_n2_info_notification_rsp_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_n2_information_ext1 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt1");
    if (binary_data_n2_information_ext1) {
    decoded_str_binary_data_n2_information_ext1 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext1);
    if (!cJSON_IsString(binary_data_n2_information_ext1)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext1->data = OpenAPI_base64decode(binary_data_n2_information_ext1->valuestring, strlen(binary_data_n2_information_ext1->valuestring), &decoded_str_binary_data_n2_information_ext1->len);
    if (!decoded_str_binary_data_n2_information_ext1->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    }

    binary_data_n2_information_ext2 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt2");
    if (binary_data_n2_information_ext2) {
    decoded_str_binary_data_n2_information_ext2 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext2);
    if (!cJSON_IsString(binary_data_n2_information_ext2)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext2->data = OpenAPI_base64decode(binary_data_n2_information_ext2->valuestring, strlen(binary_data_n2_information_ext2->valuestring), &decoded_str_binary_data_n2_information_ext2->len);
    if (!decoded_str_binary_data_n2_information_ext2->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    }

    binary_data_n2_information_ext3 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt3");
    if (binary_data_n2_information_ext3) {
    decoded_str_binary_data_n2_information_ext3 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext3);
    if (!cJSON_IsString(binary_data_n2_information_ext3)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext3->data = OpenAPI_base64decode(binary_data_n2_information_ext3->valuestring, strlen(binary_data_n2_information_ext3->valuestring), &decoded_str_binary_data_n2_information_ext3->len);
    if (!decoded_str_binary_data_n2_information_ext3->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    }

    binary_data_n2_information_ext4 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt4");
    if (binary_data_n2_information_ext4) {
    decoded_str_binary_data_n2_information_ext4 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext4);
    if (!cJSON_IsString(binary_data_n2_information_ext4)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext4->data = OpenAPI_base64decode(binary_data_n2_information_ext4->valuestring, strlen(binary_data_n2_information_ext4->valuestring), &decoded_str_binary_data_n2_information_ext4->len);
    if (!decoded_str_binary_data_n2_information_ext4->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    }

    binary_data_n2_information_ext5 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt5");
    if (binary_data_n2_information_ext5) {
    decoded_str_binary_data_n2_information_ext5 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext5);
    if (!cJSON_IsString(binary_data_n2_information_ext5)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext5->data = OpenAPI_base64decode(binary_data_n2_information_ext5->valuestring, strlen(binary_data_n2_information_ext5->valuestring), &decoded_str_binary_data_n2_information_ext5->len);
    if (!decoded_str_binary_data_n2_information_ext5->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    }

    binary_data_n2_information_ext6 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt6");
    if (binary_data_n2_information_ext6) {
    decoded_str_binary_data_n2_information_ext6 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext6);
    if (!cJSON_IsString(binary_data_n2_information_ext6)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext6->data = OpenAPI_base64decode(binary_data_n2_information_ext6->valuestring, strlen(binary_data_n2_information_ext6->valuestring), &decoded_str_binary_data_n2_information_ext6->len);
    if (!decoded_str_binary_data_n2_information_ext6->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    }

    binary_data_n2_information_ext7 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt7");
    if (binary_data_n2_information_ext7) {
    decoded_str_binary_data_n2_information_ext7 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext7);
    if (!cJSON_IsString(binary_data_n2_information_ext7)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext7->data = OpenAPI_base64decode(binary_data_n2_information_ext7->valuestring, strlen(binary_data_n2_information_ext7->valuestring), &decoded_str_binary_data_n2_information_ext7->len);
    if (!decoded_str_binary_data_n2_information_ext7->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    }

    binary_data_n2_information_ext8 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt8");
    if (binary_data_n2_information_ext8) {
    decoded_str_binary_data_n2_information_ext8 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext8);
    if (!cJSON_IsString(binary_data_n2_information_ext8)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext8->data = OpenAPI_base64decode(binary_data_n2_information_ext8->valuestring, strlen(binary_data_n2_information_ext8->valuestring), &decoded_str_binary_data_n2_information_ext8->len);
    if (!decoded_str_binary_data_n2_information_ext8->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    }

    binary_data_n2_information_ext9 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt9");
    if (binary_data_n2_information_ext9) {
    decoded_str_binary_data_n2_information_ext9 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext9);
    if (!cJSON_IsString(binary_data_n2_information_ext9)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext9->data = OpenAPI_base64decode(binary_data_n2_information_ext9->valuestring, strlen(binary_data_n2_information_ext9->valuestring), &decoded_str_binary_data_n2_information_ext9->len);
    if (!decoded_str_binary_data_n2_information_ext9->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    }

    binary_data_n2_information_ext10 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt10");
    if (binary_data_n2_information_ext10) {
    decoded_str_binary_data_n2_information_ext10 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext10);
    if (!cJSON_IsString(binary_data_n2_information_ext10)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext10->data = OpenAPI_base64decode(binary_data_n2_information_ext10->valuestring, strlen(binary_data_n2_information_ext10->valuestring), &decoded_str_binary_data_n2_information_ext10->len);
    if (!decoded_str_binary_data_n2_information_ext10->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    }

    binary_data_n2_information_ext11 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt11");
    if (binary_data_n2_information_ext11) {
    decoded_str_binary_data_n2_information_ext11 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext11);
    if (!cJSON_IsString(binary_data_n2_information_ext11)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext11->data = OpenAPI_base64decode(binary_data_n2_information_ext11->valuestring, strlen(binary_data_n2_information_ext11->valuestring), &decoded_str_binary_data_n2_information_ext11->len);
    if (!decoded_str_binary_data_n2_information_ext11->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    }

    binary_data_n2_information_ext12 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt12");
    if (binary_data_n2_information_ext12) {
    decoded_str_binary_data_n2_information_ext12 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext12);
    if (!cJSON_IsString(binary_data_n2_information_ext12)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext12->data = OpenAPI_base64decode(binary_data_n2_information_ext12->valuestring, strlen(binary_data_n2_information_ext12->valuestring), &decoded_str_binary_data_n2_information_ext12->len);
    if (!decoded_str_binary_data_n2_information_ext12->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    }

    binary_data_n2_information_ext13 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt13");
    if (binary_data_n2_information_ext13) {
    decoded_str_binary_data_n2_information_ext13 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext13);
    if (!cJSON_IsString(binary_data_n2_information_ext13)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext13->data = OpenAPI_base64decode(binary_data_n2_information_ext13->valuestring, strlen(binary_data_n2_information_ext13->valuestring), &decoded_str_binary_data_n2_information_ext13->len);
    if (!decoded_str_binary_data_n2_information_ext13->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    }

    binary_data_n2_information_ext14 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt14");
    if (binary_data_n2_information_ext14) {
    decoded_str_binary_data_n2_information_ext14 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext14);
    if (!cJSON_IsString(binary_data_n2_information_ext14)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext14->data = OpenAPI_base64decode(binary_data_n2_information_ext14->valuestring, strlen(binary_data_n2_information_ext14->valuestring), &decoded_str_binary_data_n2_information_ext14->len);
    if (!decoded_str_binary_data_n2_information_ext14->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    }

    binary_data_n2_information_ext15 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt15");
    if (binary_data_n2_information_ext15) {
    decoded_str_binary_data_n2_information_ext15 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext15);
    if (!cJSON_IsString(binary_data_n2_information_ext15)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext15->data = OpenAPI_base64decode(binary_data_n2_information_ext15->valuestring, strlen(binary_data_n2_information_ext15->valuestring), &decoded_str_binary_data_n2_information_ext15->len);
    if (!decoded_str_binary_data_n2_information_ext15->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    }

    binary_data_n2_information_ext16 = cJSON_GetObjectItemCaseSensitive(n2_info_notify_handover_complete_200_responseJSON, "binaryDataN2InformationExt16");
    if (binary_data_n2_information_ext16) {
    decoded_str_binary_data_n2_information_ext16 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext16);
    if (!cJSON_IsString(binary_data_n2_information_ext16)) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext16->data = OpenAPI_base64decode(binary_data_n2_information_ext16->valuestring, strlen(binary_data_n2_information_ext16->valuestring), &decoded_str_binary_data_n2_information_ext16->len);
    if (!decoded_str_binary_data_n2_information_ext16->data) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    }

    n2_info_notify_handover_complete_200_response_local_var = OpenAPI_n2_info_notify_handover_complete_200_response_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_n2_information_ext1 ? decoded_str_binary_data_n2_information_ext1 : NULL,
        binary_data_n2_information_ext2 ? decoded_str_binary_data_n2_information_ext2 : NULL,
        binary_data_n2_information_ext3 ? decoded_str_binary_data_n2_information_ext3 : NULL,
        binary_data_n2_information_ext4 ? decoded_str_binary_data_n2_information_ext4 : NULL,
        binary_data_n2_information_ext5 ? decoded_str_binary_data_n2_information_ext5 : NULL,
        binary_data_n2_information_ext6 ? decoded_str_binary_data_n2_information_ext6 : NULL,
        binary_data_n2_information_ext7 ? decoded_str_binary_data_n2_information_ext7 : NULL,
        binary_data_n2_information_ext8 ? decoded_str_binary_data_n2_information_ext8 : NULL,
        binary_data_n2_information_ext9 ? decoded_str_binary_data_n2_information_ext9 : NULL,
        binary_data_n2_information_ext10 ? decoded_str_binary_data_n2_information_ext10 : NULL,
        binary_data_n2_information_ext11 ? decoded_str_binary_data_n2_information_ext11 : NULL,
        binary_data_n2_information_ext12 ? decoded_str_binary_data_n2_information_ext12 : NULL,
        binary_data_n2_information_ext13 ? decoded_str_binary_data_n2_information_ext13 : NULL,
        binary_data_n2_information_ext14 ? decoded_str_binary_data_n2_information_ext14 : NULL,
        binary_data_n2_information_ext15 ? decoded_str_binary_data_n2_information_ext15 : NULL,
        binary_data_n2_information_ext16 ? decoded_str_binary_data_n2_information_ext16 : NULL
    );

    return n2_info_notify_handover_complete_200_response_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_n2_info_notification_rsp_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_n2_info_notify_handover_complete_200_response_t *OpenAPI_n2_info_notify_handover_complete_200_response_copy(OpenAPI_n2_info_notify_handover_complete_200_response_t *dst, OpenAPI_n2_info_notify_handover_complete_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_info_notify_handover_complete_200_response_convertToJSON() failed");
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

    OpenAPI_n2_info_notify_handover_complete_200_response_free(dst);
    dst = OpenAPI_n2_info_notify_handover_complete_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

