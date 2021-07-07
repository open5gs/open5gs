/*
 * periodic_communication_indicator.h
 *
 * 
 */

#ifndef _OpenAPI_periodic_communication_indicator_H_
#define _OpenAPI_periodic_communication_indicator_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_periodic_communication_indicator_NULL = 0, OpenAPI_periodic_communication_indicator_PIORIODICALLY, OpenAPI_periodic_communication_indicator_ON_DEMAND } OpenAPI_periodic_communication_indicator_e;

char* OpenAPI_periodic_communication_indicator_ToString(OpenAPI_periodic_communication_indicator_e periodic_communication_indicator);

OpenAPI_periodic_communication_indicator_e OpenAPI_periodic_communication_indicator_FromString(char* periodic_communication_indicator);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_periodic_communication_indicator_H_ */

