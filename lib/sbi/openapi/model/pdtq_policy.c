
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdtq_policy.h"

OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_create(
    int pdtq_policy_id,
    OpenAPI_time_window_t *rec_time_int
)
{
    OpenAPI_pdtq_policy_t *pdtq_policy_local_var = ogs_malloc(sizeof(OpenAPI_pdtq_policy_t));
    ogs_assert(pdtq_policy_local_var);

    pdtq_policy_local_var->pdtq_policy_id = pdtq_policy_id;
    pdtq_policy_local_var->rec_time_int = rec_time_int;

    return pdtq_policy_local_var;
}

void OpenAPI_pdtq_policy_free(OpenAPI_pdtq_policy_t *pdtq_policy)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdtq_policy) {
        return;
    }
    if (pdtq_policy->rec_time_int) {
        OpenAPI_time_window_free(pdtq_policy->rec_time_int);
        pdtq_policy->rec_time_int = NULL;
    }
    ogs_free(pdtq_policy);
}

cJSON *OpenAPI_pdtq_policy_convertToJSON(OpenAPI_pdtq_policy_t *pdtq_policy)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdtq_policy == NULL) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed [PdtqPolicy]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "pdtqPolicyId", pdtq_policy->pdtq_policy_id) == NULL) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed [pdtq_policy_id]");
        goto end;
    }

    if (!pdtq_policy->rec_time_int) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed [rec_time_int]");
        return NULL;
    }
    cJSON *rec_time_int_local_JSON = OpenAPI_time_window_convertToJSON(pdtq_policy->rec_time_int);
    if (rec_time_int_local_JSON == NULL) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed [rec_time_int]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recTimeInt", rec_time_int_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed [rec_time_int]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_parseFromJSON(cJSON *pdtq_policyJSON)
{
    OpenAPI_pdtq_policy_t *pdtq_policy_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdtq_policy_id = NULL;
    cJSON *rec_time_int = NULL;
    OpenAPI_time_window_t *rec_time_int_local_nonprim = NULL;
    pdtq_policy_id = cJSON_GetObjectItemCaseSensitive(pdtq_policyJSON, "pdtqPolicyId");
    if (!pdtq_policy_id) {
        ogs_error("OpenAPI_pdtq_policy_parseFromJSON() failed [pdtq_policy_id]");
        goto end;
    }
    if (!cJSON_IsNumber(pdtq_policy_id)) {
        ogs_error("OpenAPI_pdtq_policy_parseFromJSON() failed [pdtq_policy_id]");
        goto end;
    }

    rec_time_int = cJSON_GetObjectItemCaseSensitive(pdtq_policyJSON, "recTimeInt");
    if (!rec_time_int) {
        ogs_error("OpenAPI_pdtq_policy_parseFromJSON() failed [rec_time_int]");
        goto end;
    }
    rec_time_int_local_nonprim = OpenAPI_time_window_parseFromJSON(rec_time_int);
    if (!rec_time_int_local_nonprim) {
        ogs_error("OpenAPI_time_window_parseFromJSON failed [rec_time_int]");
        goto end;
    }

    pdtq_policy_local_var = OpenAPI_pdtq_policy_create (
        
        pdtq_policy_id->valuedouble,
        rec_time_int_local_nonprim
    );

    return pdtq_policy_local_var;
end:
    if (rec_time_int_local_nonprim) {
        OpenAPI_time_window_free(rec_time_int_local_nonprim);
        rec_time_int_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_copy(OpenAPI_pdtq_policy_t *dst, OpenAPI_pdtq_policy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdtq_policy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdtq_policy_convertToJSON() failed");
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

    OpenAPI_pdtq_policy_free(dst);
    dst = OpenAPI_pdtq_policy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

