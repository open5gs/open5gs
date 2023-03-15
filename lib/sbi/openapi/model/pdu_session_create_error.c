
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_create_error.h"

OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_create(
    OpenAPI_problem_details_t *error,
    char *n1sm_cause,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    bool is_back_off_timer,
    int back_off_timer,
    char *recovery_time
)
{
    OpenAPI_pdu_session_create_error_t *pdu_session_create_error_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_create_error_t));
    ogs_assert(pdu_session_create_error_local_var);

    pdu_session_create_error_local_var->error = error;
    pdu_session_create_error_local_var->n1sm_cause = n1sm_cause;
    pdu_session_create_error_local_var->n1_sm_info_to_ue = n1_sm_info_to_ue;
    pdu_session_create_error_local_var->is_back_off_timer = is_back_off_timer;
    pdu_session_create_error_local_var->back_off_timer = back_off_timer;
    pdu_session_create_error_local_var->recovery_time = recovery_time;

    return pdu_session_create_error_local_var;
}

void OpenAPI_pdu_session_create_error_free(OpenAPI_pdu_session_create_error_t *pdu_session_create_error)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_create_error) {
        return;
    }
    if (pdu_session_create_error->error) {
        OpenAPI_problem_details_free(pdu_session_create_error->error);
        pdu_session_create_error->error = NULL;
    }
    if (pdu_session_create_error->n1sm_cause) {
        ogs_free(pdu_session_create_error->n1sm_cause);
        pdu_session_create_error->n1sm_cause = NULL;
    }
    if (pdu_session_create_error->n1_sm_info_to_ue) {
        OpenAPI_ref_to_binary_data_free(pdu_session_create_error->n1_sm_info_to_ue);
        pdu_session_create_error->n1_sm_info_to_ue = NULL;
    }
    if (pdu_session_create_error->recovery_time) {
        ogs_free(pdu_session_create_error->recovery_time);
        pdu_session_create_error->recovery_time = NULL;
    }
    ogs_free(pdu_session_create_error);
}

cJSON *OpenAPI_pdu_session_create_error_convertToJSON(OpenAPI_pdu_session_create_error_t *pdu_session_create_error)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_create_error == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [PduSessionCreateError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pdu_session_create_error->error) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [error]");
        return NULL;
    }
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(pdu_session_create_error->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [error]");
        goto end;
    }

    if (pdu_session_create_error->n1sm_cause) {
    if (cJSON_AddStringToObject(item, "n1smCause", pdu_session_create_error->n1sm_cause) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [n1sm_cause]");
        goto end;
    }
    }

    if (pdu_session_create_error->n1_sm_info_to_ue) {
    cJSON *n1_sm_info_to_ue_local_JSON = OpenAPI_ref_to_binary_data_convertToJSON(pdu_session_create_error->n1_sm_info_to_ue);
    if (n1_sm_info_to_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [n1_sm_info_to_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n1SmInfoToUe", n1_sm_info_to_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [n1_sm_info_to_ue]");
        goto end;
    }
    }

    if (pdu_session_create_error->is_back_off_timer) {
    if (cJSON_AddNumberToObject(item, "backOffTimer", pdu_session_create_error->back_off_timer) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [back_off_timer]");
        goto end;
    }
    }

    if (pdu_session_create_error->recovery_time) {
    if (cJSON_AddStringToObject(item, "recoveryTime", pdu_session_create_error->recovery_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed [recovery_time]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_parseFromJSON(cJSON *pdu_session_create_errorJSON)
{
    OpenAPI_pdu_session_create_error_t *pdu_session_create_error_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    cJSON *n1sm_cause = NULL;
    cJSON *n1_sm_info_to_ue = NULL;
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue_local_nonprim = NULL;
    cJSON *back_off_timer = NULL;
    cJSON *recovery_time = NULL;
    error = cJSON_GetObjectItemCaseSensitive(pdu_session_create_errorJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_pdu_session_create_error_parseFromJSON() failed [error]");
        goto end;
    }
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    if (!error_local_nonprim) {
        ogs_error("OpenAPI_problem_details_parseFromJSON failed [error]");
        goto end;
    }

    n1sm_cause = cJSON_GetObjectItemCaseSensitive(pdu_session_create_errorJSON, "n1smCause");
    if (n1sm_cause) {
    if (!cJSON_IsString(n1sm_cause) && !cJSON_IsNull(n1sm_cause)) {
        ogs_error("OpenAPI_pdu_session_create_error_parseFromJSON() failed [n1sm_cause]");
        goto end;
    }
    }

    n1_sm_info_to_ue = cJSON_GetObjectItemCaseSensitive(pdu_session_create_errorJSON, "n1SmInfoToUe");
    if (n1_sm_info_to_ue) {
    n1_sm_info_to_ue_local_nonprim = OpenAPI_ref_to_binary_data_parseFromJSON(n1_sm_info_to_ue);
    if (!n1_sm_info_to_ue_local_nonprim) {
        ogs_error("OpenAPI_ref_to_binary_data_parseFromJSON failed [n1_sm_info_to_ue]");
        goto end;
    }
    }

    back_off_timer = cJSON_GetObjectItemCaseSensitive(pdu_session_create_errorJSON, "backOffTimer");
    if (back_off_timer) {
    if (!cJSON_IsNumber(back_off_timer)) {
        ogs_error("OpenAPI_pdu_session_create_error_parseFromJSON() failed [back_off_timer]");
        goto end;
    }
    }

    recovery_time = cJSON_GetObjectItemCaseSensitive(pdu_session_create_errorJSON, "recoveryTime");
    if (recovery_time) {
    if (!cJSON_IsString(recovery_time) && !cJSON_IsNull(recovery_time)) {
        ogs_error("OpenAPI_pdu_session_create_error_parseFromJSON() failed [recovery_time]");
        goto end;
    }
    }

    pdu_session_create_error_local_var = OpenAPI_pdu_session_create_error_create (
        error_local_nonprim,
        n1sm_cause && !cJSON_IsNull(n1sm_cause) ? ogs_strdup(n1sm_cause->valuestring) : NULL,
        n1_sm_info_to_ue ? n1_sm_info_to_ue_local_nonprim : NULL,
        back_off_timer ? true : false,
        back_off_timer ? back_off_timer->valuedouble : 0,
        recovery_time && !cJSON_IsNull(recovery_time) ? ogs_strdup(recovery_time->valuestring) : NULL
    );

    return pdu_session_create_error_local_var;
end:
    if (error_local_nonprim) {
        OpenAPI_problem_details_free(error_local_nonprim);
        error_local_nonprim = NULL;
    }
    if (n1_sm_info_to_ue_local_nonprim) {
        OpenAPI_ref_to_binary_data_free(n1_sm_info_to_ue_local_nonprim);
        n1_sm_info_to_ue_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_create_error_t *OpenAPI_pdu_session_create_error_copy(OpenAPI_pdu_session_create_error_t *dst, OpenAPI_pdu_session_create_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_create_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_create_error_convertToJSON() failed");
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

    OpenAPI_pdu_session_create_error_free(dst);
    dst = OpenAPI_pdu_session_create_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

