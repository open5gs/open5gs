/*
 * traffic_forwarding_way.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_forwarding_way_H_
#define _OpenAPI_traffic_forwarding_way_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_traffic_forwarding_way_NULL = 0, OpenAPI_traffic_forwarding_way_N6, OpenAPI_traffic_forwarding_way_N19, OpenAPI_traffic_forwarding_way_LOCAL_SWITCH } OpenAPI_traffic_forwarding_way_e;

char* OpenAPI_traffic_forwarding_way_ToString(OpenAPI_traffic_forwarding_way_e traffic_forwarding_way);

OpenAPI_traffic_forwarding_way_e OpenAPI_traffic_forwarding_way_FromString(char* traffic_forwarding_way);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_forwarding_way_H_ */

