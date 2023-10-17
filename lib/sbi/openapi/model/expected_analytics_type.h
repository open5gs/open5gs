/*
 * expected_analytics_type.h
 *
 * Possible values are: - MOBILITY: Mobility related abnormal behaviour analytics is expected by the consumer. - COMMUN: Communication related abnormal behaviour analytics is expected by the consumer. - MOBILITY_AND_COMMUN: Both mobility and communication related abnormal behaviour analytics is expected by the consumer. 
 */

#ifndef _OpenAPI_expected_analytics_type_H_
#define _OpenAPI_expected_analytics_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "expected_analytics_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_expected_analytics_type_s OpenAPI_expected_analytics_type_t;
typedef struct OpenAPI_expected_analytics_type_s {
} OpenAPI_expected_analytics_type_t;

OpenAPI_expected_analytics_type_t *OpenAPI_expected_analytics_type_create(
);
void OpenAPI_expected_analytics_type_free(OpenAPI_expected_analytics_type_t *expected_analytics_type);
OpenAPI_expected_analytics_type_t *OpenAPI_expected_analytics_type_parseFromJSON(cJSON *expected_analytics_typeJSON);
cJSON *OpenAPI_expected_analytics_type_convertToJSON(OpenAPI_expected_analytics_type_t *expected_analytics_type);
OpenAPI_expected_analytics_type_t *OpenAPI_expected_analytics_type_copy(OpenAPI_expected_analytics_type_t *dst, OpenAPI_expected_analytics_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_analytics_type_H_ */

