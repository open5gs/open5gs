/*
 * notif_cap.h
 *
 * Indicates whether the notified capability is supported or not supported.
 */

#ifndef _OpenAPI_notif_cap_H_
#define _OpenAPI_notif_cap_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notif_cap_NULL = 0, OpenAPI_notif_cap_SUPPORTED, OpenAPI_notif_cap_NOT_SUPPORTED } OpenAPI_notif_cap_e;

char* OpenAPI_notif_cap_ToString(OpenAPI_notif_cap_e notif_cap);

OpenAPI_notif_cap_e OpenAPI_notif_cap_FromString(char* notif_cap);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notif_cap_H_ */

