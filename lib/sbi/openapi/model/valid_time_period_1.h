/*
 * valid_time_period_1.h
 *
 * 
 */

#ifndef _OpenAPI_valid_time_period_1_H_
#define _OpenAPI_valid_time_period_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_valid_time_period_1_s OpenAPI_valid_time_period_1_t;
typedef struct OpenAPI_valid_time_period_1_s {
    char *start_time;
    char *end_time;
} OpenAPI_valid_time_period_1_t;

OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_create(
    char *start_time,
    char *end_time
);
void OpenAPI_valid_time_period_1_free(OpenAPI_valid_time_period_1_t *valid_time_period_1);
OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_parseFromJSON(cJSON *valid_time_period_1JSON);
cJSON *OpenAPI_valid_time_period_1_convertToJSON(OpenAPI_valid_time_period_1_t *valid_time_period_1);
OpenAPI_valid_time_period_1_t *OpenAPI_valid_time_period_1_copy(OpenAPI_valid_time_period_1_t *dst, OpenAPI_valid_time_period_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_valid_time_period_1_H_ */

