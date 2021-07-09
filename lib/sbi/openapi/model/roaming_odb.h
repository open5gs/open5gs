/*
 * roaming_odb.h
 *
 * 
 */

#ifndef _OpenAPI_roaming_odb_H_
#define _OpenAPI_roaming_odb_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_roaming_odb_NULL = 0, OpenAPI_roaming_odb_OUTSIDE_HOME_PLMN, OpenAPI_roaming_odb_OUTSIDE_HOME_PLMN_COUNTRY } OpenAPI_roaming_odb_e;

char* OpenAPI_roaming_odb_ToString(OpenAPI_roaming_odb_e roaming_odb);

OpenAPI_roaming_odb_e OpenAPI_roaming_odb_FromString(char* roaming_odb);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_odb_H_ */

