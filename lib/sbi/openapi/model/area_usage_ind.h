/*
 * area_usage_ind.h
 *
 * Indicates one of the mutually exclusive global settings (whether the UE is allowed to generate and send the reports inside or outside the event report expected area). 
 */

#ifndef _OpenAPI_area_usage_ind_H_
#define _OpenAPI_area_usage_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_area_usage_ind_NULL = 0, OpenAPI_area_usage_ind_INSIDE_REPORTING, OpenAPI_area_usage_ind_OUTSIDE_REPORTING } OpenAPI_area_usage_ind_e;

char* OpenAPI_area_usage_ind_ToString(OpenAPI_area_usage_ind_e area_usage_ind);

OpenAPI_area_usage_ind_e OpenAPI_area_usage_ind_FromString(char* area_usage_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_area_usage_ind_H_ */

