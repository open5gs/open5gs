/*
 * periodicity_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_periodicity_any_of_H_
#define _OpenAPI_periodicity_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_periodicity_any_of_NULL = 0, OpenAPI_periodicity_any_of_YEARLY, OpenAPI_periodicity_any_of_MONTHLY, OpenAPI_periodicity_any_of_WEEKLY, OpenAPI_periodicity_any_of_DAILY, OpenAPI_periodicity_any_of_HOURLY } OpenAPI_periodicity_any_of_e;

char* OpenAPI_periodicity_any_of_ToString(OpenAPI_periodicity_any_of_e periodicity_any_of);

OpenAPI_periodicity_any_of_e OpenAPI_periodicity_any_of_FromString(char* periodicity_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodicity_any_of_H_ */

