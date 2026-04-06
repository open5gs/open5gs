/*
 * bluetooth_measurement.h
 *
 * contains the bluetooth measurements to be collected for UE.
 */

#ifndef _OpenAPI_bluetooth_measurement_H_
#define _OpenAPI_bluetooth_measurement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_bluetooth_measurement_s OpenAPI_bluetooth_measurement_t;
#include "bluetooth_rssi.h"
#include "measurement_name.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_bluetooth_measurement_s {
    OpenAPI_list_t *measurement_name_list;
    OpenAPI_bluetooth_rssi_e bluetooth_rssi;
};

OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_create(
    OpenAPI_list_t *measurement_name_list,
    OpenAPI_bluetooth_rssi_e bluetooth_rssi
);
void OpenAPI_bluetooth_measurement_free(OpenAPI_bluetooth_measurement_t *bluetooth_measurement);
OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_parseFromJSON(cJSON *bluetooth_measurementJSON);
cJSON *OpenAPI_bluetooth_measurement_convertToJSON(OpenAPI_bluetooth_measurement_t *bluetooth_measurement);
OpenAPI_bluetooth_measurement_t *OpenAPI_bluetooth_measurement_copy(OpenAPI_bluetooth_measurement_t *dst, OpenAPI_bluetooth_measurement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bluetooth_measurement_H_ */

