
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_smf_data.h"

OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_create(
    OpenAPI_list_t* pdu_sessions,
    OpenAPI_list_t *pgw_info,
    OpenAPI_emergency_info_t *emergency_info
    )
{
    OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_ue_context_in_smf_data_t));
    if (!ue_context_in_smf_data_local_var) {
        return NULL;
    }
    ue_context_in_smf_data_local_var->pdu_sessions = pdu_sessions;
    ue_context_in_smf_data_local_var->pgw_info = pgw_info;
    ue_context_in_smf_data_local_var->emergency_info = emergency_info;

    return ue_context_in_smf_data_local_var;
}

void OpenAPI_ue_context_in_smf_data_free(OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data)
{
    if (NULL == ue_context_in_smf_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(ue_context_in_smf_data->pdu_sessions, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_pdu_session_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ue_context_in_smf_data->pdu_sessions);
    OpenAPI_list_for_each(ue_context_in_smf_data->pgw_info, node) {
        OpenAPI_pgw_info_free(node->data);
    }
    OpenAPI_list_free(ue_context_in_smf_data->pgw_info);
    OpenAPI_emergency_info_free(ue_context_in_smf_data->emergency_info);
    ogs_free(ue_context_in_smf_data);
}

cJSON *OpenAPI_ue_context_in_smf_data_convertToJSON(OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data)
{
    cJSON *item = NULL;

    if (ue_context_in_smf_data == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [UeContextInSmfData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_smf_data->pdu_sessions) {
        cJSON *pdu_sessions = cJSON_AddObjectToObject(item, "pduSessions");
        if (pdu_sessions == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [pdu_sessions]");
            goto end;
        }
        cJSON *localMapObject = pdu_sessions;
        OpenAPI_lnode_t *pdu_sessions_node;
        if (ue_context_in_smf_data->pdu_sessions) {
            OpenAPI_list_for_each(ue_context_in_smf_data->pdu_sessions, pdu_sessions_node) {
                OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pdu_sessions_node->data;
                cJSON *itemLocal = OpenAPI_pdu_session_convertToJSON(localKeyValue->value);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [pdu_sessions]");
                    goto end;
                }
                cJSON_AddItemToObject(pdu_sessions, localKeyValue->key, itemLocal);
            }
        }
    }

    if (ue_context_in_smf_data->pgw_info) {
        cJSON *pgw_infoList = cJSON_AddArrayToObject(item, "pgwInfo");
        if (pgw_infoList == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [pgw_info]");
            goto end;
        }

        OpenAPI_lnode_t *pgw_info_node;
        if (ue_context_in_smf_data->pgw_info) {
            OpenAPI_list_for_each(ue_context_in_smf_data->pgw_info, pgw_info_node) {
                cJSON *itemLocal = OpenAPI_pgw_info_convertToJSON(pgw_info_node->data);
                if (itemLocal == NULL) {
                    ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [pgw_info]");
                    goto end;
                }
                cJSON_AddItemToArray(pgw_infoList, itemLocal);
            }
        }
    }

    if (ue_context_in_smf_data->emergency_info) {
        cJSON *emergency_info_local_JSON = OpenAPI_emergency_info_convertToJSON(ue_context_in_smf_data->emergency_info);
        if (emergency_info_local_JSON == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [emergency_info]");
            goto end;
        }
        cJSON_AddItemToObject(item, "emergencyInfo", emergency_info_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed [emergency_info]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_parseFromJSON(cJSON *ue_context_in_smf_dataJSON)
{
    OpenAPI_ue_context_in_smf_data_t *ue_context_in_smf_data_local_var = NULL;
    cJSON *pdu_sessions = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_dataJSON, "pduSessions");

    OpenAPI_list_t *pdu_sessionsList;
    if (pdu_sessions) {
        cJSON *pdu_sessions_local_map;
        if (!cJSON_IsObject(pdu_sessions)) {
            ogs_error("OpenAPI_ue_context_in_smf_data_parseFromJSON() failed [pdu_sessions]");
            goto end;
        }
        pdu_sessionsList = OpenAPI_list_create();
        OpenAPI_map_t *localMapKeyPair = NULL;
        cJSON_ArrayForEach(pdu_sessions_local_map, pdu_sessions) {
            cJSON *localMapObject = pdu_sessions_local_map;
            if (!cJSON_IsObject(pdu_sessions_local_map)) {
                ogs_error("OpenAPI_ue_context_in_smf_data_parseFromJSON() failed [pdu_sessions]");
                goto end;
            }
            localMapKeyPair = OpenAPI_map_create(
                localMapObject->string, OpenAPI_pdu_session_parseFromJSON(localMapObject));
            OpenAPI_list_add(pdu_sessionsList, localMapKeyPair);
        }
    }

    cJSON *pgw_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_dataJSON, "pgwInfo");

    OpenAPI_list_t *pgw_infoList;
    if (pgw_info) {
        cJSON *pgw_info_local_nonprimitive;
        if (!cJSON_IsArray(pgw_info)) {
            ogs_error("OpenAPI_ue_context_in_smf_data_parseFromJSON() failed [pgw_info]");
            goto end;
        }

        pgw_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(pgw_info_local_nonprimitive, pgw_info ) {
            if (!cJSON_IsObject(pgw_info_local_nonprimitive)) {
                ogs_error("OpenAPI_ue_context_in_smf_data_parseFromJSON() failed [pgw_info]");
                goto end;
            }
            OpenAPI_pgw_info_t *pgw_infoItem = OpenAPI_pgw_info_parseFromJSON(pgw_info_local_nonprimitive);

            OpenAPI_list_add(pgw_infoList, pgw_infoItem);
        }
    }

    cJSON *emergency_info = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_dataJSON, "emergencyInfo");

    OpenAPI_emergency_info_t *emergency_info_local_nonprim = NULL;
    if (emergency_info) {
        emergency_info_local_nonprim = OpenAPI_emergency_info_parseFromJSON(emergency_info);
    }

    ue_context_in_smf_data_local_var = OpenAPI_ue_context_in_smf_data_create (
        pdu_sessions ? pdu_sessionsList : NULL,
        pgw_info ? pgw_infoList : NULL,
        emergency_info ? emergency_info_local_nonprim : NULL
        );

    return ue_context_in_smf_data_local_var;
end:
    return NULL;
}

OpenAPI_ue_context_in_smf_data_t *OpenAPI_ue_context_in_smf_data_copy(OpenAPI_ue_context_in_smf_data_t *dst, OpenAPI_ue_context_in_smf_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_smf_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_smf_data_convertToJSON() failed");
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

    OpenAPI_ue_context_in_smf_data_free(dst);
    dst = OpenAPI_ue_context_in_smf_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

