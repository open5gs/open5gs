/*
 * trigger_category.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_category_H_
#define _OpenAPI_trigger_category_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_trigger_category_NULL = 0, OpenAPI_trigger_category_IMMEDIATE_REPORT, OpenAPI_trigger_category_DEFERRED_REPORT } OpenAPI_trigger_category_e;

char* OpenAPI_trigger_category_ToString(OpenAPI_trigger_category_e trigger_category);

OpenAPI_trigger_category_e OpenAPI_trigger_category_FromString(char* trigger_category);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_category_H_ */

