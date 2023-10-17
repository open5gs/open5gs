/*
 * exception.h
 *
 * Represents the Exception information.
 */

#ifndef _OpenAPI_exception_H_
#define _OpenAPI_exception_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "exception_id.h"
#include "exception_trend.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_exception_s OpenAPI_exception_t;
typedef struct OpenAPI_exception_s {
    struct OpenAPI_exception_id_s *excep_id;
    bool is_excep_level;
    int excep_level;
    struct OpenAPI_exception_trend_s *excep_trend;
} OpenAPI_exception_t;

OpenAPI_exception_t *OpenAPI_exception_create(
    OpenAPI_exception_id_t *excep_id,
    bool is_excep_level,
    int excep_level,
    OpenAPI_exception_trend_t *excep_trend
);
void OpenAPI_exception_free(OpenAPI_exception_t *exception);
OpenAPI_exception_t *OpenAPI_exception_parseFromJSON(cJSON *exceptionJSON);
cJSON *OpenAPI_exception_convertToJSON(OpenAPI_exception_t *exception);
OpenAPI_exception_t *OpenAPI_exception_copy(OpenAPI_exception_t *dst, OpenAPI_exception_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_exception_H_ */

