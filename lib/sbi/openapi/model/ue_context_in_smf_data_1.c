
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_smf_data_1.h"

OpenAPI_ue_context_in_smf_data_1_t *OpenAPI_ue_context_in_smf_data_1_create(
    OpenAPI_list_t* pdu_sessions,
    OpenAPI_list_t *pgw_info,
    OpenAPI_emergency_info_1_t *emergency_info
)
{
    OpenAPI_ue_context_in_smf_data_1_t *ue_context_in_smf_data_1_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_smf_data_1_t));
    ogs_assert(ue_context_in_smf_data_1_local_var);

    ue_context_in_smf_data_1_local_var->pdu_sessions = pdu_sessions;
    ue_context_in_smf_data_1_local_var->pgw_info = pgw_info;
    ue_context_in_smf_data_1_local_var->emergency_info = emergency_info;

    return ue_context_in_smf_data_1_local_var;
}

void OpenAPI_ue_context_in_smf_data_1_free(OpenAPI_ue_context_in_smf_data_1_t *ue_context_in_smf_data_1)
{
    if (NULL == ue_context_in_smf_data_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ue_context_in_smf_data_1->pdu_sessions, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_pdu_session_1_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_context_in_smf_data_1->pdu_sessions);
    OpenAPI_list_for_each(ue_context_in_smf_data_1->pgw_info, node) {
        OpenAPI_pgw_info_1_free(node->data);
    }
    OpenAPI_list_free(ue_context_in_smf_data_1->pgw_info);
    OpenAPI_emergency_info_1_free(ue_context_in_smf_data_1->emergency_info);
    ogs_free(ue_context_in_smf_data_1);
}

cJSON *OpenAPI_ue_context_in_smf_data_1_convertToJSON(OpenAPI_ue_context_in_smf_data_1_t *ue_context_in_smf_data_1)
{
    cJSON *item = NULL;

    if (ue_context_in_smf_data_1 == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [UeContextInSmfData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_smf_data_1->pdu_sessions) {
    cJSON *pdu_sessions = cJSON_AddObjectToObject(item, "pduSessions");
    if (pdu_sessions == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [pdu_sessions]");
        goto end;
    }
    cJSON *localMapObject = pdu_sessions;
    OpenAPI_lnode_t *pdu_sessions_node;
    if (ue_context_in_smf_data_1->pdu_sessions) {
        OpenAPI_list_for_each(ue_context_in_smf_data_1->pdu_sessions, pdu_sessions_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pdu_sessions_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_pdu_session_1_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [pdu_sessions]");
            goto end;
        }
        cJSON_AddItemToObject(pdu_sessions, localKeyValue->key, itemLocal);
            }
        }
    }

    if (ue_context_in_smf_data_1->pgw_info) {
    cJSON *pgw_infoList = cJSON_AddArrayToObject(item, "pgwInfo");
    if (pgw_infoList == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [pgw_info]");
        goto end;
    }

    OpenAPI_lnode_t *pgw_info_node;
    if (ue_context_in_smf_data_1->pgw_info) {
        OpenAPI_list_for_each(ue_context_in_smf_data_1->pgw_info, pgw_info_node) {
            cJSON *itemLocal = OpenAPI_pgw_info_1_convertToJSON(pgw_info_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [pgw_info]");
                goto end;
            }
            cJSON_AddItemToArray(pgw_infoList, itemLocal);
        }
    }
    }

    if (ue_context_in_smf_data_1->emergency_info) {
    cJSON *emergency_info_local_JSON = OpenAPI_emergency_info_1_convertToJSON(ue_context_in_smf_data_1->emergency_info);
    if (emergency_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [emergency_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "emergencyInfo", emergency_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed [emergency_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_smf_data_1_t *OpenAPI_ue_context_in_smf_data_1_parseFromJSON(cJSON *ue_context_in_smf_data_1JSON)
{
    OpenAPI_ue_context_in_smf_data_1_t *ue_context_in_smf_data_1_local_var = NULL;
    cJSON *pdu_sessions = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_1JSON, "pduSessions");

    OpenAPI_list_t *pdu_sessionsList;
    if (pdu_sessions) {
    cJSON *pdu_sessions_local_map;
    if (!cJSON_IsObject(pdu_sessions)) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_parseFromJSON() failed [pdu_sessions]");
        goto end;
    }
    pdu_sessionsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(pdu_sessions_local_map, pdu_sessions) {
        cJSON *localMapObject = pdu_sessions_local_map;
        if (cJSON_IsObject(pdu_sessions_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_pdu_session_1_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(pdu_sessions_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_ue_context_in_smf_data_1_parseFromJSON() failed [pdu_sessions]");
            goto end;
        }
        OpenAPI_list_add(pdu_sessionsList , localMapKeyPair);
    }
    }

    cJSON *pgw_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_1JSON, "pgwInfo");

    OpenAPI_list_t *pgw_infoList;
    if (pgw_info) {
    cJSON *pgw_info_local_nonprimitive;
    if (!cJSON_IsArray(pgw_info)){
        ogs_error("OpenAPI_ue_context_in_smf_data_1_parseFromJSON() failed [pgw_info]");
        goto end;
    }

    pgw_infoList = OpenAPI_list_create();

    cJSON_ArrayForEach(pgw_info_local_nonprimitive, pgw_info ) {
        if (!cJSON_IsObject(pgw_info_local_nonprimitive)) {
            ogs_error("OpenAPI_ue_context_in_smf_data_1_parseFromJSON() failed [pgw_info]");
            goto end;
        }
        OpenAPI_pgw_info_1_t *pgw_infoItem = OpenAPI_pgw_info_1_parseFromJSON(pgw_info_local_nonprimitive);

        if (!pgw_infoItem) {
            ogs_error("No pgw_infoItem");
            OpenAPI_list_free(pgw_infoList);
            goto end;
        }

        OpenAPI_list_add(pgw_infoList, pgw_infoItem);
    }
    }

    cJSON *emergency_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_1JSON, "emergencyInfo");

    OpenAPI_emergency_info_1_t *emergency_info_local_nonprim = NULL;
    if (emergency_info) {
    emergency_info_local_nonprim = OpenAPI_emergency_info_1_parseFromJSON(emergency_info);
    }

    ue_context_in_smf_data_1_local_var = OpenAPI_ue_context_in_smf_data_1_create (
        pdu_sessions ? pdu_sessionsList : NULL,
        pgw_info ? pgw_infoList : NULL,
        emergency_info ? emergency_info_local_nonprim : NULL
    );

    return ue_context_in_smf_data_1_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_in_smf_data_1_t *OpenAPI_ue_context_in_smf_data_1_copy(OpenAPI_ue_context_in_smf_data_1_t *dst, OpenAPI_ue_context_in_smf_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_smf_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_smf_data_1_convertToJSON() failed");
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

    OpenAPI_ue_context_in_smf_data_1_free(dst);
    dst = OpenAPI_ue_context_in_smf_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

