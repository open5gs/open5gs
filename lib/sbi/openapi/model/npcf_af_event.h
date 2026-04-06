/*
 * npcf_af_event.h
 *
 * Represents an event to notify to the AF.
 */

#ifndef _OpenAPI_npcf_af_event_H_
#define _OpenAPI_npcf_af_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_npcf_af_event_NULL = 0, OpenAPI_npcf_af_event_ACCESS_TYPE_CHANGE, OpenAPI_npcf_af_event_ANI_REPORT, OpenAPI_npcf_af_event_APP_DETECTION, OpenAPI_npcf_af_event_CHARGING_CORRELATION, OpenAPI_npcf_af_event_EPS_FALLBACK, OpenAPI_npcf_af_event_EXTRA_UE_ADDR, OpenAPI_npcf_af_event_FAILED_QOS_UPDATE, OpenAPI_npcf_af_event_FAILED_RESOURCES_ALLOCATION, OpenAPI_npcf_af_event_OUT_OF_CREDIT, OpenAPI_npcf_af_event_PDU_SESSION_STATUS, OpenAPI_npcf_af_event_PLMN_CHG, OpenAPI_npcf_af_event_QOS_MONITORING, OpenAPI_npcf_af_event_QOS_MON_CAP_REPO, OpenAPI_npcf_af_event_QOS_NOTIF, OpenAPI_npcf_af_event_RAN_NAS_CAUSE, OpenAPI_npcf_af_event_REALLOCATION_OF_CREDIT, OpenAPI_npcf_af_event_SAT_CATEGORY_CHG, OpenAPI_npcf_af_event_SUCCESSFUL_QOS_UPDATE, OpenAPI_npcf_af_event_SUCCESSFUL_RESOURCES_ALLOCATION, OpenAPI_npcf_af_event_TSN_BRIDGE_INFO, OpenAPI_npcf_af_event_UP_PATH_CHG_FAILURE, OpenAPI_npcf_af_event_USAGE_REPORT, OpenAPI_npcf_af_event_UE_REACH_STATUS_CH, OpenAPI_npcf_af_event_BAT_OFFSET_INFO, OpenAPI_npcf_af_event_URSP_ENF_INFO, OpenAPI_npcf_af_event_PACK_DEL_VAR, OpenAPI_npcf_af_event_L4S_SUPP, OpenAPI_npcf_af_event_RT_DELAY_TWO_QOS_FLOWS, OpenAPI_npcf_af_event_RATE_LIMIT_INFO_REPO, OpenAPI_npcf_af_event_UP_PATH_CHANGE, OpenAPI_npcf_af_event_CN_HEALTH_MONITOR, OpenAPI_npcf_af_event_UE_TEMPORARILY_UNAVAILABLE } OpenAPI_npcf_af_event_e;

char* OpenAPI_npcf_af_event_ToString(OpenAPI_npcf_af_event_e npcf_af_event);

OpenAPI_npcf_af_event_e OpenAPI_npcf_af_event_FromString(char* npcf_af_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_npcf_af_event_H_ */

