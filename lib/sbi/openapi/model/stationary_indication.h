/*
 * stationary_indication.h
 *
 *
 */

#ifndef _OpenAPI_stationary_indication_H_
#define _OpenAPI_stationary_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_stationary_indication_NULL = 0, OpenAPI_stationary_indication_STATIONARY, OpenAPI_stationary_indication_MOBILE } OpenAPI_stationary_indication_e;

char* OpenAPI_stationary_indication_ToString(OpenAPI_stationary_indication_e stationary_indication);

OpenAPI_stationary_indication_e OpenAPI_stationary_indication_FromString(char* stationary_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_stationary_indication_H_ */

