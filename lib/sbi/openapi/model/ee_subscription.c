
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ee_subscription.h"

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_create(
    char *callback_reference,
    OpenAPI_list_t* monitoring_configurations,
    OpenAPI_reporting_options_t *reporting_options,
    char *supported_features,
    char *subscription_id
    )
{
    OpenAPI_ee_subscription_t *ee_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_ee_subscription_t));
    if (!ee_subscription_local_var) {
        return NULL;
    }
    ee_subscription_local_var->callback_reference = callback_reference;
    ee_subscription_local_var->monitoring_configurations = monitoring_configurations;
    ee_subscription_local_var->reporting_options = reporting_options;
    ee_subscription_local_var->supported_features = supported_features;
    ee_subscription_local_var->subscription_id = subscription_id;

    return ee_subscription_local_var;
}

void OpenAPI_ee_subscription_free(OpenAPI_ee_subscription_t *ee_subscription)
{
    if (NULL == ee_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ee_subscription->callback_reference);
    OpenAPI_list_for_each(ee_subscription->monitoring_configurations, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        OpenAPI_monitoring_configuration_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(ee_subscription->monitoring_configurations);
    OpenAPI_reporting_options_free(ee_subscription->reporting_options);
    ogs_free(ee_subscription->supported_features);
    ogs_free(ee_subscription->subscription_id);
    ogs_free(ee_subscription);
}

cJSON *OpenAPI_ee_subscription_convertToJSON(OpenAPI_ee_subscription_t *ee_subscription)
{
    cJSON *item = NULL;

    if (ee_subscription == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [EeSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ee_subscription->callback_reference) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "callbackReference", ee_subscription->callback_reference) == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [callback_reference]");
        goto end;
    }

    if (!ee_subscription->monitoring_configurations) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
        goto end;
    }
    cJSON *monitoring_configurations = cJSON_AddObjectToObject(item, "monitoringConfigurations");
    if (monitoring_configurations == NULL) {
        ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
        goto end;
    }
    cJSON *localMapObject = monitoring_configurations;
    OpenAPI_lnode_t *monitoring_configurations_node;
    if (ee_subscription->monitoring_configurations) {
        OpenAPI_list_for_each(ee_subscription->monitoring_configurations, monitoring_configurations_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)monitoring_configurations_node->data;
            cJSON *itemLocal = OpenAPI_monitoring_configuration_convertToJSON(localKeyValue->value);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_ee_subscription_convertToJSON() failed [monitoring_configurations]");
                goto end;
            }
            cJSON_AddItemToObject(monitoring_configurations, localKeyValue->key, itemLocal);
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

end:
    return item;
}

OpenAPI_ee_subscription_t *OpenAPI_ee_subscription_parseFromJSON(cJSON *ee_subscriptionJSON)
{
    OpenAPI_ee_subscription_t *ee_subscription_local_var = NULL;
    cJSON *callback_reference = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "callbackReference");
    if (!callback_reference) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }


    if (!cJSON_IsString(callback_reference)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [callback_reference]");
        goto end;
    }

    cJSON *monitoring_configurations = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "monitoringConfigurations");
    if (!monitoring_configurations) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [monitoring_configurations]");
        goto end;
    }

    OpenAPI_list_t *monitoring_configurationsList;

    cJSON *monitoring_configurations_local_map;
    if (!cJSON_IsObject(monitoring_configurations)) {
        ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [monitoring_configurations]");
        goto end;
    }
    monitoring_configurationsList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(monitoring_configurations_local_map, monitoring_configurations) {
        cJSON *localMapObject = monitoring_configurations_local_map;
        if (!cJSON_IsObject(monitoring_configurations_local_map)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [monitoring_configurations]");
            goto end;
        }
        localMapKeyPair = OpenAPI_map_create(
            localMapObject->string, OpenAPI_monitoring_configuration_parseFromJSON(localMapObject));
        OpenAPI_list_add(monitoring_configurationsList, localMapKeyPair);
    }

    cJSON *reporting_options = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "reportingOptions");

    OpenAPI_reporting_options_t *reporting_options_local_nonprim = NULL;
    if (reporting_options) {
        reporting_options_local_nonprim = OpenAPI_reporting_options_parseFromJSON(reporting_options);
    }

    cJSON *supported_features = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "supportedFeatures");

    if (supported_features) {
        if (!cJSON_IsString(supported_features)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [supported_features]");
            goto end;
        }
    }

    cJSON *subscription_id = cJSON_GetObjectItemCaseSensitive(ee_subscriptionJSON, "subscriptionId");

    if (subscription_id) {
        if (!cJSON_IsString(subscription_id)) {
            ogs_error("OpenAPI_ee_subscription_parseFromJSON() failed [subscription_id]");
            goto end;
        }
    }

    ee_subscription_local_var = OpenAPI_ee_subscription_create (
        ogs_strdup(callback_reference->valuestring),
        monitoring_configurationsList,
        reporting_options ? reporting_options_local_nonprim : NULL,
        supported_features ? ogs_strdup(supported_features->valuestring) : NULL,
        subscription_id ? ogs_strdup(subscription_id->valuestring) : NULL
        );

    return ee_subscription_local_var;
end:
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

