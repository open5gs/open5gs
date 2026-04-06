/*
 * expected_analytics_type.h
 *
 * 
 */

#ifndef _OpenAPI_expected_analytics_type_H_
#define _OpenAPI_expected_analytics_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_expected_analytics_type_NULL = 0, OpenAPI_expected_analytics_type_MOBILITY, OpenAPI_expected_analytics_type_COMMUN, OpenAPI_expected_analytics_type_MOBILITY_AND_COMMUN } OpenAPI_expected_analytics_type_e;

char* OpenAPI_expected_analytics_type_ToString(OpenAPI_expected_analytics_type_e expected_analytics_type);

OpenAPI_expected_analytics_type_e OpenAPI_expected_analytics_type_FromString(char* expected_analytics_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_expected_analytics_type_H_ */

