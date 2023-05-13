
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assign_ebi_data.h"

OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_create(
    int pdu_session_id,
    OpenAPI_list_t *arp_list,
    OpenAPI_list_t *released_ebi_list,
    OpenAPI_guami_t *old_guami,
    OpenAPI_list_t *modified_ebi_list
)
{
    OpenAPI_assign_ebi_data_t *assign_ebi_data_local_var = ogs_malloc(sizeof(OpenAPI_assign_ebi_data_t));
    ogs_assert(assign_ebi_data_local_var);

    assign_ebi_data_local_var->pdu_session_id = pdu_session_id;
    assign_ebi_data_local_var->arp_list = arp_list;
    assign_ebi_data_local_var->released_ebi_list = released_ebi_list;
    assign_ebi_data_local_var->old_guami = old_guami;
    assign_ebi_data_local_var->modified_ebi_list = modified_ebi_list;

    return assign_ebi_data_local_var;
}

void OpenAPI_assign_ebi_data_free(OpenAPI_assign_ebi_data_t *assign_ebi_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == assign_ebi_data) {
        return;
    }
    if (assign_ebi_data->arp_list) {
        OpenAPI_list_for_each(assign_ebi_data->arp_list, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(assign_ebi_data->arp_list);
        assign_ebi_data->arp_list = NULL;
    }
    if (assign_ebi_data->released_ebi_list) {
        OpenAPI_list_for_each(assign_ebi_data->released_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(assign_ebi_data->released_ebi_list);
        assign_ebi_data->released_ebi_list = NULL;
    }
    if (assign_ebi_data->old_guami) {
        OpenAPI_guami_free(assign_ebi_data->old_guami);
        assign_ebi_data->old_guami = NULL;
    }
    if (assign_ebi_data->modified_ebi_list) {
        OpenAPI_list_for_each(assign_ebi_data->modified_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(assign_ebi_data->modified_ebi_list);
        assign_ebi_data->modified_ebi_list = NULL;
    }
    ogs_free(assign_ebi_data);
}

cJSON *OpenAPI_assign_ebi_data_convertToJSON(OpenAPI_assign_ebi_data_t *assign_ebi_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (assign_ebi_data == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [AssignEbiData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", assign_ebi_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (assign_ebi_data->arp_list) {
    cJSON *arp_listList = cJSON_AddArrayToObject(item, "arpList");
    if (arp_listList == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [arp_list]");
        goto end;
    }
    OpenAPI_list_for_each(assign_ebi_data->arp_list, node) {
        cJSON *itemLocal = OpenAPI_arp_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [arp_list]");
            goto end;
        }
        cJSON_AddItemToArray(arp_listList, itemLocal);
    }
    }

    if (assign_ebi_data->released_ebi_list) {
    cJSON *released_ebi_listList = cJSON_AddArrayToObject(item, "releasedEbiList");
    if (released_ebi_listList == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [released_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(assign_ebi_data->released_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(released_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
    }
    }

    if (assign_ebi_data->old_guami) {
    cJSON *old_guami_local_JSON = OpenAPI_guami_convertToJSON(assign_ebi_data->old_guami);
    if (old_guami_local_JSON == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [old_guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "oldGuami", old_guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [old_guami]");
        goto end;
    }
    }

    if (assign_ebi_data->modified_ebi_list) {
    cJSON *modified_ebi_listList = cJSON_AddArrayToObject(item, "modifiedEbiList");
    if (modified_ebi_listList == NULL) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [modified_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(assign_ebi_data->modified_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed [modified_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(modified_ebi_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_parseFromJSON(cJSON *assign_ebi_dataJSON)
{
    OpenAPI_assign_ebi_data_t *assign_ebi_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *arp_list = NULL;
    OpenAPI_list_t *arp_listList = NULL;
    cJSON *released_ebi_list = NULL;
    OpenAPI_list_t *released_ebi_listList = NULL;
    cJSON *old_guami = NULL;
    OpenAPI_guami_t *old_guami_local_nonprim = NULL;
    cJSON *modified_ebi_list = NULL;
    OpenAPI_list_t *modified_ebi_listList = NULL;
    pdu_session_id = cJSON_GetObjectItemCaseSensitive(assign_ebi_dataJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    arp_list = cJSON_GetObjectItemCaseSensitive(assign_ebi_dataJSON, "arpList");
    if (arp_list) {
        cJSON *arp_list_local = NULL;
        if (!cJSON_IsArray(arp_list)) {
            ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [arp_list]");
            goto end;
        }

        arp_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(arp_list_local, arp_list) {
            if (!cJSON_IsObject(arp_list_local)) {
                ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [arp_list]");
                goto end;
            }
            OpenAPI_arp_t *arp_listItem = OpenAPI_arp_parseFromJSON(arp_list_local);
            if (!arp_listItem) {
                ogs_error("No arp_listItem");
                goto end;
            }
            OpenAPI_list_add(arp_listList, arp_listItem);
        }
    }

    released_ebi_list = cJSON_GetObjectItemCaseSensitive(assign_ebi_dataJSON, "releasedEbiList");
    if (released_ebi_list) {
        cJSON *released_ebi_list_local = NULL;
        if (!cJSON_IsArray(released_ebi_list)) {
            ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [released_ebi_list]");
            goto end;
        }

        released_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(released_ebi_list_local, released_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(released_ebi_list_local)) {
                ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            *localDouble = released_ebi_list_local->valuedouble;
            OpenAPI_list_add(released_ebi_listList, localDouble);
        }
    }

    old_guami = cJSON_GetObjectItemCaseSensitive(assign_ebi_dataJSON, "oldGuami");
    if (old_guami) {
    old_guami_local_nonprim = OpenAPI_guami_parseFromJSON(old_guami);
    if (!old_guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [old_guami]");
        goto end;
    }
    }

    modified_ebi_list = cJSON_GetObjectItemCaseSensitive(assign_ebi_dataJSON, "modifiedEbiList");
    if (modified_ebi_list) {
        cJSON *modified_ebi_list_local = NULL;
        if (!cJSON_IsArray(modified_ebi_list)) {
            ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [modified_ebi_list]");
            goto end;
        }

        modified_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(modified_ebi_list_local, modified_ebi_list) {
            if (!cJSON_IsObject(modified_ebi_list_local)) {
                ogs_error("OpenAPI_assign_ebi_data_parseFromJSON() failed [modified_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *modified_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(modified_ebi_list_local);
            if (!modified_ebi_listItem) {
                ogs_error("No modified_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(modified_ebi_listList, modified_ebi_listItem);
        }
    }

    assign_ebi_data_local_var = OpenAPI_assign_ebi_data_create (
        
        pdu_session_id->valuedouble,
        arp_list ? arp_listList : NULL,
        released_ebi_list ? released_ebi_listList : NULL,
        old_guami ? old_guami_local_nonprim : NULL,
        modified_ebi_list ? modified_ebi_listList : NULL
    );

    return assign_ebi_data_local_var;
end:
    if (arp_listList) {
        OpenAPI_list_for_each(arp_listList, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(arp_listList);
        arp_listList = NULL;
    }
    if (released_ebi_listList) {
        OpenAPI_list_for_each(released_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(released_ebi_listList);
        released_ebi_listList = NULL;
    }
    if (old_guami_local_nonprim) {
        OpenAPI_guami_free(old_guami_local_nonprim);
        old_guami_local_nonprim = NULL;
    }
    if (modified_ebi_listList) {
        OpenAPI_list_for_each(modified_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(modified_ebi_listList);
        modified_ebi_listList = NULL;
    }
    return NULL;
}

OpenAPI_assign_ebi_data_t *OpenAPI_assign_ebi_data_copy(OpenAPI_assign_ebi_data_t *dst, OpenAPI_assign_ebi_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_assign_ebi_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_assign_ebi_data_convertToJSON() failed");
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

    OpenAPI_assign_ebi_data_free(dst);
    dst = OpenAPI_assign_ebi_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

