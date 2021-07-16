/*
 * mme_capabilities.h
 *
 * 
 */

#ifndef _OpenAPI_mme_capabilities_H_
#define _OpenAPI_mme_capabilities_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mme_capabilities_s OpenAPI_mme_capabilities_t;
typedef struct OpenAPI_mme_capabilities_s {
    bool is_non_ip_supported;
    int non_ip_supported;
    bool is_ethernet_supported;
    int ethernet_supported;
} OpenAPI_mme_capabilities_t;

OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_create(
    bool is_non_ip_supported,
    int non_ip_supported,
    bool is_ethernet_supported,
    int ethernet_supported
);
void OpenAPI_mme_capabilities_free(OpenAPI_mme_capabilities_t *mme_capabilities);
OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_parseFromJSON(cJSON *mme_capabilitiesJSON);
cJSON *OpenAPI_mme_capabilities_convertToJSON(OpenAPI_mme_capabilities_t *mme_capabilities);
OpenAPI_mme_capabilities_t *OpenAPI_mme_capabilities_copy(OpenAPI_mme_capabilities_t *dst, OpenAPI_mme_capabilities_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mme_capabilities_H_ */

