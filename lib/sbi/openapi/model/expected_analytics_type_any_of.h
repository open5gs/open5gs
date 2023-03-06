/*
 * expected_analytics_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_expected_analytics_type_any_of_H_
#define _OpenAPI_expected_analytics_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_expected_analytics_type_any_of_NULL = 0, OpenAPI_expected_analytics_type_any_of_MOBILITY, OpenAPI_expected_analytics_type_any_of_COMMUN, OpenAPI_expected_analytics_type_any_of_MOBILITY_AND_COMMUN } OpenAPI_expected_analytics_type_any_of_e;

char* OpenAPI_expected_analytics_type_any_of_ToString(OpenAPI_expected_analytics_type_any_of_e expected_analytics_type_any_of);

OpenAPI_expected_analytics_type_any_of_e OpenAPI_expected_analytics_type_any_of_FromString(char* expected_analytics_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_analytics_type_any_of_H_ */

