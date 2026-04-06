/*
 * bluetooth_measurement_1.h
 *
 * contains the bluetooth measurements to be collected for UE.
 */

#ifndef _OpenAPI_bluetooth_measurement_1_H_
#define _OpenAPI_bluetooth_measurement_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_bluetooth_measurement_1_s OpenAPI_bluetooth_measurement_1_t;
#include "bluetooth_rssi.h"
#include "measurement_name.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_bluetooth_measurement_1_s {
    OpenAPI_list_t *measurement_name_list;
    OpenAPI_bluetooth_rssi_e bluetooth_rssi;
};

OpenAPI_bluetooth_measurement_1_t *OpenAPI_bluetooth_measurement_1_create(
    OpenAPI_list_t *measurement_name_list,
    OpenAPI_bluetooth_rssi_e bluetooth_rssi
);
void OpenAPI_bluetooth_measurement_1_free(OpenAPI_bluetooth_measurement_1_t *bluetooth_measurement_1);
OpenAPI_bluetooth_measurement_1_t *OpenAPI_bluetooth_measurement_1_parseFromJSON(cJSON *bluetooth_measurement_1JSON);
cJSON *OpenAPI_bluetooth_measurement_1_convertToJSON(OpenAPI_bluetooth_measurement_1_t *bluetooth_measurement_1);
OpenAPI_bluetooth_measurement_1_t *OpenAPI_bluetooth_measurement_1_copy(OpenAPI_bluetooth_measurement_1_t *dst, OpenAPI_bluetooth_measurement_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bluetooth_measurement_1_H_ */

