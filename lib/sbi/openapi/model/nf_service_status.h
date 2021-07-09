/*
 * nf_service_status.h
 *
 * 
 */

#ifndef _OpenAPI_nf_service_status_H_
#define _OpenAPI_nf_service_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nf_service_status_NULL = 0, OpenAPI_nf_service_status_REGISTERED, OpenAPI_nf_service_status_SUSPENDED, OpenAPI_nf_service_status_UNDISCOVERABLE } OpenAPI_nf_service_status_e;

char* OpenAPI_nf_service_status_ToString(OpenAPI_nf_service_status_e nf_service_status);

OpenAPI_nf_service_status_e OpenAPI_nf_service_status_FromString(char* nf_service_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_service_status_H_ */

