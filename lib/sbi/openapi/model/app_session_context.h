/*
 * app_session_context.h
 *
 * Represents an Individual Application Session Context resource.
 */

#ifndef _OpenAPI_app_session_context_H_
#define _OpenAPI_app_session_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "app_session_context_req_data.h"
#include "app_session_context_resp_data.h"
#include "events_notification.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_app_session_context_s OpenAPI_app_session_context_t;
typedef struct OpenAPI_app_session_context_s {
    struct OpenAPI_app_session_context_req_data_s *asc_req_data;
    struct OpenAPI_app_session_context_resp_data_s *asc_resp_data;
    struct OpenAPI_events_notification_s *evs_notif;
} OpenAPI_app_session_context_t;

OpenAPI_app_session_context_t *OpenAPI_app_session_context_create(
    OpenAPI_app_session_context_req_data_t *asc_req_data,
    OpenAPI_app_session_context_resp_data_t *asc_resp_data,
    OpenAPI_events_notification_t *evs_notif
);
void OpenAPI_app_session_context_free(OpenAPI_app_session_context_t *app_session_context);
OpenAPI_app_session_context_t *OpenAPI_app_session_context_parseFromJSON(cJSON *app_session_contextJSON);
cJSON *OpenAPI_app_session_context_convertToJSON(OpenAPI_app_session_context_t *app_session_context);
OpenAPI_app_session_context_t *OpenAPI_app_session_context_copy(OpenAPI_app_session_context_t *dst, OpenAPI_app_session_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_app_session_context_H_ */

