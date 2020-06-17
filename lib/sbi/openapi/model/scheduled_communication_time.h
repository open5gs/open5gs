/*
 * scheduled_communication_time.h
 *
 *
 */

#ifndef _OpenAPI_scheduled_communication_time_H_
#define _OpenAPI_scheduled_communication_time_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scheduled_communication_time_s OpenAPI_scheduled_communication_time_t;
typedef struct OpenAPI_scheduled_communication_time_s {
    OpenAPI_list_t *days_of_week;
    char *time_of_day_start;
    char *time_of_day_end;
} OpenAPI_scheduled_communication_time_t;

OpenAPI_scheduled_communication_time_t *OpenAPI_scheduled_communication_time_create(
    OpenAPI_list_t *days_of_week,
    char *time_of_day_start,
    char *time_of_day_end
    );
void OpenAPI_scheduled_communication_time_free(OpenAPI_scheduled_communication_time_t *scheduled_communication_time);
OpenAPI_scheduled_communication_time_t *OpenAPI_scheduled_communication_time_parseFromJSON(cJSON *scheduled_communication_timeJSON);
cJSON *OpenAPI_scheduled_communication_time_convertToJSON(OpenAPI_scheduled_communication_time_t *scheduled_communication_time);
OpenAPI_scheduled_communication_time_t *OpenAPI_scheduled_communication_time_copy(OpenAPI_scheduled_communication_time_t *dst, OpenAPI_scheduled_communication_time_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_communication_time_H_ */

