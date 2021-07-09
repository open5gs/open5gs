/*
 * reporting_trigger.h
 *
 * 
 */

#ifndef _OpenAPI_reporting_trigger_H_
#define _OpenAPI_reporting_trigger_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reporting_trigger_NULL = 0, OpenAPI_reporting_trigger_PERIODICAL, OpenAPI_reporting_trigger_EVENT_A2, OpenAPI_reporting_trigger_EVENT_A2_PERIODIC, OpenAPI_reporting_trigger_ALL_RRM_EVENT_TRIGGERS } OpenAPI_reporting_trigger_e;

char* OpenAPI_reporting_trigger_ToString(OpenAPI_reporting_trigger_e reporting_trigger);

OpenAPI_reporting_trigger_e OpenAPI_reporting_trigger_FromString(char* reporting_trigger);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_trigger_H_ */

