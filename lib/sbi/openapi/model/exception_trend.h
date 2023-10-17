/*
 * exception_trend.h
 *
 * Possible values are: - UP: Up trend of the exception level. - DOWN: Down trend of the exception level. - UNKNOW: Unknown trend of the exception level. - STABLE: Stable trend of the exception level. 
 */

#ifndef _OpenAPI_exception_trend_H_
#define _OpenAPI_exception_trend_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "exception_trend_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exception_trend_s OpenAPI_exception_trend_t;
typedef struct OpenAPI_exception_trend_s {
} OpenAPI_exception_trend_t;

OpenAPI_exception_trend_t *OpenAPI_exception_trend_create(
);
void OpenAPI_exception_trend_free(OpenAPI_exception_trend_t *exception_trend);
OpenAPI_exception_trend_t *OpenAPI_exception_trend_parseFromJSON(cJSON *exception_trendJSON);
cJSON *OpenAPI_exception_trend_convertToJSON(OpenAPI_exception_trend_t *exception_trend);
OpenAPI_exception_trend_t *OpenAPI_exception_trend_copy(OpenAPI_exception_trend_t *dst, OpenAPI_exception_trend_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_trend_H_ */

