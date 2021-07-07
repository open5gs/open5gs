/*
 * association_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_association_type_any_of_H_
#define _OpenAPI_association_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_association_type_any_of_NULL = 0, OpenAPI_association_type_any_of_IMEI_CHANGE, OpenAPI_association_type_any_of_IMEISV_CHANGE } OpenAPI_association_type_any_of_e;

char* OpenAPI_association_type_any_of_ToString(OpenAPI_association_type_any_of_e association_type_any_of);

OpenAPI_association_type_any_of_e OpenAPI_association_type_any_of_FromString(char* association_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_association_type_any_of_H_ */

