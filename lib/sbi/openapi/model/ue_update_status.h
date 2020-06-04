/*
 * ue_update_status.h
 *
 *
 */

#ifndef _OpenAPI_ue_update_status_H_
#define _OpenAPI_ue_update_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_update_status_NULL = 0, OpenAPI_ue_update_status_NOT_SENT, OpenAPI_ue_update_status_SENT_NO_ACK_REQUIRED, OpenAPI_ue_update_status_WAITING_FOR_ACK, OpenAPI_ue_update_status_ACK_RECEIVED } OpenAPI_ue_update_status_e;

char* OpenAPI_ue_update_status_ToString(OpenAPI_ue_update_status_e ue_update_status);

OpenAPI_ue_update_status_e OpenAPI_ue_update_status_FromString(char* ue_update_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_update_status_H_ */

