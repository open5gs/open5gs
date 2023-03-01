
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "provisioned_data_sets.h"

OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_create(
    OpenAPI_access_and_mobility_subscription_data_t *am_data,
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data,
    OpenAPI_sms_subscription_data_t *sms_subs_data,
    OpenAPI_list_t *sm_data,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_bca_data,
    OpenAPI_v2x_subscription_data_t *v2x_data
)
{
    OpenAPI_provisioned_data_sets_t *provisioned_data_sets_local_var = ogs_malloc(sizeof(OpenAPI_provisioned_data_sets_t));
    ogs_assert(provisioned_data_sets_local_var);

    provisioned_data_sets_local_var->am_data = am_data;
    provisioned_data_sets_local_var->smf_sel_data = smf_sel_data;
    provisioned_data_sets_local_var->sms_subs_data = sms_subs_data;
    provisioned_data_sets_local_var->sm_data = sm_data;
    provisioned_data_sets_local_var->trace_data = trace_data;
    provisioned_data_sets_local_var->sms_mng_data = sms_mng_data;
    provisioned_data_sets_local_var->lcs_privacy_data = lcs_privacy_data;
    provisioned_data_sets_local_var->lcs_mo_data = lcs_mo_data;
    provisioned_data_sets_local_var->lcs_bca_data = lcs_bca_data;
    provisioned_data_sets_local_var->v2x_data = v2x_data;

    return provisioned_data_sets_local_var;
}

void OpenAPI_provisioned_data_sets_free(OpenAPI_provisioned_data_sets_t *provisioned_data_sets)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == provisioned_data_sets) {
        return;
    }
    if (provisioned_data_sets->am_data) {
        OpenAPI_access_and_mobility_subscription_data_free(provisioned_data_sets->am_data);
        provisioned_data_sets->am_data = NULL;
    }
    if (provisioned_data_sets->smf_sel_data) {
        OpenAPI_smf_selection_subscription_data_free(provisioned_data_sets->smf_sel_data);
        provisioned_data_sets->smf_sel_data = NULL;
    }
    if (provisioned_data_sets->sms_subs_data) {
        OpenAPI_sms_subscription_data_free(provisioned_data_sets->sms_subs_data);
        provisioned_data_sets->sms_subs_data = NULL;
    }
    if (provisioned_data_sets->sm_data) {
        OpenAPI_list_for_each(provisioned_data_sets->sm_data, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(provisioned_data_sets->sm_data);
        provisioned_data_sets->sm_data = NULL;
    }
    if (provisioned_data_sets->trace_data) {
        OpenAPI_trace_data_free(provisioned_data_sets->trace_data);
        provisioned_data_sets->trace_data = NULL;
    }
    if (provisioned_data_sets->sms_mng_data) {
        OpenAPI_sms_management_subscription_data_free(provisioned_data_sets->sms_mng_data);
        provisioned_data_sets->sms_mng_data = NULL;
    }
    if (provisioned_data_sets->lcs_privacy_data) {
        OpenAPI_lcs_privacy_data_free(provisioned_data_sets->lcs_privacy_data);
        provisioned_data_sets->lcs_privacy_data = NULL;
    }
    if (provisioned_data_sets->lcs_mo_data) {
        OpenAPI_lcs_mo_data_free(provisioned_data_sets->lcs_mo_data);
        provisioned_data_sets->lcs_mo_data = NULL;
    }
    if (provisioned_data_sets->lcs_bca_data) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(provisioned_data_sets->lcs_bca_data);
        provisioned_data_sets->lcs_bca_data = NULL;
    }
    if (provisioned_data_sets->v2x_data) {
        OpenAPI_v2x_subscription_data_free(provisioned_data_sets->v2x_data);
        provisioned_data_sets->v2x_data = NULL;
    }
    ogs_free(provisioned_data_sets);
}

cJSON *OpenAPI_provisioned_data_sets_convertToJSON(OpenAPI_provisioned_data_sets_t *provisioned_data_sets)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (provisioned_data_sets == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [ProvisionedDataSets]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (provisioned_data_sets->am_data) {
    cJSON *am_data_local_JSON = OpenAPI_access_and_mobility_subscription_data_convertToJSON(provisioned_data_sets->am_data);
    if (am_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [am_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amData", am_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [am_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->smf_sel_data) {
    cJSON *smf_sel_data_local_JSON = OpenAPI_smf_selection_subscription_data_convertToJSON(provisioned_data_sets->smf_sel_data);
    if (smf_sel_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [smf_sel_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelData", smf_sel_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [smf_sel_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->sms_subs_data) {
    cJSON *sms_subs_data_local_JSON = OpenAPI_sms_subscription_data_convertToJSON(provisioned_data_sets->sms_subs_data);
    if (sms_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sms_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsSubsData", sms_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sms_subs_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->sm_data) {
    cJSON *sm_dataList = cJSON_AddArrayToObject(item, "smData");
    if (sm_dataList == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sm_data]");
        goto end;
    }
    OpenAPI_list_for_each(provisioned_data_sets->sm_data, node) {
        cJSON *itemLocal = OpenAPI_session_management_subscription_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sm_data]");
            goto end;
        }
        cJSON_AddItemToArray(sm_dataList, itemLocal);
    }
    }

    if (provisioned_data_sets->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(provisioned_data_sets->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [trace_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->sms_mng_data) {
    cJSON *sms_mng_data_local_JSON = OpenAPI_sms_management_subscription_data_convertToJSON(provisioned_data_sets->sms_mng_data);
    if (sms_mng_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sms_mng_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsMngData", sms_mng_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [sms_mng_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->lcs_privacy_data) {
    cJSON *lcs_privacy_data_local_JSON = OpenAPI_lcs_privacy_data_convertToJSON(provisioned_data_sets->lcs_privacy_data);
    if (lcs_privacy_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_privacy_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsPrivacyData", lcs_privacy_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_privacy_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->lcs_mo_data) {
    cJSON *lcs_mo_data_local_JSON = OpenAPI_lcs_mo_data_convertToJSON(provisioned_data_sets->lcs_mo_data);
    if (lcs_mo_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_mo_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsMoData", lcs_mo_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_mo_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->lcs_bca_data) {
    cJSON *lcs_bca_data_local_JSON = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(provisioned_data_sets->lcs_bca_data);
    if (lcs_bca_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_bca_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsBcaData", lcs_bca_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [lcs_bca_data]");
        goto end;
    }
    }

    if (provisioned_data_sets->v2x_data) {
    cJSON *v2x_data_local_JSON = OpenAPI_v2x_subscription_data_convertToJSON(provisioned_data_sets->v2x_data);
    if (v2x_data_local_JSON == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [v2x_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xData", v2x_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed [v2x_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_parseFromJSON(cJSON *provisioned_data_setsJSON)
{
    OpenAPI_provisioned_data_sets_t *provisioned_data_sets_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *am_data = NULL;
    OpenAPI_access_and_mobility_subscription_data_t *am_data_local_nonprim = NULL;
    cJSON *smf_sel_data = NULL;
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data_local_nonprim = NULL;
    cJSON *sms_subs_data = NULL;
    OpenAPI_sms_subscription_data_t *sms_subs_data_local_nonprim = NULL;
    cJSON *sm_data = NULL;
    OpenAPI_list_t *sm_dataList = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *sms_mng_data = NULL;
    OpenAPI_sms_management_subscription_data_t *sms_mng_data_local_nonprim = NULL;
    cJSON *lcs_privacy_data = NULL;
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data_local_nonprim = NULL;
    cJSON *lcs_mo_data = NULL;
    OpenAPI_lcs_mo_data_t *lcs_mo_data_local_nonprim = NULL;
    cJSON *lcs_bca_data = NULL;
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_bca_data_local_nonprim = NULL;
    cJSON *v2x_data = NULL;
    OpenAPI_v2x_subscription_data_t *v2x_data_local_nonprim = NULL;
    am_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "amData");
    if (am_data) {
    am_data_local_nonprim = OpenAPI_access_and_mobility_subscription_data_parseFromJSON(am_data);
    }

    smf_sel_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "smfSelData");
    if (smf_sel_data) {
    smf_sel_data_local_nonprim = OpenAPI_smf_selection_subscription_data_parseFromJSON(smf_sel_data);
    }

    sms_subs_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "smsSubsData");
    if (sms_subs_data) {
    sms_subs_data_local_nonprim = OpenAPI_sms_subscription_data_parseFromJSON(sms_subs_data);
    }

    sm_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "smData");
    if (sm_data) {
        cJSON *sm_data_local = NULL;
        if (!cJSON_IsArray(sm_data)) {
            ogs_error("OpenAPI_provisioned_data_sets_parseFromJSON() failed [sm_data]");
            goto end;
        }

        sm_dataList = OpenAPI_list_create();

        cJSON_ArrayForEach(sm_data_local, sm_data) {
            if (!cJSON_IsObject(sm_data_local)) {
                ogs_error("OpenAPI_provisioned_data_sets_parseFromJSON() failed [sm_data]");
                goto end;
            }
            OpenAPI_session_management_subscription_data_t *sm_dataItem = OpenAPI_session_management_subscription_data_parseFromJSON(sm_data_local);
            if (!sm_dataItem) {
                ogs_error("No sm_dataItem");
                OpenAPI_list_free(sm_dataList);
                goto end;
            }
            OpenAPI_list_add(sm_dataList, sm_dataItem);
        }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "traceData");
    if (trace_data) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    }

    sms_mng_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "smsMngData");
    if (sms_mng_data) {
    sms_mng_data_local_nonprim = OpenAPI_sms_management_subscription_data_parseFromJSON(sms_mng_data);
    }

    lcs_privacy_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "lcsPrivacyData");
    if (lcs_privacy_data) {
    lcs_privacy_data_local_nonprim = OpenAPI_lcs_privacy_data_parseFromJSON(lcs_privacy_data);
    }

    lcs_mo_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "lcsMoData");
    if (lcs_mo_data) {
    lcs_mo_data_local_nonprim = OpenAPI_lcs_mo_data_parseFromJSON(lcs_mo_data);
    }

    lcs_bca_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "lcsBcaData");
    if (lcs_bca_data) {
    lcs_bca_data_local_nonprim = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(lcs_bca_data);
    }

    v2x_data = cJSON_GetObjectItemCaseSensitive(provisioned_data_setsJSON, "v2xData");
    if (v2x_data) {
    v2x_data_local_nonprim = OpenAPI_v2x_subscription_data_parseFromJSON(v2x_data);
    }

    provisioned_data_sets_local_var = OpenAPI_provisioned_data_sets_create (
        am_data ? am_data_local_nonprim : NULL,
        smf_sel_data ? smf_sel_data_local_nonprim : NULL,
        sms_subs_data ? sms_subs_data_local_nonprim : NULL,
        sm_data ? sm_dataList : NULL,
        trace_data ? trace_data_local_nonprim : NULL,
        sms_mng_data ? sms_mng_data_local_nonprim : NULL,
        lcs_privacy_data ? lcs_privacy_data_local_nonprim : NULL,
        lcs_mo_data ? lcs_mo_data_local_nonprim : NULL,
        lcs_bca_data ? lcs_bca_data_local_nonprim : NULL,
        v2x_data ? v2x_data_local_nonprim : NULL
    );

    return provisioned_data_sets_local_var;
end:
    if (am_data_local_nonprim) {
        OpenAPI_access_and_mobility_subscription_data_free(am_data_local_nonprim);
        am_data_local_nonprim = NULL;
    }
    if (smf_sel_data_local_nonprim) {
        OpenAPI_smf_selection_subscription_data_free(smf_sel_data_local_nonprim);
        smf_sel_data_local_nonprim = NULL;
    }
    if (sms_subs_data_local_nonprim) {
        OpenAPI_sms_subscription_data_free(sms_subs_data_local_nonprim);
        sms_subs_data_local_nonprim = NULL;
    }
    if (sm_dataList) {
        OpenAPI_list_for_each(sm_dataList, node) {
            OpenAPI_session_management_subscription_data_free(node->data);
        }
        OpenAPI_list_free(sm_dataList);
        sm_dataList = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (sms_mng_data_local_nonprim) {
        OpenAPI_sms_management_subscription_data_free(sms_mng_data_local_nonprim);
        sms_mng_data_local_nonprim = NULL;
    }
    if (lcs_privacy_data_local_nonprim) {
        OpenAPI_lcs_privacy_data_free(lcs_privacy_data_local_nonprim);
        lcs_privacy_data_local_nonprim = NULL;
    }
    if (lcs_mo_data_local_nonprim) {
        OpenAPI_lcs_mo_data_free(lcs_mo_data_local_nonprim);
        lcs_mo_data_local_nonprim = NULL;
    }
    if (lcs_bca_data_local_nonprim) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(lcs_bca_data_local_nonprim);
        lcs_bca_data_local_nonprim = NULL;
    }
    if (v2x_data_local_nonprim) {
        OpenAPI_v2x_subscription_data_free(v2x_data_local_nonprim);
        v2x_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_provisioned_data_sets_t *OpenAPI_provisioned_data_sets_copy(OpenAPI_provisioned_data_sets_t *dst, OpenAPI_provisioned_data_sets_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_provisioned_data_sets_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_provisioned_data_sets_convertToJSON() failed");
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

    OpenAPI_provisioned_data_sets_free(dst);
    dst = OpenAPI_provisioned_data_sets_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

