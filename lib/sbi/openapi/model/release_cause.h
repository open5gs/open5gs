/*
 * release_cause.h
 *
 * 
 */

#ifndef _OpenAPI_release_cause_H_
#define _OpenAPI_release_cause_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_release_cause_NULL = 0, OpenAPI_release_cause_SNPN_SNPN_MOBILITY, OpenAPI_release_cause_NO_HR_AGREEMENT, OpenAPI_release_cause_UNSPECIFIED } OpenAPI_release_cause_e;

char* OpenAPI_release_cause_ToString(OpenAPI_release_cause_e release_cause);

OpenAPI_release_cause_e OpenAPI_release_cause_FromString(char* release_cause);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_release_cause_H_ */

