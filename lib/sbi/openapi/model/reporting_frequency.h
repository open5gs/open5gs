/*
 * reporting_frequency.h
 *
 * 
 */

#ifndef _OpenAPI_reporting_frequency_H_
#define _OpenAPI_reporting_frequency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reporting_frequency_NULL = 0, OpenAPI_reporting_frequency_EVENT_TRIGGERED, OpenAPI_reporting_frequency_PERIODIC, OpenAPI_reporting_frequency_SESSION_RELEASE } OpenAPI_reporting_frequency_e;

char* OpenAPI_reporting_frequency_ToString(OpenAPI_reporting_frequency_e reporting_frequency);

OpenAPI_reporting_frequency_e OpenAPI_reporting_frequency_FromString(char* reporting_frequency);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_frequency_H_ */

