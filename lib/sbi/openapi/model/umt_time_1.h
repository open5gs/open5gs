/*
 * umt_time_1.h
 *
 * 
 */

#ifndef _OpenAPI_umt_time_1_H_
#define _OpenAPI_umt_time_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_umt_time_1_s OpenAPI_umt_time_1_t;
typedef struct OpenAPI_umt_time_1_s {
    char *time_of_day;
    int day_of_week;
} OpenAPI_umt_time_1_t;

OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_create(
    char *time_of_day,
    int day_of_week
);
void OpenAPI_umt_time_1_free(OpenAPI_umt_time_1_t *umt_time_1);
OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_parseFromJSON(cJSON *umt_time_1JSON);
cJSON *OpenAPI_umt_time_1_convertToJSON(OpenAPI_umt_time_1_t *umt_time_1);
OpenAPI_umt_time_1_t *OpenAPI_umt_time_1_copy(OpenAPI_umt_time_1_t *dst, OpenAPI_umt_time_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_umt_time_1_H_ */

