/*
 * pdu_session_event_notification.h
 *
 * Indicates PDU session information for the concerned established/terminated PDU session.
 */

#ifndef _OpenAPI_pdu_session_event_notification_H_
#define _OpenAPI_pdu_session_event_notification_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "af_event_notification.h"
#include "npcf_pdu_session_status.h"
#include "pcf_addressing_info.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_event_notification_s OpenAPI_pdu_session_event_notification_t;
typedef struct OpenAPI_pdu_session_event_notification_s {
    struct OpenAPI_af_event_notification_s *ev_notif;
    char *supi;
    char *ue_ipv4;
    char *ue_ipv6;
    char *ue_mac;
    OpenAPI_npcf_pdu_session_status_e status;
    struct OpenAPI_pcf_addressing_info_s *pcf_info;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *gpsi;
} OpenAPI_pdu_session_event_notification_t;

OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_create(
    OpenAPI_af_event_notification_t *ev_notif,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    OpenAPI_npcf_pdu_session_status_e status,
    OpenAPI_pcf_addressing_info_t *pcf_info,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *gpsi
);
void OpenAPI_pdu_session_event_notification_free(OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification);
OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_parseFromJSON(cJSON *pdu_session_event_notificationJSON);
cJSON *OpenAPI_pdu_session_event_notification_convertToJSON(OpenAPI_pdu_session_event_notification_t *pdu_session_event_notification);
OpenAPI_pdu_session_event_notification_t *OpenAPI_pdu_session_event_notification_copy(OpenAPI_pdu_session_event_notification_t *dst, OpenAPI_pdu_session_event_notification_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_event_notification_H_ */

