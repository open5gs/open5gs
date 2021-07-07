/*
 * temporal_validity.h
 *
 * Indicates the time interval(s) during which the AF request is to be applied
 */

#ifndef _OpenAPI_temporal_validity_H_
#define _OpenAPI_temporal_validity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_temporal_validity_s OpenAPI_temporal_validity_t;
typedef struct OpenAPI_temporal_validity_s {
    char *start_time;
    char *stop_time;
} OpenAPI_temporal_validity_t;

OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_create(
    char *start_time,
    char *stop_time
);
void OpenAPI_temporal_validity_free(OpenAPI_temporal_validity_t *temporal_validity);
OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_parseFromJSON(cJSON *temporal_validityJSON);
cJSON *OpenAPI_temporal_validity_convertToJSON(OpenAPI_temporal_validity_t *temporal_validity);
OpenAPI_temporal_validity_t *OpenAPI_temporal_validity_copy(OpenAPI_temporal_validity_t *dst, OpenAPI_temporal_validity_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_temporal_validity_H_ */

