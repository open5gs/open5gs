/*
 * sim_conn_fail_event.h
 *
 * Represents the simultaneous connectivity failure event subscription from the AF. 
 */

#ifndef _OpenAPI_sim_conn_fail_event_H_
#define _OpenAPI_sim_conn_fail_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sim_conn_fail_event_s OpenAPI_sim_conn_fail_event_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sim_conn_fail_event_s {
    char *notification_uri;
    char *notif_corre_id;
};

OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_create(
    char *notification_uri,
    char *notif_corre_id
);
void OpenAPI_sim_conn_fail_event_free(OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event);
OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_parseFromJSON(cJSON *sim_conn_fail_eventJSON);
cJSON *OpenAPI_sim_conn_fail_event_convertToJSON(OpenAPI_sim_conn_fail_event_t *sim_conn_fail_event);
OpenAPI_sim_conn_fail_event_t *OpenAPI_sim_conn_fail_event_copy(OpenAPI_sim_conn_fail_event_t *dst, OpenAPI_sim_conn_fail_event_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sim_conn_fail_event_H_ */

