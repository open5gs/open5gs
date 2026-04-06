/*
 * prose_direct_allowed.h
 *
 * Indicates the 5G ProSe Direct services that can be authorised to  use in the given PLMN for the UE. 
 */

#ifndef _OpenAPI_prose_direct_allowed_H_
#define _OpenAPI_prose_direct_allowed_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_prose_direct_allowed_NULL = 0, OpenAPI_prose_direct_allowed_ANNOUNCE, OpenAPI_prose_direct_allowed_MONITOR, OpenAPI_prose_direct_allowed_RESTRICTD_ANNOUNCE, OpenAPI_prose_direct_allowed_RESTRICTD_MONITOR, OpenAPI_prose_direct_allowed_DISCOVERER, OpenAPI_prose_direct_allowed_DISCOVEREE, OpenAPI_prose_direct_allowed_BROADCAST, OpenAPI_prose_direct_allowed_GROUPCAST, OpenAPI_prose_direct_allowed_UNICAST, OpenAPI_prose_direct_allowed_LAYER2_RELAY, OpenAPI_prose_direct_allowed_LAYER3_RELAY, OpenAPI_prose_direct_allowed_LAYER3_REMOTE, OpenAPI_prose_direct_allowed_LAYER2_UE_RELAY, OpenAPI_prose_direct_allowed_LAYER3_UE_RELAY, OpenAPI_prose_direct_allowed_LAYER2_END, OpenAPI_prose_direct_allowed_LAYER3_END, OpenAPI_prose_direct_allowed_LAYER2_RELAY_INTERMEDIATE, OpenAPI_prose_direct_allowed_LAYER3_RELAY_INTERMEDIATE, OpenAPI_prose_direct_allowed_LAYER2_RELAY_MULTI_HOP, OpenAPI_prose_direct_allowed_LAYER3_RELAY_MULTI_HOP, OpenAPI_prose_direct_allowed_LAYER3_UE_RELAY_MULTI_HOP, OpenAPI_prose_direct_allowed_LAYER3_END_MULTI_HOP, OpenAPI_prose_direct_allowed_LAYER2_REMOTE_MULTI_HOP } OpenAPI_prose_direct_allowed_e;

char* OpenAPI_prose_direct_allowed_ToString(OpenAPI_prose_direct_allowed_e prose_direct_allowed);

OpenAPI_prose_direct_allowed_e OpenAPI_prose_direct_allowed_FromString(char* prose_direct_allowed);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_direct_allowed_H_ */

