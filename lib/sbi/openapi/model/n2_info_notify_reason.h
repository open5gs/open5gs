/*
 * n2_info_notify_reason.h
 *
 *
 */

#ifndef _OpenAPI_n2_info_notify_reason_H_
#define _OpenAPI_n2_info_notify_reason_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n2_info_notify_reason_NULL = 0, OpenAPI_n2_info_notify_reason_HANDOVER_COMPLETED } OpenAPI_n2_info_notify_reason_e;

char* OpenAPI_n2_info_notify_reason_ToString(OpenAPI_n2_info_notify_reason_e n2_info_notify_reason);

OpenAPI_n2_info_notify_reason_e OpenAPI_n2_info_notify_reason_FromString(char* n2_info_notify_reason);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n2_info_notify_reason_H_ */

