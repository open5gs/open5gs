/*
 * periodic_event_info.h
 *
 * Indicates the information of periodic event reporting.
 */

#ifndef _OpenAPI_periodic_event_info_H_
#define _OpenAPI_periodic_event_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_periodic_event_info_s OpenAPI_periodic_event_info_t;
typedef struct OpenAPI_periodic_event_info_s {
    int reporting_amount;
    int reporting_interval;
} OpenAPI_periodic_event_info_t;

OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_create(
    int reporting_amount,
    int reporting_interval
);
void OpenAPI_periodic_event_info_free(OpenAPI_periodic_event_info_t *periodic_event_info);
OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_parseFromJSON(cJSON *periodic_event_infoJSON);
cJSON *OpenAPI_periodic_event_info_convertToJSON(OpenAPI_periodic_event_info_t *periodic_event_info);
OpenAPI_periodic_event_info_t *OpenAPI_periodic_event_info_copy(OpenAPI_periodic_event_info_t *dst, OpenAPI_periodic_event_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodic_event_info_H_ */

