/*
 * snssai_status.h
 *
 * 
 */

#ifndef _OpenAPI_snssai_status_H_
#define _OpenAPI_snssai_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_snssai_status_NULL = 0, OpenAPI_snssai_status_AVAILABLE, OpenAPI_snssai_status_UNAVAILABLE } OpenAPI_snssai_status_e;

char* OpenAPI_snssai_status_ToString(OpenAPI_snssai_status_e snssai_status);

OpenAPI_snssai_status_e OpenAPI_snssai_status_FromString(char* snssai_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_snssai_status_H_ */

