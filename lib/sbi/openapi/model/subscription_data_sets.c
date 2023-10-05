
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data_sets.h"

OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_create(
    OpenAPI_access_and_mobility_subscription_data_t *am_data,
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data,
    OpenAPI_ue_context_in_amf_data_t *uec_amf_data,
    OpenAPI_ue_context_in_smf_data_t *uec_smf_data,
    OpenAPI_ue_context_in_smsf_data_t *uec_smsf_data,
    OpenAPI_sms_subscription_data_t *sms_subs_data,
    OpenAPI_sm_subs_data_t *sm_data,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_sms_management_subscription_data_t *sms_mng_data,
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data,
    OpenAPI_lcs_mo_data_t *lcs_mo_data,
    OpenAPI_v2x_subscription_data_t *v2x_data,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data,
    OpenAPI_prose_subscription_data_t *prose_data,
    OpenAPI_mbs_subscription_data_t *mbs_data,
    OpenAPI_uc_subscription_data_t *uc_data
)
{
    OpenAPI_subscription_data_sets_t *subscription_data_sets_local_var = ogs_malloc(sizeof(OpenAPI_subscription_data_sets_t));
    ogs_assert(subscription_data_sets_local_var);

    subscription_data_sets_local_var->am_data = am_data;
    subscription_data_sets_local_var->smf_sel_data = smf_sel_data;
    subscription_data_sets_local_var->uec_amf_data = uec_amf_data;
    subscription_data_sets_local_var->uec_smf_data = uec_smf_data;
    subscription_data_sets_local_var->uec_smsf_data = uec_smsf_data;
    subscription_data_sets_local_var->sms_subs_data = sms_subs_data;
    subscription_data_sets_local_var->sm_data = sm_data;
    subscription_data_sets_local_var->is_trace_data_null = is_trace_data_null;
    subscription_data_sets_local_var->trace_data = trace_data;
    subscription_data_sets_local_var->sms_mng_data = sms_mng_data;
    subscription_data_sets_local_var->lcs_privacy_data = lcs_privacy_data;
    subscription_data_sets_local_var->lcs_mo_data = lcs_mo_data;
    subscription_data_sets_local_var->v2x_data = v2x_data;
    subscription_data_sets_local_var->lcs_broadcast_assistance_types_data = lcs_broadcast_assistance_types_data;
    subscription_data_sets_local_var->prose_data = prose_data;
    subscription_data_sets_local_var->mbs_data = mbs_data;
    subscription_data_sets_local_var->uc_data = uc_data;

    return subscription_data_sets_local_var;
}

void OpenAPI_subscription_data_sets_free(OpenAPI_subscription_data_sets_t *subscription_data_sets)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscription_data_sets) {
        return;
    }
    if (subscription_data_sets->am_data) {
        OpenAPI_access_and_mobility_subscription_data_free(subscription_data_sets->am_data);
        subscription_data_sets->am_data = NULL;
    }
    if (subscription_data_sets->smf_sel_data) {
        OpenAPI_smf_selection_subscription_data_free(subscription_data_sets->smf_sel_data);
        subscription_data_sets->smf_sel_data = NULL;
    }
    if (subscription_data_sets->uec_amf_data) {
        OpenAPI_ue_context_in_amf_data_free(subscription_data_sets->uec_amf_data);
        subscription_data_sets->uec_amf_data = NULL;
    }
    if (subscription_data_sets->uec_smf_data) {
        OpenAPI_ue_context_in_smf_data_free(subscription_data_sets->uec_smf_data);
        subscription_data_sets->uec_smf_data = NULL;
    }
    if (subscription_data_sets->uec_smsf_data) {
        OpenAPI_ue_context_in_smsf_data_free(subscription_data_sets->uec_smsf_data);
        subscription_data_sets->uec_smsf_data = NULL;
    }
    if (subscription_data_sets->sms_subs_data) {
        OpenAPI_sms_subscription_data_free(subscription_data_sets->sms_subs_data);
        subscription_data_sets->sms_subs_data = NULL;
    }
    if (subscription_data_sets->sm_data) {
        OpenAPI_sm_subs_data_free(subscription_data_sets->sm_data);
        subscription_data_sets->sm_data = NULL;
    }
    if (subscription_data_sets->trace_data) {
        OpenAPI_trace_data_free(subscription_data_sets->trace_data);
        subscription_data_sets->trace_data = NULL;
    }
    if (subscription_data_sets->sms_mng_data) {
        OpenAPI_sms_management_subscription_data_free(subscription_data_sets->sms_mng_data);
        subscription_data_sets->sms_mng_data = NULL;
    }
    if (subscription_data_sets->lcs_privacy_data) {
        OpenAPI_lcs_privacy_data_free(subscription_data_sets->lcs_privacy_data);
        subscription_data_sets->lcs_privacy_data = NULL;
    }
    if (subscription_data_sets->lcs_mo_data) {
        OpenAPI_lcs_mo_data_free(subscription_data_sets->lcs_mo_data);
        subscription_data_sets->lcs_mo_data = NULL;
    }
    if (subscription_data_sets->v2x_data) {
        OpenAPI_v2x_subscription_data_free(subscription_data_sets->v2x_data);
        subscription_data_sets->v2x_data = NULL;
    }
    if (subscription_data_sets->lcs_broadcast_assistance_types_data) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(subscription_data_sets->lcs_broadcast_assistance_types_data);
        subscription_data_sets->lcs_broadcast_assistance_types_data = NULL;
    }
    if (subscription_data_sets->prose_data) {
        OpenAPI_prose_subscription_data_free(subscription_data_sets->prose_data);
        subscription_data_sets->prose_data = NULL;
    }
    if (subscription_data_sets->mbs_data) {
        OpenAPI_mbs_subscription_data_free(subscription_data_sets->mbs_data);
        subscription_data_sets->mbs_data = NULL;
    }
    if (subscription_data_sets->uc_data) {
        OpenAPI_uc_subscription_data_free(subscription_data_sets->uc_data);
        subscription_data_sets->uc_data = NULL;
    }
    ogs_free(subscription_data_sets);
}

cJSON *OpenAPI_subscription_data_sets_convertToJSON(OpenAPI_subscription_data_sets_t *subscription_data_sets)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscription_data_sets == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [SubscriptionDataSets]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (subscription_data_sets->am_data) {
    cJSON *am_data_local_JSON = OpenAPI_access_and_mobility_subscription_data_convertToJSON(subscription_data_sets->am_data);
    if (am_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [am_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "amData", am_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [am_data]");
        goto end;
    }
    }

    if (subscription_data_sets->smf_sel_data) {
    cJSON *smf_sel_data_local_JSON = OpenAPI_smf_selection_subscription_data_convertToJSON(subscription_data_sets->smf_sel_data);
    if (smf_sel_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [smf_sel_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelData", smf_sel_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [smf_sel_data]");
        goto end;
    }
    }

    if (subscription_data_sets->uec_amf_data) {
    cJSON *uec_amf_data_local_JSON = OpenAPI_ue_context_in_amf_data_convertToJSON(subscription_data_sets->uec_amf_data);
    if (uec_amf_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_amf_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uecAmfData", uec_amf_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_amf_data]");
        goto end;
    }
    }

    if (subscription_data_sets->uec_smf_data) {
    cJSON *uec_smf_data_local_JSON = OpenAPI_ue_context_in_smf_data_convertToJSON(subscription_data_sets->uec_smf_data);
    if (uec_smf_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_smf_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uecSmfData", uec_smf_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_smf_data]");
        goto end;
    }
    }

    if (subscription_data_sets->uec_smsf_data) {
    cJSON *uec_smsf_data_local_JSON = OpenAPI_ue_context_in_smsf_data_convertToJSON(subscription_data_sets->uec_smsf_data);
    if (uec_smsf_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_smsf_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uecSmsfData", uec_smsf_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uec_smsf_data]");
        goto end;
    }
    }

    if (subscription_data_sets->sms_subs_data) {
    cJSON *sms_subs_data_local_JSON = OpenAPI_sms_subscription_data_convertToJSON(subscription_data_sets->sms_subs_data);
    if (sms_subs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sms_subs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsSubsData", sms_subs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sms_subs_data]");
        goto end;
    }
    }

    if (subscription_data_sets->sm_data) {
    cJSON *sm_data_local_JSON = OpenAPI_sm_subs_data_convertToJSON(subscription_data_sets->sm_data);
    if (sm_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sm_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smData", sm_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sm_data]");
        goto end;
    }
    }

    if (subscription_data_sets->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(subscription_data_sets->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (subscription_data_sets->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (subscription_data_sets->sms_mng_data) {
    cJSON *sms_mng_data_local_JSON = OpenAPI_sms_management_subscription_data_convertToJSON(subscription_data_sets->sms_mng_data);
    if (sms_mng_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sms_mng_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smsMngData", sms_mng_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [sms_mng_data]");
        goto end;
    }
    }

    if (subscription_data_sets->lcs_privacy_data) {
    cJSON *lcs_privacy_data_local_JSON = OpenAPI_lcs_privacy_data_convertToJSON(subscription_data_sets->lcs_privacy_data);
    if (lcs_privacy_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_privacy_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsPrivacyData", lcs_privacy_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_privacy_data]");
        goto end;
    }
    }

    if (subscription_data_sets->lcs_mo_data) {
    cJSON *lcs_mo_data_local_JSON = OpenAPI_lcs_mo_data_convertToJSON(subscription_data_sets->lcs_mo_data);
    if (lcs_mo_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_mo_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsMoData", lcs_mo_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_mo_data]");
        goto end;
    }
    }

    if (subscription_data_sets->v2x_data) {
    cJSON *v2x_data_local_JSON = OpenAPI_v2x_subscription_data_convertToJSON(subscription_data_sets->v2x_data);
    if (v2x_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [v2x_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "v2xData", v2x_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [v2x_data]");
        goto end;
    }
    }

    if (subscription_data_sets->lcs_broadcast_assistance_types_data) {
    cJSON *lcs_broadcast_assistance_types_data_local_JSON = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(subscription_data_sets->lcs_broadcast_assistance_types_data);
    if (lcs_broadcast_assistance_types_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_broadcast_assistance_types_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsBroadcastAssistanceTypesData", lcs_broadcast_assistance_types_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [lcs_broadcast_assistance_types_data]");
        goto end;
    }
    }

    if (subscription_data_sets->prose_data) {
    cJSON *prose_data_local_JSON = OpenAPI_prose_subscription_data_convertToJSON(subscription_data_sets->prose_data);
    if (prose_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [prose_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proseData", prose_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [prose_data]");
        goto end;
    }
    }

    if (subscription_data_sets->mbs_data) {
    cJSON *mbs_data_local_JSON = OpenAPI_mbs_subscription_data_convertToJSON(subscription_data_sets->mbs_data);
    if (mbs_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [mbs_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsData", mbs_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [mbs_data]");
        goto end;
    }
    }

    if (subscription_data_sets->uc_data) {
    cJSON *uc_data_local_JSON = OpenAPI_uc_subscription_data_convertToJSON(subscription_data_sets->uc_data);
    if (uc_data_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uc_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ucData", uc_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed [uc_data]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_parseFromJSON(cJSON *subscription_data_setsJSON)
{
    OpenAPI_subscription_data_sets_t *subscription_data_sets_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *am_data = NULL;
    OpenAPI_access_and_mobility_subscription_data_t *am_data_local_nonprim = NULL;
    cJSON *smf_sel_data = NULL;
    OpenAPI_smf_selection_subscription_data_t *smf_sel_data_local_nonprim = NULL;
    cJSON *uec_amf_data = NULL;
    OpenAPI_ue_context_in_amf_data_t *uec_amf_data_local_nonprim = NULL;
    cJSON *uec_smf_data = NULL;
    OpenAPI_ue_context_in_smf_data_t *uec_smf_data_local_nonprim = NULL;
    cJSON *uec_smsf_data = NULL;
    OpenAPI_ue_context_in_smsf_data_t *uec_smsf_data_local_nonprim = NULL;
    cJSON *sms_subs_data = NULL;
    OpenAPI_sms_subscription_data_t *sms_subs_data_local_nonprim = NULL;
    cJSON *sm_data = NULL;
    OpenAPI_sm_subs_data_t *sm_data_local_nonprim = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *sms_mng_data = NULL;
    OpenAPI_sms_management_subscription_data_t *sms_mng_data_local_nonprim = NULL;
    cJSON *lcs_privacy_data = NULL;
    OpenAPI_lcs_privacy_data_t *lcs_privacy_data_local_nonprim = NULL;
    cJSON *lcs_mo_data = NULL;
    OpenAPI_lcs_mo_data_t *lcs_mo_data_local_nonprim = NULL;
    cJSON *v2x_data = NULL;
    OpenAPI_v2x_subscription_data_t *v2x_data_local_nonprim = NULL;
    cJSON *lcs_broadcast_assistance_types_data = NULL;
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcast_assistance_types_data_local_nonprim = NULL;
    cJSON *prose_data = NULL;
    OpenAPI_prose_subscription_data_t *prose_data_local_nonprim = NULL;
    cJSON *mbs_data = NULL;
    OpenAPI_mbs_subscription_data_t *mbs_data_local_nonprim = NULL;
    cJSON *uc_data = NULL;
    OpenAPI_uc_subscription_data_t *uc_data_local_nonprim = NULL;
    am_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "amData");
    if (am_data) {
    am_data_local_nonprim = OpenAPI_access_and_mobility_subscription_data_parseFromJSON(am_data);
    if (!am_data_local_nonprim) {
        ogs_error("OpenAPI_access_and_mobility_subscription_data_parseFromJSON failed [am_data]");
        goto end;
    }
    }

    smf_sel_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "smfSelData");
    if (smf_sel_data) {
    smf_sel_data_local_nonprim = OpenAPI_smf_selection_subscription_data_parseFromJSON(smf_sel_data);
    if (!smf_sel_data_local_nonprim) {
        ogs_error("OpenAPI_smf_selection_subscription_data_parseFromJSON failed [smf_sel_data]");
        goto end;
    }
    }

    uec_amf_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "uecAmfData");
    if (uec_amf_data) {
    uec_amf_data_local_nonprim = OpenAPI_ue_context_in_amf_data_parseFromJSON(uec_amf_data);
    if (!uec_amf_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_amf_data_parseFromJSON failed [uec_amf_data]");
        goto end;
    }
    }

    uec_smf_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "uecSmfData");
    if (uec_smf_data) {
    uec_smf_data_local_nonprim = OpenAPI_ue_context_in_smf_data_parseFromJSON(uec_smf_data);
    if (!uec_smf_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_smf_data_parseFromJSON failed [uec_smf_data]");
        goto end;
    }
    }

    uec_smsf_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "uecSmsfData");
    if (uec_smsf_data) {
    uec_smsf_data_local_nonprim = OpenAPI_ue_context_in_smsf_data_parseFromJSON(uec_smsf_data);
    if (!uec_smsf_data_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_smsf_data_parseFromJSON failed [uec_smsf_data]");
        goto end;
    }
    }

    sms_subs_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "smsSubsData");
    if (sms_subs_data) {
    sms_subs_data_local_nonprim = OpenAPI_sms_subscription_data_parseFromJSON(sms_subs_data);
    if (!sms_subs_data_local_nonprim) {
        ogs_error("OpenAPI_sms_subscription_data_parseFromJSON failed [sms_subs_data]");
        goto end;
    }
    }

    sm_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "smData");
    if (sm_data) {
    sm_data_local_nonprim = OpenAPI_sm_subs_data_parseFromJSON(sm_data);
    if (!sm_data_local_nonprim) {
        ogs_error("OpenAPI_sm_subs_data_parseFromJSON failed [sm_data]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    sms_mng_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "smsMngData");
    if (sms_mng_data) {
    sms_mng_data_local_nonprim = OpenAPI_sms_management_subscription_data_parseFromJSON(sms_mng_data);
    if (!sms_mng_data_local_nonprim) {
        ogs_error("OpenAPI_sms_management_subscription_data_parseFromJSON failed [sms_mng_data]");
        goto end;
    }
    }

    lcs_privacy_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "lcsPrivacyData");
    if (lcs_privacy_data) {
    lcs_privacy_data_local_nonprim = OpenAPI_lcs_privacy_data_parseFromJSON(lcs_privacy_data);
    if (!lcs_privacy_data_local_nonprim) {
        ogs_error("OpenAPI_lcs_privacy_data_parseFromJSON failed [lcs_privacy_data]");
        goto end;
    }
    }

    lcs_mo_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "lcsMoData");
    if (lcs_mo_data) {
    lcs_mo_data_local_nonprim = OpenAPI_lcs_mo_data_parseFromJSON(lcs_mo_data);
    if (!lcs_mo_data_local_nonprim) {
        ogs_error("OpenAPI_lcs_mo_data_parseFromJSON failed [lcs_mo_data]");
        goto end;
    }
    }

    v2x_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "v2xData");
    if (v2x_data) {
    v2x_data_local_nonprim = OpenAPI_v2x_subscription_data_parseFromJSON(v2x_data);
    if (!v2x_data_local_nonprim) {
        ogs_error("OpenAPI_v2x_subscription_data_parseFromJSON failed [v2x_data]");
        goto end;
    }
    }

    lcs_broadcast_assistance_types_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "lcsBroadcastAssistanceTypesData");
    if (lcs_broadcast_assistance_types_data) {
    lcs_broadcast_assistance_types_data_local_nonprim = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(lcs_broadcast_assistance_types_data);
    if (!lcs_broadcast_assistance_types_data_local_nonprim) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON failed [lcs_broadcast_assistance_types_data]");
        goto end;
    }
    }

    prose_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "proseData");
    if (prose_data) {
    prose_data_local_nonprim = OpenAPI_prose_subscription_data_parseFromJSON(prose_data);
    if (!prose_data_local_nonprim) {
        ogs_error("OpenAPI_prose_subscription_data_parseFromJSON failed [prose_data]");
        goto end;
    }
    }

    mbs_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "mbsData");
    if (mbs_data) {
    mbs_data_local_nonprim = OpenAPI_mbs_subscription_data_parseFromJSON(mbs_data);
    if (!mbs_data_local_nonprim) {
        ogs_error("OpenAPI_mbs_subscription_data_parseFromJSON failed [mbs_data]");
        goto end;
    }
    }

    uc_data = cJSON_GetObjectItemCaseSensitive(subscription_data_setsJSON, "ucData");
    if (uc_data) {
    uc_data_local_nonprim = OpenAPI_uc_subscription_data_parseFromJSON(uc_data);
    if (!uc_data_local_nonprim) {
        ogs_error("OpenAPI_uc_subscription_data_parseFromJSON failed [uc_data]");
        goto end;
    }
    }

    subscription_data_sets_local_var = OpenAPI_subscription_data_sets_create (
        am_data ? am_data_local_nonprim : NULL,
        smf_sel_data ? smf_sel_data_local_nonprim : NULL,
        uec_amf_data ? uec_amf_data_local_nonprim : NULL,
        uec_smf_data ? uec_smf_data_local_nonprim : NULL,
        uec_smsf_data ? uec_smsf_data_local_nonprim : NULL,
        sms_subs_data ? sms_subs_data_local_nonprim : NULL,
        sm_data ? sm_data_local_nonprim : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        sms_mng_data ? sms_mng_data_local_nonprim : NULL,
        lcs_privacy_data ? lcs_privacy_data_local_nonprim : NULL,
        lcs_mo_data ? lcs_mo_data_local_nonprim : NULL,
        v2x_data ? v2x_data_local_nonprim : NULL,
        lcs_broadcast_assistance_types_data ? lcs_broadcast_assistance_types_data_local_nonprim : NULL,
        prose_data ? prose_data_local_nonprim : NULL,
        mbs_data ? mbs_data_local_nonprim : NULL,
        uc_data ? uc_data_local_nonprim : NULL
    );

    return subscription_data_sets_local_var;
end:
    if (am_data_local_nonprim) {
        OpenAPI_access_and_mobility_subscription_data_free(am_data_local_nonprim);
        am_data_local_nonprim = NULL;
    }
    if (smf_sel_data_local_nonprim) {
        OpenAPI_smf_selection_subscription_data_free(smf_sel_data_local_nonprim);
        smf_sel_data_local_nonprim = NULL;
    }
    if (uec_amf_data_local_nonprim) {
        OpenAPI_ue_context_in_amf_data_free(uec_amf_data_local_nonprim);
        uec_amf_data_local_nonprim = NULL;
    }
    if (uec_smf_data_local_nonprim) {
        OpenAPI_ue_context_in_smf_data_free(uec_smf_data_local_nonprim);
        uec_smf_data_local_nonprim = NULL;
    }
    if (uec_smsf_data_local_nonprim) {
        OpenAPI_ue_context_in_smsf_data_free(uec_smsf_data_local_nonprim);
        uec_smsf_data_local_nonprim = NULL;
    }
    if (sms_subs_data_local_nonprim) {
        OpenAPI_sms_subscription_data_free(sms_subs_data_local_nonprim);
        sms_subs_data_local_nonprim = NULL;
    }
    if (sm_data_local_nonprim) {
        OpenAPI_sm_subs_data_free(sm_data_local_nonprim);
        sm_data_local_nonprim = NULL;
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
    if (v2x_data_local_nonprim) {
        OpenAPI_v2x_subscription_data_free(v2x_data_local_nonprim);
        v2x_data_local_nonprim = NULL;
    }
    if (lcs_broadcast_assistance_types_data_local_nonprim) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(lcs_broadcast_assistance_types_data_local_nonprim);
        lcs_broadcast_assistance_types_data_local_nonprim = NULL;
    }
    if (prose_data_local_nonprim) {
        OpenAPI_prose_subscription_data_free(prose_data_local_nonprim);
        prose_data_local_nonprim = NULL;
    }
    if (mbs_data_local_nonprim) {
        OpenAPI_mbs_subscription_data_free(mbs_data_local_nonprim);
        mbs_data_local_nonprim = NULL;
    }
    if (uc_data_local_nonprim) {
        OpenAPI_uc_subscription_data_free(uc_data_local_nonprim);
        uc_data_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_subscription_data_sets_t *OpenAPI_subscription_data_sets_copy(OpenAPI_subscription_data_sets_t *dst, OpenAPI_subscription_data_sets_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_data_sets_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_data_sets_convertToJSON() failed");
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

    OpenAPI_subscription_data_sets_free(dst);
    dst = OpenAPI_subscription_data_sets_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

