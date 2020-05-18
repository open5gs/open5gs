/*
 * uri_scheme.h
 *
 *
 */

#ifndef _OpenAPI_uri_scheme_H_
#define _OpenAPI_uri_scheme_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_uri_scheme_NULL = 0, OpenAPI_uri_scheme_http, OpenAPI_uri_scheme_https } OpenAPI_uri_scheme_e;

char* OpenAPI_uri_scheme_ToString(OpenAPI_uri_scheme_e uri_scheme);

OpenAPI_uri_scheme_e OpenAPI_uri_scheme_FromString(char* uri_scheme);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uri_scheme_H_ */

