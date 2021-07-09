/*
 * up_confidentiality.h
 *
 * 
 */

#ifndef _OpenAPI_up_confidentiality_H_
#define _OpenAPI_up_confidentiality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_up_confidentiality_NULL = 0, OpenAPI_up_confidentiality_REQUIRED, OpenAPI_up_confidentiality_PREFERRED, OpenAPI_up_confidentiality_NOT_NEEDED } OpenAPI_up_confidentiality_e;

char* OpenAPI_up_confidentiality_ToString(OpenAPI_up_confidentiality_e up_confidentiality);

OpenAPI_up_confidentiality_e OpenAPI_up_confidentiality_FromString(char* up_confidentiality);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_up_confidentiality_H_ */

