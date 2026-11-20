/*
 * range_class.h
 *
 * 
 */

#ifndef _OpenAPI_range_class_H_
#define _OpenAPI_range_class_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_range_class_NULL = 0, OpenAPI_range_class_RESERVED, OpenAPI_range_class__50_METER, OpenAPI_range_class__100_METER, OpenAPI_range_class__200_METER, OpenAPI_range_class__500_METER, OpenAPI_range_class__1000_METER, OpenAPI_range_class_UNUSED } OpenAPI_range_class_e;

char* OpenAPI_range_class_ToString(OpenAPI_range_class_e range_class);

OpenAPI_range_class_e OpenAPI_range_class_FromString(char* range_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_range_class_H_ */

