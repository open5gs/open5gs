
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "transfer_policy.h"

OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_create(
    char *max_bit_rate_dl,
    char *max_bit_rate_ul,
    int rating_group,
    OpenAPI_time_window_t *rec_time_int,
    int trans_policy_id
    )
{
    OpenAPI_transfer_policy_t *transfer_policy_local_var = OpenAPI_malloc(sizeof(OpenAPI_transfer_policy_t));
    if (!transfer_policy_local_var) {
        return NULL;
    }
    transfer_policy_local_var->max_bit_rate_dl = max_bit_rate_dl;
    transfer_policy_local_var->max_bit_rate_ul = max_bit_rate_ul;
    transfer_policy_local_var->rating_group = rating_group;
    transfer_policy_local_var->rec_time_int = rec_time_int;
    transfer_policy_local_var->trans_policy_id = trans_policy_id;

    return transfer_policy_local_var;
}

void OpenAPI_transfer_policy_free(OpenAPI_transfer_policy_t *transfer_policy)
{
    if (NULL == transfer_policy) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(transfer_policy->max_bit_rate_dl);
    ogs_free(transfer_policy->max_bit_rate_ul);
    OpenAPI_time_window_free(transfer_policy->rec_time_int);
    ogs_free(transfer_policy);
}

cJSON *OpenAPI_transfer_policy_convertToJSON(OpenAPI_transfer_policy_t *transfer_policy)
{
    cJSON *item = NULL;

    if (transfer_policy == NULL) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [TransferPolicy]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (transfer_policy->max_bit_rate_dl) {
        if (cJSON_AddStringToObject(item, "maxBitRateDl", transfer_policy->max_bit_rate_dl) == NULL) {
            ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [max_bit_rate_dl]");
            goto end;
        }
    }

    if (transfer_policy->max_bit_rate_ul) {
        if (cJSON_AddStringToObject(item, "maxBitRateUl", transfer_policy->max_bit_rate_ul) == NULL) {
            ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [max_bit_rate_ul]");
            goto end;
        }
    }

    if (!transfer_policy->rating_group) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [rating_group]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "ratingGroup", transfer_policy->rating_group) == NULL) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [rating_group]");
        goto end;
    }

    if (!transfer_policy->rec_time_int) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [rec_time_int]");
        goto end;
    }
    cJSON *rec_time_int_local_JSON = OpenAPI_time_window_convertToJSON(transfer_policy->rec_time_int);
    if (rec_time_int_local_JSON == NULL) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [rec_time_int]");
        goto end;
    }
    cJSON_AddItemToObject(item, "recTimeInt", rec_time_int_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [rec_time_int]");
        goto end;
    }

    if (!transfer_policy->trans_policy_id) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [trans_policy_id]");
        goto end;
    }
    if (cJSON_AddNumberToObject(item, "transPolicyId", transfer_policy->trans_policy_id) == NULL) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed [trans_policy_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_parseFromJSON(cJSON *transfer_policyJSON)
{
    OpenAPI_transfer_policy_t *transfer_policy_local_var = NULL;
    cJSON *max_bit_rate_dl = cJSON_GetObjectItemCaseSensitive(transfer_policyJSON, "maxBitRateDl");

    if (max_bit_rate_dl) {
        if (!cJSON_IsString(max_bit_rate_dl)) {
            ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [max_bit_rate_dl]");
            goto end;
        }
    }

    cJSON *max_bit_rate_ul = cJSON_GetObjectItemCaseSensitive(transfer_policyJSON, "maxBitRateUl");

    if (max_bit_rate_ul) {
        if (!cJSON_IsString(max_bit_rate_ul)) {
            ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [max_bit_rate_ul]");
            goto end;
        }
    }

    cJSON *rating_group = cJSON_GetObjectItemCaseSensitive(transfer_policyJSON, "ratingGroup");
    if (!rating_group) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [rating_group]");
        goto end;
    }


    if (!cJSON_IsNumber(rating_group)) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [rating_group]");
        goto end;
    }

    cJSON *rec_time_int = cJSON_GetObjectItemCaseSensitive(transfer_policyJSON, "recTimeInt");
    if (!rec_time_int) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [rec_time_int]");
        goto end;
    }

    OpenAPI_time_window_t *rec_time_int_local_nonprim = NULL;

    rec_time_int_local_nonprim = OpenAPI_time_window_parseFromJSON(rec_time_int);

    cJSON *trans_policy_id = cJSON_GetObjectItemCaseSensitive(transfer_policyJSON, "transPolicyId");
    if (!trans_policy_id) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [trans_policy_id]");
        goto end;
    }


    if (!cJSON_IsNumber(trans_policy_id)) {
        ogs_error("OpenAPI_transfer_policy_parseFromJSON() failed [trans_policy_id]");
        goto end;
    }

    transfer_policy_local_var = OpenAPI_transfer_policy_create (
        max_bit_rate_dl ? ogs_strdup(max_bit_rate_dl->valuestring) : NULL,
        max_bit_rate_ul ? ogs_strdup(max_bit_rate_ul->valuestring) : NULL,
        rating_group->valuedouble,
        rec_time_int_local_nonprim,
        trans_policy_id->valuedouble
        );

    return transfer_policy_local_var;
end:
    return NULL;
}

OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_copy(OpenAPI_transfer_policy_t *dst, OpenAPI_transfer_policy_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_transfer_policy_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_transfer_policy_convertToJSON() failed");
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

    OpenAPI_transfer_policy_free(dst);
    dst = OpenAPI_transfer_policy_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

