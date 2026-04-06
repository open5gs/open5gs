/*
 * analytics_accuracy_indication.h
 *
 * 
 */

#ifndef _OpenAPI_analytics_accuracy_indication_H_
#define _OpenAPI_analytics_accuracy_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_analytics_accuracy_indication_NULL = 0, OpenAPI_analytics_accuracy_indication_MEET, OpenAPI_analytics_accuracy_indication_NOT_MEET } OpenAPI_analytics_accuracy_indication_e;

char* OpenAPI_analytics_accuracy_indication_ToString(OpenAPI_analytics_accuracy_indication_e analytics_accuracy_indication);

OpenAPI_analytics_accuracy_indication_e OpenAPI_analytics_accuracy_indication_FromString(char* analytics_accuracy_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_accuracy_indication_H_ */

