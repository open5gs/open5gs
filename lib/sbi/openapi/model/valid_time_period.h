/*
 * valid_time_period.h
 *
 * 
 */

#ifndef _OpenAPI_valid_time_period_H_
#define _OpenAPI_valid_time_period_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_valid_time_period_s OpenAPI_valid_time_period_t;
typedef struct OpenAPI_valid_time_period_s {
    char *start_time;
    char *end_time;
} OpenAPI_valid_time_period_t;

OpenAPI_valid_time_period_t *OpenAPI_valid_time_period_create(
    char *start_time,
    char *end_time
);
void OpenAPI_valid_time_period_free(OpenAPI_valid_time_period_t *valid_time_period);
OpenAPI_valid_time_period_t *OpenAPI_valid_time_period_parseFromJSON(cJSON *valid_time_periodJSON);
cJSON *OpenAPI_valid_time_period_convertToJSON(OpenAPI_valid_time_period_t *valid_time_period);
OpenAPI_valid_time_period_t *OpenAPI_valid_time_period_copy(OpenAPI_valid_time_period_t *dst, OpenAPI_valid_time_period_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_valid_time_period_H_ */

