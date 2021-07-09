/*
 * reporting_level.h
 *
 * 
 */

#ifndef _OpenAPI_reporting_level_H_
#define _OpenAPI_reporting_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reporting_level_NULL = 0, OpenAPI_reporting_level_SER_ID_LEVEL, OpenAPI_reporting_level_RAT_GR_LEVEL, OpenAPI_reporting_level_SPON_CON_LEVEL } OpenAPI_reporting_level_e;

char* OpenAPI_reporting_level_ToString(OpenAPI_reporting_level_e reporting_level);

OpenAPI_reporting_level_e OpenAPI_reporting_level_FromString(char* reporting_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reporting_level_H_ */

