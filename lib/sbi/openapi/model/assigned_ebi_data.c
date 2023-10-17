
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assigned_ebi_data.h"

OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_create(
    int pdu_session_id,
    OpenAPI_list_t *assigned_ebi_list,
    OpenAPI_list_t *failed_arp_list,
    OpenAPI_list_t *released_ebi_list,
    OpenAPI_list_t *modified_ebi_list
)
{
    OpenAPI_assigned_ebi_data_t *assigned_ebi_data_local_var = ogs_malloc(sizeof(OpenAPI_assigned_ebi_data_t));
    ogs_assert(assigned_ebi_data_local_var);

    assigned_ebi_data_local_var->pdu_session_id = pdu_session_id;
    assigned_ebi_data_local_var->assigned_ebi_list = assigned_ebi_list;
    assigned_ebi_data_local_var->failed_arp_list = failed_arp_list;
    assigned_ebi_data_local_var->released_ebi_list = released_ebi_list;
    assigned_ebi_data_local_var->modified_ebi_list = modified_ebi_list;

    return assigned_ebi_data_local_var;
}

void OpenAPI_assigned_ebi_data_free(OpenAPI_assigned_ebi_data_t *assigned_ebi_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == assigned_ebi_data) {
        return;
    }
    if (assigned_ebi_data->assigned_ebi_list) {
        OpenAPI_list_for_each(assigned_ebi_data->assigned_ebi_list, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_data->assigned_ebi_list);
        assigned_ebi_data->assigned_ebi_list = NULL;
    }
    if (assigned_ebi_data->failed_arp_list) {
        OpenAPI_list_for_each(assigned_ebi_data->failed_arp_list, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_data->failed_arp_list);
        assigned_ebi_data->failed_arp_list = NULL;
    }
    if (assigned_ebi_data->released_ebi_list) {
        OpenAPI_list_for_each(assigned_ebi_data->released_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_data->released_ebi_list);
        assigned_ebi_data->released_ebi_list = NULL;
    }
    if (assigned_ebi_data->modified_ebi_list) {
        OpenAPI_list_for_each(assigned_ebi_data->modified_ebi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_data->modified_ebi_list);
        assigned_ebi_data->modified_ebi_list = NULL;
    }
    ogs_free(assigned_ebi_data);
}

cJSON *OpenAPI_assigned_ebi_data_convertToJSON(OpenAPI_assigned_ebi_data_t *assigned_ebi_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (assigned_ebi_data == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [AssignedEbiData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pduSessionId", assigned_ebi_data->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (!assigned_ebi_data->assigned_ebi_list) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [assigned_ebi_list]");
        return NULL;
    }
    cJSON *assigned_ebi_listList = cJSON_AddArrayToObject(item, "assignedEbiList");
    if (assigned_ebi_listList == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [assigned_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(assigned_ebi_data->assigned_ebi_list, node) {
        cJSON *itemLocal = OpenAPI_ebi_arp_mapping_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [assigned_ebi_list]");
            goto end;
        }
        cJSON_AddItemToArray(assigned_ebi_listList, itemLocal);
    }

    if (assigned_ebi_data->failed_arp_list) {
    cJSON *failed_arp_listList = cJSON_AddArrayToObject(item, "failedArpList");
    if (failed_arp_listList == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [failed_arp_list]");
        goto end;
    }
    OpenAPI_list_for_each(assigned_ebi_data->failed_arp_list, node) {
        cJSON *itemLocal = OpenAPI_arp_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [failed_arp_list]");
            goto end;
        }
        cJSON_AddItemToArray(failed_arp_listList, itemLocal);
    }
    }

    if (assigned_ebi_data->released_ebi_list) {
    cJSON *released_ebi_listList = cJSON_AddArrayToObject(item, "releasedEbiList");
    if (released_ebi_listList == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [released_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(assigned_ebi_data->released_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(released_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [released_ebi_list]");
            goto end;
        }
    }
    }

    if (assigned_ebi_data->modified_ebi_list) {
    cJSON *modified_ebi_listList = cJSON_AddArrayToObject(item, "modifiedEbiList");
    if (modified_ebi_listList == NULL) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [modified_ebi_list]");
        goto end;
    }
    OpenAPI_list_for_each(assigned_ebi_data->modified_ebi_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [modified_ebi_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(modified_ebi_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed [modified_ebi_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_parseFromJSON(cJSON *assigned_ebi_dataJSON)
{
    OpenAPI_assigned_ebi_data_t *assigned_ebi_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_id = NULL;
    cJSON *assigned_ebi_list = NULL;
    OpenAPI_list_t *assigned_ebi_listList = NULL;
    cJSON *failed_arp_list = NULL;
    OpenAPI_list_t *failed_arp_listList = NULL;
    cJSON *released_ebi_list = NULL;
    OpenAPI_list_t *released_ebi_listList = NULL;
    cJSON *modified_ebi_list = NULL;
    OpenAPI_list_t *modified_ebi_listList = NULL;
    pdu_session_id = cJSON_GetObjectItemCaseSensitive(assigned_ebi_dataJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    assigned_ebi_list = cJSON_GetObjectItemCaseSensitive(assigned_ebi_dataJSON, "assignedEbiList");
    if (!assigned_ebi_list) {
        ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [assigned_ebi_list]");
        goto end;
    }
        cJSON *assigned_ebi_list_local = NULL;
        if (!cJSON_IsArray(assigned_ebi_list)) {
            ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [assigned_ebi_list]");
            goto end;
        }

        assigned_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(assigned_ebi_list_local, assigned_ebi_list) {
            if (!cJSON_IsObject(assigned_ebi_list_local)) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [assigned_ebi_list]");
                goto end;
            }
            OpenAPI_ebi_arp_mapping_t *assigned_ebi_listItem = OpenAPI_ebi_arp_mapping_parseFromJSON(assigned_ebi_list_local);
            if (!assigned_ebi_listItem) {
                ogs_error("No assigned_ebi_listItem");
                goto end;
            }
            OpenAPI_list_add(assigned_ebi_listList, assigned_ebi_listItem);
        }

    failed_arp_list = cJSON_GetObjectItemCaseSensitive(assigned_ebi_dataJSON, "failedArpList");
    if (failed_arp_list) {
        cJSON *failed_arp_list_local = NULL;
        if (!cJSON_IsArray(failed_arp_list)) {
            ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [failed_arp_list]");
            goto end;
        }

        failed_arp_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_arp_list_local, failed_arp_list) {
            if (!cJSON_IsObject(failed_arp_list_local)) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [failed_arp_list]");
                goto end;
            }
            OpenAPI_arp_t *failed_arp_listItem = OpenAPI_arp_parseFromJSON(failed_arp_list_local);
            if (!failed_arp_listItem) {
                ogs_error("No failed_arp_listItem");
                goto end;
            }
            OpenAPI_list_add(failed_arp_listList, failed_arp_listItem);
        }
    }

    released_ebi_list = cJSON_GetObjectItemCaseSensitive(assigned_ebi_dataJSON, "releasedEbiList");
    if (released_ebi_list) {
        cJSON *released_ebi_list_local = NULL;
        if (!cJSON_IsArray(released_ebi_list)) {
            ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [released_ebi_list]");
            goto end;
        }

        released_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(released_ebi_list_local, released_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(released_ebi_list_local)) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [released_ebi_list]");
                goto end;
            }
            *localDouble = released_ebi_list_local->valuedouble;
            OpenAPI_list_add(released_ebi_listList, localDouble);
        }
    }

    modified_ebi_list = cJSON_GetObjectItemCaseSensitive(assigned_ebi_dataJSON, "modifiedEbiList");
    if (modified_ebi_list) {
        cJSON *modified_ebi_list_local = NULL;
        if (!cJSON_IsArray(modified_ebi_list)) {
            ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [modified_ebi_list]");
            goto end;
        }

        modified_ebi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(modified_ebi_list_local, modified_ebi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(modified_ebi_list_local)) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [modified_ebi_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_assigned_ebi_data_parseFromJSON() failed [modified_ebi_list]");
                goto end;
            }
            *localDouble = modified_ebi_list_local->valuedouble;
            OpenAPI_list_add(modified_ebi_listList, localDouble);
        }
    }

    assigned_ebi_data_local_var = OpenAPI_assigned_ebi_data_create (
        
        pdu_session_id->valuedouble,
        assigned_ebi_listList,
        failed_arp_list ? failed_arp_listList : NULL,
        released_ebi_list ? released_ebi_listList : NULL,
        modified_ebi_list ? modified_ebi_listList : NULL
    );

    return assigned_ebi_data_local_var;
end:
    if (assigned_ebi_listList) {
        OpenAPI_list_for_each(assigned_ebi_listList, node) {
            OpenAPI_ebi_arp_mapping_free(node->data);
        }
        OpenAPI_list_free(assigned_ebi_listList);
        assigned_ebi_listList = NULL;
    }
    if (failed_arp_listList) {
        OpenAPI_list_for_each(failed_arp_listList, node) {
            OpenAPI_arp_free(node->data);
        }
        OpenAPI_list_free(failed_arp_listList);
        failed_arp_listList = NULL;
    }
    if (released_ebi_listList) {
        OpenAPI_list_for_each(released_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(released_ebi_listList);
        released_ebi_listList = NULL;
    }
    if (modified_ebi_listList) {
        OpenAPI_list_for_each(modified_ebi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(modified_ebi_listList);
        modified_ebi_listList = NULL;
    }
    return NULL;
}

OpenAPI_assigned_ebi_data_t *OpenAPI_assigned_ebi_data_copy(OpenAPI_assigned_ebi_data_t *dst, OpenAPI_assigned_ebi_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_assigned_ebi_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_assigned_ebi_data_convertToJSON() failed");
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

    OpenAPI_assigned_ebi_data_free(dst);
    dst = OpenAPI_assigned_ebi_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

