/*
 * location_filter.h
 *
 * Describes the supported filters of LOCATION_REPORT event type
 */

#ifndef _OpenAPI_location_filter_H_
#define _OpenAPI_location_filter_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_filter_NULL = 0, OpenAPI_location_filter_TAI, OpenAPI_location_filter_CELL_ID, OpenAPI_location_filter_RAN_NODE, OpenAPI_location_filter_N3IWF, OpenAPI_location_filter_UE_IP, OpenAPI_location_filter_UDP_PORT, OpenAPI_location_filter_TNAP_ID, OpenAPI_location_filter_GLI, OpenAPI_location_filter_TWAP_ID } OpenAPI_location_filter_e;

char* OpenAPI_location_filter_ToString(OpenAPI_location_filter_e location_filter);

OpenAPI_location_filter_e OpenAPI_location_filter_FromString(char* location_filter);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_filter_H_ */

