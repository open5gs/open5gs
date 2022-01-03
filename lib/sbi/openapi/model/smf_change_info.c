
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "smf_change_info.h"

OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_create(
    OpenAPI_list_t *pdu_session_id_list,
    OpenAPI_smf_change_indication_e smf_change_ind
)
{
    OpenAPI_smf_change_info_t *smf_change_info_local_var = ogs_malloc(sizeof(OpenAPI_smf_change_info_t));
    ogs_assert(smf_change_info_local_var);

    smf_change_info_local_var->pdu_session_id_list = pdu_session_id_list;
    smf_change_info_local_var->smf_change_ind = smf_change_ind;

    return smf_change_info_local_var;
}

void OpenAPI_smf_change_info_free(OpenAPI_smf_change_info_t *smf_change_info)
{
    if (NULL == smf_change_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(smf_change_info->pdu_session_id_list, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(smf_change_info->pdu_session_id_list);
    ogs_free(smf_change_info);
}

cJSON *OpenAPI_smf_change_info_convertToJSON(OpenAPI_smf_change_info_t *smf_change_info)
{
    cJSON *item = NULL;

    if (smf_change_info == NULL) {
        ogs_error("OpenAPI_smf_change_info_convertToJSON() failed [SmfChangeInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *pdu_session_id_list = cJSON_AddArrayToObject(item, "pduSessionIdList");
    if (pdu_session_id_list == NULL) {
        ogs_error("OpenAPI_smf_change_info_convertToJSON() failed [pdu_session_id_list]");
        goto end;
    }

    OpenAPI_lnode_t *pdu_session_id_list_node;
    OpenAPI_list_for_each(smf_change_info->pdu_session_id_list, pdu_session_id_list_node)  {
    if (cJSON_AddNumberToObject(pdu_session_id_list, "", *(double *)pdu_session_id_list_node->data) == NULL) {
        ogs_error("OpenAPI_smf_change_info_convertToJSON() failed [pdu_session_id_list]");
        goto end;
    }
                    }

    if (cJSON_AddStringToObject(item, "smfChangeInd", OpenAPI_smf_change_indication_ToString(smf_change_info->smf_change_ind)) == NULL) {
        ogs_error("OpenAPI_smf_change_info_convertToJSON() failed [smf_change_ind]");
        goto end;
    }

end:
    return item;
}

OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_parseFromJSON(cJSON *smf_change_infoJSON)
{
    OpenAPI_smf_change_info_t *smf_change_info_local_var = NULL;
    cJSON *pdu_session_id_list = cJSON_GetObjectItemCaseSensitive(smf_change_infoJSON, "pduSessionIdList");
    if (!pdu_session_id_list) {
        ogs_error("OpenAPI_smf_change_info_parseFromJSON() failed [pdu_session_id_list]");
        goto end;
    }

    OpenAPI_list_t *pdu_session_id_listList;
    cJSON *pdu_session_id_list_local;
    if (!cJSON_IsArray(pdu_session_id_list)) {
        ogs_error("OpenAPI_smf_change_info_parseFromJSON() failed [pdu_session_id_list]");
        goto end;
    }
    pdu_session_id_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(pdu_session_id_list_local, pdu_session_id_list) {
    if (!cJSON_IsNumber(pdu_session_id_list_local)) {
        ogs_error("OpenAPI_smf_change_info_parseFromJSON() failed [pdu_session_id_list]");
        goto end;
    }
    OpenAPI_list_add(pdu_session_id_listList , &pdu_session_id_list_local->valuedouble);
    }

    cJSON *smf_change_ind = cJSON_GetObjectItemCaseSensitive(smf_change_infoJSON, "smfChangeInd");
    if (!smf_change_ind) {
        ogs_error("OpenAPI_smf_change_info_parseFromJSON() failed [smf_change_ind]");
        goto end;
    }

    OpenAPI_smf_change_indication_e smf_change_indVariable;
    if (!cJSON_IsString(smf_change_ind)) {
        ogs_error("OpenAPI_smf_change_info_parseFromJSON() failed [smf_change_ind]");
        goto end;
    }
    smf_change_indVariable = OpenAPI_smf_change_indication_FromString(smf_change_ind->valuestring);

    smf_change_info_local_var = OpenAPI_smf_change_info_create (
        pdu_session_id_listList,
        smf_change_indVariable
    );

    return smf_change_info_local_var;
end:
    return NULL;
}

OpenAPI_smf_change_info_t *OpenAPI_smf_change_info_copy(OpenAPI_smf_change_info_t *dst, OpenAPI_smf_change_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_smf_change_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_smf_change_info_convertToJSON() failed");
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

    OpenAPI_smf_change_info_free(dst);
    dst = OpenAPI_smf_change_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

