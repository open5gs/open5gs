/*
 * steer_mode_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_steer_mode_indicator_H_
#define _OpenAPI_steer_mode_indicator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_steer_mode_indicator_NULL = 0, OpenAPI_steer_mode_indicator_AUTO_LOAD_BALANCE, OpenAPI_steer_mode_indicator_UE_ASSISTANCE } OpenAPI_steer_mode_indicator_e;

char* OpenAPI_steer_mode_indicator_ToString(OpenAPI_steer_mode_indicator_e steer_mode_indicator);

OpenAPI_steer_mode_indicator_e OpenAPI_steer_mode_indicator_FromString(char* steer_mode_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_steer_mode_indicator_H_ */

