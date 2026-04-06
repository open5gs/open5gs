
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
    OpenAPI_service_name_e amf_service_name,
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
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_con_smf_data_sub_filter,
    OpenAPI_list_t *adjacent_plmns,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    char *data_restoration_callback_uri,
    bool is_udr_restart_ind,
    int udr_restart_ind,
    char *last_synchronization_time,
    OpenAPI_list_t* expected_ue_behaviour_thresholds,
    OpenAPI_guami_t *guami
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
    sdm_subscription_local_var->adjacent_plmns = adjacent_plmns;
    sdm_subscription_local_var->is_disaster_roaming_ind = is_disaster_roaming_ind;
    sdm_subscription_local_var->disaster_roaming_ind = disaster_roaming_ind;
    sdm_subscription_local_var->data_restoration_callback_uri = data_restoration_callback_uri;
    sdm_subscription_local_var->is_udr_restart_ind = is_udr_restart_ind;
    sdm_subscription_local_var->udr_restart_ind = udr_restart_ind;
    sdm_subscription_local_var->last_synchronization_time = last_synchronization_time;
    sdm_subscription_local_var->expected_ue_behaviour_thresholds = expected_ue_behaviour_thresholds;
    sdm_subscription_local_var->guami = guami;

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
    if (sdm_subscription->adjacent_plmns) {
        OpenAPI_list_for_each(sdm_subscription->adjacent_plmns, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(sdm_subscription->adjacent_plmns);
        sdm_subscription->adjacent_plmns = NULL;
    }
    if (sdm_subscription->data_restoration_callback_uri) {
        ogs_free(sdm_subscription->data_restoration_callback_uri);
        sdm_subscription->data_restoration_callback_uri = NULL;
    }
    if (sdm_subscription->last_synchronization_time) {
        ogs_free(sdm_subscription->last_synchronization_time);
        sdm_subscription->last_synchronization_time = NULL;
    }
    if (sdm_subscription->expected_ue_behaviour_thresholds) {
        OpenAPI_list_for_each(sdm_subscription->expected_ue_behaviour_thresholds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_threshold_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sdm_subscription->expected_ue_behaviour_thresholds);
        sdm_subscription->expected_ue_behaviour_thresholds = NULL;
    }
    if (sdm_subscription->guami) {
        OpenAPI_guami_free(sdm_subscription->guami);
        sdm_subscription->guami = NULL;
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

    if (sdm_subscription->amf_service_name != OpenAPI_service_name_NULL) {
    if (cJSON_AddStringToObject(item, "amfServiceName", OpenAPI_service_name_ToString(sdm_subscription->amf_service_name)) == NULL) {
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

    if (sdm_subscription->adjacent_plmns) {
    cJSON *adjacent_plmnsList = cJSON_AddArrayToObject(item, "adjacentPlmns");
    if (adjacent_plmnsList == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [adjacent_plmns]");
        goto end;
    }
    OpenAPI_list_for_each(sdm_subscription->adjacent_plmns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [adjacent_plmns]");
            goto end;
        }
        cJSON_AddItemToArray(adjacent_plmnsList, itemLocal);
    }
    }

    if (sdm_subscription->is_disaster_roaming_ind) {
    if (cJSON_AddBoolToObject(item, "disasterRoamingInd", sdm_subscription->disaster_roaming_ind) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    if (sdm_subscription->data_restoration_callback_uri) {
    if (cJSON_AddStringToObject(item, "dataRestorationCallbackUri", sdm_subscription->data_restoration_callback_uri) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

    if (sdm_subscription->is_udr_restart_ind) {
    if (cJSON_AddBoolToObject(item, "udrRestartInd", sdm_subscription->udr_restart_ind) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [udr_restart_ind]");
        goto end;
    }
    }

    if (sdm_subscription->last_synchronization_time) {
    if (cJSON_AddStringToObject(item, "lastSynchronizationTime", sdm_subscription->last_synchronization_time) == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [last_synchronization_time]");
        goto end;
    }
    }

    if (sdm_subscription->expected_ue_behaviour_thresholds) {
    cJSON *expected_ue_behaviour_thresholds = cJSON_AddObjectToObject(item, "expectedUeBehaviourThresholds");
    if (expected_ue_behaviour_thresholds == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [expected_ue_behaviour_thresholds]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviour_thresholds;
    if (sdm_subscription->expected_ue_behaviour_thresholds) {
        OpenAPI_list_for_each(sdm_subscription->expected_ue_behaviour_thresholds, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [expected_ue_behaviour_thresholds]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [expected_ue_behaviour_thresholds]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_expected_ue_behaviour_threshold_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sdm_subscription->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(sdm_subscription->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sdm_subscription_convertToJSON() failed [guami]");
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
    OpenAPI_service_name_e amf_service_nameVariable = 0;
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
    cJSON *adjacent_plmns = NULL;
    OpenAPI_list_t *adjacent_plmnsList = NULL;
    cJSON *disaster_roaming_ind = NULL;
    cJSON *data_restoration_callback_uri = NULL;
    cJSON *udr_restart_ind = NULL;
    cJSON *last_synchronization_time = NULL;
    cJSON *expected_ue_behaviour_thresholds = NULL;
    OpenAPI_list_t *expected_ue_behaviour_thresholdsList = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
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
    if (!cJSON_IsString(amf_service_name)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [amf_service_name]");
        goto end;
    }
    amf_service_nameVariable = OpenAPI_service_name_FromString(amf_service_name->valuestring);
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

    adjacent_plmns = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "adjacentPlmns");
    if (adjacent_plmns) {
        cJSON *adjacent_plmns_local = NULL;
        if (!cJSON_IsArray(adjacent_plmns)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [adjacent_plmns]");
            goto end;
        }

        adjacent_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(adjacent_plmns_local, adjacent_plmns) {
            if (!cJSON_IsObject(adjacent_plmns_local)) {
                ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [adjacent_plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *adjacent_plmnsItem = OpenAPI_plmn_id_parseFromJSON(adjacent_plmns_local);
            if (!adjacent_plmnsItem) {
                ogs_error("No adjacent_plmnsItem");
                goto end;
            }
            OpenAPI_list_add(adjacent_plmnsList, adjacent_plmnsItem);
        }
    }

    disaster_roaming_ind = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "disasterRoamingInd");
    if (disaster_roaming_ind) {
    if (!cJSON_IsBool(disaster_roaming_ind)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [disaster_roaming_ind]");
        goto end;
    }
    }

    data_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "dataRestorationCallbackUri");
    if (data_restoration_callback_uri) {
    if (!cJSON_IsString(data_restoration_callback_uri) && !cJSON_IsNull(data_restoration_callback_uri)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

    udr_restart_ind = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "udrRestartInd");
    if (udr_restart_ind) {
    if (!cJSON_IsBool(udr_restart_ind)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [udr_restart_ind]");
        goto end;
    }
    }

    last_synchronization_time = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "lastSynchronizationTime");
    if (last_synchronization_time) {
    if (!cJSON_IsString(last_synchronization_time) && !cJSON_IsNull(last_synchronization_time)) {
        ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [last_synchronization_time]");
        goto end;
    }
    }

    expected_ue_behaviour_thresholds = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "expectedUeBehaviourThresholds");
    if (expected_ue_behaviour_thresholds) {
        cJSON *expected_ue_behaviour_thresholds_local_map = NULL;
        if (!cJSON_IsObject(expected_ue_behaviour_thresholds) && !cJSON_IsNull(expected_ue_behaviour_thresholds)) {
            ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [expected_ue_behaviour_thresholds]");
            goto end;
        }
        if (cJSON_IsObject(expected_ue_behaviour_thresholds)) {
            expected_ue_behaviour_thresholdsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(expected_ue_behaviour_thresholds_local_map, expected_ue_behaviour_thresholds) {
                cJSON *localMapObject = expected_ue_behaviour_thresholds_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_threshold_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sdm_subscription_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(expected_ue_behaviour_thresholdsList, localMapKeyPair);
            }
        }
    }

    guami = cJSON_GetObjectItemCaseSensitive(sdm_subscriptionJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    sdm_subscription_local_var = OpenAPI_sdm_subscription_create (
        ogs_strdup(nf_instance_id->valuestring),
        implicit_unsubscribe ? true : false,
        implicit_unsubscribe ? implicit_unsubscribe->valueint : 0,
        expires && !cJSON_IsNull(expires) ? ogs_strdup(expires->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        amf_service_name ? amf_service_nameVariable : 0,
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
        ue_con_smf_data_sub_filter ? ue_con_smf_data_sub_filter_local_nonprim : NULL,
        adjacent_plmns ? adjacent_plmnsList : NULL,
        disaster_roaming_ind ? true : false,
        disaster_roaming_ind ? disaster_roaming_ind->valueint : 0,
        data_restoration_callback_uri && !cJSON_IsNull(data_restoration_callback_uri) ? ogs_strdup(data_restoration_callback_uri->valuestring) : NULL,
        udr_restart_ind ? true : false,
        udr_restart_ind ? udr_restart_ind->valueint : 0,
        last_synchronization_time && !cJSON_IsNull(last_synchronization_time) ? ogs_strdup(last_synchronization_time->valuestring) : NULL,
        expected_ue_behaviour_thresholds ? expected_ue_behaviour_thresholdsList : NULL,
        guami ? guami_local_nonprim : NULL
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
    if (adjacent_plmnsList) {
        OpenAPI_list_for_each(adjacent_plmnsList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(adjacent_plmnsList);
        adjacent_plmnsList = NULL;
    }
    if (expected_ue_behaviour_thresholdsList) {
        OpenAPI_list_for_each(expected_ue_behaviour_thresholdsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_threshold_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviour_thresholdsList);
        expected_ue_behaviour_thresholdsList = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
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

