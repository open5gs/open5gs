/*
 * af_notif_method.h
 *
 * 
 */

#ifndef _OpenAPI_af_notif_method_H_
#define _OpenAPI_af_notif_method_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_af_notif_method_NULL = 0, OpenAPI_af_notif_method_EVENT_DETECTION, OpenAPI_af_notif_method_ONE_TIME, OpenAPI_af_notif_method_PERIODIC, OpenAPI_af_notif_method_PDU_SESSION_RELEASE } OpenAPI_af_notif_method_e;

char* OpenAPI_af_notif_method_ToString(OpenAPI_af_notif_method_e af_notif_method);

OpenAPI_af_notif_method_e OpenAPI_af_notif_method_FromString(char* af_notif_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_notif_method_H_ */

