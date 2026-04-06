/*
 * temporal_validity_1.h
 *
 * Indicates the time interval(s) during which the AF request is to be applied.
 */

#ifndef _OpenAPI_temporal_validity_1_H_
#define _OpenAPI_temporal_validity_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_temporal_validity_1_s OpenAPI_temporal_validity_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_temporal_validity_1_s {
    char *start_time;
    char *stop_time;
};

OpenAPI_temporal_validity_1_t *OpenAPI_temporal_validity_1_create(
    char *start_time,
    char *stop_time
);
void OpenAPI_temporal_validity_1_free(OpenAPI_temporal_validity_1_t *temporal_validity_1);
OpenAPI_temporal_validity_1_t *OpenAPI_temporal_validity_1_parseFromJSON(cJSON *temporal_validity_1JSON);
cJSON *OpenAPI_temporal_validity_1_convertToJSON(OpenAPI_temporal_validity_1_t *temporal_validity_1);
OpenAPI_temporal_validity_1_t *OpenAPI_temporal_validity_1_copy(OpenAPI_temporal_validity_1_t *dst, OpenAPI_temporal_validity_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_temporal_validity_1_H_ */

