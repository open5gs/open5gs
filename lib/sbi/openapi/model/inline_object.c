
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "inline_object.h"

OpenAPI_inline_object_t *OpenAPI_inline_object_create(
    OpenAPI_ue_context_create_data_t *json_data,
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
    OpenAPI_inline_object_t *inline_object_local_var = OpenAPI_malloc(sizeof(OpenAPI_inline_object_t));
    if (!inline_object_local_var) {
        return NULL;
    }
    inline_object_local_var->json_data = json_data;
    inline_object_local_var->binary_data_n2_information = binary_data_n2_information;
    inline_object_local_var->binary_data_n2_information_ext1 = binary_data_n2_information_ext1;
    inline_object_local_var->binary_data_n2_information_ext2 = binary_data_n2_information_ext2;
    inline_object_local_var->binary_data_n2_information_ext3 = binary_data_n2_information_ext3;
    inline_object_local_var->binary_data_n2_information_ext4 = binary_data_n2_information_ext4;
    inline_object_local_var->binary_data_n2_information_ext5 = binary_data_n2_information_ext5;
    inline_object_local_var->binary_data_n2_information_ext6 = binary_data_n2_information_ext6;
    inline_object_local_var->binary_data_n2_information_ext7 = binary_data_n2_information_ext7;
    inline_object_local_var->binary_data_n2_information_ext8 = binary_data_n2_information_ext8;
    inline_object_local_var->binary_data_n2_information_ext9 = binary_data_n2_information_ext9;
    inline_object_local_var->binary_data_n2_information_ext10 = binary_data_n2_information_ext10;
    inline_object_local_var->binary_data_n2_information_ext11 = binary_data_n2_information_ext11;
    inline_object_local_var->binary_data_n2_information_ext12 = binary_data_n2_information_ext12;
    inline_object_local_var->binary_data_n2_information_ext13 = binary_data_n2_information_ext13;
    inline_object_local_var->binary_data_n2_information_ext14 = binary_data_n2_information_ext14;
    inline_object_local_var->binary_data_n2_information_ext15 = binary_data_n2_information_ext15;
    inline_object_local_var->binary_data_n2_information_ext16 = binary_data_n2_information_ext16;

    return inline_object_local_var;
}

void OpenAPI_inline_object_free(OpenAPI_inline_object_t *inline_object)
{
    if (NULL == inline_object) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ue_context_create_data_free(inline_object->json_data);
    ogs_free(inline_object->binary_data_n2_information->data);
    ogs_free(inline_object->binary_data_n2_information_ext1->data);
    ogs_free(inline_object->binary_data_n2_information_ext2->data);
    ogs_free(inline_object->binary_data_n2_information_ext3->data);
    ogs_free(inline_object->binary_data_n2_information_ext4->data);
    ogs_free(inline_object->binary_data_n2_information_ext5->data);
    ogs_free(inline_object->binary_data_n2_information_ext6->data);
    ogs_free(inline_object->binary_data_n2_information_ext7->data);
    ogs_free(inline_object->binary_data_n2_information_ext8->data);
    ogs_free(inline_object->binary_data_n2_information_ext9->data);
    ogs_free(inline_object->binary_data_n2_information_ext10->data);
    ogs_free(inline_object->binary_data_n2_information_ext11->data);
    ogs_free(inline_object->binary_data_n2_information_ext12->data);
    ogs_free(inline_object->binary_data_n2_information_ext13->data);
    ogs_free(inline_object->binary_data_n2_information_ext14->data);
    ogs_free(inline_object->binary_data_n2_information_ext15->data);
    ogs_free(inline_object->binary_data_n2_information_ext16->data);
    ogs_free(inline_object);
}

cJSON *OpenAPI_inline_object_convertToJSON(OpenAPI_inline_object_t *inline_object)
{
    cJSON *item = NULL;

    if (inline_object == NULL) {
        ogs_error("OpenAPI_inline_object_convertToJSON() failed [inline_object]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (inline_object->json_data) {
        cJSON *json_data_local_JSON = OpenAPI_ue_context_create_data_convertToJSON(inline_object->json_data);
        if (json_data_local_JSON == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [json_data]");
            goto end;
        }
        cJSON_AddItemToObject(item, "jsonData", json_data_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [json_data]");
            goto end;
        }
    }

    if (inline_object->binary_data_n2_information) {
        char* encoded_str_binary_data_n2_information = OpenAPI_base64encode(inline_object->binary_data_n2_information->data,inline_object->binary_data_n2_information->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2Information", encoded_str_binary_data_n2_information) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information);
    }

    if (inline_object->binary_data_n2_information_ext1) {
        char* encoded_str_binary_data_n2_information_ext1 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext1->data,inline_object->binary_data_n2_information_ext1->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt1", encoded_str_binary_data_n2_information_ext1) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext1]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext1);
    }

    if (inline_object->binary_data_n2_information_ext2) {
        char* encoded_str_binary_data_n2_information_ext2 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext2->data,inline_object->binary_data_n2_information_ext2->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt2", encoded_str_binary_data_n2_information_ext2) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext2]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext2);
    }

    if (inline_object->binary_data_n2_information_ext3) {
        char* encoded_str_binary_data_n2_information_ext3 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext3->data,inline_object->binary_data_n2_information_ext3->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt3", encoded_str_binary_data_n2_information_ext3) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext3]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext3);
    }

    if (inline_object->binary_data_n2_information_ext4) {
        char* encoded_str_binary_data_n2_information_ext4 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext4->data,inline_object->binary_data_n2_information_ext4->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt4", encoded_str_binary_data_n2_information_ext4) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext4]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext4);
    }

    if (inline_object->binary_data_n2_information_ext5) {
        char* encoded_str_binary_data_n2_information_ext5 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext5->data,inline_object->binary_data_n2_information_ext5->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt5", encoded_str_binary_data_n2_information_ext5) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext5]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext5);
    }

    if (inline_object->binary_data_n2_information_ext6) {
        char* encoded_str_binary_data_n2_information_ext6 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext6->data,inline_object->binary_data_n2_information_ext6->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt6", encoded_str_binary_data_n2_information_ext6) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext6]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext6);
    }

    if (inline_object->binary_data_n2_information_ext7) {
        char* encoded_str_binary_data_n2_information_ext7 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext7->data,inline_object->binary_data_n2_information_ext7->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt7", encoded_str_binary_data_n2_information_ext7) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext7]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext7);
    }

    if (inline_object->binary_data_n2_information_ext8) {
        char* encoded_str_binary_data_n2_information_ext8 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext8->data,inline_object->binary_data_n2_information_ext8->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt8", encoded_str_binary_data_n2_information_ext8) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext8]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext8);
    }

    if (inline_object->binary_data_n2_information_ext9) {
        char* encoded_str_binary_data_n2_information_ext9 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext9->data,inline_object->binary_data_n2_information_ext9->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt9", encoded_str_binary_data_n2_information_ext9) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext9]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext9);
    }

    if (inline_object->binary_data_n2_information_ext10) {
        char* encoded_str_binary_data_n2_information_ext10 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext10->data,inline_object->binary_data_n2_information_ext10->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt10", encoded_str_binary_data_n2_information_ext10) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext10]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext10);
    }

    if (inline_object->binary_data_n2_information_ext11) {
        char* encoded_str_binary_data_n2_information_ext11 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext11->data,inline_object->binary_data_n2_information_ext11->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt11", encoded_str_binary_data_n2_information_ext11) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext11]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext11);
    }

    if (inline_object->binary_data_n2_information_ext12) {
        char* encoded_str_binary_data_n2_information_ext12 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext12->data,inline_object->binary_data_n2_information_ext12->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt12", encoded_str_binary_data_n2_information_ext12) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext12]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext12);
    }

    if (inline_object->binary_data_n2_information_ext13) {
        char* encoded_str_binary_data_n2_information_ext13 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext13->data,inline_object->binary_data_n2_information_ext13->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt13", encoded_str_binary_data_n2_information_ext13) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext13]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext13);
    }

    if (inline_object->binary_data_n2_information_ext14) {
        char* encoded_str_binary_data_n2_information_ext14 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext14->data,inline_object->binary_data_n2_information_ext14->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt14", encoded_str_binary_data_n2_information_ext14) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext14]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext14);
    }

    if (inline_object->binary_data_n2_information_ext15) {
        char* encoded_str_binary_data_n2_information_ext15 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext15->data,inline_object->binary_data_n2_information_ext15->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt15", encoded_str_binary_data_n2_information_ext15) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext15]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext15);
    }

    if (inline_object->binary_data_n2_information_ext16) {
        char* encoded_str_binary_data_n2_information_ext16 = OpenAPI_base64encode(inline_object->binary_data_n2_information_ext16->data,inline_object->binary_data_n2_information_ext16->len);
        if (cJSON_AddStringToObject(item, "binaryDataN2InformationExt16", encoded_str_binary_data_n2_information_ext16) == NULL) {
            ogs_error("OpenAPI_inline_object_convertToJSON() failed [binary_data_n2_information_ext16]");
            goto end;
        }
        ogs_free(encoded_str_binary_data_n2_information_ext16);
    }

end:
    return item;
}

OpenAPI_inline_object_t *OpenAPI_inline_object_parseFromJSON(cJSON *inline_objectJSON)
{
    OpenAPI_inline_object_t *inline_object_local_var = NULL;
    cJSON *json_data = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "jsonData");

    OpenAPI_ue_context_create_data_t *json_data_local_nonprim = NULL;
    if (json_data) {
        json_data_local_nonprim = OpenAPI_ue_context_create_data_parseFromJSON(json_data);
    }

    cJSON *binary_data_n2_information = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2Information");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information) {
        if (!cJSON_IsString(binary_data_n2_information)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information]");
            goto end;
        }
        decoded_str_binary_data_n2_information->data = OpenAPI_base64decode(binary_data_n2_information->valuestring, strlen(binary_data_n2_information->valuestring), &decoded_str_binary_data_n2_information->len);
        if (!decoded_str_binary_data_n2_information->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext1 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt1");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext1 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext1) {
        if (!cJSON_IsString(binary_data_n2_information_ext1)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext1]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext1->data = OpenAPI_base64decode(binary_data_n2_information_ext1->valuestring, strlen(binary_data_n2_information_ext1->valuestring), &decoded_str_binary_data_n2_information_ext1->len);
        if (!decoded_str_binary_data_n2_information_ext1->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext1]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext2 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt2");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext2 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext2) {
        if (!cJSON_IsString(binary_data_n2_information_ext2)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext2]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext2->data = OpenAPI_base64decode(binary_data_n2_information_ext2->valuestring, strlen(binary_data_n2_information_ext2->valuestring), &decoded_str_binary_data_n2_information_ext2->len);
        if (!decoded_str_binary_data_n2_information_ext2->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext2]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext3 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt3");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext3 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext3) {
        if (!cJSON_IsString(binary_data_n2_information_ext3)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext3]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext3->data = OpenAPI_base64decode(binary_data_n2_information_ext3->valuestring, strlen(binary_data_n2_information_ext3->valuestring), &decoded_str_binary_data_n2_information_ext3->len);
        if (!decoded_str_binary_data_n2_information_ext3->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext3]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext4 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt4");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext4 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext4) {
        if (!cJSON_IsString(binary_data_n2_information_ext4)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext4]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext4->data = OpenAPI_base64decode(binary_data_n2_information_ext4->valuestring, strlen(binary_data_n2_information_ext4->valuestring), &decoded_str_binary_data_n2_information_ext4->len);
        if (!decoded_str_binary_data_n2_information_ext4->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext4]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext5 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt5");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext5 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext5) {
        if (!cJSON_IsString(binary_data_n2_information_ext5)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext5]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext5->data = OpenAPI_base64decode(binary_data_n2_information_ext5->valuestring, strlen(binary_data_n2_information_ext5->valuestring), &decoded_str_binary_data_n2_information_ext5->len);
        if (!decoded_str_binary_data_n2_information_ext5->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext5]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext6 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt6");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext6 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext6) {
        if (!cJSON_IsString(binary_data_n2_information_ext6)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext6]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext6->data = OpenAPI_base64decode(binary_data_n2_information_ext6->valuestring, strlen(binary_data_n2_information_ext6->valuestring), &decoded_str_binary_data_n2_information_ext6->len);
        if (!decoded_str_binary_data_n2_information_ext6->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext6]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext7 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt7");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext7 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext7) {
        if (!cJSON_IsString(binary_data_n2_information_ext7)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext7]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext7->data = OpenAPI_base64decode(binary_data_n2_information_ext7->valuestring, strlen(binary_data_n2_information_ext7->valuestring), &decoded_str_binary_data_n2_information_ext7->len);
        if (!decoded_str_binary_data_n2_information_ext7->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext7]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext8 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt8");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext8 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext8) {
        if (!cJSON_IsString(binary_data_n2_information_ext8)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext8]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext8->data = OpenAPI_base64decode(binary_data_n2_information_ext8->valuestring, strlen(binary_data_n2_information_ext8->valuestring), &decoded_str_binary_data_n2_information_ext8->len);
        if (!decoded_str_binary_data_n2_information_ext8->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext8]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext9 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt9");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext9 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext9) {
        if (!cJSON_IsString(binary_data_n2_information_ext9)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext9]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext9->data = OpenAPI_base64decode(binary_data_n2_information_ext9->valuestring, strlen(binary_data_n2_information_ext9->valuestring), &decoded_str_binary_data_n2_information_ext9->len);
        if (!decoded_str_binary_data_n2_information_ext9->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext9]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext10 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt10");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext10 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext10) {
        if (!cJSON_IsString(binary_data_n2_information_ext10)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext10]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext10->data = OpenAPI_base64decode(binary_data_n2_information_ext10->valuestring, strlen(binary_data_n2_information_ext10->valuestring), &decoded_str_binary_data_n2_information_ext10->len);
        if (!decoded_str_binary_data_n2_information_ext10->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext10]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext11 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt11");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext11 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext11) {
        if (!cJSON_IsString(binary_data_n2_information_ext11)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext11]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext11->data = OpenAPI_base64decode(binary_data_n2_information_ext11->valuestring, strlen(binary_data_n2_information_ext11->valuestring), &decoded_str_binary_data_n2_information_ext11->len);
        if (!decoded_str_binary_data_n2_information_ext11->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext11]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext12 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt12");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext12 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext12) {
        if (!cJSON_IsString(binary_data_n2_information_ext12)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext12]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext12->data = OpenAPI_base64decode(binary_data_n2_information_ext12->valuestring, strlen(binary_data_n2_information_ext12->valuestring), &decoded_str_binary_data_n2_information_ext12->len);
        if (!decoded_str_binary_data_n2_information_ext12->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext12]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext13 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt13");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext13 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext13) {
        if (!cJSON_IsString(binary_data_n2_information_ext13)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext13]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext13->data = OpenAPI_base64decode(binary_data_n2_information_ext13->valuestring, strlen(binary_data_n2_information_ext13->valuestring), &decoded_str_binary_data_n2_information_ext13->len);
        if (!decoded_str_binary_data_n2_information_ext13->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext13]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext14 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt14");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext14 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext14) {
        if (!cJSON_IsString(binary_data_n2_information_ext14)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext14]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext14->data = OpenAPI_base64decode(binary_data_n2_information_ext14->valuestring, strlen(binary_data_n2_information_ext14->valuestring), &decoded_str_binary_data_n2_information_ext14->len);
        if (!decoded_str_binary_data_n2_information_ext14->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext14]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext15 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt15");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext15 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext15) {
        if (!cJSON_IsString(binary_data_n2_information_ext15)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext15]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext15->data = OpenAPI_base64decode(binary_data_n2_information_ext15->valuestring, strlen(binary_data_n2_information_ext15->valuestring), &decoded_str_binary_data_n2_information_ext15->len);
        if (!decoded_str_binary_data_n2_information_ext15->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext15]");
            goto end;
        }
    }

    cJSON *binary_data_n2_information_ext16 = cJSON_GetObjectItemCaseSensitive(inline_objectJSON, "binaryDataN2InformationExt16");

    OpenAPI_binary_t* decoded_str_binary_data_n2_information_ext16 = OpenAPI_malloc(sizeof(OpenAPI_binary_t));
    if (binary_data_n2_information_ext16) {
        if (!cJSON_IsString(binary_data_n2_information_ext16)) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext16]");
            goto end;
        }
        decoded_str_binary_data_n2_information_ext16->data = OpenAPI_base64decode(binary_data_n2_information_ext16->valuestring, strlen(binary_data_n2_information_ext16->valuestring), &decoded_str_binary_data_n2_information_ext16->len);
        if (!decoded_str_binary_data_n2_information_ext16->data) {
            ogs_error("OpenAPI_inline_object_parseFromJSON() failed [binary_data_n2_information_ext16]");
            goto end;
        }
    }

    inline_object_local_var = OpenAPI_inline_object_create (
        json_data ? json_data_local_nonprim : NULL,
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

    return inline_object_local_var;
end:
    return NULL;
}

OpenAPI_inline_object_t *OpenAPI_inline_object_copy(OpenAPI_inline_object_t *dst, OpenAPI_inline_object_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_inline_object_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_inline_object_convertToJSON() failed");
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

    OpenAPI_inline_object_free(dst);
    dst = OpenAPI_inline_object_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

