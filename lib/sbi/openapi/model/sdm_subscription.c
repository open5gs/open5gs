
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdm_subscription.h"

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_create(
    char *nf_instance_id,
    bool is_implicit_unsubscribe,
    int implicit_unsubscribe,
    char *expires,
    char *callback_reference,
    char *amf_service_name,
    OpenAPI_list_t *monitored_resource_uris,
    OpenAPI_snssai_t *single_nssai,
    char *dnn,
    char *subscription_id,
    OpenAPI_plmn_id_t *plmn_id,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_immediate_report_t *report,
    char *supported_features,
    OpenAPI_context_info_t *context_info,
    bool is_nf_change_filter,
    int nf_change_filter,
    bool is_unique_subscription,
    int unique_subscription,
    OpenAPI_list_t *reset_ids,
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter
)
{
    OpenAPI_sdm_subscription_t *sdm_subscription_local_var = ogs_malloc(sizeof(OpenAPI_sdm_subscription_t));
    ogs_assert(sdm_subscription_local_var);

    sdm_subscription_local_var->nf_instance_id = nf_instance_id;
    sdm_subscription_local_var->is_implicit_unsubscribe = is_implicit_unsubscribe;
    sdm_subscription_local_var->implicit_unsubscribe = implicit_unsubscribe;
    sdm_subscription_local_var->expires = expires;
    sdm_subscription_local_var->callback_reference = callback_reference;
    sdm_subscription_local_var->amf_service_name = amf_service_name;
    sdm_subscription_local_var->monitored_resource_uris = monitored_resource_uris;
    sdm_subscription_local_var->single_nssai = single_nssai;
    sdm_subscription_local_var->dnn = dnn;
    sdm_subscription_local_var->subscription_id = subscription_id;
    sdm_subscription_local_var->plmn_id = plmn_id;
    sdm_subscription_local_var->is_immediate_report = is_immediate_report;
    sdm_subscription_local_var->immediate_report = immediate_report;
    sdm_subscription_local_var->report = report;
    sdm_subscription_local_var->supported_features = supported_features;
    sdm_subscription_local_var->context_info = context_info;
    sdm_subscription_local_var->is_nf_change_filter = is_nf_change_filter;
    sdm_subscription_local_var->nf_change_filter = nf_change_filter;
    sdm_subscription_local_var->is_unique_subscription = is_unique_subscription;
    sdm_subscription_local_var->unique_subscription = unique_subscription;
    sdm_subscription_local_var->reset_ids = reset_ids;
    sdm_subscription_local_var->ue_con_smf_data_sub_filter = ue_con_smf_data_sub_filter;

    return sdm_subscription_local_var;
}

void OpenAPI_sdm_subscription_free(OpenAPI_sdm_subscription_t *sdm_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sdm_subscription) {
        return;
    }
    if (sdm_subscription->nf_instance_id) {
        ogs_free(sdm_subscription->nf_instance_id);
        sdm_subscription->nf_instance_id = NULL;
    }
    if (sdm_subscription->expires) {
        ogs_free(sdm_subscription->expires);
        sdm_subscription->expires = NULL;
    }
    if (sdm_subscription->callback_reference) {
        ogs_free(sdm_subscription->callback_reference);
        sdm_subscription->callback_reference = NULL;
    }
    if (sdm_subscription->amf_service_name) {
        ogs_free(sdm_subscription->amf_service_name);
        sdm_subscription->amf_service_name = NULL;
    }
    if (sdm_subscription->monitored_resource_uris) {
        OpenAPI_list_for_each(sdm_subscription->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdm_subscription->monitored_resource_uris);
        sdm_subscription->monitored_resource_uris = NULL;
    }
    if (sdm_subscription->single_nssai) {
        OpenAPI_snssai_free(sdm_subscription->single_nssai);
        sdm_subscription->single_nssai = NULL;
    }
    if (sdm_subscription->dnn) {
        ogs_free(sdm_subscription->dnn);
        sdm_subscription->dnn = NULL;
    }
    if (sdm_subscription->subscription_id) {
        ogs_free(sdm_subscription->subscription_id);
        sdm_subscription->subscription_id = NULL;
    }
    if (sdm_subscription->plmn_id) {
        OpenAPI_plmn_id_free(sdm_subscription->plmn_id);
        sdm_subscription->plmn_id = NULL;
    }
    if (sdm_subscription->report) {
        OpenAPI_immediate_report_free(sdm_subscription->report);
        sdm_subscription->report = NULL;
    }
    if (sdm_subscription->supported_features) {
        ogs_free(sdm_subscription->supported_features);
        sdm_subscription->supported_features = NULL;
    }
    if (sdm_subscription->context_info) {
        OpenAPI_context_info_free(sdm_subscription->context_info);
        sdm_subscription->context_info = NULL;
    }
    if (sdm_subscription->reset_ids) {
        OpenAPI_list_for_each(sdm_subscription->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(sdm_subscription->reset_ids);
        sdm_subscription->reset_ids = NULL;
    }
    if (sdm_subscription->ue_con_smf_data_sub_filter) {
        OpenAPI_ue_context_in_smf_data_sub_filter_free(sdm_subscription->ue_con_smf_data_sub_filter);
        sdm_subscription->ue_con_smf_data_sub_filter = NULL;
    }
    ogs_free(sdm_subscription);
}

cJSON *OpenAPI_sdm_subscription_convertToJSON(OpenAPI_sdm_subscription_t *sdm_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sdm_subscription == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [SdmSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sdm_subscription->nf_instance_id) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [nf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", sdm_subscription->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (sdm_subscription->is_implicit_unsubscribe) {
    if (cJSON_AddBoolToObject(item, "implicitUnsubscribe", sdm_subscription->implicit_unsubscribe) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    if (sdm_subscription->expires) {
    if (cJSON_AddStringToObject(item, "expires", sdm_subscription->expires) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [expires]");
        goto end;
    }
    }

    if (!sdm_subscription->callback_reference) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [callback_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", sdm_subscription->callback_reference) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (sdm_subscription->amf_service_name) {
    if (cJSON_AddStringToObject(item, "amfServiceName", sdm_subscription->amf_service_name) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [amf_service_name]");
        goto end;
    }
    }

    if (!sdm_subscription->monitored_resource_uris) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(sdm_subscription->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (sdm_subscription->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(sdm_subscription->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (sdm_subscription->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", sdm_subscription->dnn) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (sdm_subscription->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", sdm_subscription->subscription_id) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (sdm_subscription->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(sdm_subscription->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (sdm_subscription->is_immediate_report) {
    if (cJSON_AddBoolToObject(item, "immediateReport", sdm_subscription->immediate_report) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [immediate_report]");
        goto end;
    }
    }

    if (sdm_subscription->report) {
    cJSON *report_local_JSON = OpenAPI_immediate_report_convertToJSON(sdm_subscription->report);
    if (report_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [report]");
        goto end;
    }
    cJSON_AddItemToObject(item, "report", report_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [report]");
        goto end;
    }
    }

    if (sdm_subscription->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sdm_subscription->supported_features) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (sdm_subscription->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(sdm_subscription->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [context_info]");
        goto end;
    }
    }

    if (sdm_subscription->is_nf_change_filter) {
    if (cJSON_AddBoolToObject(item, "nfChangeFilter", sdm_subscription->nf_change_filter) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    if (sdm_subscription->is_unique_subscription) {
    if (cJSON_AddBoolToObject(item, "uniqueSubscription", sdm_subscription->unique_subscription) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [unique_subscription]");
        goto end;
    }
    }

    if (sdm_subscription->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(sdm_subscription->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (sdm_subscription->ue_con_smf_data_sub_filter) {
    cJSON *ue_con_smf_data_sub_filter_local_JSON = OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON(sdm_subscription->ue_con_smf_data_sub_filter);
    if (ue_con_smf_data_sub_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueConSmfDataSubFilter", ue_con_smf_data_sub_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_parseFromJSON(cJSON *sdm_subscriptionJSON)
{
    OpenAPI_sdm_subscription_t *sdm_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *nf_instance_id = NULL;
    cJSON *implicit_unsubscribe = NULL;
    cJSON *expires = NULL;
    cJSON *callback_reference = NULL;
    cJSON *amf_service_name = NULL;
    cJSON *monitored_resource_uris = NULL;
    OpenAPI_list_t *monitored_resource_urisList = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *subscription_id = NULL;
    cJSON *plmn_id = NULL;
    OpenAPI_plmn_id_t *plmn_id_local_nonprim = NULL;
    cJSON *immediate_report = NULL;
    cJSON *report = NULL;
    OpenAPI_immediate_report_t *report_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    cJSON *nf_change_filter = NULL;
    cJSON *unique_subscription = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *ue_con_smf_data_sub_filter = NULL;
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter_local_nonprim = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    implicit_unsubscribe = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "implicitUnsubscribe");
    if (implicit_unsubscribe) {
    if (!cJSON_IsBool(implicit_unsubscribe)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    expires = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "expires");
    if (expires) {
    if (!cJSON_IsString(expires) && !cJSON_IsNull(expires)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [expires]");
        goto end;
    }
    }

    callback_reference = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }
    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    amf_service_name = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "amfServiceName");
    if (amf_service_name) {
    if (!cJSON_IsString(amf_service_name) && !cJSON_IsNull(amf_service_name)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [amf_service_name]");
        goto end;
    }
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    single_nssai = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    immediate_report = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "immediateReport");
    if (immediate_report) {
    if (!cJSON_IsBool(immediate_report)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [immediate_report]");
        goto end;
    }
    }

    report = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "report");
    if (report) {
    report_local_nonprim = OpenAPI_immediate_report_parseFromJSON(report);
    if (!report_local_nonprim) {
        ogs_error("OpenAPI_immediate_report_parseFromJSON failed [report]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    nf_change_filter = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "nfChangeFilter");
    if (nf_change_filter) {
    if (!cJSON_IsBool(nf_change_filter)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    unique_subscription = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "uniqueSubscription");
    if (unique_subscription) {
    if (!cJSON_IsBool(unique_subscription)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [unique_subscription]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    ue_con_smf_data_sub_filter = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "ueConSmfDataSubFilter");
    if (ue_con_smf_data_sub_filter) {
    ue_con_smf_data_sub_filter_local_nonprim = OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON(ue_con_smf_data_sub_filter);
    if (!ue_con_smf_data_sub_filter_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

    sdm_subscription_local_var = OpenAPI_sdm_subscription_create (
        ogs_strdup(nf_instance_id->valuestring),
        implicit_unsubscribe ? true : false,
        implicit_unsubscribe ? implicit_unsubscribe->valueint : 0,
        expires && !cJSON_IsNull(expires) ? ogs_strdup(expires->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        amf_service_name && !cJSON_IsNull(amf_service_name) ? ogs_strdup(amf_service_name->valuestring) : NULL,
        monitored_resource_urisList,
        single_nssai ? single_nssai_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        subscription_id && !cJSON_IsNull(subscription_id) ? ogs_strdup(subscription_id->valuestring) : NULL,
        plmn_id ? plmn_id_local_nonprim : NULL,
        immediate_report ? true : false,
        immediate_report ? immediate_report->valueint : 0,
        report ? report_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL,
        nf_change_filter ? true : false,
        nf_change_filter ? nf_change_filter->valueint : 0,
        unique_subscription ? true : false,
        unique_subscription ? unique_subscription->valueint : 0,
        reset_ids ? reset_idsList : NULL,
        ue_con_smf_data_sub_filter ? ue_con_smf_data_sub_filter_local_nonprim : NULL
    );

    return sdm_subscription_local_var;
end:
    if (monitored_resource_urisList) {
        OpenAPI_list_for_each(monitored_resource_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitored_resource_urisList);
        monitored_resource_urisList = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (plmn_id_local_nonprim) {
        OpenAPI_plmn_id_free(plmn_id_local_nonprim);
        plmn_id_local_nonprim = NULL;
    }
    if (report_local_nonprim) {
        OpenAPI_immediate_report_free(report_local_nonprim);
        report_local_nonprim = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    if (ue_con_smf_data_sub_filter_local_nonprim) {
        OpenAPI_ue_context_in_smf_data_sub_filter_free(ue_con_smf_data_sub_filter_local_nonprim);
        ue_con_smf_data_sub_filter_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sdm_subscription_t *OpenAPI_sdm_subscription_copy(OpenAPI_sdm_subscription_t *dst, OpenAPI_sdm_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdm_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed");
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

    OpenAPI_sdm_subscription_free(dst);
    dst = OpenAPI_sdm_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

