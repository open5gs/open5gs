
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vendor_specific_feature.h"

OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_create(
    char *feature_name,
    char *feature_version
    )
{
    OpenAPI_vendor_specific_feature_t *vendor_specific_feature_local_var = OpenAPI_malloc(sizeof(OpenAPI_vendor_specific_feature_t));
    if (!vendor_specific_feature_local_var) {
        return NULL;
    }
    vendor_specific_feature_local_var->feature_name = feature_name;
    vendor_specific_feature_local_var->feature_version = feature_version;

    return vendor_specific_feature_local_var;
}

void OpenAPI_vendor_specific_feature_free(OpenAPI_vendor_specific_feature_t *vendor_specific_feature)
{
    if (NULL == vendor_specific_feature) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(vendor_specific_feature->feature_name);
    ogs_free(vendor_specific_feature->feature_version);
    ogs_free(vendor_specific_feature);
}

cJSON *OpenAPI_vendor_specific_feature_convertToJSON(OpenAPI_vendor_specific_feature_t *vendor_specific_feature)
{
    cJSON *item = NULL;

    if (vendor_specific_feature == NULL) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed [VendorSpecificFeature]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!vendor_specific_feature->feature_name) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed [feature_name]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "featureName", vendor_specific_feature->feature_name) == NULL) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed [feature_name]");
        goto end;
    }

    if (!vendor_specific_feature->feature_version) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed [feature_version]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "featureVersion", vendor_specific_feature->feature_version) == NULL) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed [feature_version]");
        goto end;
    }

end:
    return item;
}

OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_parseFromJSON(cJSON *vendor_specific_featureJSON)
{
    OpenAPI_vendor_specific_feature_t *vendor_specific_feature_local_var = NULL;
    cJSON *feature_name = cJSON_GetObjectItemCaseSensitive(vendor_specific_featureJSON, "featureName");
    if (!feature_name) {
        ogs_error("OpenAPI_vendor_specific_feature_parseFromJSON() failed [feature_name]");
        goto end;
    }


    if (!cJSON_IsString(feature_name)) {
        ogs_error("OpenAPI_vendor_specific_feature_parseFromJSON() failed [feature_name]");
        goto end;
    }

    cJSON *feature_version = cJSON_GetObjectItemCaseSensitive(vendor_specific_featureJSON, "featureVersion");
    if (!feature_version) {
        ogs_error("OpenAPI_vendor_specific_feature_parseFromJSON() failed [feature_version]");
        goto end;
    }


    if (!cJSON_IsString(feature_version)) {
        ogs_error("OpenAPI_vendor_specific_feature_parseFromJSON() failed [feature_version]");
        goto end;
    }

    vendor_specific_feature_local_var = OpenAPI_vendor_specific_feature_create (
        ogs_strdup(feature_name->valuestring),
        ogs_strdup(feature_version->valuestring)
        );

    return vendor_specific_feature_local_var;
end:
    return NULL;
}

OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_copy(OpenAPI_vendor_specific_feature_t *dst, OpenAPI_vendor_specific_feature_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vendor_specific_feature_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vendor_specific_feature_convertToJSON() failed");
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

    OpenAPI_vendor_specific_feature_free(dst);
    dst = OpenAPI_vendor_specific_feature_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

