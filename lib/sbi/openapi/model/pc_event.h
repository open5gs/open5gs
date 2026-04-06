/*
 * pc_event.h
 *
 * Represents the policy control events that can be subscribed.
 */

#ifndef _OpenAPI_pc_event_H_
#define _OpenAPI_pc_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_pc_event_NULL = 0, OpenAPI_pc_event_AC_TY_CH, OpenAPI_pc_event_PLMN_CH, OpenAPI_pc_event_SAC_CH, OpenAPI_pc_event_SAT_CATEGORY_CH, OpenAPI_pc_event_SUCCESS_UE_POL_DEL_SP, OpenAPI_pc_event_UNSUCCESS_UE_POL_DEL_SP, OpenAPI_pc_event_UNSUCCESS_PCF_SERVICE_AUTHORIZATION, OpenAPI_pc_event_APPLICATION_START, OpenAPI_pc_event_APPLICATION_STOP, OpenAPI_pc_event_RATE_LIMIT_INFO_REPO, OpenAPI_pc_event_SIGNALLING_INFO, OpenAPI_pc_event_SLICE_REPLACE_OUTCOME, OpenAPI_pc_event_PARTLY_UNSUCC_UE_POL_DEL_SP } OpenAPI_pc_event_e;

char* OpenAPI_pc_event_ToString(OpenAPI_pc_event_e pc_event);

OpenAPI_pc_event_e OpenAPI_pc_event_FromString(char* pc_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pc_event_H_ */

