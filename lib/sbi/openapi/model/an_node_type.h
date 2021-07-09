/*
 * an_node_type.h
 *
 * 
 */

#ifndef _OpenAPI_an_node_type_H_
#define _OpenAPI_an_node_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_an_node_type_NULL = 0, OpenAPI_an_node_type_GNB, OpenAPI_an_node_type_NG_ENB } OpenAPI_an_node_type_e;

char* OpenAPI_an_node_type_ToString(OpenAPI_an_node_type_e an_node_type);

OpenAPI_an_node_type_e OpenAPI_an_node_type_FromString(char* an_node_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_an_node_type_H_ */

