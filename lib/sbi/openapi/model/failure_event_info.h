/*
 * failure_event_info.h
 *
 * Contains information on the event for which the subscription is not successful.
 */

#ifndef _OpenAPI_failure_event_info_H_
#define _OpenAPI_failure_event_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "nwdaf_event.h"
#include "nwdaf_failure_code.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_failure_event_info_s OpenAPI_failure_event_info_t;
typedef struct OpenAPI_failure_event_info_s {
    struct OpenAPI_nwdaf_event_s *event;
    struct OpenAPI_nwdaf_failure_code_s *failure_code;
} OpenAPI_failure_event_info_t;

OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_create(
    OpenAPI_nwdaf_event_t *event,
    OpenAPI_nwdaf_failure_code_t *failure_code
);
void OpenAPI_failure_event_info_free(OpenAPI_failure_event_info_t *failure_event_info);
OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_parseFromJSON(cJSON *failure_event_infoJSON);
cJSON *OpenAPI_failure_event_info_convertToJSON(OpenAPI_failure_event_info_t *failure_event_info);
OpenAPI_failure_event_info_t *OpenAPI_failure_event_info_copy(OpenAPI_failure_event_info_t *dst, OpenAPI_failure_event_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_failure_event_info_H_ */

