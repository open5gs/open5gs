/*
 * proximity_criterion.h
 *
 * 
 */

#ifndef _OpenAPI_proximity_criterion_H_
#define _OpenAPI_proximity_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_proximity_criterion_NULL = 0, OpenAPI_proximity_criterion_VELOCITY, OpenAPI_proximity_criterion_AVG_SPD, OpenAPI_proximity_criterion_ORIENTATION, OpenAPI_proximity_criterion_TRAJECTORY } OpenAPI_proximity_criterion_e;

char* OpenAPI_proximity_criterion_ToString(OpenAPI_proximity_criterion_e proximity_criterion);

OpenAPI_proximity_criterion_e OpenAPI_proximity_criterion_FromString(char* proximity_criterion);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_proximity_criterion_H_ */

