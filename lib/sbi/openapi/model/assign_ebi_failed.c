
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "assign_ebi_failed.h"

OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_create(
    int pdu_session_id,
    OpenAPI_list_t *failed_arp_list
    )
{
    OpenAPI_assign_ebi_failed_t *assign_ebi_failed_local_var = OpenAPI_malloc(sizeof(OpenAPI_assign_ebi_failed_t));
    if (!assign_ebi_failed_local_var) {
        return NULL;
    }
    assign_ebi_failed_local_var->pdu_session_id = pdu_session_id;
    assign_ebi_failed_local_var->failed_arp_list = failed_arp_list;

    return assign_ebi_failed_local_var;
}

void OpenAPI_assign_ebi_failed_free(OpenAPI_assign_ebi_failed_t *assign_ebi_failed)
{
    if (NULL == assign_ebi_failed) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(assign_ebi_failed->failed_arp_list, node) {
        OpenAPI_arp_free(node->data);
    }
    OpenAPI_list_free(assign_ebi_failed->failed_arp_list);
    ogs_free(assign_ebi_failed);
}

cJSON *OpenAPI_assign_ebi_failed_convertToJSON(OpenAPI_assign_ebi_failed_t *assign_ebi_failed)
{
    cJSON *item = NULL;

    if (assign_ebi_failed == NULL) {
        ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed [AssignEbiFailed]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!assign_ebi_failed->pdu_session_id) {
        ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed [pdu_session_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "pduSessionId", assign_ebi_failed->pdu_session_id) == NULL) {
        ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed [pdu_session_id]");
        goto end;
    }

    if (assign_ebi_failed->failed_arp_list) {
        cJSON *failed_arp_listList = cJSON_AddArrayToObject(item, "failedArpList");
        if (failed_arp_listList == NULL) {
            ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed [failed_arp_list]");
            goto end;
        }

        OpenAPI_lnode_t *failed_arp_list_node;
        if (assign_ebi_failed->failed_arp_list) {
            OpenAPI_list_for_each(assign_ebi_failed->failed_arp_list, failed_arp_list_node) {
                cJSON *itemLocal = OpenAPI_arp_convertToJSON(failed_arp_list_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed [failed_arp_list]");
                    goto end;
                }
                cJSON_AddItemToArray(failed_arp_listList, itemLocal);
            }
        }
    }

end:
    return item;
}

OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_parseFromJSON(cJSON *assign_ebi_failedJSON)
{
    OpenAPI_assign_ebi_failed_t *assign_ebi_failed_local_var = NULL;
    cJSON *pdu_session_id = cJSON_GetObjectItemCaseSensitive(assign_ebi_failedJSON, "pduSessionId");
    if (!pdu_session_id) {
        ogs_error("OpenAPI_assign_ebi_failed_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }


    if (!cJSON_IsNumber(pdu_session_id)) {
        ogs_error("OpenAPI_assign_ebi_failed_parseFromJSON() failed [pdu_session_id]");
        goto end;
    }

    cJSON *failed_arp_list = cJSON_GetObjectItemCaseSensitive(assign_ebi_failedJSON, "failedArpList");

    OpenAPI_list_t *failed_arp_listList;
    if (failed_arp_list) {
        cJSON *failed_arp_list_local_nonprimitive;
        if (!cJSON_IsArray(failed_arp_list)) {
            ogs_error("OpenAPI_assign_ebi_failed_parseFromJSON() failed [failed_arp_list]");
            goto end;
        }

        failed_arp_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(failed_arp_list_local_nonprimitive, failed_arp_list ) {
            if (!cJSON_IsObject(failed_arp_list_local_nonprimitive)) {
                ogs_error("OpenAPI_assign_ebi_failed_parseFromJSON() failed [failed_arp_list]");
                goto end;
            }
            OpenAPI_arp_t *failed_arp_listItem = OpenAPI_arp_parseFromJSON(failed_arp_list_local_nonprimitive);

            OpenAPI_list_add(failed_arp_listList, failed_arp_listItem);
        }
    }

    assign_ebi_failed_local_var = OpenAPI_assign_ebi_failed_create (
        pdu_session_id->valuedouble,
        failed_arp_list ? failed_arp_listList : NULL
        );

    return assign_ebi_failed_local_var;
end:
    return NULL;
}

OpenAPI_assign_ebi_failed_t *OpenAPI_assign_ebi_failed_copy(OpenAPI_assign_ebi_failed_t *dst, OpenAPI_assign_ebi_failed_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_assign_ebi_failed_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_assign_ebi_failed_convertToJSON() failed");
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

    OpenAPI_assign_ebi_failed_free(dst);
    dst = OpenAPI_assign_ebi_failed_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

