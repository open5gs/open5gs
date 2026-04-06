/*
 * uli_change_granularity.h
 *
 * 
 */

#ifndef _OpenAPI_uli_change_granularity_H_
#define _OpenAPI_uli_change_granularity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_uli_change_granularity_NULL = 0, OpenAPI_uli_change_granularity_GNB, OpenAPI_uli_change_granularity_TAI } OpenAPI_uli_change_granularity_e;

char* OpenAPI_uli_change_granularity_ToString(OpenAPI_uli_change_granularity_e uli_change_granularity);

OpenAPI_uli_change_granularity_e OpenAPI_uli_change_granularity_FromString(char* uli_change_granularity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uli_change_granularity_H_ */

