/*
 * nwdaf_capability.h
 *
 * Indicates the capability supported by the NWDAF
 */

#ifndef _OpenAPI_nwdaf_capability_H_
#define _OpenAPI_nwdaf_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nwdaf_capability_s OpenAPI_nwdaf_capability_t;
typedef struct OpenAPI_nwdaf_capability_s {
    bool is_analytics_aggregation;
    int analytics_aggregation;
    bool is_analytics_metadata_provisioning;
    int analytics_metadata_provisioning;
} OpenAPI_nwdaf_capability_t;

OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_create(
    bool is_analytics_aggregation,
    int analytics_aggregation,
    bool is_analytics_metadata_provisioning,
    int analytics_metadata_provisioning
);
void OpenAPI_nwdaf_capability_free(OpenAPI_nwdaf_capability_t *nwdaf_capability);
OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_parseFromJSON(cJSON *nwdaf_capabilityJSON);
cJSON *OpenAPI_nwdaf_capability_convertToJSON(OpenAPI_nwdaf_capability_t *nwdaf_capability);
OpenAPI_nwdaf_capability_t *OpenAPI_nwdaf_capability_copy(OpenAPI_nwdaf_capability_t *dst, OpenAPI_nwdaf_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nwdaf_capability_H_ */

