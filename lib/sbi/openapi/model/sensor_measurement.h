/*
 * sensor_measurement.h
 *
 * 
 */

#ifndef _OpenAPI_sensor_measurement_H_
#define _OpenAPI_sensor_measurement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sensor_measurement_NULL = 0, OpenAPI_sensor_measurement_BAROMETRIC_PRESSURE, OpenAPI_sensor_measurement_UE_SPEED, OpenAPI_sensor_measurement_UE_ORIENTATION } OpenAPI_sensor_measurement_e;

char* OpenAPI_sensor_measurement_ToString(OpenAPI_sensor_measurement_e sensor_measurement);

OpenAPI_sensor_measurement_e OpenAPI_sensor_measurement_FromString(char* sensor_measurement);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sensor_measurement_H_ */

