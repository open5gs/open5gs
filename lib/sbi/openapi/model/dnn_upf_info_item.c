
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dnn_upf_info_item.h"

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_create(
    char *dnn,
    OpenAPI_list_t *dnai_list,
    OpenAPI_list_t *pdu_session_types
    )
{
    OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item_local_var = OpenAPI_malloc(sizeof(OpenAPI_dnn_upf_info_item_t));
    if (!dnn_upf_info_item_local_var) {
        return NULL;
    }
    dnn_upf_info_item_local_var->dnn = dnn;
    dnn_upf_info_item_local_var->dnai_list = dnai_list;
    dnn_upf_info_item_local_var->pdu_session_types = pdu_session_types;

    return dnn_upf_info_item_local_var;
}

void OpenAPI_dnn_upf_info_item_free(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    if (NULL == dnn_upf_info_item) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(dnn_upf_info_item->dnn);
    OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(dnn_upf_info_item->dnai_list);
    OpenAPI_list_for_each(dnn_upf_info_item->pdu_session_types, node) {
        OpenAPI_pdu_session_type_free(node->data);
    }
    OpenAPI_list_free(dnn_upf_info_item->pdu_session_types);
    ogs_free(dnn_upf_info_item);
}

cJSON *OpenAPI_dnn_upf_info_item_convertToJSON(OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item)
{
    cJSON *item = NULL;

    if (dnn_upf_info_item == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [DnnUpfInfoItem]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!dnn_upf_info_item->dnn) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "dnn", dnn_upf_info_item->dnn) == NULL) {
        ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnn]");
        goto end;
    }

    if (dnn_upf_info_item->dnai_list) {
        cJSON *dnai_list = cJSON_AddArrayToObject(item, "dnaiList");
        if (dnai_list == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
            goto end;
        }

        OpenAPI_lnode_t *dnai_list_node;
        OpenAPI_list_for_each(dnn_upf_info_item->dnai_list, dnai_list_node)  {
            if (cJSON_AddStringToObject(dnai_list, "", (char*)dnai_list_node->data) == NULL) {
                ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [dnai_list]");
                goto end;
            }
        }
    }

    if (dnn_upf_info_item->pdu_session_types) {
        cJSON *pdu_session_typesList = cJSON_AddArrayToObject(item, "pduSessionTypes");
        if (pdu_session_typesList == NULL) {
            ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [pdu_session_types]");
            goto end;
        }

        OpenAPI_lnode_t *pdu_session_types_node;
        if (dnn_upf_info_item->pdu_session_types) {
            OpenAPI_list_for_each(dnn_upf_info_item->pdu_session_types, pdu_session_types_node) {
                cJSON *itemLocal = OpenAPI_pdu_session_type_convertToJSON(pdu_session_types_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_dnn_upf_info_item_convertToJSON() failed [pdu_session_types]");
                    goto end;
                }
                cJSON_AddItemToArray(pdu_session_typesList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_dnn_upf_info_item_t *OpenAPI_dnn_upf_info_item_parseFromJSON(cJSON *dnn_upf_info_itemJSON)
{
    OpenAPI_dnn_upf_info_item_t *dnn_upf_info_item_local_var = NULL;
    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }


    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnn]");
        goto end;
    }

    cJSON *dnai_list = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "dnaiList");

    OpenAPI_list_t *dnai_listList;
    if (dnai_list) {
        cJSON *dnai_list_local;
        if (!cJSON_IsArray(dnai_list)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
            goto end;
        }
        dnai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnai_list_local, dnai_list) {
            if (!cJSON_IsString(dnai_list_local)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [dnai_list]");
                goto end;
            }
            OpenAPI_list_add(dnai_listList, ogs_strdup(dnai_list_local->valuestring));
        }
    }

    cJSON *pdu_session_types = cJSON_GetObjectItemCaseSensitive(dnn_upf_info_itemJSON, "pduSessionTypes");

    OpenAPI_list_t *pdu_session_typesList;
    if (pdu_session_types) {
        cJSON *pdu_session_types_local_nonprimitive;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local_nonprimitive, pdu_session_types ) {
            if (!cJSON_IsObject(pdu_session_types_local_nonprimitive)) {
                ogs_error("OpenAPI_dnn_upf_info_item_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }
            OpenAPI_pdu_session_type_t *pdu_session_typesItem = OpenAPI_pdu_session_type_parseFromJSON(pdu_session_types_local_nonprimitive);

            OpenAPI_list_add(pdu_session_typesList, pdu_session_typesItem);
        }
    }

    dnn_upf_info_item_local_var = OpenAPI_dnn_upf_info_item_create (
        ogs_strdup(dnn->valuestring),
        dnai_list ? dnai_listList : NULL,
        pdu_session_types ? pdu_session_typesList : NULL
        );

    return dnn_upf_info_item_local_var;
end:
    return NULL;
}

