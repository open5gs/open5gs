/*
 * fl_capability_type.h
 *
 * Type of Federated Learning Capability. 
 */

#ifndef _OpenAPI_fl_capability_type_H_
#define _OpenAPI_fl_capability_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_fl_capability_type_NULL = 0, OpenAPI_fl_capability_type_FL_SERVER, OpenAPI_fl_capability_type_FL_CLIENT, OpenAPI_fl_capability_type_FL_SERVER_AND_CLIENT } OpenAPI_fl_capability_type_e;

char* OpenAPI_fl_capability_type_ToString(OpenAPI_fl_capability_type_e fl_capability_type);

OpenAPI_fl_capability_type_e OpenAPI_fl_capability_type_FromString(char* fl_capability_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_fl_capability_type_H_ */

