/*
 * measurement_nr_for_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_measurement_nr_for_mdt_H_
#define _OpenAPI_measurement_nr_for_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_measurement_nr_for_mdt_NULL = 0, OpenAPI_measurement_nr_for_mdt_M1, OpenAPI_measurement_nr_for_mdt_M2, OpenAPI_measurement_nr_for_mdt_M3, OpenAPI_measurement_nr_for_mdt_M4_DL, OpenAPI_measurement_nr_for_mdt_M4_UL, OpenAPI_measurement_nr_for_mdt_M5_DL, OpenAPI_measurement_nr_for_mdt_M5_UL, OpenAPI_measurement_nr_for_mdt_M6_DL, OpenAPI_measurement_nr_for_mdt_M6_UL, OpenAPI_measurement_nr_for_mdt_M7_DL, OpenAPI_measurement_nr_for_mdt_M7_UL, OpenAPI_measurement_nr_for_mdt_M8, OpenAPI_measurement_nr_for_mdt_M9 } OpenAPI_measurement_nr_for_mdt_e;

char* OpenAPI_measurement_nr_for_mdt_ToString(OpenAPI_measurement_nr_for_mdt_e measurement_nr_for_mdt);

OpenAPI_measurement_nr_for_mdt_e OpenAPI_measurement_nr_for_mdt_FromString(char* measurement_nr_for_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_measurement_nr_for_mdt_H_ */

