/*
 * exception_id.h
 *
 * Possible values are: - UNEXPECTED_UE_LOCATION: Unexpected UE location - UNEXPECTED_LONG_LIVE_FLOW: Unexpected long-live rate flows - UNEXPECTED_LARGE_RATE_FLOW: Unexpected large rate flows - UNEXPECTED_WAKEUP: Unexpected wakeup - SUSPICION_OF_DDOS_ATTACK: Suspicion of DDoS attack - WRONG_DESTINATION_ADDRESS: Wrong destination address - TOO_FREQUENT_SERVICE_ACCESS: Too frequent Service Access - UNEXPECTED_RADIO_LINK_FAILURES: Unexpected radio link failures - PING_PONG_ACROSS_CELLS: Ping-ponging across neighbouring cells 
 */

#ifndef _OpenAPI_exception_id_H_
#define _OpenAPI_exception_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "exception_id_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exception_id_s OpenAPI_exception_id_t;
typedef struct OpenAPI_exception_id_s {
} OpenAPI_exception_id_t;

OpenAPI_exception_id_t *OpenAPI_exception_id_create(
);
void OpenAPI_exception_id_free(OpenAPI_exception_id_t *exception_id);
OpenAPI_exception_id_t *OpenAPI_exception_id_parseFromJSON(cJSON *exception_idJSON);
cJSON *OpenAPI_exception_id_convertToJSON(OpenAPI_exception_id_t *exception_id);
OpenAPI_exception_id_t *OpenAPI_exception_id_copy(OpenAPI_exception_id_t *dst, OpenAPI_exception_id_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_id_H_ */

