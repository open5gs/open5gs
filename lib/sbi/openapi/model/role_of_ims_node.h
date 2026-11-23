/*
 * role_of_ims_node.h
 *
 * 
 */

#ifndef _OpenAPI_role_of_ims_node_H_
#define _OpenAPI_role_of_ims_node_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_role_of_ims_node_NULL = 0, OpenAPI_role_of_ims_node_ORIGINATING, OpenAPI_role_of_ims_node_TERMINATING, OpenAPI_role_of_ims_node_FORWARDING } OpenAPI_role_of_ims_node_e;

char* OpenAPI_role_of_ims_node_ToString(OpenAPI_role_of_ims_node_e role_of_ims_node);

OpenAPI_role_of_ims_node_e OpenAPI_role_of_ims_node_FromString(char* role_of_ims_node);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_role_of_ims_node_H_ */

