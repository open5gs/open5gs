
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modify_200_response.h"

OpenAPI_modify_200_response_t *OpenAPI_modify_200_response_create(
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
    OpenAPI_list_t *report,
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
    OpenAPI_modify_200_response_t *modify_200_response_local_var = ogs_malloc(sizeof(OpenAPI_modify_200_response_t));
    ogs_assert(modify_200_response_local_var);

    modify_200_response_local_var->nf_instance_id = nf_instance_id;
    modify_200_response_local_var->is_implicit_unsubscribe = is_implicit_unsubscribe;
    modify_200_response_local_var->implicit_unsubscribe = implicit_unsubscribe;
    modify_200_response_local_var->expires = expires;
    modify_200_response_local_var->callback_reference = callback_reference;
    modify_200_response_local_var->amf_service_name = amf_service_name;
    modify_200_response_local_var->monitored_resource_uris = monitored_resource_uris;
    modify_200_response_local_var->single_nssai = single_nssai;
    modify_200_response_local_var->dnn = dnn;
    modify_200_response_local_var->subscription_id = subscription_id;
    modify_200_response_local_var->plmn_id = plmn_id;
    modify_200_response_local_var->is_immediate_report = is_immediate_report;
    modify_200_response_local_var->immediate_report = immediate_report;
    modify_200_response_local_var->report = report;
    modify_200_response_local_var->supported_features = supported_features;
    modify_200_response_local_var->context_info = context_info;
    modify_200_response_local_var->is_nf_change_filter = is_nf_change_filter;
    modify_200_response_local_var->nf_change_filter = nf_change_filter;
    modify_200_response_local_var->is_unique_subscription = is_unique_subscription;
    modify_200_response_local_var->unique_subscription = unique_subscription;
    modify_200_response_local_var->reset_ids = reset_ids;
    modify_200_response_local_var->ue_con_smf_data_sub_filter = ue_con_smf_data_sub_filter;

    return modify_200_response_local_var;
}

void OpenAPI_modify_200_response_free(OpenAPI_modify_200_response_t *modify_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == modify_200_response) {
        return;
    }
    if (modify_200_response->nf_instance_id) {
        ogs_free(modify_200_response->nf_instance_id);
        modify_200_response->nf_instance_id = NULL;
    }
    if (modify_200_response->expires) {
        ogs_free(modify_200_response->expires);
        modify_200_response->expires = NULL;
    }
    if (modify_200_response->callback_reference) {
        ogs_free(modify_200_response->callback_reference);
        modify_200_response->callback_reference = NULL;
    }
    if (modify_200_response->amf_service_name) {
        ogs_free(modify_200_response->amf_service_name);
        modify_200_response->amf_service_name = NULL;
    }
    if (modify_200_response->monitored_resource_uris) {
        OpenAPI_list_for_each(modify_200_response->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(modify_200_response->monitored_resource_uris);
        modify_200_response->monitored_resource_uris = NULL;
    }
    if (modify_200_response->single_nssai) {
        OpenAPI_snssai_free(modify_200_response->single_nssai);
        modify_200_response->single_nssai = NULL;
    }
    if (modify_200_response->dnn) {
        ogs_free(modify_200_response->dnn);
        modify_200_response->dnn = NULL;
    }
    if (modify_200_response->subscription_id) {
        ogs_free(modify_200_response->subscription_id);
        modify_200_response->subscription_id = NULL;
    }
    if (modify_200_response->plmn_id) {
        OpenAPI_plmn_id_free(modify_200_response->plmn_id);
        modify_200_response->plmn_id = NULL;
    }
    if (modify_200_response->report) {
        OpenAPI_list_for_each(modify_200_response->report, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(modify_200_response->report);
        modify_200_response->report = NULL;
    }
    if (modify_200_response->supported_features) {
        ogs_free(modify_200_response->supported_features);
        modify_200_response->supported_features = NULL;
    }
    if (modify_200_response->context_info) {
        OpenAPI_context_info_free(modify_200_response->context_info);
        modify_200_response->context_info = NULL;
    }
    if (modify_200_response->reset_ids) {
        OpenAPI_list_for_each(modify_200_response->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(modify_200_response->reset_ids);
        modify_200_response->reset_ids = NULL;
    }
    if (modify_200_response->ue_con_smf_data_sub_filter) {
        OpenAPI_ue_context_in_smf_data_sub_filter_free(modify_200_response->ue_con_smf_data_sub_filter);
        modify_200_response->ue_con_smf_data_sub_filter = NULL;
    }
    ogs_free(modify_200_response);
}

cJSON *OpenAPI_modify_200_response_convertToJSON(OpenAPI_modify_200_response_t *modify_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (modify_200_response == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [Modify_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!modify_200_response->nf_instance_id) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [nf_instance_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceId", modify_200_response->nf_instance_id) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [nf_instance_id]");
        goto end;
    }

    if (modify_200_response->is_implicit_unsubscribe) {
    if (cJSON_AddBoolToObject(item, "implicitUnsubscribe", modify_200_response->implicit_unsubscribe) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    if (modify_200_response->expires) {
    if (cJSON_AddStringToObject(item, "expires", modify_200_response->expires) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [expires]");
        goto end;
    }
    }

    if (!modify_200_response->callback_reference) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [callback_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", modify_200_response->callback_reference) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (modify_200_response->amf_service_name) {
    if (cJSON_AddStringToObject(item, "amfServiceName", modify_200_response->amf_service_name) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [amf_service_name]");
        goto end;
    }
    }

    if (!modify_200_response->monitored_resource_uris) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(modify_200_response->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (modify_200_response->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(modify_200_response->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (modify_200_response->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", modify_200_response->dnn) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (modify_200_response->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", modify_200_response->subscription_id) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (modify_200_response->plmn_id) {
    cJSON *plmn_id_local_JSON = OpenAPI_plmn_id_convertToJSON(modify_200_response->plmn_id);
    if (plmn_id_local_JSON == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [plmn_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "plmnId", plmn_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [plmn_id]");
        goto end;
    }
    }

    if (modify_200_response->is_immediate_report) {
    if (cJSON_AddBoolToObject(item, "immediateReport", modify_200_response->immediate_report) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [immediate_report]");
        goto end;
    }
    }

    if (!modify_200_response->report) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [report]");
        return NULL;
    }
    cJSON *reportList = cJSON_AddArrayToObject(item, "report");
    if (reportList == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [report]");
        goto end;
    }
    OpenAPI_list_for_each(modify_200_response->report, node) {
        cJSON *itemLocal = OpenAPI_report_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [report]");
            goto end;
        }
        cJSON_AddItemToArray(reportList, itemLocal);
    }

    if (modify_200_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", modify_200_response->supported_features) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (modify_200_response->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(modify_200_response->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [context_info]");
        goto end;
    }
    }

    if (modify_200_response->is_nf_change_filter) {
    if (cJSON_AddBoolToObject(item, "nfChangeFilter", modify_200_response->nf_change_filter) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    if (modify_200_response->is_unique_subscription) {
    if (cJSON_AddBoolToObject(item, "uniqueSubscription", modify_200_response->unique_subscription) == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [unique_subscription]");
        goto end;
    }
    }

    if (modify_200_response->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(modify_200_response->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

    if (modify_200_response->ue_con_smf_data_sub_filter) {
    cJSON *ue_con_smf_data_sub_filter_local_JSON = OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON(modify_200_response->ue_con_smf_data_sub_filter);
    if (ue_con_smf_data_sub_filter_local_JSON == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueConSmfDataSubFilter", ue_con_smf_data_sub_filter_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_modify_200_response_t *OpenAPI_modify_200_response_parseFromJSON(cJSON *modify_200_responseJSON)
{
    OpenAPI_modify_200_response_t *modify_200_response_local_var = NULL;
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
    OpenAPI_list_t *reportList = NULL;
    cJSON *supported_features = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    cJSON *nf_change_filter = NULL;
    cJSON *unique_subscription = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    cJSON *ue_con_smf_data_sub_filter = NULL;
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter_local_nonprim = NULL;
    nf_instance_id = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "nfInstanceId");
    if (!nf_instance_id) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_id)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [nf_instance_id]");
        goto end;
    }

    implicit_unsubscribe = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "implicitUnsubscribe");
    if (implicit_unsubscribe) {
    if (!cJSON_IsBool(implicit_unsubscribe)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [implicit_unsubscribe]");
        goto end;
    }
    }

    expires = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "expires");
    if (expires) {
    if (!cJSON_IsString(expires) && !cJSON_IsNull(expires)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [expires]");
        goto end;
    }
    }

    callback_reference = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [callback_reference]");
        goto end;
    }
    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    amf_service_name = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "amfServiceName");
    if (amf_service_name) {
    if (!cJSON_IsString(amf_service_name) && !cJSON_IsNull(amf_service_name)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [amf_service_name]");
        goto end;
    }
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    single_nssai = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    plmn_id = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "plmnId");
    if (plmn_id) {
    plmn_id_local_nonprim = OpenAPI_plmn_id_parseFromJSON(plmn_id);
    if (!plmn_id_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON failed [plmn_id]");
        goto end;
    }
    }

    immediate_report = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "immediateReport");
    if (immediate_report) {
    if (!cJSON_IsBool(immediate_report)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [immediate_report]");
        goto end;
    }
    }

    report = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "report");
    if (!report) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [report]");
        goto end;
    }
        cJSON *report_local = NULL;
        if (!cJSON_IsArray(report)) {
            ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [report]");
            goto end;
        }

        reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(report_local, report) {
            if (!cJSON_IsObject(report_local)) {
                ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [report]");
                goto end;
            }
            OpenAPI_report_item_t *reportItem = OpenAPI_report_item_parseFromJSON(report_local);
            if (!reportItem) {
                ogs_error("No reportItem");
                goto end;
            }
            OpenAPI_list_add(reportList, reportItem);
        }

    supported_features = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    nf_change_filter = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "nfChangeFilter");
    if (nf_change_filter) {
    if (!cJSON_IsBool(nf_change_filter)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [nf_change_filter]");
        goto end;
    }
    }

    unique_subscription = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "uniqueSubscription");
    if (unique_subscription) {
    if (!cJSON_IsBool(unique_subscription)) {
        ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [unique_subscription]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_modify_200_response_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    ue_con_smf_data_sub_filter = cJSON_GetObjectItemCaseSensitive(modify_200_responseJSON, "ueConSmfDataSubFilter");
    if (ue_con_smf_data_sub_filter) {
    ue_con_smf_data_sub_filter_local_nonprim = OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON(ue_con_smf_data_sub_filter);
    if (!ue_con_smf_data_sub_filter_local_nonprim) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON failed [ue_con_smf_data_sub_filter]");
        goto end;
    }
    }

    modify_200_response_local_var = OpenAPI_modify_200_response_create (
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
        reportList,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL,
        nf_change_filter ? true : false,
        nf_change_filter ? nf_change_filter->valueint : 0,
        unique_subscription ? true : false,
        unique_subscription ? unique_subscription->valueint : 0,
        reset_ids ? reset_idsList : NULL,
        ue_con_smf_data_sub_filter ? ue_con_smf_data_sub_filter_local_nonprim : NULL
    );

    return modify_200_response_local_var;
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
    if (reportList) {
        OpenAPI_list_for_each(reportList, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(reportList);
        reportList = NULL;
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

OpenAPI_modify_200_response_t *OpenAPI_modify_200_response_copy(OpenAPI_modify_200_response_t *dst, OpenAPI_modify_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_modify_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_modify_200_response_convertToJSON() failed");
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

    OpenAPI_modify_200_response_free(dst);
    dst = OpenAPI_modify_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

