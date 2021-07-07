/*
 * up_interface_type.h
 *
 * 
 */

#ifndef _OpenAPI_up_interface_type_H_
#define _OpenAPI_up_interface_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_interface_type_NULL = 0, OpenAPI_up_interface_type_N3, OpenAPI_up_interface_type_N6, OpenAPI_up_interface_type_N9, OpenAPI_up_interface_type_DATA_FORWARDING } OpenAPI_up_interface_type_e;

char* OpenAPI_up_interface_type_ToString(OpenAPI_up_interface_type_e up_interface_type);

OpenAPI_up_interface_type_e OpenAPI_up_interface_type_FromString(char* up_interface_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_interface_type_H_ */

