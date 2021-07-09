/*
 * ip_reachability.h
 *
 * 
 */

#ifndef _OpenAPI_ip_reachability_H_
#define _OpenAPI_ip_reachability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ip_reachability_NULL = 0, OpenAPI_ip_reachability_IPV4, OpenAPI_ip_reachability_IPV6, OpenAPI_ip_reachability_IPV4V6 } OpenAPI_ip_reachability_e;

char* OpenAPI_ip_reachability_ToString(OpenAPI_ip_reachability_e ip_reachability);

OpenAPI_ip_reachability_e OpenAPI_ip_reachability_FromString(char* ip_reachability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ip_reachability_H_ */

