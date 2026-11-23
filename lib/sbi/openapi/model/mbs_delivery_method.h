/*
 * mbs_delivery_method.h
 *
 * 
 */

#ifndef _OpenAPI_mbs_delivery_method_H_
#define _OpenAPI_mbs_delivery_method_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mbs_delivery_method_NULL = 0, OpenAPI_mbs_delivery_method_SHARED, OpenAPI_mbs_delivery_method_INDIVIDUAL } OpenAPI_mbs_delivery_method_e;

char* OpenAPI_mbs_delivery_method_ToString(OpenAPI_mbs_delivery_method_e mbs_delivery_method);

OpenAPI_mbs_delivery_method_e OpenAPI_mbs_delivery_method_FromString(char* mbs_delivery_method);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mbs_delivery_method_H_ */

