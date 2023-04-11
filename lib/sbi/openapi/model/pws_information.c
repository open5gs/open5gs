
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pws_information.h"

OpenAPI_pws_information_t *OpenAPI_pws_information_create(
    int message_identifier,
    int serial_number,
    OpenAPI_n2_info_content_t *pws_container,
    OpenAPI_list_t *bc_empty_area_list,
    bool is_send_ran_response,
    int send_ran_response,
    char *omc_id,
    char *nf_id
)
{
    OpenAPI_pws_information_t *pws_information_local_var = ogs_malloc(sizeof(OpenAPI_pws_information_t));
    ogs_assert(pws_information_local_var);

    pws_information_local_var->message_identifier = message_identifier;
    pws_information_local_var->serial_number = serial_number;
    pws_information_local_var->pws_container = pws_container;
    pws_information_local_var->bc_empty_area_list = bc_empty_area_list;
    pws_information_local_var->is_send_ran_response = is_send_ran_response;
    pws_information_local_var->send_ran_response = send_ran_response;
    pws_information_local_var->omc_id = omc_id;
    pws_information_local_var->nf_id = nf_id;

    return pws_information_local_var;
}

void OpenAPI_pws_information_free(OpenAPI_pws_information_t *pws_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pws_information) {
        return;
    }
    if (pws_information->pws_container) {
        OpenAPI_n2_info_content_free(pws_information->pws_container);
        pws_information->pws_container = NULL;
    }
    if (pws_information->bc_empty_area_list) {
        OpenAPI_list_for_each(pws_information->bc_empty_area_list, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(pws_information->bc_empty_area_list);
        pws_information->bc_empty_area_list = NULL;
    }
    if (pws_information->omc_id) {
        ogs_free(pws_information->omc_id);
        pws_information->omc_id = NULL;
    }
    if (pws_information->nf_id) {
        ogs_free(pws_information->nf_id);
        pws_information->nf_id = NULL;
    }
    ogs_free(pws_information);
}

cJSON *OpenAPI_pws_information_convertToJSON(OpenAPI_pws_information_t *pws_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pws_information == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [PwsInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "messageIdentifier", pws_information->message_identifier) == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [message_identifier]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "serialNumber", pws_information->serial_number) == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [serial_number]");
        goto end;
    }

    if (!pws_information->pws_container) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [pws_container]");
        return NULL;
    }
    cJSON *pws_container_local_JSON = OpenAPI_n2_info_content_convertToJSON(pws_information->pws_container);
    if (pws_container_local_JSON == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [pws_container]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pwsContainer", pws_container_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [pws_container]");
        goto end;
    }

    if (pws_information->bc_empty_area_list) {
    cJSON *bc_empty_area_listList = cJSON_AddArrayToObject(item, "bcEmptyAreaList");
    if (bc_empty_area_listList == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [bc_empty_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(pws_information->bc_empty_area_list, node) {
        cJSON *itemLocal = OpenAPI_global_ran_node_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pws_information_convertToJSON() failed [bc_empty_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(bc_empty_area_listList, itemLocal);
    }
    }

    if (pws_information->is_send_ran_response) {
    if (cJSON_AddBoolToObject(item, "sendRanResponse", pws_information->send_ran_response) == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [send_ran_response]");
        goto end;
    }
    }

    if (pws_information->omc_id) {
    if (cJSON_AddStringToObject(item, "omcId", pws_information->omc_id) == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [omc_id]");
        goto end;
    }
    }

    if (pws_information->nf_id) {
    if (cJSON_AddStringToObject(item, "nfId", pws_information->nf_id) == NULL) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed [nf_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pws_information_t *OpenAPI_pws_information_parseFromJSON(cJSON *pws_informationJSON)
{
    OpenAPI_pws_information_t *pws_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *message_identifier = NULL;
    cJSON *serial_number = NULL;
    cJSON *pws_container = NULL;
    OpenAPI_n2_info_content_t *pws_container_local_nonprim = NULL;
    cJSON *bc_empty_area_list = NULL;
    OpenAPI_list_t *bc_empty_area_listList = NULL;
    cJSON *send_ran_response = NULL;
    cJSON *omc_id = NULL;
    cJSON *nf_id = NULL;
    message_identifier = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "messageIdentifier");
    if (!message_identifier) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [message_identifier]");
        goto end;
    }
    if (!cJSON_IsNumber(message_identifier)) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [message_identifier]");
        goto end;
    }

    serial_number = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "serialNumber");
    if (!serial_number) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [serial_number]");
        goto end;
    }
    if (!cJSON_IsNumber(serial_number)) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [serial_number]");
        goto end;
    }

    pws_container = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "pwsContainer");
    if (!pws_container) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [pws_container]");
        goto end;
    }
    pws_container_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(pws_container);
    if (!pws_container_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [pws_container]");
        goto end;
    }

    bc_empty_area_list = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "bcEmptyAreaList");
    if (bc_empty_area_list) {
        cJSON *bc_empty_area_list_local = NULL;
        if (!cJSON_IsArray(bc_empty_area_list)) {
            ogs_error("OpenAPI_pws_information_parseFromJSON() failed [bc_empty_area_list]");
            goto end;
        }

        bc_empty_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(bc_empty_area_list_local, bc_empty_area_list) {
            if (!cJSON_IsObject(bc_empty_area_list_local)) {
                ogs_error("OpenAPI_pws_information_parseFromJSON() failed [bc_empty_area_list]");
                goto end;
            }
            OpenAPI_global_ran_node_id_t *bc_empty_area_listItem = OpenAPI_global_ran_node_id_parseFromJSON(bc_empty_area_list_local);
            if (!bc_empty_area_listItem) {
                ogs_error("No bc_empty_area_listItem");
                goto end;
            }
            OpenAPI_list_add(bc_empty_area_listList, bc_empty_area_listItem);
        }
    }

    send_ran_response = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "sendRanResponse");
    if (send_ran_response) {
    if (!cJSON_IsBool(send_ran_response)) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [send_ran_response]");
        goto end;
    }
    }

    omc_id = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "omcId");
    if (omc_id) {
    if (!cJSON_IsString(omc_id) && !cJSON_IsNull(omc_id)) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [omc_id]");
        goto end;
    }
    }

    nf_id = cJSON_GetObjectItemCaseSensitive(pws_informationJSON, "nfId");
    if (nf_id) {
    if (!cJSON_IsString(nf_id) && !cJSON_IsNull(nf_id)) {
        ogs_error("OpenAPI_pws_information_parseFromJSON() failed [nf_id]");
        goto end;
    }
    }

    pws_information_local_var = OpenAPI_pws_information_create (
        
        message_identifier->valuedouble,
        
        serial_number->valuedouble,
        pws_container_local_nonprim,
        bc_empty_area_list ? bc_empty_area_listList : NULL,
        send_ran_response ? true : false,
        send_ran_response ? send_ran_response->valueint : 0,
        omc_id && !cJSON_IsNull(omc_id) ? ogs_strdup(omc_id->valuestring) : NULL,
        nf_id && !cJSON_IsNull(nf_id) ? ogs_strdup(nf_id->valuestring) : NULL
    );

    return pws_information_local_var;
end:
    if (pws_container_local_nonprim) {
        OpenAPI_n2_info_content_free(pws_container_local_nonprim);
        pws_container_local_nonprim = NULL;
    }
    if (bc_empty_area_listList) {
        OpenAPI_list_for_each(bc_empty_area_listList, node) {
            OpenAPI_global_ran_node_id_free(node->data);
        }
        OpenAPI_list_free(bc_empty_area_listList);
        bc_empty_area_listList = NULL;
    }
    return NULL;
}

OpenAPI_pws_information_t *OpenAPI_pws_information_copy(OpenAPI_pws_information_t *dst, OpenAPI_pws_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pws_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pws_information_convertToJSON() failed");
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

    OpenAPI_pws_information_free(dst);
    dst = OpenAPI_pws_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

