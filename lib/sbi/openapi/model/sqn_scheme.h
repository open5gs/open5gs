/*
 * sqn_scheme.h
 *
 * 
 */

#ifndef _OpenAPI_sqn_scheme_H_
#define _OpenAPI_sqn_scheme_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sqn_scheme_NULL = 0, OpenAPI_sqn_scheme_GENERAL, OpenAPI_sqn_scheme_NON_TIME_BASED, OpenAPI_sqn_scheme_TIME_BASED } OpenAPI_sqn_scheme_e;

char* OpenAPI_sqn_scheme_ToString(OpenAPI_sqn_scheme_e sqn_scheme);

OpenAPI_sqn_scheme_e OpenAPI_sqn_scheme_FromString(char* sqn_scheme);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sqn_scheme_H_ */

