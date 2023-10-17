/*
 * npcf_multicast_access_control.h
 *
 * 
 */

#ifndef _OpenAPI_npcf_multicast_access_control_H_
#define _OpenAPI_npcf_multicast_access_control_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_npcf_multicast_access_control_NULL = 0, OpenAPI_npcf_multicast_access_control_ALLOWED, OpenAPI_npcf_multicast_access_control_NOT_ALLOWED } OpenAPI_npcf_multicast_access_control_e;

char* OpenAPI_npcf_multicast_access_control_ToString(OpenAPI_npcf_multicast_access_control_e npcf_multicast_access_control);

OpenAPI_npcf_multicast_access_control_e OpenAPI_npcf_multicast_access_control_FromString(char* npcf_multicast_access_control);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_npcf_multicast_access_control_H_ */

