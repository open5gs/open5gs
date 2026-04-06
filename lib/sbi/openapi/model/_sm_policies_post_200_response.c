
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "_sm_policies_post_200_response.h"

OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_create(
    OpenAPI_ue_camping_rep_t *ue_camping_rep,
    OpenAPI_list_t *partial_success_report_list,
    OpenAPI_list_t *policy_decision_failure_code_list
)
{
    OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response_local_var = ogs_malloc(sizeof(OpenAPI__sm_policies_post_200_response_t));
    ogs_assert(_sm_policies_post_200_response_local_var);

    _sm_policies_post_200_response_local_var->ue_camping_rep = ue_camping_rep;
    _sm_policies_post_200_response_local_var->partial_success_report_list = partial_success_report_list;
    _sm_policies_post_200_response_local_var->policy_decision_failure_code_list = policy_decision_failure_code_list;

    return _sm_policies_post_200_response_local_var;
}

void OpenAPI__sm_policies_post_200_response_free(OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == _sm_policies_post_200_response) {
        return;
    }
    if (_sm_policies_post_200_response->ue_camping_rep) {
        OpenAPI_ue_camping_rep_free(_sm_policies_post_200_response->ue_camping_rep);
        _sm_policies_post_200_response->ue_camping_rep = NULL;
    }
    if (_sm_policies_post_200_response->partial_success_report_list) {
        OpenAPI_list_for_each(_sm_policies_post_200_response->partial_success_report_list, node) {
            OpenAPI_partial_success_report_free(node->data);
        }
        OpenAPI_list_free(_sm_policies_post_200_response->partial_success_report_list);
        _sm_policies_post_200_response->partial_success_report_list = NULL;
    }
    if (_sm_policies_post_200_response->policy_decision_failure_code_list) {
        OpenAPI_list_free(_sm_policies_post_200_response->policy_decision_failure_code_list);
        _sm_policies_post_200_response->policy_decision_failure_code_list = NULL;
    }
    ogs_free(_sm_policies_post_200_response);
}

cJSON *OpenAPI__sm_policies_post_200_response_convertToJSON(OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (_sm_policies_post_200_response == NULL) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [_sm_policies_post_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (_sm_policies_post_200_response->ue_camping_rep) {
    cJSON *ue_camping_rep_local_JSON = OpenAPI_ue_camping_rep_convertToJSON(_sm_policies_post_200_response->ue_camping_rep);
    if (ue_camping_rep_local_JSON == NULL) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [ue_camping_rep]");
        goto end;
    }
    cJSON_AddItemToObject(item, "UeCampingRep", ue_camping_rep_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [ue_camping_rep]");
        goto end;
    }
    }

    if (_sm_policies_post_200_response->partial_success_report_list) {
    cJSON *partial_success_report_listList = cJSON_AddArrayToObject(item, "PartialSuccessReportList");
    if (partial_success_report_listList == NULL) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [partial_success_report_list]");
        goto end;
    }
    OpenAPI_list_for_each(_sm_policies_post_200_response->partial_success_report_list, node) {
        cJSON *itemLocal = OpenAPI_partial_success_report_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [partial_success_report_list]");
            goto end;
        }
        cJSON_AddItemToArray(partial_success_report_listList, itemLocal);
    }
    }

    if (_sm_policies_post_200_response->policy_decision_failure_code_list != OpenAPI_policy_decision_failure_code_NULL) {
    cJSON *policy_decision_failure_code_listList = cJSON_AddArrayToObject(item, "PolicyDecisionFailureCodeList");
    if (policy_decision_failure_code_listList == NULL) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [policy_decision_failure_code_list]");
        goto end;
    }
    OpenAPI_list_for_each(_sm_policies_post_200_response->policy_decision_failure_code_list, node) {
        if (cJSON_AddStringToObject(policy_decision_failure_code_listList, "", OpenAPI_policy_decision_failure_code_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed [policy_decision_failure_code_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_parseFromJSON(cJSON *_sm_policies_post_200_responseJSON)
{
    OpenAPI__sm_policies_post_200_response_t *_sm_policies_post_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_camping_rep = NULL;
    OpenAPI_ue_camping_rep_t *ue_camping_rep_local_nonprim = NULL;
    cJSON *partial_success_report_list = NULL;
    OpenAPI_list_t *partial_success_report_listList = NULL;
    cJSON *policy_decision_failure_code_list = NULL;
    OpenAPI_list_t *policy_decision_failure_code_listList = NULL;
    ue_camping_rep = cJSON_GetObjectItemCaseSensitive(_sm_policies_post_200_responseJSON, "UeCampingRep");
    if (ue_camping_rep) {
    ue_camping_rep_local_nonprim = OpenAPI_ue_camping_rep_parseFromJSON(ue_camping_rep);
    if (!ue_camping_rep_local_nonprim) {
        ogs_error("OpenAPI_ue_camping_rep_parseFromJSON failed [ue_camping_rep]");
        goto end;
    }
    }

    partial_success_report_list = cJSON_GetObjectItemCaseSensitive(_sm_policies_post_200_responseJSON, "PartialSuccessReportList");
    if (partial_success_report_list) {
        cJSON *partial_success_report_list_local = NULL;
        if (!cJSON_IsArray(partial_success_report_list)) {
            ogs_error("OpenAPI__sm_policies_post_200_response_parseFromJSON() failed [partial_success_report_list]");
            goto end;
        }

        partial_success_report_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(partial_success_report_list_local, partial_success_report_list) {
            if (!cJSON_IsObject(partial_success_report_list_local)) {
                ogs_error("OpenAPI__sm_policies_post_200_response_parseFromJSON() failed [partial_success_report_list]");
                goto end;
            }
            OpenAPI_partial_success_report_t *partial_success_report_listItem = OpenAPI_partial_success_report_parseFromJSON(partial_success_report_list_local);
            if (!partial_success_report_listItem) {
                ogs_error("No partial_success_report_listItem");
                goto end;
            }
            OpenAPI_list_add(partial_success_report_listList, partial_success_report_listItem);
        }
    }

    policy_decision_failure_code_list = cJSON_GetObjectItemCaseSensitive(_sm_policies_post_200_responseJSON, "PolicyDecisionFailureCodeList");
    if (policy_decision_failure_code_list) {
        cJSON *policy_decision_failure_code_list_local = NULL;
        if (!cJSON_IsArray(policy_decision_failure_code_list)) {
            ogs_error("OpenAPI__sm_policies_post_200_response_parseFromJSON() failed [policy_decision_failure_code_list]");
            goto end;
        }

        policy_decision_failure_code_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(policy_decision_failure_code_list_local, policy_decision_failure_code_list) {
            OpenAPI_policy_decision_failure_code_e localEnum = OpenAPI_policy_decision_failure_code_NULL;
            if (!cJSON_IsString(policy_decision_failure_code_list_local)) {
                ogs_error("OpenAPI__sm_policies_post_200_response_parseFromJSON() failed [policy_decision_failure_code_list]");
                goto end;
            }
            localEnum = OpenAPI_policy_decision_failure_code_FromString(policy_decision_failure_code_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"policy_decision_failure_code_list\" is not supported. Ignoring it ...",
                         policy_decision_failure_code_list_local->valuestring);
            } else {
                OpenAPI_list_add(policy_decision_failure_code_listList, (void *)localEnum);
            }
        }
        if (policy_decision_failure_code_listList->count == 0) {
            ogs_error("OpenAPI__sm_policies_post_200_response_parseFromJSON() failed: Expected policy_decision_failure_code_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    _sm_policies_post_200_response_local_var = OpenAPI__sm_policies_post_200_response_create (
        ue_camping_rep ? ue_camping_rep_local_nonprim : NULL,
        partial_success_report_list ? partial_success_report_listList : NULL,
        policy_decision_failure_code_list ? policy_decision_failure_code_listList : NULL
    );

    return _sm_policies_post_200_response_local_var;
end:
    if (ue_camping_rep_local_nonprim) {
        OpenAPI_ue_camping_rep_free(ue_camping_rep_local_nonprim);
        ue_camping_rep_local_nonprim = NULL;
    }
    if (partial_success_report_listList) {
        OpenAPI_list_for_each(partial_success_report_listList, node) {
            OpenAPI_partial_success_report_free(node->data);
        }
        OpenAPI_list_free(partial_success_report_listList);
        partial_success_report_listList = NULL;
    }
    if (policy_decision_failure_code_listList) {
        OpenAPI_list_free(policy_decision_failure_code_listList);
        policy_decision_failure_code_listList = NULL;
    }
    return NULL;
}

OpenAPI__sm_policies_post_200_response_t *OpenAPI__sm_policies_post_200_response_copy(OpenAPI__sm_policies_post_200_response_t *dst, OpenAPI__sm_policies_post_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI__sm_policies_post_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI__sm_policies_post_200_response_convertToJSON() failed");
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

    OpenAPI__sm_policies_post_200_response_free(dst);
    dst = OpenAPI__sm_policies_post_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

