
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ee_subscription.h"

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_create(
    char *callback_reference,
    OpenAPI_list_t* monitoring_configurations,
    OpenAPI_reporting_options_t *reporting_options,
    char *supported_features,
    char *subscription_id,
    OpenAPI_context_info_t *context_info,
    bool is_epc_applied_ind,
    int epc_applied_ind,
    char *scef_diam_host,
    char *scef_diam_realm,
    char *notify_correlation_id,
    char *second_callback_ref,
    char *gpsi,
    OpenAPI_list_t *exclude_gpsi_list,
    OpenAPI_list_t *include_gpsi_list,
    char *data_restoration_callback_uri
)
{
    OpenAPI_ee_subscription_t *ee_subscription_local_var = ogs_malloc(sizeof(OpenAPI_ee_subscription_t));
    ogs_assert(ee_subscription_local_var);

    ee_subscription_local_var->callback_reference = callback_reference;
    ee_subscription_local_var->monitoring_configurations = monitoring_configurations;
    ee_subscription_local_var->reporting_options = reporting_options;
    ee_subscription_local_var->supported_features = supported_features;
    ee_subscription_local_var->subscription_id = subscription_id;
    ee_subscription_local_var->context_info = context_info;
    ee_subscription_local_var->is_epc_applied_ind = is_epc_applied_ind;
    ee_subscription_local_var->epc_applied_ind = epc_applied_ind;
    ee_subscription_local_var->scef_diam_host = scef_diam_host;
    ee_subscription_local_var->scef_diam_realm = scef_diam_realm;
    ee_subscription_local_var->notify_correlation_id = notify_correlation_id;
    ee_subscription_local_var->second_callback_ref = second_callback_ref;
    ee_subscription_local_var->gpsi = gpsi;
    ee_subscription_local_var->exclude_gpsi_list = exclude_gpsi_list;
    ee_subscription_local_var->include_gpsi_list = include_gpsi_list;
    ee_subscription_local_var->data_restoration_callback_uri = data_restoration_callback_uri;

    return ee_subscription_local_var;
}

void OpenAPI_ee_subscription_free(OpenAPI_ee_subscription_t *ee_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ee_subscription) {
        return;
    }
    if (ee_subscription->callback_reference) {
        ogs_free(ee_subscription->callback_reference);
        ee_subscription->callback_reference = NULL;
    }
    if (ee_subscription->monitoring_configurations) {
        OpenAPI_list_for_each(ee_subscription->monitoring_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_monitoring_configuration_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(ee_subscription->monitoring_configurations);
        ee_subscription->monitoring_configurations = NULL;
    }
    if (ee_subscription->reporting_options) {
        OpenAPI_reporting_options_free(ee_subscription->reporting_options);
        ee_subscription->reporting_options = NULL;
    }
    if (ee_subscription->supported_features) {
        ogs_free(ee_subscription->supported_features);
        ee_subscription->supported_features = NULL;
    }
    if (ee_subscription->subscription_id) {
        ogs_free(ee_subscription->subscription_id);
        ee_subscription->subscription_id = NULL;
    }
    if (ee_subscription->context_info) {
        OpenAPI_context_info_free(ee_subscription->context_info);
        ee_subscription->context_info = NULL;
    }
    if (ee_subscription->scef_diam_host) {
        ogs_free(ee_subscription->scef_diam_host);
        ee_subscription->scef_diam_host = NULL;
    }
    if (ee_subscription->scef_diam_realm) {
        ogs_free(ee_subscription->scef_diam_realm);
        ee_subscription->scef_diam_realm = NULL;
    }
    if (ee_subscription->notify_correlation_id) {
        ogs_free(ee_subscription->notify_correlation_id);
        ee_subscription->notify_correlation_id = NULL;
    }
    if (ee_subscription->second_callback_ref) {
        ogs_free(ee_subscription->second_callback_ref);
        ee_subscription->second_callback_ref = NULL;
    }
    if (ee_subscription->gpsi) {
        ogs_free(ee_subscription->gpsi);
        ee_subscription->gpsi = NULL;
    }
    if (ee_subscription->exclude_gpsi_list) {
        OpenAPI_list_for_each(ee_subscription->exclude_gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ee_subscription->exclude_gpsi_list);
        ee_subscription->exclude_gpsi_list = NULL;
    }
    if (ee_subscription->include_gpsi_list) {
        OpenAPI_list_for_each(ee_subscription->include_gpsi_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ee_subscription->include_gpsi_list);
        ee_subscription->include_gpsi_list = NULL;
    }
    if (ee_subscription->data_restoration_callback_uri) {
        ogs_free(ee_subscription->data_restoration_callback_uri);
        ee_subscription->data_restoration_callback_uri = NULL;
    }
    ogs_free(ee_subscription);
}

cJSON *OpenAPI_ee_subscription_convertToJSON(OpenAPI_ee_subscription_t *ee_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ee_subscription == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [EeSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ee_subscription->callback_reference) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [callback_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", ee_subscription->callback_reference) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (!ee_subscription->monitoring_configurations) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
        return NULL;
    }
    cJSON *monitoring_configurations = cJSON_AddObjectToObject(item, "monitoringConfigurations");
    if (monitoring_configurations == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
        goto end;
    }
    cJSON *localMapObject = monitoring_configurations;
    if (ee_subscription->monitoring_configurations) {
        OpenAPI_list_for_each(ee_subscription->monitoring_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_monitoring_configuration_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }

    if (ee_subscription->reporting_options) {
    cJSON *reporting_options_local_JSON = OpenAPI_reporting_options_convertToJSON(ee_subscription->reporting_options);
    if (reporting_options_local_JSON == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [reporting_options]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reportingOptions", reporting_options_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [reporting_options]");
        goto end;
    }
    }

    if (ee_subscription->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ee_subscription->supported_features) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (ee_subscription->subscription_id) {
    if (cJSON_AddStringToObject(item, "subscriptionId", ee_subscription->subscription_id) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [subscription_id]");
        goto end;
    }
    }

    if (ee_subscription->context_info) {
    cJSON *context_info_local_JSON = OpenAPI_context_info_convertToJSON(ee_subscription->context_info);
    if (context_info_local_JSON == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [context_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "contextInfo", context_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [context_info]");
        goto end;
    }
    }

    if (ee_subscription->is_epc_applied_ind) {
    if (cJSON_AddBoolToObject(item, "epcAppliedInd", ee_subscription->epc_applied_ind) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [epc_applied_ind]");
        goto end;
    }
    }

    if (ee_subscription->scef_diam_host) {
    if (cJSON_AddStringToObject(item, "scefDiamHost", ee_subscription->scef_diam_host) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [scef_diam_host]");
        goto end;
    }
    }

    if (ee_subscription->scef_diam_realm) {
    if (cJSON_AddStringToObject(item, "scefDiamRealm", ee_subscription->scef_diam_realm) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [scef_diam_realm]");
        goto end;
    }
    }

    if (ee_subscription->notify_correlation_id) {
    if (cJSON_AddStringToObject(item, "notifyCorrelationId", ee_subscription->notify_correlation_id) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [notify_correlation_id]");
        goto end;
    }
    }

    if (ee_subscription->second_callback_ref) {
    if (cJSON_AddStringToObject(item, "secondCallbackRef", ee_subscription->second_callback_ref) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [second_callback_ref]");
        goto end;
    }
    }

    if (ee_subscription->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", ee_subscription->gpsi) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (ee_subscription->exclude_gpsi_list) {
    cJSON *exclude_gpsi_listList = cJSON_AddArrayToObject(item, "excludeGpsiList");
    if (exclude_gpsi_listList == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [exclude_gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ee_subscription->exclude_gpsi_list, node) {
        if (cJSON_AddStringToObject(exclude_gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [exclude_gpsi_list]");
            goto end;
        }
    }
    }

    if (ee_subscription->include_gpsi_list) {
    cJSON *include_gpsi_listList = cJSON_AddArrayToObject(item, "includeGpsiList");
    if (include_gpsi_listList == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [include_gpsi_list]");
        goto end;
    }
    OpenAPI_list_for_each(ee_subscription->include_gpsi_list, node) {
        if (cJSON_AddStringToObject(include_gpsi_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [include_gpsi_list]");
            goto end;
        }
    }
    }

    if (ee_subscription->data_restoration_callback_uri) {
    if (cJSON_AddStringToObject(item, "dataRestorationCallbackUri", ee_subscription->data_restoration_callback_uri) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_parseFromJSON(cJSON *ee_subscriptionJSON)
{
    OpenAPI_ee_subscription_t *ee_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *callback_reference = NULL;
    cJSON *monitoring_configurations = NULL;
    OpenAPI_list_t *monitoring_configurationsList = NULL;
    cJSON *reporting_options = NULL;
    OpenAPI_reporting_options_t *reporting_options_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *subscription_id = NULL;
    cJSON *context_info = NULL;
    OpenAPI_context_info_t *context_info_local_nonprim = NULL;
    cJSON *epc_applied_ind = NULL;
    cJSON *scef_diam_host = NULL;
    cJSON *scef_diam_realm = NULL;
    cJSON *notify_correlation_id = NULL;
    cJSON *second_callback_ref = NULL;
    cJSON *gpsi = NULL;
    cJSON *exclude_gpsi_list = NULL;
    OpenAPI_list_t *exclude_gpsi_listList = NULL;
    cJSON *include_gpsi_list = NULL;
    OpenAPI_list_t *include_gpsi_listList = NULL;
    cJSON *data_restoration_callback_uri = NULL;
    callback_reference = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }
    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    monitoring_configurations = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "monitoringConfigurations");
    if (!monitoring_configurations) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [monitoring_configurations]");
        goto end;
    }
        cJSON *monitoring_configurations_local_map = NULL;
        if (!cJSON_IsObject(monitoring_configurations) && !cJSON_IsNull(monitoring_configurations)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [monitoring_configurations]");
            goto end;
        }
        if (cJSON_IsObject(monitoring_configurations)) {
            monitoring_configurationsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(monitoring_configurations_local_map, monitoring_configurations) {
                cJSON *localMapObject = monitoring_configurations_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_monitoring_configuration_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(monitoring_configurationsList, localMapKeyPair);
            }
        }

    reporting_options = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "reportingOptions");
    if (reporting_options) {
    reporting_options_local_nonprim = OpenAPI_reporting_options_parseFromJSON(reporting_options);
    if (!reporting_options_local_nonprim) {
        ogs_error("OpenAPI_reporting_options_parseFromJSON failed [reporting_options]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    subscription_id = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "subscriptionId");
    if (subscription_id) {
    if (!cJSON_IsString(subscription_id) && !cJSON_IsNull(subscription_id)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [subscription_id]");
        goto end;
    }
    }

    context_info = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "contextInfo");
    if (context_info) {
    context_info_local_nonprim = OpenAPI_context_info_parseFromJSON(context_info);
    if (!context_info_local_nonprim) {
        ogs_error("OpenAPI_context_info_parseFromJSON failed [context_info]");
        goto end;
    }
    }

    epc_applied_ind = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "epcAppliedInd");
    if (epc_applied_ind) {
    if (!cJSON_IsBool(epc_applied_ind)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [epc_applied_ind]");
        goto end;
    }
    }

    scef_diam_host = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "scefDiamHost");
    if (scef_diam_host) {
    if (!cJSON_IsString(scef_diam_host) && !cJSON_IsNull(scef_diam_host)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [scef_diam_host]");
        goto end;
    }
    }

    scef_diam_realm = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "scefDiamRealm");
    if (scef_diam_realm) {
    if (!cJSON_IsString(scef_diam_realm) && !cJSON_IsNull(scef_diam_realm)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [scef_diam_realm]");
        goto end;
    }
    }

    notify_correlation_id = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "notifyCorrelationId");
    if (notify_correlation_id) {
    if (!cJSON_IsString(notify_correlation_id) && !cJSON_IsNull(notify_correlation_id)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [notify_correlation_id]");
        goto end;
    }
    }

    second_callback_ref = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "secondCallbackRef");
    if (second_callback_ref) {
    if (!cJSON_IsString(second_callback_ref) && !cJSON_IsNull(second_callback_ref)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [second_callback_ref]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    exclude_gpsi_list = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "excludeGpsiList");
    if (exclude_gpsi_list) {
        cJSON *exclude_gpsi_list_local = NULL;
        if (!cJSON_IsArray(exclude_gpsi_list)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [exclude_gpsi_list]");
            goto end;
        }

        exclude_gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(exclude_gpsi_list_local, exclude_gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(exclude_gpsi_list_local)) {
                ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [exclude_gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(exclude_gpsi_listList, ogs_strdup(exclude_gpsi_list_local->valuestring));
        }
    }

    include_gpsi_list = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "includeGpsiList");
    if (include_gpsi_list) {
        cJSON *include_gpsi_list_local = NULL;
        if (!cJSON_IsArray(include_gpsi_list)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [include_gpsi_list]");
            goto end;
        }

        include_gpsi_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(include_gpsi_list_local, include_gpsi_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(include_gpsi_list_local)) {
                ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [include_gpsi_list]");
                goto end;
            }
            OpenAPI_list_add(include_gpsi_listList, ogs_strdup(include_gpsi_list_local->valuestring));
        }
    }

    data_restoration_callback_uri = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "dataRestorationCallbackUri");
    if (data_restoration_callback_uri) {
    if (!cJSON_IsString(data_restoration_callback_uri) && !cJSON_IsNull(data_restoration_callback_uri)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [data_restoration_callback_uri]");
        goto end;
    }
    }

    ee_subscription_local_var = OpenAPI_ee_subscription_create (
        ogs_strdup(callback_reference->valuestring),
        monitoring_configurationsList,
        reporting_options ? reporting_options_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        subscription_id && !cJSON_IsNull(subscription_id) ? ogs_strdup(subscription_id->valuestring) : NULL,
        context_info ? context_info_local_nonprim : NULL,
        epc_applied_ind ? true : false,
        epc_applied_ind ? epc_applied_ind->valueint : 0,
        scef_diam_host && !cJSON_IsNull(scef_diam_host) ? ogs_strdup(scef_diam_host->valuestring) : NULL,
        scef_diam_realm && !cJSON_IsNull(scef_diam_realm) ? ogs_strdup(scef_diam_realm->valuestring) : NULL,
        notify_correlation_id && !cJSON_IsNull(notify_correlation_id) ? ogs_strdup(notify_correlation_id->valuestring) : NULL,
        second_callback_ref && !cJSON_IsNull(second_callback_ref) ? ogs_strdup(second_callback_ref->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        exclude_gpsi_list ? exclude_gpsi_listList : NULL,
        include_gpsi_list ? include_gpsi_listList : NULL,
        data_restoration_callback_uri && !cJSON_IsNull(data_restoration_callback_uri) ? ogs_strdup(data_restoration_callback_uri->valuestring) : NULL
    );

    return ee_subscription_local_var;
end:
    if (monitoring_configurationsList) {
        OpenAPI_list_for_each(monitoring_configurationsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_monitoring_configuration_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(monitoring_configurationsList);
        monitoring_configurationsList = NULL;
    }
    if (reporting_options_local_nonprim) {
        OpenAPI_reporting_options_free(reporting_options_local_nonprim);
        reporting_options_local_nonprim = NULL;
    }
    if (context_info_local_nonprim) {
        OpenAPI_context_info_free(context_info_local_nonprim);
        context_info_local_nonprim = NULL;
    }
    if (exclude_gpsi_listList) {
        OpenAPI_list_for_each(exclude_gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(exclude_gpsi_listList);
        exclude_gpsi_listList = NULL;
    }
    if (include_gpsi_listList) {
        OpenAPI_list_for_each(include_gpsi_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(include_gpsi_listList);
        include_gpsi_listList = NULL;
    }
    return NULL;
}

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_copy(OpenAPI_ee_subscription_t *dst, OpenAPI_ee_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ee_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed");
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

    OpenAPI_ee_subscription_free(dst);
    dst = OpenAPI_ee_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

