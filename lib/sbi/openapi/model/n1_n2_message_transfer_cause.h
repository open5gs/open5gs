/*
 * n1_n2_message_transfer_cause.h
 *
 * 
 */

#ifndef _OpenAPI_n1_n2_message_transfer_cause_H_
#define _OpenAPI_n1_n2_message_transfer_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n1_n2_message_transfer_cause_NULL = 0, OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE, OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED, OpenAPI_n1_n2_message_transfer_cause_WAITING_FOR_ASYNCHRONOUS_TRANSFER, OpenAPI_n1_n2_message_transfer_cause_UE_NOT_RESPONDING, OpenAPI_n1_n2_message_transfer_cause_N1_MSG_NOT_TRANSFERRED, OpenAPI_n1_n2_message_transfer_cause_N2_MSG_NOT_TRANSFERRED, OpenAPI_n1_n2_message_transfer_cause_UE_NOT_REACHABLE_FOR_SESSION, OpenAPI_n1_n2_message_transfer_cause_TEMPORARY_REJECT_REGISTRATION_ONGOING, OpenAPI_n1_n2_message_transfer_cause_TEMPORARY_REJECT_HANDOVER_ONGOING, OpenAPI_n1_n2_message_transfer_cause_REJECTION_DUE_TO_PAGING_RESTRICTION, OpenAPI_n1_n2_message_transfer_cause_AN_NOT_RESPONDING, OpenAPI_n1_n2_message_transfer_cause_FAILURE_CAUSE_UNSPECIFIED } OpenAPI_n1_n2_message_transfer_cause_e;

char* OpenAPI_n1_n2_message_transfer_cause_ToString(OpenAPI_n1_n2_message_transfer_cause_e n1_n2_message_transfer_cause);

OpenAPI_n1_n2_message_transfer_cause_e OpenAPI_n1_n2_message_transfer_cause_FromString(char* n1_n2_message_transfer_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_message_transfer_cause_H_ */

