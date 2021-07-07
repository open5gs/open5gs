/*
 * roaming_indication.h
 *
 * 
 */

#ifndef _OpenAPI_roaming_indication_H_
#define _OpenAPI_roaming_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_roaming_indication_NULL = 0, OpenAPI_roaming_indication_NON_ROAMING, OpenAPI_roaming_indication_LOCAL_BREAKOUT, OpenAPI_roaming_indication_HOME_ROUTED_ROAMING } OpenAPI_roaming_indication_e;

char* OpenAPI_roaming_indication_ToString(OpenAPI_roaming_indication_e roaming_indication);

OpenAPI_roaming_indication_e OpenAPI_roaming_indication_FromString(char* roaming_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_roaming_indication_H_ */

