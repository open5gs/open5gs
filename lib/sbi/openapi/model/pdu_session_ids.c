
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_ids.h"

OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_create(
    OpenAPI_list_t *pdu_session_id_list
)
{
    OpenAPI_pdu_session_ids_t *pdu_session_ids_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_ids_t));
    ogs_assert(pdu_session_ids_local_var);

    pdu_session_ids_local_var->pdu_session_id_list = pdu_session_id_list;

    return pdu_session_ids_local_var;
}

void OpenAPI_pdu_session_ids_free(OpenAPI_pdu_session_ids_t *pdu_session_ids)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_ids) {
        return;
    }
    if (pdu_session_ids->pdu_session_id_list) {
        OpenAPI_list_for_each(pdu_session_ids->pdu_session_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_ids->pdu_session_id_list);
        pdu_session_ids->pdu_session_id_list = NULL;
    }
    ogs_free(pdu_session_ids);
}

cJSON *OpenAPI_pdu_session_ids_convertToJSON(OpenAPI_pdu_session_ids_t *pdu_session_ids)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_ids == NULL) {
        ogs_error("OpenAPI_pdu_session_ids_convertToJSON() failed [PduSessionIds]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_ids->pdu_session_id_list) {
    cJSON *pdu_session_id_listList = cJSON_AddArrayToObject(item, "pduSessionIdList");
    if (pdu_session_id_listList == NULL) {
        ogs_error("OpenAPI_pdu_session_ids_convertToJSON() failed [pdu_session_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(pdu_session_ids->pdu_session_id_list, node) {
        if (node->data == NULL) {
            ogs_error("OpenAPI_pdu_session_ids_convertToJSON() failed [pdu_session_id_list]");
            goto end;
        }
        if (cJSON_AddNumberToObject(pdu_session_id_listList, "", *(double *)node->data) == NULL) {
            ogs_error("OpenAPI_pdu_session_ids_convertToJSON() failed [pdu_session_id_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_parseFromJSON(cJSON *pdu_session_idsJSON)
{
    OpenAPI_pdu_session_ids_t *pdu_session_ids_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_id_list = NULL;
    OpenAPI_list_t *pdu_session_id_listList = NULL;
    pdu_session_id_list = cJSON_GetObjectItemCaseSensitive(pdu_session_idsJSON, "pduSessionIdList");
    if (pdu_session_id_list) {
        cJSON *pdu_session_id_list_local = NULL;
        if (!cJSON_IsArray(pdu_session_id_list)) {
            ogs_error("OpenAPI_pdu_session_ids_parseFromJSON() failed [pdu_session_id_list]");
            goto end;
        }

        pdu_session_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_id_list_local, pdu_session_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsNumber(pdu_session_id_list_local)) {
                ogs_error("OpenAPI_pdu_session_ids_parseFromJSON() failed [pdu_session_id_list]");
                goto end;
            }
            localDouble = (double *)ogs_calloc(1, sizeof(double));
            if (!localDouble) {
                ogs_error("OpenAPI_pdu_session_ids_parseFromJSON() failed [pdu_session_id_list]");
                goto end;
            }
            *localDouble = pdu_session_id_list_local->valuedouble;
            OpenAPI_list_add(pdu_session_id_listList, localDouble);
        }
    }

    pdu_session_ids_local_var = OpenAPI_pdu_session_ids_create (
        pdu_session_id_list ? pdu_session_id_listList : NULL
    );

    return pdu_session_ids_local_var;
end:
    if (pdu_session_id_listList) {
        OpenAPI_list_for_each(pdu_session_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(pdu_session_id_listList);
        pdu_session_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_ids_t *OpenAPI_pdu_session_ids_copy(OpenAPI_pdu_session_ids_t *dst, OpenAPI_pdu_session_ids_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_ids_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_ids_convertToJSON() failed");
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

    OpenAPI_pdu_session_ids_free(dst);
    dst = OpenAPI_pdu_session_ids_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

