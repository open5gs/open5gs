/*
 * qos_notif_type.h
 *
 * 
 */

#ifndef _OpenAPI_qos_notif_type_H_
#define _OpenAPI_qos_notif_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_qos_notif_type_NULL = 0, OpenAPI_qos_notif_type_GUARANTEED, OpenAPI_qos_notif_type_NOT_GUARANTEED } OpenAPI_qos_notif_type_e;

char* OpenAPI_qos_notif_type_ToString(OpenAPI_qos_notif_type_e qos_notif_type);

OpenAPI_qos_notif_type_e OpenAPI_qos_notif_type_FromString(char* qos_notif_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_qos_notif_type_H_ */

