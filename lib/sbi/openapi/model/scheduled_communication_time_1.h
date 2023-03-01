/*
 * scheduled_communication_time_1.h
 *
 * Represents an offered scheduled communication time.
 */

#ifndef _OpenAPI_scheduled_communication_time_1_H_
#define _OpenAPI_scheduled_communication_time_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_scheduled_communication_time_1_s OpenAPI_scheduled_communication_time_1_t;
typedef struct OpenAPI_scheduled_communication_time_1_s {
    OpenAPI_list_t *days_of_week;
    char *time_of_day_start;
    char *time_of_day_end;
} OpenAPI_scheduled_communication_time_1_t;

OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_create(
    OpenAPI_list_t *days_of_week,
    char *time_of_day_start,
    char *time_of_day_end
);
void OpenAPI_scheduled_communication_time_1_free(OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1);
OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_parseFromJSON(cJSON *scheduled_communication_time_1JSON);
cJSON *OpenAPI_scheduled_communication_time_1_convertToJSON(OpenAPI_scheduled_communication_time_1_t *scheduled_communication_time_1);
OpenAPI_scheduled_communication_time_1_t *OpenAPI_scheduled_communication_time_1_copy(OpenAPI_scheduled_communication_time_1_t *dst, OpenAPI_scheduled_communication_time_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_scheduled_communication_time_1_H_ */

