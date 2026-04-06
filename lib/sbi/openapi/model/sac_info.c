
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sac_info.h"

OpenAPI_sac_info_t *OpenAPI_sac_info_create(
    bool is_numeric_val_num_ues,
    int numeric_val_num_ues,
    bool is_numeric_val_num_pdu_sess,
    int numeric_val_num_pdu_sess,
    bool is_perc_value_num_ues,
    int perc_value_num_ues,
    bool is_perc_value_num_pdu_sess,
    int perc_value_num_pdu_sess,
    bool is_ues_with_pdu_session_ind,
    int ues_with_pdu_session_ind
)
{
    OpenAPI_sac_info_t *sac_info_local_var = ogs_malloc(sizeof(OpenAPI_sac_info_t));
    ogs_assert(sac_info_local_var);

    sac_info_local_var->is_numeric_val_num_ues = is_numeric_val_num_ues;
    sac_info_local_var->numeric_val_num_ues = numeric_val_num_ues;
    sac_info_local_var->is_numeric_val_num_pdu_sess = is_numeric_val_num_pdu_sess;
    sac_info_local_var->numeric_val_num_pdu_sess = numeric_val_num_pdu_sess;
    sac_info_local_var->is_perc_value_num_ues = is_perc_value_num_ues;
    sac_info_local_var->perc_value_num_ues = perc_value_num_ues;
    sac_info_local_var->is_perc_value_num_pdu_sess = is_perc_value_num_pdu_sess;
    sac_info_local_var->perc_value_num_pdu_sess = perc_value_num_pdu_sess;
    sac_info_local_var->is_ues_with_pdu_session_ind = is_ues_with_pdu_session_ind;
    sac_info_local_var->ues_with_pdu_session_ind = ues_with_pdu_session_ind;

    return sac_info_local_var;
}

void OpenAPI_sac_info_free(OpenAPI_sac_info_t *sac_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sac_info) {
        return;
    }
    ogs_free(sac_info);
}

cJSON *OpenAPI_sac_info_convertToJSON(OpenAPI_sac_info_t *sac_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sac_info == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [SACInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sac_info->is_numeric_val_num_ues) {
    if (cJSON_AddNumberToObject(item, "numericValNumUes", sac_info->numeric_val_num_ues) == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [numeric_val_num_ues]");
        goto end;
    }
    }

    if (sac_info->is_numeric_val_num_pdu_sess) {
    if (cJSON_AddNumberToObject(item, "numericValNumPduSess", sac_info->numeric_val_num_pdu_sess) == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [numeric_val_num_pdu_sess]");
        goto end;
    }
    }

    if (sac_info->is_perc_value_num_ues) {
    if (cJSON_AddNumberToObject(item, "percValueNumUes", sac_info->perc_value_num_ues) == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [perc_value_num_ues]");
        goto end;
    }
    }

    if (sac_info->is_perc_value_num_pdu_sess) {
    if (cJSON_AddNumberToObject(item, "percValueNumPduSess", sac_info->perc_value_num_pdu_sess) == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [perc_value_num_pdu_sess]");
        goto end;
    }
    }

    if (sac_info->is_ues_with_pdu_session_ind) {
    if (cJSON_AddBoolToObject(item, "uesWithPduSessionInd", sac_info->ues_with_pdu_session_ind) == NULL) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed [ues_with_pdu_session_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sac_info_t *OpenAPI_sac_info_parseFromJSON(cJSON *sac_infoJSON)
{
    OpenAPI_sac_info_t *sac_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *numeric_val_num_ues = NULL;
    cJSON *numeric_val_num_pdu_sess = NULL;
    cJSON *perc_value_num_ues = NULL;
    cJSON *perc_value_num_pdu_sess = NULL;
    cJSON *ues_with_pdu_session_ind = NULL;
    numeric_val_num_ues = cJSON_GetObjectItemCaseSensitive(sac_infoJSON, "numericValNumUes");
    if (numeric_val_num_ues) {
    if (!cJSON_IsNumber(numeric_val_num_ues)) {
        ogs_error("OpenAPI_sac_info_parseFromJSON() failed [numeric_val_num_ues]");
        goto end;
    }
    }

    numeric_val_num_pdu_sess = cJSON_GetObjectItemCaseSensitive(sac_infoJSON, "numericValNumPduSess");
    if (numeric_val_num_pdu_sess) {
    if (!cJSON_IsNumber(numeric_val_num_pdu_sess)) {
        ogs_error("OpenAPI_sac_info_parseFromJSON() failed [numeric_val_num_pdu_sess]");
        goto end;
    }
    }

    perc_value_num_ues = cJSON_GetObjectItemCaseSensitive(sac_infoJSON, "percValueNumUes");
    if (perc_value_num_ues) {
    if (!cJSON_IsNumber(perc_value_num_ues)) {
        ogs_error("OpenAPI_sac_info_parseFromJSON() failed [perc_value_num_ues]");
        goto end;
    }
    }

    perc_value_num_pdu_sess = cJSON_GetObjectItemCaseSensitive(sac_infoJSON, "percValueNumPduSess");
    if (perc_value_num_pdu_sess) {
    if (!cJSON_IsNumber(perc_value_num_pdu_sess)) {
        ogs_error("OpenAPI_sac_info_parseFromJSON() failed [perc_value_num_pdu_sess]");
        goto end;
    }
    }

    ues_with_pdu_session_ind = cJSON_GetObjectItemCaseSensitive(sac_infoJSON, "uesWithPduSessionInd");
    if (ues_with_pdu_session_ind) {
    if (!cJSON_IsBool(ues_with_pdu_session_ind)) {
        ogs_error("OpenAPI_sac_info_parseFromJSON() failed [ues_with_pdu_session_ind]");
        goto end;
    }
    }

    sac_info_local_var = OpenAPI_sac_info_create (
        numeric_val_num_ues ? true : false,
        numeric_val_num_ues ? numeric_val_num_ues->valuedouble : 0,
        numeric_val_num_pdu_sess ? true : false,
        numeric_val_num_pdu_sess ? numeric_val_num_pdu_sess->valuedouble : 0,
        perc_value_num_ues ? true : false,
        perc_value_num_ues ? perc_value_num_ues->valuedouble : 0,
        perc_value_num_pdu_sess ? true : false,
        perc_value_num_pdu_sess ? perc_value_num_pdu_sess->valuedouble : 0,
        ues_with_pdu_session_ind ? true : false,
        ues_with_pdu_session_ind ? ues_with_pdu_session_ind->valueint : 0
    );

    return sac_info_local_var;
end:
    return NULL;
}

OpenAPI_sac_info_t *OpenAPI_sac_info_copy(OpenAPI_sac_info_t *dst, OpenAPI_sac_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sac_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sac_info_convertToJSON() failed");
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

    OpenAPI_sac_info_free(dst);
    dst = OpenAPI_sac_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

