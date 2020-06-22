
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n4_information.h"

OpenAPI_n4_information_t *OpenAPI_n4_information_create(
    OpenAPI_n4_message_type_e n4_message_type,
    OpenAPI_ref_to_binary_data_t *n4_message_payload,
    OpenAPI_dnai_information_t *n4_dnai_info
    )
{
    OpenAPI_n4_information_t *n4_information_local_var = OpenAPI_malloc(sizeof(OpenAPI_n4_information_t));
    if (!n4_information_local_var) {
        return NULL;
    }
    n4_information_local_var->n4_message_type = n4_message_type;
    n4_information_local_var->n4_message_payload = n4_message_payload;
    n4_information_local_var->n4_dnai_info = n4_dnai_info;

    return n4_information_local_var;
}

void OpenAPI_n4_information_free(OpenAPI_n4_information_t *n4_information)
{
    if (NULL == n4_information) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_ref_to_binary_data_free(n4_information->n4_message_payload);
    OpenAPI_dnai_information_free(n4_information->n4_dnai_info);
    ogs_free(n4_information);
}

cJSON *OpenAPI_n4_information_convertToJSON(OpenAPI_n4_information_t *n4_information)
{
    cJSON *item = NULL;

    if (n4_information == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [N4Information]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!n4_information->n4_message_type) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_type]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "n4MessageType", OpenAPI_n4_message_type_ToString(n4_information->n4_message_type)) == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_type]");
        goto end;
    }

    if (!n4_information->n4_message_payload) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        goto end;
    }
    cJSON *n4_message_payload_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(n4_information->n4_message_payload);
    if (n4_message_payload_local_JSON == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n4MessagePayload", n4_message_payload_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_message_payload]");
        goto end;
    }

    if (n4_information->n4_dnai_info) {
        cJSON *n4_dnai_info_local_JSON = OpenAPI_dnai_information_convertToJSON(n4_information->n4_dnai_info);
        if (n4_dnai_info_local_JSON == NULL) {
            ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_dnai_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "n4DnaiInfo", n4_dnai_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_n4_information_convertToJSON() failed [n4_dnai_info]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_n4_information_t *OpenAPI_n4_information_parseFromJSON(cJSON *n4_informationJSON)
{
    OpenAPI_n4_information_t *n4_information_local_var = NULL;
    cJSON *n4_message_type = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4MessageType");
    if (!n4_message_type) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_type]");
        goto end;
    }

    OpenAPI_n4_message_type_e n4_message_typeVariable;

    if (!cJSON_IsString(n4_message_type)) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_type]");
        goto end;
    }
    n4_message_typeVariable = OpenAPI_n4_message_type_FromString(n4_message_type->valuestring);

    cJSON *n4_message_payload = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4MessagePayload");
    if (!n4_message_payload) {
        ogs_error("OpenAPI_n4_information_parseFromJSON() failed [n4_message_payload]");
        goto end;
    }

    OpenAPI_ref_to_binary_data_t *n4_message_payload_local_nonprim = NULL;

    n4_message_payload_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n4_message_payload);

    cJSON *n4_dnai_info = cJSON_GetObjectItemCaseSensitive(n4_informationJSON, "n4DnaiInfo");

    OpenAPI_dnai_information_t *n4_dnai_info_local_nonprim = NULL;
    if (n4_dnai_info) {
        n4_dnai_info_local_nonprim = OpenAPI_dnai_information_parseFromJSON(n4_dnai_info);
    }

    n4_information_local_var = OpenAPI_n4_information_create (
        n4_message_typeVariable,
        n4_message_payload_local_nonprim,
        n4_dnai_info ? n4_dnai_info_local_nonprim : NULL
        );

    return n4_information_local_var;
end:
    return NULL;
}

OpenAPI_n4_information_t *OpenAPI_n4_information_copy(OpenAPI_n4_information_t *dst, OpenAPI_n4_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n4_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n4_information_convertToJSON() failed");
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

    OpenAPI_n4_information_free(dst);
    dst = OpenAPI_n4_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

