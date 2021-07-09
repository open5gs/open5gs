/*
 * reserv_priority.h
 *
 * 
 */

#ifndef _OpenAPI_reserv_priority_H_
#define _OpenAPI_reserv_priority_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reserv_priority_NULL = 0, OpenAPI_reserv_priority_PRIO_1, OpenAPI_reserv_priority_PRIO_2, OpenAPI_reserv_priority_PRIO_3, OpenAPI_reserv_priority_PRIO_4, OpenAPI_reserv_priority_PRIO_5, OpenAPI_reserv_priority_PRIO_6, OpenAPI_reserv_priority_PRIO_7, OpenAPI_reserv_priority_PRIO_8, OpenAPI_reserv_priority_PRIO_9, OpenAPI_reserv_priority_PRIO_10, OpenAPI_reserv_priority_PRIO_11, OpenAPI_reserv_priority_PRIO_12, OpenAPI_reserv_priority_PRIO_13, OpenAPI_reserv_priority_PRIO_14, OpenAPI_reserv_priority_PRIO_15, OpenAPI_reserv_priority_PRIO_16 } OpenAPI_reserv_priority_e;

char* OpenAPI_reserv_priority_ToString(OpenAPI_reserv_priority_e reserv_priority);

OpenAPI_reserv_priority_e OpenAPI_reserv_priority_FromString(char* reserv_priority);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reserv_priority_H_ */

