/*
 * presence_state.h
 *
 *
 */

#ifndef _OpenAPI_presence_state_H_
#define _OpenAPI_presence_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_presence_state_NULL = 0, OpenAPI_presence_state_IN_AREA, OpenAPI_presence_state_OUT_OF_AREA, OpenAPI_presence_state_UNKNOWN, OpenAPI_presence_state_INACTIVE } OpenAPI_presence_state_e;

char* OpenAPI_presence_state_ToString(OpenAPI_presence_state_e presence_state);

OpenAPI_presence_state_e OpenAPI_presence_state_FromString(char* presence_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_presence_state_H_ */

