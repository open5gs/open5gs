/*
 * reporting_urgency.h
 *
 * Reporting Urgency
 */

#ifndef _OpenAPI_reporting_urgency_H_
#define _OpenAPI_reporting_urgency_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reporting_urgency_NULL = 0, OpenAPI_reporting_urgency_DELAY_TOLERANT, OpenAPI_reporting_urgency_NON_DELAY_TOLERANT } OpenAPI_reporting_urgency_e;

char* OpenAPI_reporting_urgency_ToString(OpenAPI_reporting_urgency_e reporting_urgency);

OpenAPI_reporting_urgency_e OpenAPI_reporting_urgency_FromString(char* reporting_urgency);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_urgency_H_ */

