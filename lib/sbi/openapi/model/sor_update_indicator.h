/*
 * sor_update_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_sor_update_indicator_H_
#define _OpenAPI_sor_update_indicator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sor_update_indicator_NULL = 0, OpenAPI_sor_update_indicator_INITIAL_REGISTRATION, OpenAPI_sor_update_indicator_EMERGENCY_REGISTRATION } OpenAPI_sor_update_indicator_e;

char* OpenAPI_sor_update_indicator_ToString(OpenAPI_sor_update_indicator_e sor_update_indicator);

OpenAPI_sor_update_indicator_e OpenAPI_sor_update_indicator_FromString(char* sor_update_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sor_update_indicator_H_ */

