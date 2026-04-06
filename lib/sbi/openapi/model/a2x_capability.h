/*
 * a2x_capability.h
 *
 * Indicate the supported A2X Capability by the PCF.
 */

#ifndef _OpenAPI_a2x_capability_H_
#define _OpenAPI_a2x_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_a2x_capability_s OpenAPI_a2x_capability_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_a2x_capability_s {
    bool is_lte_a2x;
    int lte_a2x;
    bool is_nr_a2x;
    int nr_a2x;
};

OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_create(
    bool is_lte_a2x,
    int lte_a2x,
    bool is_nr_a2x,
    int nr_a2x
);
void OpenAPI_a2x_capability_free(OpenAPI_a2x_capability_t *a2x_capability);
OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_parseFromJSON(cJSON *a2x_capabilityJSON);
cJSON *OpenAPI_a2x_capability_convertToJSON(OpenAPI_a2x_capability_t *a2x_capability);
OpenAPI_a2x_capability_t *OpenAPI_a2x_capability_copy(OpenAPI_a2x_capability_t *dst, OpenAPI_a2x_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_a2x_capability_H_ */

