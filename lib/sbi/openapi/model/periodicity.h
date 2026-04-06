/*
 * periodicity.h
 *
 * Represents the time period.
 */

#ifndef _OpenAPI_periodicity_H_
#define _OpenAPI_periodicity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_periodicity_NULL = 0, OpenAPI_periodicity_YEARLY, OpenAPI_periodicity_MONTHLY, OpenAPI_periodicity_WEEKLY, OpenAPI_periodicity_DAILY, OpenAPI_periodicity_HOURLY } OpenAPI_periodicity_e;

char* OpenAPI_periodicity_ToString(OpenAPI_periodicity_e periodicity);

OpenAPI_periodicity_e OpenAPI_periodicity_FromString(char* periodicity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodicity_H_ */

