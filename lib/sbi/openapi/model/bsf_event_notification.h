/*
 * bsf_event_notification.h
 *
 * Contains an event notification.
 */

#ifndef _OpenAPI_bsf_event_notification_H_
#define _OpenAPI_bsf_event_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bsf_event.h"
#include "pcf_for_pdu_session_info.h"
#include "pcf_for_ue_info.h"
#include "snssai_dnn_pair.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bsf_event_notification_s OpenAPI_bsf_event_notification_t;
typedef struct OpenAPI_bsf_event_notification_s {
    OpenAPI_bsf_event_e event;
    struct OpenAPI_pcf_for_ue_info_s *pcf_for_ue_info;
    OpenAPI_list_t *pcf_for_pdu_sess_infos;
    OpenAPI_list_t *match_snssai_dnns;
} OpenAPI_bsf_event_notification_t;

OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_create(
    OpenAPI_bsf_event_e event,
    OpenAPI_pcf_for_ue_info_t *pcf_for_ue_info,
    OpenAPI_list_t *pcf_for_pdu_sess_infos,
    OpenAPI_list_t *match_snssai_dnns
);
void OpenAPI_bsf_event_notification_free(OpenAPI_bsf_event_notification_t *bsf_event_notification);
OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_parseFromJSON(cJSON *bsf_event_notificationJSON);
cJSON *OpenAPI_bsf_event_notification_convertToJSON(OpenAPI_bsf_event_notification_t *bsf_event_notification);
OpenAPI_bsf_event_notification_t *OpenAPI_bsf_event_notification_copy(OpenAPI_bsf_event_notification_t *dst, OpenAPI_bsf_event_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_event_notification_H_ */

