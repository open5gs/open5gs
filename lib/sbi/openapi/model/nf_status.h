/*
 * nf_status.h
 *
 *
 */

#ifndef _OpenAPI_nf_status_H_
#define _OpenAPI_nf_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nf_status_NULL = 0, OpenAPI_nf_status_REGISTERED, OpenAPI_nf_status_SUSPENDED, OpenAPI_nf_status_UNDISCOVERABLE } OpenAPI_nf_status_e;

char* OpenAPI_nf_status_ToString(OpenAPI_nf_status_e nf_status);

OpenAPI_nf_status_e OpenAPI_nf_status_FromString(char* nf_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_status_H_ */

