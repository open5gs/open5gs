/*
 * node_type.h
 *
 * 
 */

#ifndef _OpenAPI_node_type_H_
#define _OpenAPI_node_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_node_type_NULL = 0, OpenAPI_node_type_AUSF, OpenAPI_node_type_VLR, OpenAPI_node_type_SGSN, OpenAPI_node_type_S_CSCF, OpenAPI_node_type_BSF, OpenAPI_node_type_GAN_AAA_SERVER, OpenAPI_node_type_WLAN_AAA_SERVER, OpenAPI_node_type_MME } OpenAPI_node_type_e;

char* OpenAPI_node_type_ToString(OpenAPI_node_type_e node_type);

OpenAPI_node_type_e OpenAPI_node_type_FromString(char* node_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_node_type_H_ */

