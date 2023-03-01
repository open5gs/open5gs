
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nwdaf_capability.h"

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_create(
    bool is_analytics_aggregation,
    int analytics_aggregation,
    bool is_analytics_metadata_provisioning,
    int analytics_metadata_provisioning
)
{
    OpenAPI_nwdaf_capability_t *nwdaf_capability_local_var = ogs_malloc(sizeof(OpenAPI_nwdaf_capability_t));
    ogs_assert(nwdaf_capability_local_var);

    nwdaf_capability_local_var->is_analytics_aggregation = is_analytics_aggregation;
    nwdaf_capability_local_var->analytics_aggregation = analytics_aggregation;
    nwdaf_capability_local_var->is_analytics_metadata_provisioning = is_analytics_metadata_provisioning;
    nwdaf_capability_local_var->analytics_metadata_provisioning = analytics_metadata_provisioning;

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

end:
    return item;
}

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_parseFromJSON(cJSON *nwdaf_capabilityJSON)
{
    OpenAPI_nwdaf_capability_t *nwdaf_capability_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *analytics_aggregation = NULL;
    cJSON *analytics_metadata_provisioning = NULL;
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

    nwdaf_capability_local_var = OpenAPI_nwdaf_capability_create (
        analytics_aggregation ? true : false,
        analytics_aggregation ? analytics_aggregation->valueint : 0,
        analytics_metadata_provisioning ? true : false,
        analytics_metadata_provisioning ? analytics_metadata_provisioning->valueint : 0
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

