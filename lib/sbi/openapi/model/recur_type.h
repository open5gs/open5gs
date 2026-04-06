/*
 * recur_type.h
 *
 * Indicates the recurrence applicable to a time window.
 */

#ifndef _OpenAPI_recur_type_H_
#define _OpenAPI_recur_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_recur_type_NULL = 0, OpenAPI_recur_type_DAILY, OpenAPI_recur_type_WEEKLY, OpenAPI_recur_type_MONTHLY_BY_DATE, OpenAPI_recur_type_MONTHLY_BY_DAY, OpenAPI_recur_type_YEARLY, OpenAPI_recur_type_WEEKDAYS_ONLY, OpenAPI_recur_type_WEEKENDS_ONLY } OpenAPI_recur_type_e;

char* OpenAPI_recur_type_ToString(OpenAPI_recur_type_e recur_type);

OpenAPI_recur_type_e OpenAPI_recur_type_FromString(char* recur_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_recur_type_H_ */

