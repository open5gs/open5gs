/*
 * traffic_parameter_meas.h
 *
 * Indicates the traffic parameters to be measured.
 */

#ifndef _OpenAPI_traffic_parameter_meas_H_
#define _OpenAPI_traffic_parameter_meas_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_traffic_parameter_meas_NULL = 0, OpenAPI_traffic_parameter_meas_DL_N6_JITTER, OpenAPI_traffic_parameter_meas_DL_PERIOD, OpenAPI_traffic_parameter_meas_UL_PERIOD } OpenAPI_traffic_parameter_meas_e;

char* OpenAPI_traffic_parameter_meas_ToString(OpenAPI_traffic_parameter_meas_e traffic_parameter_meas);

OpenAPI_traffic_parameter_meas_e OpenAPI_traffic_parameter_meas_FromString(char* traffic_parameter_meas);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_parameter_meas_H_ */

