/*
 * cause.h
 *
 * Possible values are - REL_DUE_TO_HO - EPS_FALLBACK - REL_DUE_TO_UP_SEC - DNN_CONGESTION - S_NSSAI_CONGESTION - REL_DUE_TO_REACTIVATION - 5G_AN_NOT_RESPONDING - REL_DUE_TO_SLICE_NOT_AVAILABLE - REL_DUE_TO_DUPLICATE_SESSION_ID - PDU_SESSION_STATUS_MISMATCH - HO_FAILURE - INSUFFICIENT_UP_RESOURCES - PDU_SESSION_HANDED_OVER - PDU_SESSION_RESUMED - CN_ASSISTED_RAN_PARAMETER_TUNING - ISMF_CONTEXT_TRANSFER - SMF_CONTEXT_TRANSFER - REL_DUE_TO_PS_TO_CS_HO - REL_DUE_TO_SUBSCRIPTION_CHANGE - HO_CANCEL - REL_DUE_TO_SLICE_NOT_AUTHORIZED - PDU_SESSION_HAND_OVER_FAILURE - DDN_FAILURE_STATUS
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

typedef struct OpenAPI_cause_s OpenAPI_cause_t;
typedef struct OpenAPI_cause_s {
} OpenAPI_cause_t;

OpenAPI_cause_t *OpenAPI_cause_create(
    );
void OpenAPI_cause_free(OpenAPI_cause_t *cause);
OpenAPI_cause_t *OpenAPI_cause_parseFromJSON(cJSON *causeJSON);
cJSON *OpenAPI_cause_convertToJSON(OpenAPI_cause_t *cause);
OpenAPI_cause_t *OpenAPI_cause_copy(OpenAPI_cause_t *dst, OpenAPI_cause_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_cause_H_ */

