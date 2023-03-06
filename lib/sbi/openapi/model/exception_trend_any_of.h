/*
 * exception_trend_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_exception_trend_any_of_H_
#define _OpenAPI_exception_trend_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_exception_trend_any_of_NULL = 0, OpenAPI_exception_trend_any_of_UP, OpenAPI_exception_trend_any_of_DOWN, OpenAPI_exception_trend_any_of_UNKNOW, OpenAPI_exception_trend_any_of_STABLE } OpenAPI_exception_trend_any_of_e;

char* OpenAPI_exception_trend_any_of_ToString(OpenAPI_exception_trend_any_of_e exception_trend_any_of);

OpenAPI_exception_trend_any_of_e OpenAPI_exception_trend_any_of_FromString(char* exception_trend_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_trend_any_of_H_ */

