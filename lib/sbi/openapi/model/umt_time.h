/*
 * umt_time.h
 *
 * 
 */

#ifndef _OpenAPI_umt_time_H_
#define _OpenAPI_umt_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_umt_time_s OpenAPI_umt_time_t;
typedef struct OpenAPI_umt_time_s {
    char *time_of_day;
    int day_of_week;
} OpenAPI_umt_time_t;

OpenAPI_umt_time_t *OpenAPI_umt_time_create(
    char *time_of_day,
    int day_of_week
);
void OpenAPI_umt_time_free(OpenAPI_umt_time_t *umt_time);
OpenAPI_umt_time_t *OpenAPI_umt_time_parseFromJSON(cJSON *umt_timeJSON);
cJSON *OpenAPI_umt_time_convertToJSON(OpenAPI_umt_time_t *umt_time);
OpenAPI_umt_time_t *OpenAPI_umt_time_copy(OpenAPI_umt_time_t *dst, OpenAPI_umt_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_umt_time_H_ */

