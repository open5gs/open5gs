/*
 * gpsi_type.h
 *
 * 
 */

#ifndef _OpenAPI_gpsi_type_H_
#define _OpenAPI_gpsi_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_gpsi_type_NULL = 0, OpenAPI_gpsi_type_MSISDN, OpenAPI_gpsi_type_EXT_ID, OpenAPI_gpsi_type_EXT_GROUP_ID } OpenAPI_gpsi_type_e;

char* OpenAPI_gpsi_type_ToString(OpenAPI_gpsi_type_e gpsi_type);

OpenAPI_gpsi_type_e OpenAPI_gpsi_type_FromString(char* gpsi_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_gpsi_type_H_ */

