
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_capability.h"

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_create(
    bool is_analytics_aggregation,
    int analytics_aggregation,
    bool is_analytics_metadata_provisioning,
    int analytics_metadata_provisioning,
    bool is_ml_model_accuracy_checking,
    int ml_model_accuracy_checking,
    bool is_analytics_accuracy_checking,
    int analytics_accuracy_checking,
    bool is_roaming_exchange,
    int roaming_exchange
)
{
    OpenAPI_nwdaf_capability_t *nwdaf_capability_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_capability_t));
    ogs_assert(nwdaf_capability_local_var);

    nwdaf_capability_local_var->is_analytics_aggregation = is_analytics_aggregation;
    nwdaf_capability_local_var->analytics_aggregation = analytics_aggregation;
    nwdaf_capability_local_var->is_analytics_metadata_provisioning = is_analytics_metadata_provisioning;
    nwdaf_capability_local_var->analytics_metadata_provisioning = analytics_metadata_provisioning;
    nwdaf_capability_local_var->is_ml_model_accuracy_checking = is_ml_model_accuracy_checking;
    nwdaf_capability_local_var->ml_model_accuracy_checking = ml_model_accuracy_checking;
    nwdaf_capability_local_var->is_analytics_accuracy_checking = is_analytics_accuracy_checking;
    nwdaf_capability_local_var->analytics_accuracy_checking = analytics_accuracy_checking;
    nwdaf_capability_local_var->is_roaming_exchange = is_roaming_exchange;
    nwdaf_capability_local_var->roaming_exchange = roaming_exchange;

    return nwdaf_capability_local_var;
}

void OpenAPI_nwdaf_capability_free(OpenAPI_nwdaf_capability_t *nwdaf_capability)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nwdaf_capability) {
        return;
    }
    ogs_free(nwdaf_capability);
}

cJSON *OpenAPI_nwdaf_capability_convertToJSON(OpenAPI_nwdaf_capability_t *nwdaf_capability)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nwdaf_capability == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [NwdafCapability]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nwdaf_capability->is_analytics_aggregation) {
    if (cJSON_AddBoolToObject(item, "analyticsAggregation", nwdaf_capability->analytics_aggregation) == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [analytics_aggregation]");
        goto end;
    }
    }

    if (nwdaf_capability->is_analytics_metadata_provisioning) {
    if (cJSON_AddBoolToObject(item, "analyticsMetadataProvisioning", nwdaf_capability->analytics_metadata_provisioning) == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [analytics_metadata_provisioning]");
        goto end;
    }
    }

    if (nwdaf_capability->is_ml_model_accuracy_checking) {
    if (cJSON_AddBoolToObject(item, "mlModelAccuracyChecking", nwdaf_capability->ml_model_accuracy_checking) == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [ml_model_accuracy_checking]");
        goto end;
    }
    }

    if (nwdaf_capability->is_analytics_accuracy_checking) {
    if (cJSON_AddBoolToObject(item, "analyticsAccuracyChecking", nwdaf_capability->analytics_accuracy_checking) == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [analytics_accuracy_checking]");
        goto end;
    }
    }

    if (nwdaf_capability->is_roaming_exchange) {
    if (cJSON_AddBoolToObject(item, "roamingExchange", nwdaf_capability->roaming_exchange) == NULL) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed [roaming_exchange]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_parseFromJSON(cJSON *nwdaf_capabilityJSON)
{
    OpenAPI_nwdaf_capability_t *nwdaf_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *analytics_aggregation = NULL;
    cJSON *analytics_metadata_provisioning = NULL;
    cJSON *ml_model_accuracy_checking = NULL;
    cJSON *analytics_accuracy_checking = NULL;
    cJSON *roaming_exchange = NULL;
    analytics_aggregation = cJSON_GetObjectItemCaseSensitive(nwdaf_capabilityJSON, "analyticsAggregation");
    if (analytics_aggregation) {
    if (!cJSON_IsBool(analytics_aggregation)) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON() failed [analytics_aggregation]");
        goto end;
    }
    }

    analytics_metadata_provisioning = cJSON_GetObjectItemCaseSensitive(nwdaf_capabilityJSON, "analyticsMetadataProvisioning");
    if (analytics_metadata_provisioning) {
    if (!cJSON_IsBool(analytics_metadata_provisioning)) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON() failed [analytics_metadata_provisioning]");
        goto end;
    }
    }

    ml_model_accuracy_checking = cJSON_GetObjectItemCaseSensitive(nwdaf_capabilityJSON, "mlModelAccuracyChecking");
    if (ml_model_accuracy_checking) {
    if (!cJSON_IsBool(ml_model_accuracy_checking)) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON() failed [ml_model_accuracy_checking]");
        goto end;
    }
    }

    analytics_accuracy_checking = cJSON_GetObjectItemCaseSensitive(nwdaf_capabilityJSON, "analyticsAccuracyChecking");
    if (analytics_accuracy_checking) {
    if (!cJSON_IsBool(analytics_accuracy_checking)) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON() failed [analytics_accuracy_checking]");
        goto end;
    }
    }

    roaming_exchange = cJSON_GetObjectItemCaseSensitive(nwdaf_capabilityJSON, "roamingExchange");
    if (roaming_exchange) {
    if (!cJSON_IsBool(roaming_exchange)) {
        ogs_error("OpenAPI_nwdaf_capability_parseFromJSON() failed [roaming_exchange]");
        goto end;
    }
    }

    nwdaf_capability_local_var = OpenAPI_nwdaf_capability_create (
        analytics_aggregation ? true : false,
        analytics_aggregation ? analytics_aggregation->valueint : 0,
        analytics_metadata_provisioning ? true : false,
        analytics_metadata_provisioning ? analytics_metadata_provisioning->valueint : 0,
        ml_model_accuracy_checking ? true : false,
        ml_model_accuracy_checking ? ml_model_accuracy_checking->valueint : 0,
        analytics_accuracy_checking ? true : false,
        analytics_accuracy_checking ? analytics_accuracy_checking->valueint : 0,
        roaming_exchange ? true : false,
        roaming_exchange ? roaming_exchange->valueint : 0
    );

    return nwdaf_capability_local_var;
end:
    return NULL;
}

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_copy(OpenAPI_nwdaf_capability_t *dst, OpenAPI_nwdaf_capability_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nwdaf_capability_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nwdaf_capability_convertToJSON() failed");
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

    OpenAPI_nwdaf_capability_free(dst);
    dst = OpenAPI_nwdaf_capability_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

