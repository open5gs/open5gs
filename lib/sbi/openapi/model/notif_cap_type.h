/*
 * notif_cap_type.h
 *
 * 
 */

#ifndef _OpenAPI_notif_cap_type_H_
#define _OpenAPI_notif_cap_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_notif_cap_type_NULL = 0, OpenAPI_notif_cap_type_PACKET_DELAY, OpenAPI_notif_cap_type_CONGESTION, OpenAPI_notif_cap_type_AVAILABLE_BITRATE } OpenAPI_notif_cap_type_e;

char* OpenAPI_notif_cap_type_ToString(OpenAPI_notif_cap_type_e notif_cap_type);

OpenAPI_notif_cap_type_e OpenAPI_notif_cap_type_FromString(char* notif_cap_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_notif_cap_type_H_ */

