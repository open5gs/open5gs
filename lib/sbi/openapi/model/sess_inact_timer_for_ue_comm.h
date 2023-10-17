/*
 * sess_inact_timer_for_ue_comm.h
 *
 * Represents the N4 Session inactivity timer.
 */

#ifndef _OpenAPI_sess_inact_timer_for_ue_comm_H_
#define _OpenAPI_sess_inact_timer_for_ue_comm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sess_inact_timer_for_ue_comm_s OpenAPI_sess_inact_timer_for_ue_comm_t;
typedef struct OpenAPI_sess_inact_timer_for_ue_comm_s {
    int n4_sess_id;
    int sess_inactive_timer;
} OpenAPI_sess_inact_timer_for_ue_comm_t;

OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_create(
    int n4_sess_id,
    int sess_inactive_timer
);
void OpenAPI_sess_inact_timer_for_ue_comm_free(OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm);
OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_parseFromJSON(cJSON *sess_inact_timer_for_ue_commJSON);
cJSON *OpenAPI_sess_inact_timer_for_ue_comm_convertToJSON(OpenAPI_sess_inact_timer_for_ue_comm_t *sess_inact_timer_for_ue_comm);
OpenAPI_sess_inact_timer_for_ue_comm_t *OpenAPI_sess_inact_timer_for_ue_comm_copy(OpenAPI_sess_inact_timer_for_ue_comm_t *dst, OpenAPI_sess_inact_timer_for_ue_comm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sess_inact_timer_for_ue_comm_H_ */

