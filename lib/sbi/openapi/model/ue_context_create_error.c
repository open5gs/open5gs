
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_create_error.h"

OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_create(
    OpenAPI_problem_details_t *error,
    OpenAPI_ng_ap_cause_t *ngap_cause,
    OpenAPI_n2_info_content_t *target_to_source_failure_data
)
{
    OpenAPI_ue_context_create_error_t *ue_context_create_error_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_create_error_t));
    ogs_assert(ue_context_create_error_local_var);

    ue_context_create_error_local_var->error = error;
    ue_context_create_error_local_var->ngap_cause = ngap_cause;
    ue_context_create_error_local_var->target_to_source_failure_data = target_to_source_failure_data;

    return ue_context_create_error_local_var;
}

void OpenAPI_ue_context_create_error_free(OpenAPI_ue_context_create_error_t *ue_context_create_error)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_create_error) {
        return;
    }
    if (ue_context_create_error->error) {
        OpenAPI_problem_details_free(ue_context_create_error->error);
        ue_context_create_error->error = NULL;
    }
    if (ue_context_create_error->ngap_cause) {
        OpenAPI_ng_ap_cause_free(ue_context_create_error->ngap_cause);
        ue_context_create_error->ngap_cause = NULL;
    }
    if (ue_context_create_error->target_to_source_failure_data) {
        OpenAPI_n2_info_content_free(ue_context_create_error->target_to_source_failure_data);
        ue_context_create_error->target_to_source_failure_data = NULL;
    }
    ogs_free(ue_context_create_error);
}

cJSON *OpenAPI_ue_context_create_error_convertToJSON(OpenAPI_ue_context_create_error_t *ue_context_create_error)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_create_error == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [UeContextCreateError]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_context_create_error->error) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [error]");
        return NULL;
    }
    cJSON *error_local_JSON = OpenAPI_problem_details_convertToJSON(ue_context_create_error->error);
    if (error_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "error", error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [error]");
        goto end;
    }

    if (ue_context_create_error->ngap_cause) {
    cJSON *ngap_cause_local_JSON = OpenAPI_ng_ap_cause_convertToJSON(ue_context_create_error->ngap_cause);
    if (ngap_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ngapCause", ngap_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [ngap_cause]");
        goto end;
    }
    }

    if (ue_context_create_error->target_to_source_failure_data) {
    cJSON *target_to_source_failure_data_local_JSON = OpenAPI_n2_info_content_convertToJSON(ue_context_create_error->target_to_source_failure_data);
    if (target_to_source_failure_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [target_to_source_failure_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "targetToSourceFailureData", target_to_source_failure_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed [target_to_source_failure_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_parseFromJSON(cJSON *ue_context_create_errorJSON)
{
    OpenAPI_ue_context_create_error_t *ue_context_create_error_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *error = NULL;
    OpenAPI_problem_details_t *error_local_nonprim = NULL;
    cJSON *ngap_cause = NULL;
    OpenAPI_ng_ap_cause_t *ngap_cause_local_nonprim = NULL;
    cJSON *target_to_source_failure_data = NULL;
    OpenAPI_n2_info_content_t *target_to_source_failure_data_local_nonprim = NULL;
    error = cJSON_GetObjectItemCaseSensitive(ue_context_create_errorJSON, "error");
    if (!error) {
        ogs_error("OpenAPI_ue_context_create_error_parseFromJSON() failed [error]");
        goto end;
    }
    error_local_nonprim = OpenAPI_problem_details_parseFromJSON(error);
    if (!error_local_nonprim) {
        ogs_error("OpenAPI_problem_details_parseFromJSON failed [error]");
        goto end;
    }

    ngap_cause = cJSON_GetObjectItemCaseSensitive(ue_context_create_errorJSON, "ngapCause");
    if (ngap_cause) {
    ngap_cause_local_nonprim = OpenAPI_ng_ap_cause_parseFromJSON(ngap_cause);
    if (!ngap_cause_local_nonprim) {
        ogs_error("OpenAPI_ng_ap_cause_parseFromJSON failed [ngap_cause]");
        goto end;
    }
    }

    target_to_source_failure_data = cJSON_GetObjectItemCaseSensitive(ue_context_create_errorJSON, "targetToSourceFailureData");
    if (target_to_source_failure_data) {
    target_to_source_failure_data_local_nonprim = OpenAPI_n2_info_content_parseFromJSON(target_to_source_failure_data);
    if (!target_to_source_failure_data_local_nonprim) {
        ogs_error("OpenAPI_n2_info_content_parseFromJSON failed [target_to_source_failure_data]");
        goto end;
    }
    }

    ue_context_create_error_local_var = OpenAPI_ue_context_create_error_create (
        error_local_nonprim,
        ngap_cause ? ngap_cause_local_nonprim : NULL,
        target_to_source_failure_data ? target_to_source_failure_data_local_nonprim : NULL
    );

    return ue_context_create_error_local_var;
end:
    if (error_local_nonprim) {
        OpenAPI_problem_details_free(error_local_nonprim);
        error_local_nonprim = NULL;
    }
    if (ngap_cause_local_nonprim) {
        OpenAPI_ng_ap_cause_free(ngap_cause_local_nonprim);
        ngap_cause_local_nonprim = NULL;
    }
    if (target_to_source_failure_data_local_nonprim) {
        OpenAPI_n2_info_content_free(target_to_source_failure_data_local_nonprim);
        target_to_source_failure_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_create_error_t *OpenAPI_ue_context_create_error_copy(OpenAPI_ue_context_create_error_t *dst, OpenAPI_ue_context_create_error_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_create_error_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_create_error_convertToJSON() failed");
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

    OpenAPI_ue_context_create_error_free(dst);
    dst = OpenAPI_ue_context_create_error_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

