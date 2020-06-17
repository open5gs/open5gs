/*
 * ue_reachability.h
 *
 *
 */

#ifndef _OpenAPI_ue_reachability_H_
#define _OpenAPI_ue_reachability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_reachability_s OpenAPI_ue_reachability_t;
typedef struct OpenAPI_ue_reachability_s {
} OpenAPI_ue_reachability_t;

OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_create(
    );
void OpenAPI_ue_reachability_free(OpenAPI_ue_reachability_t *ue_reachability);
OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_parseFromJSON(cJSON *ue_reachabilityJSON);
cJSON *OpenAPI_ue_reachability_convertToJSON(OpenAPI_ue_reachability_t *ue_reachability);
OpenAPI_ue_reachability_t *OpenAPI_ue_reachability_copy(OpenAPI_ue_reachability_t *dst, OpenAPI_ue_reachability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_reachability_H_ */

