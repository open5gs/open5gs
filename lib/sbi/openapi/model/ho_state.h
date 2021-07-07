/*
 * ho_state.h
 *
 * 
 */

#ifndef _OpenAPI_ho_state_H_
#define _OpenAPI_ho_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ho_state_NULL = 0, OpenAPI_ho_state_NONE, OpenAPI_ho_state_PREPARING, OpenAPI_ho_state_PREPARED, OpenAPI_ho_state_COMPLETED, OpenAPI_ho_state_CANCELLED } OpenAPI_ho_state_e;

char* OpenAPI_ho_state_ToString(OpenAPI_ho_state_e ho_state);

OpenAPI_ho_state_e OpenAPI_ho_state_FromString(char* ho_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ho_state_H_ */

