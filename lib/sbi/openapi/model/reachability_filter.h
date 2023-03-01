/*
 * reachability_filter.h
 *
 * Event filter for REACHABILITY_REPORT event type
 */

#ifndef _OpenAPI_reachability_filter_H_
#define _OpenAPI_reachability_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "reachability_filter_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_reachability_filter_s OpenAPI_reachability_filter_t;
typedef struct OpenAPI_reachability_filter_s {
} OpenAPI_reachability_filter_t;

OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_create(
);
void OpenAPI_reachability_filter_free(OpenAPI_reachability_filter_t *reachability_filter);
OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_parseFromJSON(cJSON *reachability_filterJSON);
cJSON *OpenAPI_reachability_filter_convertToJSON(OpenAPI_reachability_filter_t *reachability_filter);
OpenAPI_reachability_filter_t *OpenAPI_reachability_filter_copy(OpenAPI_reachability_filter_t *dst, OpenAPI_reachability_filter_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reachability_filter_H_ */

