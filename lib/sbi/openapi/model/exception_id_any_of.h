/*
 * exception_id_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_exception_id_any_of_H_
#define _OpenAPI_exception_id_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_exception_id_any_of_NULL = 0, OpenAPI_exception_id_any_of_UNEXPECTED_UE_LOCATION, OpenAPI_exception_id_any_of_UNEXPECTED_LONG_LIVE_FLOW, OpenAPI_exception_id_any_of_UNEXPECTED_LARGE_RATE_FLOW, OpenAPI_exception_id_any_of_UNEXPECTED_WAKEUP, OpenAPI_exception_id_any_of_SUSPICION_OF_DDOS_ATTACK, OpenAPI_exception_id_any_of_WRONG_DESTINATION_ADDRESS, OpenAPI_exception_id_any_of_TOO_FREQUENT_SERVICE_ACCESS, OpenAPI_exception_id_any_of_UNEXPECTED_RADIO_LINK_FAILURES, OpenAPI_exception_id_any_of_PING_PONG_ACROSS_CELLS } OpenAPI_exception_id_any_of_e;

char* OpenAPI_exception_id_any_of_ToString(OpenAPI_exception_id_any_of_e exception_id_any_of);

OpenAPI_exception_id_any_of_e OpenAPI_exception_id_any_of_FromString(char* exception_id_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_id_any_of_H_ */

