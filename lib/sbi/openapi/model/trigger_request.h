/*
 * trigger_request.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_request_H_
#define _OpenAPI_trigger_request_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "pcscf_address.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_trigger_request_s OpenAPI_trigger_request_t;
typedef struct OpenAPI_trigger_request_s {
    char *supi;
    struct OpenAPI_pcscf_address_s *failed_pcscf;
} OpenAPI_trigger_request_t;

OpenAPI_trigger_request_t *OpenAPI_trigger_request_create(
    char *supi,
    OpenAPI_pcscf_address_t *failed_pcscf
);
void OpenAPI_trigger_request_free(OpenAPI_trigger_request_t *trigger_request);
OpenAPI_trigger_request_t *OpenAPI_trigger_request_parseFromJSON(cJSON *trigger_requestJSON);
cJSON *OpenAPI_trigger_request_convertToJSON(OpenAPI_trigger_request_t *trigger_request);
OpenAPI_trigger_request_t *OpenAPI_trigger_request_copy(OpenAPI_trigger_request_t *dst, OpenAPI_trigger_request_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_request_H_ */

