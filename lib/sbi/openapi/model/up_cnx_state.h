/*
 * up_cnx_state.h
 *
 *
 */

#ifndef _OpenAPI_up_cnx_state_H_
#define _OpenAPI_up_cnx_state_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_cnx_state_NULL = 0, OpenAPI_up_cnx_state_ACTIVATED, OpenAPI_up_cnx_state_DEACTIVATED, OpenAPI_up_cnx_state_ACTIVATING, OpenAPI_up_cnx_state_SUSPENDED } OpenAPI_up_cnx_state_e;

char* OpenAPI_up_cnx_state_ToString(OpenAPI_up_cnx_state_e up_cnx_state);

OpenAPI_up_cnx_state_e OpenAPI_up_cnx_state_FromString(char* up_cnx_state);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_cnx_state_H_ */

