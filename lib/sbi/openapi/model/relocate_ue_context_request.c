
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "relocate_ue_context_request.h"

OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_create(
    OpenAPI_ue_context_relocate_data_t *json_data,
    OpenAPI_binary_t* binary_data_forward_relocation_request,
    OpenAPI_binary_t* binary_data_n2_information,
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
    OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request_local_var = ogs_malloc(sizeof(OpenAPI_relocate_ue_context_request_t));
    ogs_assert(relocate_ue_context_request_local_var);

    relocate_ue_context_request_local_var->json_data = json_data;
    relocate_ue_context_request_local_var->binary_data_forward_relocation_request = binary_data_forward_relocation_request;
    relocate_ue_context_request_local_var->binary_data_n2_information = binary_data_n2_information;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext1 = binary_data_n2_information_ext1;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext2 = binary_data_n2_information_ext2;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext3 = binary_data_n2_information_ext3;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext4 = binary_data_n2_information_ext4;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext5 = binary_data_n2_information_ext5;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext6 = binary_data_n2_information_ext6;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext7 = binary_data_n2_information_ext7;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext8 = binary_data_n2_information_ext8;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext9 = binary_data_n2_information_ext9;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext10 = binary_data_n2_information_ext10;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext11 = binary_data_n2_information_ext11;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext12 = binary_data_n2_information_ext12;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext13 = binary_data_n2_information_ext13;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext14 = binary_data_n2_information_ext14;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext15 = binary_data_n2_information_ext15;
    relocate_ue_context_request_local_var->binary_data_n2_information_ext16 = binary_data_n2_information_ext16;

    return relocate_ue_context_request_local_var;
}

void OpenAPI_relocate_ue_context_request_free(OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == relocate_ue_context_request) {
        return;
    }
    if (relocate_ue_context_request->json_data) {
        OpenAPI_ue_context_relocate_data_free(relocate_ue_context_request->json_data);
        relocate_ue_context_request->json_data = NULL;
    }
    if (relocate_ue_context_request->binary_data_forward_relocation_request) {
        ogs_free(relocate_ue_context_request->binary_data_forward_relocation_request->data);
        relocate_ue_context_request->binary_data_forward_relocation_request = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information->data);
        relocate_ue_context_request->binary_data_n2_information = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext1) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext1->data);
        relocate_ue_context_request->binary_data_n2_information_ext1 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext2) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext2->data);
        relocate_ue_context_request->binary_data_n2_information_ext2 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext3) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext3->data);
        relocate_ue_context_request->binary_data_n2_information_ext3 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext4) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext4->data);
        relocate_ue_context_request->binary_data_n2_information_ext4 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext5) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext5->data);
        relocate_ue_context_request->binary_data_n2_information_ext5 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext6) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext6->data);
        relocate_ue_context_request->binary_data_n2_information_ext6 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext7) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext7->data);
        relocate_ue_context_request->binary_data_n2_information_ext7 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext8) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext8->data);
        relocate_ue_context_request->binary_data_n2_information_ext8 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext9) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext9->data);
        relocate_ue_context_request->binary_data_n2_information_ext9 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext10) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext10->data);
        relocate_ue_context_request->binary_data_n2_information_ext10 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext11) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext11->data);
        relocate_ue_context_request->binary_data_n2_information_ext11 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext12) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext12->data);
        relocate_ue_context_request->binary_data_n2_information_ext12 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext13) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext13->data);
        relocate_ue_context_request->binary_data_n2_information_ext13 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext14) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext14->data);
        relocate_ue_context_request->binary_data_n2_information_ext14 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext15) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext15->data);
        relocate_ue_context_request->binary_data_n2_information_ext15 = NULL;
    }
    if (relocate_ue_context_request->binary_data_n2_information_ext16) {
        ogs_free(relocate_ue_context_request->binary_data_n2_information_ext16->data);
        relocate_ue_context_request->binary_data_n2_information_ext16 = NULL;
    }
    ogs_free(relocate_ue_context_request);
}

cJSON *OpenAPI_relocate_ue_context_request_convertToJSON(OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (relocate_ue_context_request == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [RelocateUEContext_request]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (relocate_ue_context_request->json_data) {
    cJSON *json_data_local_JSON = OpenAPI_ue_context_relocate_data_convertToJSON(relocate_ue_context_request->json_data);
    if (json_data_local_JSON == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [json_data]");
        goto end;
    }
    }

    if (relocate_ue_context_request->binary_data_forward_relocation_request) {
    char* encoded_str_binary_data_forward_relocation_request = OpenAPI_base64encode(relocate_ue_context_request->binary_data_forward_relocation_request->data,relocate_ue_context_request->binary_data_forward_relocation_request->len);
    if (cJSON_AddStringToObject(item, "binaryDataForwardRelocationRequest", encoded_str_binary_data_forward_relocation_request) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_forward_relocation_request]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_forward_relocation_request);
    }

    if (relocate_ue_context_request->binary_data_n2_information) {
    char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information->data,relocate_ue_context_request->binary_data_n2_information->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext1) {
    char* encoded_str_binary_data_n2_information_ext1 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext1->data,relocate_ue_context_request->binary_data_n2_information_ext1->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt1", encoded_str_binary_data_n2_information_ext1) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext1);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext2) {
    char* encoded_str_binary_data_n2_information_ext2 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext2->data,relocate_ue_context_request->binary_data_n2_information_ext2->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt2", encoded_str_binary_data_n2_information_ext2) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext2);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext3) {
    char* encoded_str_binary_data_n2_information_ext3 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext3->data,relocate_ue_context_request->binary_data_n2_information_ext3->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt3", encoded_str_binary_data_n2_information_ext3) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext3);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext4) {
    char* encoded_str_binary_data_n2_information_ext4 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext4->data,relocate_ue_context_request->binary_data_n2_information_ext4->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt4", encoded_str_binary_data_n2_information_ext4) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext4);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext5) {
    char* encoded_str_binary_data_n2_information_ext5 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext5->data,relocate_ue_context_request->binary_data_n2_information_ext5->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt5", encoded_str_binary_data_n2_information_ext5) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext5);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext6) {
    char* encoded_str_binary_data_n2_information_ext6 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext6->data,relocate_ue_context_request->binary_data_n2_information_ext6->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt6", encoded_str_binary_data_n2_information_ext6) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext6);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext7) {
    char* encoded_str_binary_data_n2_information_ext7 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext7->data,relocate_ue_context_request->binary_data_n2_information_ext7->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt7", encoded_str_binary_data_n2_information_ext7) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext7);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext8) {
    char* encoded_str_binary_data_n2_information_ext8 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext8->data,relocate_ue_context_request->binary_data_n2_information_ext8->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt8", encoded_str_binary_data_n2_information_ext8) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext8);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext9) {
    char* encoded_str_binary_data_n2_information_ext9 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext9->data,relocate_ue_context_request->binary_data_n2_information_ext9->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt9", encoded_str_binary_data_n2_information_ext9) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext9);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext10) {
    char* encoded_str_binary_data_n2_information_ext10 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext10->data,relocate_ue_context_request->binary_data_n2_information_ext10->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt10", encoded_str_binary_data_n2_information_ext10) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext10);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext11) {
    char* encoded_str_binary_data_n2_information_ext11 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext11->data,relocate_ue_context_request->binary_data_n2_information_ext11->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt11", encoded_str_binary_data_n2_information_ext11) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext11);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext12) {
    char* encoded_str_binary_data_n2_information_ext12 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext12->data,relocate_ue_context_request->binary_data_n2_information_ext12->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt12", encoded_str_binary_data_n2_information_ext12) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext12);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext13) {
    char* encoded_str_binary_data_n2_information_ext13 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext13->data,relocate_ue_context_request->binary_data_n2_information_ext13->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt13", encoded_str_binary_data_n2_information_ext13) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext13);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext14) {
    char* encoded_str_binary_data_n2_information_ext14 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext14->data,relocate_ue_context_request->binary_data_n2_information_ext14->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt14", encoded_str_binary_data_n2_information_ext14) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext14);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext15) {
    char* encoded_str_binary_data_n2_information_ext15 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext15->data,relocate_ue_context_request->binary_data_n2_information_ext15->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt15", encoded_str_binary_data_n2_information_ext15) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext15);
    }

    if (relocate_ue_context_request->binary_data_n2_information_ext16) {
    char* encoded_str_binary_data_n2_information_ext16 = OpenAPI_base64encode(relocate_ue_context_request->binary_data_n2_information_ext16->data,relocate_ue_context_request->binary_data_n2_information_ext16->len);
    if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt16", encoded_str_binary_data_n2_information_ext16) == NULL) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    ogs_free(encoded_str_binary_data_n2_information_ext16);
    }

end:
    return item;
}

OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_parseFromJSON(cJSON *relocate_ue_context_requestJSON)
{
    OpenAPI_relocate_ue_context_request_t *relocate_ue_context_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *json_data = NULL;
    OpenAPI_ue_context_relocate_data_t *json_data_local_nonprim = NULL;
    cJSON *binary_data_forward_relocation_request = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_forward_relocation_request = NULL;
    cJSON *binary_data_n2_information = NULL;
    OpenAPI_binary_t *decoded_str_binary_data_n2_information = NULL;
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
    json_data = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "jsonData");
    if (json_data) {
    json_data_local_nonprim = OpenAPI_ue_context_relocate_data_parseFromJSON(json_data);
    if (!json_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_relocate_data_parseFromJSON failed [json_data]");
        goto end;
    }
    }

    binary_data_forward_relocation_request = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataForwardRelocationRequest");
    if (binary_data_forward_relocation_request) {
    decoded_str_binary_data_forward_relocation_request = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_forward_relocation_request);
    if (!cJSON_IsString(binary_data_forward_relocation_request)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_forward_relocation_request]");
        goto end;
    }
    decoded_str_binary_data_forward_relocation_request->data = OpenAPI_base64decode(binary_data_forward_relocation_request->valuestring, strlen(binary_data_forward_relocation_request->valuestring), &decoded_str_binary_data_forward_relocation_request->len);
    if (!decoded_str_binary_data_forward_relocation_request->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_forward_relocation_request]");
        goto end;
    }
    }

    binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2Information");
    if (binary_data_n2_information) {
    decoded_str_binary_data_n2_information = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information);
    if (!cJSON_IsString(binary_data_n2_information)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
    if (!decoded_str_binary_data_n2_information->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information]");
        goto end;
    }
    }

    binary_data_n2_information_ext1 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt1");
    if (binary_data_n2_information_ext1) {
    decoded_str_binary_data_n2_information_ext1 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext1);
    if (!cJSON_IsString(binary_data_n2_information_ext1)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext1->data = OpenAPI_base64decode(binary_data_n2_information_ext1->valuestring, strlen(binary_data_n2_information_ext1->valuestring), &decoded_str_binary_data_n2_information_ext1->len);
    if (!decoded_str_binary_data_n2_information_ext1->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext1]");
        goto end;
    }
    }

    binary_data_n2_information_ext2 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt2");
    if (binary_data_n2_information_ext2) {
    decoded_str_binary_data_n2_information_ext2 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext2);
    if (!cJSON_IsString(binary_data_n2_information_ext2)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext2->data = OpenAPI_base64decode(binary_data_n2_information_ext2->valuestring, strlen(binary_data_n2_information_ext2->valuestring), &decoded_str_binary_data_n2_information_ext2->len);
    if (!decoded_str_binary_data_n2_information_ext2->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext2]");
        goto end;
    }
    }

    binary_data_n2_information_ext3 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt3");
    if (binary_data_n2_information_ext3) {
    decoded_str_binary_data_n2_information_ext3 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext3);
    if (!cJSON_IsString(binary_data_n2_information_ext3)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext3->data = OpenAPI_base64decode(binary_data_n2_information_ext3->valuestring, strlen(binary_data_n2_information_ext3->valuestring), &decoded_str_binary_data_n2_information_ext3->len);
    if (!decoded_str_binary_data_n2_information_ext3->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext3]");
        goto end;
    }
    }

    binary_data_n2_information_ext4 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt4");
    if (binary_data_n2_information_ext4) {
    decoded_str_binary_data_n2_information_ext4 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext4);
    if (!cJSON_IsString(binary_data_n2_information_ext4)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext4->data = OpenAPI_base64decode(binary_data_n2_information_ext4->valuestring, strlen(binary_data_n2_information_ext4->valuestring), &decoded_str_binary_data_n2_information_ext4->len);
    if (!decoded_str_binary_data_n2_information_ext4->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext4]");
        goto end;
    }
    }

    binary_data_n2_information_ext5 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt5");
    if (binary_data_n2_information_ext5) {
    decoded_str_binary_data_n2_information_ext5 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext5);
    if (!cJSON_IsString(binary_data_n2_information_ext5)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext5->data = OpenAPI_base64decode(binary_data_n2_information_ext5->valuestring, strlen(binary_data_n2_information_ext5->valuestring), &decoded_str_binary_data_n2_information_ext5->len);
    if (!decoded_str_binary_data_n2_information_ext5->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext5]");
        goto end;
    }
    }

    binary_data_n2_information_ext6 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt6");
    if (binary_data_n2_information_ext6) {
    decoded_str_binary_data_n2_information_ext6 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext6);
    if (!cJSON_IsString(binary_data_n2_information_ext6)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext6->data = OpenAPI_base64decode(binary_data_n2_information_ext6->valuestring, strlen(binary_data_n2_information_ext6->valuestring), &decoded_str_binary_data_n2_information_ext6->len);
    if (!decoded_str_binary_data_n2_information_ext6->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext6]");
        goto end;
    }
    }

    binary_data_n2_information_ext7 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt7");
    if (binary_data_n2_information_ext7) {
    decoded_str_binary_data_n2_information_ext7 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext7);
    if (!cJSON_IsString(binary_data_n2_information_ext7)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext7->data = OpenAPI_base64decode(binary_data_n2_information_ext7->valuestring, strlen(binary_data_n2_information_ext7->valuestring), &decoded_str_binary_data_n2_information_ext7->len);
    if (!decoded_str_binary_data_n2_information_ext7->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext7]");
        goto end;
    }
    }

    binary_data_n2_information_ext8 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt8");
    if (binary_data_n2_information_ext8) {
    decoded_str_binary_data_n2_information_ext8 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext8);
    if (!cJSON_IsString(binary_data_n2_information_ext8)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext8->data = OpenAPI_base64decode(binary_data_n2_information_ext8->valuestring, strlen(binary_data_n2_information_ext8->valuestring), &decoded_str_binary_data_n2_information_ext8->len);
    if (!decoded_str_binary_data_n2_information_ext8->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext8]");
        goto end;
    }
    }

    binary_data_n2_information_ext9 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt9");
    if (binary_data_n2_information_ext9) {
    decoded_str_binary_data_n2_information_ext9 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext9);
    if (!cJSON_IsString(binary_data_n2_information_ext9)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext9->data = OpenAPI_base64decode(binary_data_n2_information_ext9->valuestring, strlen(binary_data_n2_information_ext9->valuestring), &decoded_str_binary_data_n2_information_ext9->len);
    if (!decoded_str_binary_data_n2_information_ext9->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext9]");
        goto end;
    }
    }

    binary_data_n2_information_ext10 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt10");
    if (binary_data_n2_information_ext10) {
    decoded_str_binary_data_n2_information_ext10 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext10);
    if (!cJSON_IsString(binary_data_n2_information_ext10)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext10->data = OpenAPI_base64decode(binary_data_n2_information_ext10->valuestring, strlen(binary_data_n2_information_ext10->valuestring), &decoded_str_binary_data_n2_information_ext10->len);
    if (!decoded_str_binary_data_n2_information_ext10->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext10]");
        goto end;
    }
    }

    binary_data_n2_information_ext11 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt11");
    if (binary_data_n2_information_ext11) {
    decoded_str_binary_data_n2_information_ext11 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext11);
    if (!cJSON_IsString(binary_data_n2_information_ext11)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext11->data = OpenAPI_base64decode(binary_data_n2_information_ext11->valuestring, strlen(binary_data_n2_information_ext11->valuestring), &decoded_str_binary_data_n2_information_ext11->len);
    if (!decoded_str_binary_data_n2_information_ext11->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext11]");
        goto end;
    }
    }

    binary_data_n2_information_ext12 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt12");
    if (binary_data_n2_information_ext12) {
    decoded_str_binary_data_n2_information_ext12 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext12);
    if (!cJSON_IsString(binary_data_n2_information_ext12)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext12->data = OpenAPI_base64decode(binary_data_n2_information_ext12->valuestring, strlen(binary_data_n2_information_ext12->valuestring), &decoded_str_binary_data_n2_information_ext12->len);
    if (!decoded_str_binary_data_n2_information_ext12->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext12]");
        goto end;
    }
    }

    binary_data_n2_information_ext13 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt13");
    if (binary_data_n2_information_ext13) {
    decoded_str_binary_data_n2_information_ext13 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext13);
    if (!cJSON_IsString(binary_data_n2_information_ext13)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext13->data = OpenAPI_base64decode(binary_data_n2_information_ext13->valuestring, strlen(binary_data_n2_information_ext13->valuestring), &decoded_str_binary_data_n2_information_ext13->len);
    if (!decoded_str_binary_data_n2_information_ext13->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext13]");
        goto end;
    }
    }

    binary_data_n2_information_ext14 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt14");
    if (binary_data_n2_information_ext14) {
    decoded_str_binary_data_n2_information_ext14 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext14);
    if (!cJSON_IsString(binary_data_n2_information_ext14)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext14->data = OpenAPI_base64decode(binary_data_n2_information_ext14->valuestring, strlen(binary_data_n2_information_ext14->valuestring), &decoded_str_binary_data_n2_information_ext14->len);
    if (!decoded_str_binary_data_n2_information_ext14->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext14]");
        goto end;
    }
    }

    binary_data_n2_information_ext15 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt15");
    if (binary_data_n2_information_ext15) {
    decoded_str_binary_data_n2_information_ext15 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext15);
    if (!cJSON_IsString(binary_data_n2_information_ext15)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext15->data = OpenAPI_base64decode(binary_data_n2_information_ext15->valuestring, strlen(binary_data_n2_information_ext15->valuestring), &decoded_str_binary_data_n2_information_ext15->len);
    if (!decoded_str_binary_data_n2_information_ext15->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext15]");
        goto end;
    }
    }

    binary_data_n2_information_ext16 = cJSON_GetObjectItemCaseSensitive(relocate_ue_context_requestJSON, "binaryDataN2InformationExt16");
    if (binary_data_n2_information_ext16) {
    decoded_str_binary_data_n2_information_ext16 = ogs_malloc(sizeof(OpenAPI_binary_t));
    ogs_assert(decoded_str_binary_data_n2_information_ext16);
    if (!cJSON_IsString(binary_data_n2_information_ext16)) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    decoded_str_binary_data_n2_information_ext16->data = OpenAPI_base64decode(binary_data_n2_information_ext16->valuestring, strlen(binary_data_n2_information_ext16->valuestring), &decoded_str_binary_data_n2_information_ext16->len);
    if (!decoded_str_binary_data_n2_information_ext16->data) {
        ogs_error("OpenAPI_relocate_ue_context_request_parseFromJSON() failed [binary_data_n2_information_ext16]");
        goto end;
    }
    }

    relocate_ue_context_request_local_var = OpenAPI_relocate_ue_context_request_create (
        json_data ? json_data_local_nonprim : NULL,
        binary_data_forward_relocation_request ? decoded_str_binary_data_forward_relocation_request : NULL,
        binary_data_n2_information ? decoded_str_binary_data_n2_information : NULL,
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

    return relocate_ue_context_request_local_var;
end:
    if (json_data_local_nonprim) {
        OpenAPI_ue_context_relocate_data_free(json_data_local_nonprim);
        json_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_relocate_ue_context_request_t *OpenAPI_relocate_ue_context_request_copy(OpenAPI_relocate_ue_context_request_t *dst, OpenAPI_relocate_ue_context_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_relocate_ue_context_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_relocate_ue_context_request_convertToJSON() failed");
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

    OpenAPI_relocate_ue_context_request_free(dst);
    dst = OpenAPI_relocate_ue_context_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

