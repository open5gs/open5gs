/*
 * preemption_control_information.h
 *
 * 
 */

#ifndef _OpenAPI_preemption_control_information_H_
#define _OpenAPI_preemption_control_information_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_preemption_control_information_NULL = 0, OpenAPI_preemption_control_information_MOST_RECENT, OpenAPI_preemption_control_information_LEAST_RECENT, OpenAPI_preemption_control_information_HIGHEST_BW } OpenAPI_preemption_control_information_e;

char* OpenAPI_preemption_control_information_ToString(OpenAPI_preemption_control_information_e preemption_control_information);

OpenAPI_preemption_control_information_e OpenAPI_preemption_control_information_FromString(char* preemption_control_information);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preemption_control_information_H_ */

