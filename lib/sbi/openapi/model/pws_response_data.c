
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pws_response_data.h"

OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_create(
    int ngap_message_type,
    int serial_number,
    int message_identifier,
    OpenAPI_list_t *unknown_tai_list
)
{
    OpenAPI_pws_response_data_t *pws_response_data_local_var = ogs_malloc(sizeof(OpenAPI_pws_response_data_t));
    ogs_assert(pws_response_data_local_var);

    pws_response_data_local_var->ngap_message_type = ngap_message_type;
    pws_response_data_local_var->serial_number = serial_number;
    pws_response_data_local_var->message_identifier = message_identifier;
    pws_response_data_local_var->unknown_tai_list = unknown_tai_list;

    return pws_response_data_local_var;
}

void OpenAPI_pws_response_data_free(OpenAPI_pws_response_data_t *pws_response_data)
{
    if (NULL == pws_response_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(pws_response_data->unknown_tai_list, node) {
        OpenAPI_tai_free(node->data);
    }
    OpenAPI_list_free(pws_response_data->unknown_tai_list);
    ogs_free(pws_response_data);
}

cJSON *OpenAPI_pws_response_data_convertToJSON(OpenAPI_pws_response_data_t *pws_response_data)
{
    cJSON *item = NULL;

    if (pws_response_data == NULL) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [PWSResponseData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "ngapMessageType", pws_response_data->ngap_message_type) == NULL) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [ngap_message_type]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "serialNumber", pws_response_data->serial_number) == NULL) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [serial_number]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "messageIdentifier", pws_response_data->message_identifier) == NULL) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [message_identifier]");
        goto end;
    }

    if (pws_response_data->unknown_tai_list) {
    cJSON *unknown_tai_listList = cJSON_AddArrayToObject(item, "unknownTaiList");
    if (unknown_tai_listList == NULL) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [unknown_tai_list]");
        goto end;
    }

    OpenAPI_lnode_t *unknown_tai_list_node;
    if (pws_response_data->unknown_tai_list) {
        OpenAPI_list_for_each(pws_response_data->unknown_tai_list, unknown_tai_list_node) {
            cJSON *itemLocal = OpenAPI_tai_convertToJSON(unknown_tai_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pws_response_data_convertToJSON() failed [unknown_tai_list]");
                goto end;
            }
            cJSON_AddItemToArray(unknown_tai_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_parseFromJSON(cJSON *pws_response_dataJSON)
{
    OpenAPI_pws_response_data_t *pws_response_data_local_var = NULL;
    cJSON *ngap_message_type = cJSON_GetObjectItemCaseSensitive(pws_response_dataJSON, "ngapMessageType");
    if (!ngap_message_type) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [ngap_message_type]");
        goto end;
    }

    if (!cJSON_IsNumber(ngap_message_type)) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [ngap_message_type]");
        goto end;
    }

    cJSON *serial_number = cJSON_GetObjectItemCaseSensitive(pws_response_dataJSON, "serialNumber");
    if (!serial_number) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [serial_number]");
        goto end;
    }

    if (!cJSON_IsNumber(serial_number)) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [serial_number]");
        goto end;
    }

    cJSON *message_identifier = cJSON_GetObjectItemCaseSensitive(pws_response_dataJSON, "messageIdentifier");
    if (!message_identifier) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [message_identifier]");
        goto end;
    }

    if (!cJSON_IsNumber(message_identifier)) {
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [message_identifier]");
        goto end;
    }

    cJSON *unknown_tai_list = cJSON_GetObjectItemCaseSensitive(pws_response_dataJSON, "unknownTaiList");

    OpenAPI_list_t *unknown_tai_listList;
    if (unknown_tai_list) {
    cJSON *unknown_tai_list_local_nonprimitive;
    if (!cJSON_IsArray(unknown_tai_list)){
        ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [unknown_tai_list]");
        goto end;
    }

    unknown_tai_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(unknown_tai_list_local_nonprimitive, unknown_tai_list ) {
        if (!cJSON_IsObject(unknown_tai_list_local_nonprimitive)) {
            ogs_error("OpenAPI_pws_response_data_parseFromJSON() failed [unknown_tai_list]");
            goto end;
        }
        OpenAPI_tai_t *unknown_tai_listItem = OpenAPI_tai_parseFromJSON(unknown_tai_list_local_nonprimitive);

        if (!unknown_tai_listItem) {
            ogs_error("No unknown_tai_listItem");
            OpenAPI_list_free(unknown_tai_listList);
            goto end;
        }

        OpenAPI_list_add(unknown_tai_listList, unknown_tai_listItem);
    }
    }

    pws_response_data_local_var = OpenAPI_pws_response_data_create (
        
        ngap_message_type->valuedouble,
        
        serial_number->valuedouble,
        
        message_identifier->valuedouble,
        unknown_tai_list ? unknown_tai_listList : NULL
    );

    return pws_response_data_local_var;
end:
    return NULL;
}

OpenAPI_pws_response_data_t *OpenAPI_pws_response_data_copy(OpenAPI_pws_response_data_t *dst, OpenAPI_pws_response_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pws_response_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pws_response_data_convertToJSON() failed");
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

    OpenAPI_pws_response_data_free(dst);
    dst = OpenAPI_pws_response_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

