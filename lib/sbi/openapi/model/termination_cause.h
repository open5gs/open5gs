/*
 * termination_cause.h
 *
 * Indicates the cause behind requesting the deletion of the Individual Application Session Context resource. 
 */

#ifndef _OpenAPI_termination_cause_H_
#define _OpenAPI_termination_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_termination_cause_NULL = 0, OpenAPI_termination_cause_ALL_SDF_DEACTIVATION, OpenAPI_termination_cause_PDU_SESSION_TERMINATION, OpenAPI_termination_cause_PS_TO_CS_HO, OpenAPI_termination_cause_INSUFFICIENT_SERVER_RESOURCES, OpenAPI_termination_cause_INSUFFICIENT_QOS_FLOW_RESOURCES, OpenAPI_termination_cause_SPONSORED_DATA_CONNECTIVITY_DISALLOWED, OpenAPI_termination_cause_REQUEST_QOS_NOT_SUPPORTED_IN_PLMN, OpenAPI_termination_cause_UE_ADDR_RELEASE, OpenAPI_termination_cause_SMF_FAILURE, OpenAPI_termination_cause_REFLECTIVE_QOS_NOT_SUPPORTED_IN_UE } OpenAPI_termination_cause_e;

char* OpenAPI_termination_cause_ToString(OpenAPI_termination_cause_e termination_cause);

OpenAPI_termination_cause_e OpenAPI_termination_cause_FromString(char* termination_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_cause_H_ */

