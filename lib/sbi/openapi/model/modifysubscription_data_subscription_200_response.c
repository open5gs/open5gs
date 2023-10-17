
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "modifysubscription_data_subscription_200_response.h"

OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_create(
    OpenAPI_list_t *report,
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
    char *supported_features
)
{
    OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response_local_var = ogs_malloc(sizeof(OpenAPI_modifysubscription_data_subscription_200_response_t));
    ogs_assert(modifysubscription_data_subscription_200_response_local_var);

    modifysubscription_data_subscription_200_response_local_var->report = report;
    modifysubscription_data_subscription_200_response_local_var->ue_id = ue_id;
    modifysubscription_data_subscription_200_response_local_var->callback_reference = callback_reference;
    modifysubscription_data_subscription_200_response_local_var->original_callback_reference = original_callback_reference;
    modifysubscription_data_subscription_200_response_local_var->monitored_resource_uris = monitored_resource_uris;
    modifysubscription_data_subscription_200_response_local_var->expiry = expiry;
    modifysubscription_data_subscription_200_response_local_var->sdm_subscription = sdm_subscription;
    modifysubscription_data_subscription_200_response_local_var->hss_subscription_info = hss_subscription_info;
    modifysubscription_data_subscription_200_response_local_var->subscription_id = subscription_id;
    modifysubscription_data_subscription_200_response_local_var->is_unique_subscription = is_unique_subscription;
    modifysubscription_data_subscription_200_response_local_var->unique_subscription = unique_subscription;
    modifysubscription_data_subscription_200_response_local_var->supported_features = supported_features;

    return modifysubscription_data_subscription_200_response_local_var;
}

void OpenAPI_modifysubscription_data_subscription_200_response_free(OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == modifysubscription_data_subscription_200_response) {
        return;
    }
    if (modifysubscription_data_subscription_200_response->report) {
        OpenAPI_list_for_each(modifysubscription_data_subscription_200_response->report, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(modifysubscription_data_subscription_200_response->report);
        modifysubscription_data_subscription_200_response->report = NULL;
    }
    if (modifysubscription_data_subscription_200_response->ue_id) {
        ogs_free(modifysubscription_data_subscription_200_response->ue_id);
        modifysubscription_data_subscription_200_response->ue_id = NULL;
    }
    if (modifysubscription_data_subscription_200_response->callback_reference) {
        ogs_free(modifysubscription_data_subscription_200_response->callback_reference);
        modifysubscription_data_subscription_200_response->callback_reference = NULL;
    }
    if (modifysubscription_data_subscription_200_response->original_callback_reference) {
        ogs_free(modifysubscription_data_subscription_200_response->original_callback_reference);
        modifysubscription_data_subscription_200_response->original_callback_reference = NULL;
    }
    if (modifysubscription_data_subscription_200_response->monitored_resource_uris) {
        OpenAPI_list_for_each(modifysubscription_data_subscription_200_response->monitored_resource_uris, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(modifysubscription_data_subscription_200_response->monitored_resource_uris);
        modifysubscription_data_subscription_200_response->monitored_resource_uris = NULL;
    }
    if (modifysubscription_data_subscription_200_response->expiry) {
        ogs_free(modifysubscription_data_subscription_200_response->expiry);
        modifysubscription_data_subscription_200_response->expiry = NULL;
    }
    if (modifysubscription_data_subscription_200_response->sdm_subscription) {
        OpenAPI_sdm_subscription_1_free(modifysubscription_data_subscription_200_response->sdm_subscription);
        modifysubscription_data_subscription_200_response->sdm_subscription = NULL;
    }
    if (modifysubscription_data_subscription_200_response->hss_subscription_info) {
        OpenAPI_hss_subscription_info_free(modifysubscription_data_subscription_200_response->hss_subscription_info);
        modifysubscription_data_subscription_200_response->hss_subscription_info = NULL;
    }
    if (modifysubscription_data_subscription_200_response->subscription_id) {
        ogs_free(modifysubscription_data_subscription_200_response->subscription_id);
        modifysubscription_data_subscription_200_response->subscription_id = NULL;
    }
    if (modifysubscription_data_subscription_200_response->supported_features) {
        ogs_free(modifysubscription_data_subscription_200_response->supported_features);
        modifysubscription_data_subscription_200_response->supported_features = NULL;
    }
    ogs_free(modifysubscription_data_subscription_200_response);
}

cJSON *OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON(OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (modifysubscription_data_subscription_200_response == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [ModifysubscriptionDataSubscription_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!modifysubscription_data_subscription_200_response->report) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [report]");
        return NULL;
    }
    cJSON *reportList = cJSON_AddArrayToObject(item, "report");
    if (reportList == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [report]");
        goto end;
    }
    OpenAPI_list_for_each(modifysubscription_data_subscription_200_response->report, node) {
        cJSON *itemLocal = OpenAPI_report_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [report]");
            goto end;
        }
        cJSON_AddItemToArray(reportList, itemLocal);
    }

    if (modifysubscription_data_subscription_200_response->ue_id) {
    if (cJSON_AddStringToObject(item, "ueId", modifysubscription_data_subscription_200_response->ue_id) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [ue_id]");
        goto end;
    }
    }

    if (!modifysubscription_data_subscription_200_response->callback_reference) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [callback_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", modifysubscription_data_subscription_200_response->callback_reference) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (modifysubscription_data_subscription_200_response->original_callback_reference) {
    if (cJSON_AddStringToObject(item, "originalCallbackReference", modifysubscription_data_subscription_200_response->original_callback_reference) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    if (!modifysubscription_data_subscription_200_response->monitored_resource_uris) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [monitored_resource_uris]");
        return NULL;
    }
    cJSON *monitored_resource_urisList = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_urisList == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_for_each(modifysubscription_data_subscription_200_response->monitored_resource_uris, node) {
        if (cJSON_AddStringToObject(monitored_resource_urisList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [monitored_resource_uris]");
            goto end;
        }
    }

    if (modifysubscription_data_subscription_200_response->expiry) {
    if (cJSON_AddStringToObject(item, "expiry", modifysubscription_data_subscription_200_response->expiry) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [expiry]");
        goto end;
    }
    }

    if (modifysubscription_data_subscription_200_response->sdm_subscription) {
    cJSON *sdm_subscription_local_JSON = OpenAPI_sdm_subscription_1_convertToJSON(modifysubscription_data_subscription_200_response->sdm_subscription);
    if (sdm_subscription_local_JSON == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sdmSubscription", sdm_subscription_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [sdm_subscription]");
        goto end;
    }
    }

    if (modifysubscription_data_subscription_200_response->hss_subscription_info) {
    cJSON *hss_subscription_info_local_JSON = OpenAPI_hss_subscription_info_convertToJSON(modifysubscription_data_subscription_200_response->hss_subscription_info);
    if (hss_subscription_info_local_JSON == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "hssSubscriptionInfo", hss_subscription_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [hss_subscription_info]");
        goto end;
    }
    }

    if (modifysubscription_data_subscription_200_response->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", modifysubscription_data_subscription_200_response->subscription_id) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (modifysubscription_data_subscription_200_response->is_unique_subscription) {
    if (cJSON_AddBoolToObject(item, "uniqueSubscription", modifysubscription_data_subscription_200_response->unique_subscription) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [unique_subscription]");
        goto end;
    }
    }

    if (modifysubscription_data_subscription_200_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", modifysubscription_data_subscription_200_response->supported_features) == NULL) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON(cJSON *modifysubscription_data_subscription_200_responseJSON)
{
    OpenAPI_modifysubscription_data_subscription_200_response_t *modifysubscription_data_subscription_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *report = NULL;
    OpenAPI_list_t *reportList = NULL;
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
    report = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "report");
    if (!report) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [report]");
        goto end;
    }
        cJSON *report_local = NULL;
        if (!cJSON_IsArray(report)) {
            ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [report]");
            goto end;
        }

        reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(report_local, report) {
            if (!cJSON_IsObject(report_local)) {
                ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [report]");
                goto end;
            }
            OpenAPI_report_item_t *reportItem = OpenAPI_report_item_parseFromJSON(report_local);
            if (!reportItem) {
                ogs_error("No reportItem");
                goto end;
            }
            OpenAPI_list_add(reportList, reportItem);
        }

    ue_id = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "ueId");
    if (ue_id) {
    if (!cJSON_IsString(ue_id) && !cJSON_IsNull(ue_id)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [ue_id]");
        goto end;
    }
    }

    callback_reference = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [callback_reference]");
        goto end;
    }
    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    original_callback_reference = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "originalCallbackReference");
    if (original_callback_reference) {
    if (!cJSON_IsString(original_callback_reference) && !cJSON_IsNull(original_callback_reference)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [original_callback_reference]");
        goto end;
    }
    }

    monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "monitoredResourceUris");
    if (!monitored_resource_uris) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
        cJSON *monitored_resource_uris_local = NULL;
        if (!cJSON_IsArray(monitored_resource_uris)) {
            ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [monitored_resource_uris]");
            goto end;
        }

        monitored_resource_urisList = OpenAPI_list_create();

        cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(monitored_resource_uris_local)) {
                ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [monitored_resource_uris]");
                goto end;
            }
            OpenAPI_list_add(monitored_resource_urisList, ogs_strdup(monitored_resource_uris_local->valuestring));
        }

    expiry = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "expiry");
    if (expiry) {
    if (!cJSON_IsString(expiry) && !cJSON_IsNull(expiry)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [expiry]");
        goto end;
    }
    }

    sdm_subscription = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "sdmSubscription");
    if (sdm_subscription) {
    sdm_subscription_local_nonprim = OpenAPI_sdm_subscription_1_parseFromJSON(sdm_subscription);
    if (!sdm_subscription_local_nonprim) {
        ogs_error("OpenAPI_sdm_subscription_1_parseFromJSON failed [sdm_subscription]");
        goto end;
    }
    }

    hss_subscription_info = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "hssSubscriptionInfo");
    if (hss_subscription_info) {
    hss_subscription_info_local_nonprim = OpenAPI_hss_subscription_info_parseFromJSON(hss_subscription_info);
    if (!hss_subscription_info_local_nonprim) {
        ogs_error("OpenAPI_hss_subscription_info_parseFromJSON failed [hss_subscription_info]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    unique_subscription = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "uniqueSubscription");
    if (unique_subscription) {
    if (!cJSON_IsBool(unique_subscription)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [unique_subscription]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(modifysubscription_data_subscription_200_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    modifysubscription_data_subscription_200_response_local_var = OpenAPI_modifysubscription_data_subscription_200_response_create (
        reportList,
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
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return modifysubscription_data_subscription_200_response_local_var;
end:
    if (reportList) {
        OpenAPI_list_for_each(reportList, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(reportList);
        reportList = NULL;
    }
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
    return NULL;
}

OpenAPI_modifysubscription_data_subscription_200_response_t *OpenAPI_modifysubscription_data_subscription_200_response_copy(OpenAPI_modifysubscription_data_subscription_200_response_t *dst, OpenAPI_modifysubscription_data_subscription_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_modifysubscription_data_subscription_200_response_convertToJSON() failed");
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

    OpenAPI_modifysubscription_data_subscription_200_response_free(dst);
    dst = OpenAPI_modifysubscription_data_subscription_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

