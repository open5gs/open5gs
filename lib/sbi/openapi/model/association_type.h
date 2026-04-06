/*
 * association_type.h
 *
 * Indicates association type to be included to identify  whether it is IMSI-IMEI or IMSI-IMEISV. 
 */

#ifndef _OpenAPI_association_type_H_
#define _OpenAPI_association_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_association_type_NULL = 0, OpenAPI_association_type_IMEI_CHANGE, OpenAPI_association_type_IMEISV_CHANGE } OpenAPI_association_type_e;

char* OpenAPI_association_type_ToString(OpenAPI_association_type_e association_type);

OpenAPI_association_type_e OpenAPI_association_type_FromString(char* association_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_association_type_H_ */

