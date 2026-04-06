/*
 * termination_indication.h
 *
 * Indicates the termination of Network Slice Replacement.
 */

#ifndef _OpenAPI_termination_indication_H_
#define _OpenAPI_termination_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_termination_indication_NULL = 0, OpenAPI_termination_indication_NEW_UES_TERMINATION, OpenAPI_termination_indication_ALL_UES_TERMINATION } OpenAPI_termination_indication_e;

char* OpenAPI_termination_indication_ToString(OpenAPI_termination_indication_e termination_indication);

OpenAPI_termination_indication_e OpenAPI_termination_indication_FromString(char* termination_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_termination_indication_H_ */

