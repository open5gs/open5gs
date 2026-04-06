/*
 * traff_route_req_outcome_event.h
 *
 * Represents the traffic routing requirements installation outcome event subscription from the AF. 
 */

#ifndef _OpenAPI_traff_route_req_outcome_event_H_
#define _OpenAPI_traff_route_req_outcome_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_traff_route_req_outcome_event_s OpenAPI_traff_route_req_outcome_event_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_traff_route_req_outcome_event_s {
    char *notification_uri;
    char *notif_corre_id;
};

OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_create(
    char *notification_uri,
    char *notif_corre_id
);
void OpenAPI_traff_route_req_outcome_event_free(OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event);
OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_parseFromJSON(cJSON *traff_route_req_outcome_eventJSON);
cJSON *OpenAPI_traff_route_req_outcome_event_convertToJSON(OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event);
OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_copy(OpenAPI_traff_route_req_outcome_event_t *dst, OpenAPI_traff_route_req_outcome_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traff_route_req_outcome_event_H_ */

