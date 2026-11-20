/*
 * time_distribution_method.h
 *
 * 
 */

#ifndef _OpenAPI_time_distribution_method_H_
#define _OpenAPI_time_distribution_method_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_time_distribution_method_NULL = 0, OpenAPI_time_distribution_method_GPTP, OpenAPI_time_distribution_method_ASTI } OpenAPI_time_distribution_method_e;

char* OpenAPI_time_distribution_method_ToString(OpenAPI_time_distribution_method_e time_distribution_method);

OpenAPI_time_distribution_method_e OpenAPI_time_distribution_method_FromString(char* time_distribution_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_distribution_method_H_ */

