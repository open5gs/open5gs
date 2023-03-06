/*
 * v2x_capability.h
 *
 * Indicate the supported V2X Capability by the PCF.
 */

#ifndef _OpenAPI_v2x_capability_H_
#define _OpenAPI_v2x_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_v2x_capability_s OpenAPI_v2x_capability_t;
typedef struct OpenAPI_v2x_capability_s {
    bool is_lte_v2x;
    int lte_v2x;
    bool is_nr_v2x;
    int nr_v2x;
} OpenAPI_v2x_capability_t;

OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_create(
    bool is_lte_v2x,
    int lte_v2x,
    bool is_nr_v2x,
    int nr_v2x
);
void OpenAPI_v2x_capability_free(OpenAPI_v2x_capability_t *v2x_capability);
OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_parseFromJSON(cJSON *v2x_capabilityJSON);
cJSON *OpenAPI_v2x_capability_convertToJSON(OpenAPI_v2x_capability_t *v2x_capability);
OpenAPI_v2x_capability_t *OpenAPI_v2x_capability_copy(OpenAPI_v2x_capability_t *dst, OpenAPI_v2x_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_v2x_capability_H_ */

