/*
 * vendor_specific_feature.h
 *
 * Information about a vendor-specific feature
 */

#ifndef _OpenAPI_vendor_specific_feature_H_
#define _OpenAPI_vendor_specific_feature_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_vendor_specific_feature_s OpenAPI_vendor_specific_feature_t;
typedef struct OpenAPI_vendor_specific_feature_s {
    char *feature_name;
    char *feature_version;
} OpenAPI_vendor_specific_feature_t;

OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_create(
    char *feature_name,
    char *feature_version
);
void OpenAPI_vendor_specific_feature_free(OpenAPI_vendor_specific_feature_t *vendor_specific_feature);
OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_parseFromJSON(cJSON *vendor_specific_featureJSON);
cJSON *OpenAPI_vendor_specific_feature_convertToJSON(OpenAPI_vendor_specific_feature_t *vendor_specific_feature);
OpenAPI_vendor_specific_feature_t *OpenAPI_vendor_specific_feature_copy(OpenAPI_vendor_specific_feature_t *dst, OpenAPI_vendor_specific_feature_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vendor_specific_feature_H_ */

