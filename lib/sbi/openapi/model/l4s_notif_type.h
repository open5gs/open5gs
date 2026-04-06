/*
 * l4s_notif_type.h
 *
 * Indicates the notification type for ECN marking for L4S support in 5GS.
 */

#ifndef _OpenAPI_l4s_notif_type_H_
#define _OpenAPI_l4s_notif_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_l4s_notif_type_NULL = 0, OpenAPI_l4s_notif_type_AVAILABLE, OpenAPI_l4s_notif_type_NOT_AVAILABLE } OpenAPI_l4s_notif_type_e;

char* OpenAPI_l4s_notif_type_ToString(OpenAPI_l4s_notif_type_e l4s_notif_type);

OpenAPI_l4s_notif_type_e OpenAPI_l4s_notif_type_FromString(char* l4s_notif_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_l4s_notif_type_H_ */

