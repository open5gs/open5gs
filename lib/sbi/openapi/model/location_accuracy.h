/*
 * location_accuracy.h
 *
 * Indicates the location accuracy level.
 */

#ifndef _OpenAPI_location_accuracy_H_
#define _OpenAPI_location_accuracy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_accuracy_NULL = 0, OpenAPI_location_accuracy_CELL_LEVEL, OpenAPI_location_accuracy_RAN_NODE_LEVEL, OpenAPI_location_accuracy_TA_LEVEL, OpenAPI_location_accuracy_PLMN_LEVEL, OpenAPI_location_accuracy_N3IWF_LEVEL, OpenAPI_location_accuracy_UE_IP, OpenAPI_location_accuracy_UE_PORT } OpenAPI_location_accuracy_e;

char* OpenAPI_location_accuracy_ToString(OpenAPI_location_accuracy_e location_accuracy);

OpenAPI_location_accuracy_e OpenAPI_location_accuracy_FromString(char* location_accuracy);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_accuracy_H_ */

