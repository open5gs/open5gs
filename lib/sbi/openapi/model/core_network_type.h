/*
 * core_network_type.h
 *
 *
 */

#ifndef _OpenAPI_core_network_type_H_
#define _OpenAPI_core_network_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_core_network_type_NULL = 0, OpenAPI_core_network_type__5GC, OpenAPI_core_network_type_EPC } OpenAPI_core_network_type_e;

char* OpenAPI_core_network_type_ToString(OpenAPI_core_network_type_e core_network_type);

OpenAPI_core_network_type_e OpenAPI_core_network_type_FromString(char* core_network_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_core_network_type_H_ */

