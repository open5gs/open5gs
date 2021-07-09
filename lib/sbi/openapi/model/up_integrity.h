/*
 * up_integrity.h
 *
 * 
 */

#ifndef _OpenAPI_up_integrity_H_
#define _OpenAPI_up_integrity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_integrity_NULL = 0, OpenAPI_up_integrity_REQUIRED, OpenAPI_up_integrity_PREFERRED, OpenAPI_up_integrity_NOT_NEEDED } OpenAPI_up_integrity_e;

char* OpenAPI_up_integrity_ToString(OpenAPI_up_integrity_e up_integrity);

OpenAPI_up_integrity_e OpenAPI_up_integrity_FromString(char* up_integrity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_integrity_H_ */

