/*
 * measurement_name.h
 *
 * contains the measurement name for measurement collection.
 */

#ifndef _OpenAPI_measurement_name_H_
#define _OpenAPI_measurement_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_measurement_name_s OpenAPI_measurement_name_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_measurement_name_s {
    char *bluetooth_name;
    char *wlan_name;
};

OpenAPI_measurement_name_t *OpenAPI_measurement_name_create(
    char *bluetooth_name,
    char *wlan_name
);
void OpenAPI_measurement_name_free(OpenAPI_measurement_name_t *measurement_name);
OpenAPI_measurement_name_t *OpenAPI_measurement_name_parseFromJSON(cJSON *measurement_nameJSON);
cJSON *OpenAPI_measurement_name_convertToJSON(OpenAPI_measurement_name_t *measurement_name);
OpenAPI_measurement_name_t *OpenAPI_measurement_name_copy(OpenAPI_measurement_name_t *dst, OpenAPI_measurement_name_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_measurement_name_H_ */

