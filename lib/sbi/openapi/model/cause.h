/*
 * cause.h
 *
 *
 */

#ifndef _OpenAPI_cause_H_
#define _OpenAPI_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_cause_NULL = 0, OpenAPI_cause_REL_DUE_TO_HO, OpenAPI_cause_EPS_FALLBACK, OpenAPI_cause_REL_DUE_TO_UP_SEC, OpenAPI_cause_DNN_CONGESTION, OpenAPI_cause_S_NSSAI_CONGESTION, OpenAPI_cause_REL_DUE_TO_REACTIVATION, OpenAPI_cause__5G_AN_NOT_RESPONDING, OpenAPI_cause_REL_DUE_TO_SLICE_NOT_AVAILABLE, OpenAPI_cause_REL_DUE_TO_DUPLICATE_SESSION_ID, OpenAPI_cause_PDU_SESSION_STATUS_MISMATCH, OpenAPI_cause_HO_FAILURE, OpenAPI_cause_INSUFFICIENT_UP_RESOURCES, OpenAPI_cause_PDU_SESSION_HANDED_OVER, OpenAPI_cause_PDU_SESSION_RESUMED, OpenAPI_cause_CN_ASSISTED_RAN_PARAMETER_TUNING, OpenAPI_cause_ISMF_CONTEXT_TRANSFER, OpenAPI_cause_SMF_CONTEXT_TRANSFER, OpenAPI_cause_REL_DUE_TO_PS_TO_CS_HO, OpenAPI_cause_REL_DUE_TO_SUBSCRIPTION_CHANGE, OpenAPI_cause_HO_CANCEL, OpenAPI_cause_REL_DUE_TO_SLICE_NOT_AUTHORIZED, OpenAPI_cause_PDU_SESSION_HAND_OVER_FAILURE, OpenAPI_cause_DDN_FAILURE_STATUS } OpenAPI_cause_e;

char* OpenAPI_cause_ToString(OpenAPI_cause_e cause);

OpenAPI_cause_e OpenAPI_cause_FromString(char* cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cause_H_ */

