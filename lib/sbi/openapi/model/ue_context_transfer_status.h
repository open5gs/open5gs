/*
 * ue_context_transfer_status.h
 *
 *
 */

#ifndef _OpenAPI_ue_context_transfer_status_H_
#define _OpenAPI_ue_context_transfer_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_context_transfer_status_NULL = 0, OpenAPI_ue_context_transfer_status_TRANSFERRED, OpenAPI_ue_context_transfer_status_NOT_TRANSFERRED } OpenAPI_ue_context_transfer_status_e;

char* OpenAPI_ue_context_transfer_status_ToString(OpenAPI_ue_context_transfer_status_e ue_context_transfer_status);

OpenAPI_ue_context_transfer_status_e OpenAPI_ue_context_transfer_status_FromString(char* ue_context_transfer_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_context_transfer_status_H_ */

