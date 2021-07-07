/*
 * trigger_category_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_trigger_category_any_of_H_
#define _OpenAPI_trigger_category_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_trigger_category_any_of_NULL = 0, OpenAPI_trigger_category_any_of_IMMEDIATE_REPORT, OpenAPI_trigger_category_any_of_DEFERRED_REPORT } OpenAPI_trigger_category_any_of_e;

char* OpenAPI_trigger_category_any_of_ToString(OpenAPI_trigger_category_any_of_e trigger_category_any_of);

OpenAPI_trigger_category_any_of_e OpenAPI_trigger_category_any_of_FromString(char* trigger_category_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_trigger_category_any_of_H_ */

