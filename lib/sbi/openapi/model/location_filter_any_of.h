/*
 * location_filter_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_location_filter_any_of_H_
#define _OpenAPI_location_filter_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_filter_any_of_NULL = 0, OpenAPI_location_filter_any_of_TAI, OpenAPI_location_filter_any_of_CELL_ID, OpenAPI_location_filter_any_of_RAN_NODE, OpenAPI_location_filter_any_of_N3IWF, OpenAPI_location_filter_any_of_UE_IP, OpenAPI_location_filter_any_of_UDP_PORT, OpenAPI_location_filter_any_of_TNAP_ID, OpenAPI_location_filter_any_of_GLI, OpenAPI_location_filter_any_of_TWAP_ID } OpenAPI_location_filter_any_of_e;

char* OpenAPI_location_filter_any_of_ToString(OpenAPI_location_filter_any_of_e location_filter_any_of);

OpenAPI_location_filter_any_of_e OpenAPI_location_filter_any_of_FromString(char* location_filter_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_filter_any_of_H_ */

