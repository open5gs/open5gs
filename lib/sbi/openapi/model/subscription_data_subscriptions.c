
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "subscription_data_subscriptions.h"

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_create(
    char *ue_id,
    char *callback_reference,
    char *original_callback_reference,
    OpenAPI_list_t *monitored_resource_uris,
    char *expiry,
    OpenAPI_sdm_subscription_1_t *sdm_subscription,
    OpenAPI_hss_subscription_info_t *hss_subscription_info,
    char *subscription_id,
    bool is_unique_subscription,
    int unique_subscription,
    char *supported_features,
    bool is_immediate_report,
    int immediate_report,
    OpenAPI_immediate_report_1_t *report,
    OpenAPI_list_t *additional_data_refs,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions_local_var = ogs_malloc(sizeof(OpenAPI_subscription_data_subscriptions_t));
    ogs_assert(subscription_data_subscriptions_local_var);

    subscription_data_subscriptions_local_var->ue_id = ue_id;
    subscription_data_subscriptions_local_var->callback_reference = callback_reference;
    subscription_data_subscriptions_local_var->original_callback_reference = original_callback_reference;
    subscription_data_subscriptions_local_var->monitored_resource_uris = monitored_resource_uris;
    subscription_data_subscriptions_local_var->expiry = expiry;
    subscription_data_subscriptions_local_var->sdm_subscription = sdm_subscription;
    subscription_data_subscriptions_local_var->hss_subscription_info = hss_subscription_info;
    subscription_data_subscriptions_local_var->subscription_id = subscription_id;
    subscription_data_subscriptions_local_var->is_unique_subscription = is_unique_subscription;
    subscription_data_subscriptions_local_var->unique_subscription = unique_subscription;
    subscription_data_subscriptions_local_var->supported_features = supported_features;
    subscription_data_subscriptions_local_var->is_immediate_report = is_immediate_report;
    subscription_data_subscriptions_local_var->immediate_report = immediate_report;
    subscription_data_subscriptions_local_var->report = report;
    subscription_data_subscriptions_local_var->additional_data_refs = additional_data_refs;
    subscription_data_subscriptions_local_var->reset_ids = reset_ids;

    return subscription_data_subscriptions_local_var;
}

void OpenAPI_subscription_data_subscriptions_free(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == subscription_data_subscriptions) {
        return;
    }
    if (subscription_data_subscriptions->ue_id) {
        ogs_free(subscription_data_subscriptions->ue_id);
        subscription_data_subscriptions->ue_id = NULL;
    }
    if (subscription_data_subscriptions->callback_reference) {
        ogs_free(subscription_data_subscriptions->callback_reference);
        subscription_data_subscriptions->callback_reference = NULL;
    }
    if (subscription_data_subscriptions->original_callback_reference) {
        ogs_free(subscription_data_subscriptions->original_callback_reference);
        subscription_data_subscriptions->original_callback_reference = NULL;
    }
    if (subscription_data_subscriptions->monitored_resource_uris) {
        OpenAPI_list_for_each(subscription_data_subscriptions->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscription_data_subscriptions->monitored_resource_uris);
        subscription_data_subscriptions->monitored_resource_uris = NULL;
    }
    if (subscription_data_subscriptions->expiry) {
        ogs_free(subscription_data_subscriptions->expiry);
        subscription_data_subscriptions->expiry = NULL;
    }
    if (subscription_data_subscriptions->sdm_subscription) {
        OpenAPI_sdm_subscription_1_free(subscription_data_subscriptions->sdm_subscription);
        subscription_data_subscriptions->sdm_subscription = NULL;
    }
    if (subscription_data_subscriptions->hss_subscription_info) {
        OpenAPI_hss_subscription_info_free(subscription_data_subscriptions->hss_subscription_info);
        subscription_data_subscriptions->hss_subscription_info = NULL;
    }
    if (subscription_data_subscriptions->subscription_id) {
        ogs_free(subscription_data_subscriptions->subscription_id);
        subscription_data_subscriptions->subscription_id = NULL;
    }
    if (subscription_data_subscriptions->supported_features) {
        ogs_free(subscription_data_subscriptions->supported_features);
        subscription_data_subscriptions->supported_features = NULL;
    }
    if (subscription_data_subscriptions->report) {
        OpenAPI_immediate_report_1_free(subscription_data_subscriptions->report);
        subscription_data_subscriptions->report = NULL;
    }
    if (subscription_data_subscriptions->additional_data_refs) {
        OpenAPI_list_for_each(subscription_data_subscriptions->additional_data_refs, node) {
            OpenAPI_additional_data_ref_free(node->data);
        }
        OpenAPI_list_free(subscription_data_subscriptions->additional_data_refs);
        subscription_data_subscriptions->additional_data_refs = NULL;
    }
    if (subscription_data_subscriptions->reset_ids) {
        OpenAPI_list_for_each(subscription_data_subscriptions->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(subscription_data_subscriptions->reset_ids);
        subscription_data_subscriptions->reset_ids = NULL;
    }
    ogs_free(subscription_data_subscriptions);
}

cJSON *OpenAPI_subscription_data_subscriptions_convertToJSON(OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (subscription_data_subscriptions == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [SubscriptionDataSubscriptions]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (subscription_data_subscriptions->ue_id) {
    if (cJSON_AddStringToObject(item, "ueId", subscription_data_subscriptions->ue_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [ue_id]");
        goto end;
    }
    }

    if (!subscription_data_subscriptions->callback_reference) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [callback_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", subscription_data_subscriptions->callback_reference) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (subscription_data_subscriptions->original_callback_reference) {
    if (cJSON_AddStringToObject(item, "originalCallbackReference", subscription_data_subscriptions->original_callback_reference) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    if (!subscription_data_subscriptions->monitored_resource_uris) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data_subscriptions->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (subscription_data_subscriptions->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", subscription_data_subscriptions->expiry) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->sdm_subscription) {
    cJSON *sdm_subscription_local_JSON = OpenAPI_sdm_subscription_1_convertToJSON(subscription_data_subscriptions->sdm_subscription);
    if (sdm_subscription_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sdmSubscription", sdm_subscription_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->hss_subscription_info) {
    cJSON *hss_subscription_info_local_JSON = OpenAPI_hss_subscription_info_convertToJSON(subscription_data_subscriptions->hss_subscription_info);
    if (hss_subscription_info_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hssSubscriptionInfo", hss_subscription_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", subscription_data_subscriptions->subscription_id) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->is_unique_subscription) {
    if (cJSON_AddBoolToObject(item, "uniqueSubscription", subscription_data_subscriptions->unique_subscription) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [unique_subscription]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", subscription_data_subscriptions->supported_features) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->is_immediate_report) {
    if (cJSON_AddBoolToObject(item, "immediateReport", subscription_data_subscriptions->immediate_report) == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [immediate_report]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->report) {
    cJSON *report_local_JSON = OpenAPI_immediate_report_1_convertToJSON(subscription_data_subscriptions->report);
    if (report_local_JSON == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [report]");
        goto end;
    }
    cJSON_AddItemToObject(item, "report", report_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [report]");
        goto end;
    }
    }

    if (subscription_data_subscriptions->additional_data_refs) {
    cJSON *additional_data_refsList = cJSON_AddArrayToObject(item, "additionalDataRefs");
    if (additional_data_refsList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [additional_data_refs]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data_subscriptions->additional_data_refs, node) {
        cJSON *itemLocal = OpenAPI_additional_data_ref_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [additional_data_refs]");
            goto end;
        }
        cJSON_AddItemToArray(additional_data_refsList, itemLocal);
    }
    }

    if (subscription_data_subscriptions->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(subscription_data_subscriptions->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_parseFromJSON(cJSON *subscription_data_subscriptionsJSON)
{
    OpenAPI_subscription_data_subscriptions_t *subscription_data_subscriptions_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_id = NULL;
    cJSON *callback_reference = NULL;
    cJSON *original_callback_reference = NULL;
    cJSON *monitored_resource_uris = NULL;
    OpenAPI_list_t *monitored_resource_urisList = NULL;
    cJSON *expiry = NULL;
    cJSON *sdm_subscription = NULL;
    OpenAPI_sdm_subscription_1_t *sdm_subscription_local_nonprim = NULL;
    cJSON *hss_subscription_info = NULL;
    OpenAPI_hss_subscription_info_t *hss_subscription_info_local_nonprim = NULL;
    cJSON *subscription_id = NULL;
    cJSON *unique_subscription = NULL;
    cJSON *supported_features = NULL;
    cJSON *immediate_report = NULL;
    cJSON *report = NULL;
    OpenAPI_immediate_report_1_t *report_local_nonprim = NULL;
    cJSON *additional_data_refs = NULL;
    OpenAPI_list_t *additional_data_refsList = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    ue_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "ueId");
    if (ue_id) {
    if (!cJSON_IsString(ue_id) && !cJSON_IsNull(ue_id)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    callback_reference = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [callback_reference]");
        goto end;
    }
    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    original_callback_reference = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "originalCallbackReference");
    if (original_callback_reference) {
    if (!cJSON_IsString(original_callback_reference) && !cJSON_IsNull(original_callback_reference)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    expiry = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    sdm_subscription = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "sdmSubscription");
    if (sdm_subscription) {
    sdm_subscription_local_nonprim = OpenAPI_sdm_subscription_1_parseFromJSON(sdm_subscription);
    if (!sdm_subscription_local_nonprim) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON failed [sdm_subscription]");
        goto end;
    }
    }

    hss_subscription_info = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "hssSubscriptionInfo");
    if (hss_subscription_info) {
    hss_subscription_info_local_nonprim = OpenAPI_hss_subscription_info_parseFromJSON(hss_subscription_info);
    if (!hss_subscription_info_local_nonprim) {
        ogs_error("OpenAPI_hss_subscription_info_parseFromJSON failed [hss_subscription_info]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    unique_subscription = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "uniqueSubscription");
    if (unique_subscription) {
    if (!cJSON_IsBool(unique_subscription)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [unique_subscription]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    immediate_report = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "immediateReport");
    if (immediate_report) {
    if (!cJSON_IsBool(immediate_report)) {
        ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [immediate_report]");
        goto end;
    }
    }

    report = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "report");
    if (report) {
    report_local_nonprim = OpenAPI_immediate_report_1_parseFromJSON(report);
    if (!report_local_nonprim) {
        ogs_error("OpenAPI_immediate_report_1_parseFromJSON failed [report]");
        goto end;
    }
    }

    additional_data_refs = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "additionalDataRefs");
    if (additional_data_refs) {
        cJSON *additional_data_refs_local = NULL;
        if (!cJSON_IsArray(additional_data_refs)) {
            ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [additional_data_refs]");
            goto end;
        }

        additional_data_refsList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_data_refs_local, additional_data_refs) {
            if (!cJSON_IsObject(additional_data_refs_local)) {
                ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [additional_data_refs]");
                goto end;
            }
            OpenAPI_additional_data_ref_t *additional_data_refsItem = OpenAPI_additional_data_ref_parseFromJSON(additional_data_refs_local);
            if (!additional_data_refsItem) {
                ogs_error("No additional_data_refsItem");
                goto end;
            }
            OpenAPI_list_add(additional_data_refsList, additional_data_refsItem);
        }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(subscription_data_subscriptionsJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_subscription_data_subscriptions_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    subscription_data_subscriptions_local_var = OpenAPI_subscription_data_subscriptions_create (
        ue_id && !cJSON_IsNull(ue_id) ? ogs_strdup(ue_id->valuestring) : NULL,
        ogs_strdup(callback_reference->valuestring),
        original_callback_reference && !cJSON_IsNull(original_callback_reference) ? ogs_strdup(original_callback_reference->valuestring) : NULL,
        monitored_resource_urisList,
        expiry && !cJSON_IsNull(expiry) ? ogs_strdup(expiry->valuestring) : NULL,
        sdm_subscription ? sdm_subscription_local_nonprim : NULL,
        hss_subscription_info ? hss_subscription_info_local_nonprim : NULL,
        subscription_id && !cJSON_IsNull(subscription_id) ? ogs_strdup(subscription_id->valuestring) : NULL,
        unique_subscription ? true : false,
        unique_subscription ? unique_subscription->valueint : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        immediate_report ? true : false,
        immediate_report ? immediate_report->valueint : 0,
        report ? report_local_nonprim : NULL,
        additional_data_refs ? additional_data_refsList : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return subscription_data_subscriptions_local_var;
end:
    if (monitored_resource_urisList) {
        OpenAPI_list_for_each(monitored_resource_urisList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitored_resource_urisList);
        monitored_resource_urisList = NULL;
    }
    if (sdm_subscription_local_nonprim) {
        OpenAPI_sdm_subscription_1_free(sdm_subscription_local_nonprim);
        sdm_subscription_local_nonprim = NULL;
    }
    if (hss_subscription_info_local_nonprim) {
        OpenAPI_hss_subscription_info_free(hss_subscription_info_local_nonprim);
        hss_subscription_info_local_nonprim = NULL;
    }
    if (report_local_nonprim) {
        OpenAPI_immediate_report_1_free(report_local_nonprim);
        report_local_nonprim = NULL;
    }
    if (additional_data_refsList) {
        OpenAPI_list_for_each(additional_data_refsList, node) {
            OpenAPI_additional_data_ref_free(node->data);
        }
        OpenAPI_list_free(additional_data_refsList);
        additional_data_refsList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_subscription_data_subscriptions_t *OpenAPI_subscription_data_subscriptions_copy(OpenAPI_subscription_data_subscriptions_t *dst, OpenAPI_subscription_data_subscriptions_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_subscription_data_subscriptions_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_subscription_data_subscriptions_convertToJSON() failed");
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

    OpenAPI_subscription_data_subscriptions_free(dst);
    dst = OpenAPI_subscription_data_subscriptions_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

