/*
 * access_network_id.h
 *
 * 
 */

#ifndef _OpenAPI_access_network_id_H_
#define _OpenAPI_access_network_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_network_id_NULL = 0, OpenAPI_access_network_id_HRPD, OpenAPI_access_network_id_WIMAX, OpenAPI_access_network_id_WLAN, OpenAPI_access_network_id_ETHERNET } OpenAPI_access_network_id_e;

char* OpenAPI_access_network_id_ToString(OpenAPI_access_network_id_e access_network_id);

OpenAPI_access_network_id_e OpenAPI_access_network_id_FromString(char* access_network_id);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_network_id_H_ */

