/*
 * exception_trend.h
 *
 * 
 */

#ifndef _OpenAPI_exception_trend_H_
#define _OpenAPI_exception_trend_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_exception_trend_NULL = 0, OpenAPI_exception_trend_UP, OpenAPI_exception_trend_DOWN, OpenAPI_exception_trend_UNKNOW, OpenAPI_exception_trend_STABLE } OpenAPI_exception_trend_e;

char* OpenAPI_exception_trend_ToString(OpenAPI_exception_trend_e exception_trend);

OpenAPI_exception_trend_e OpenAPI_exception_trend_FromString(char* exception_trend);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_trend_H_ */

