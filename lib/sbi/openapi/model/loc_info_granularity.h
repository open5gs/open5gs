/*
 * loc_info_granularity.h
 *
 * 
 */

#ifndef _OpenAPI_loc_info_granularity_H_
#define _OpenAPI_loc_info_granularity_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_loc_info_granularity_NULL = 0, OpenAPI_loc_info_granularity_TA_LEVEL, OpenAPI_loc_info_granularity_CELL_LEVEL, OpenAPI_loc_info_granularity_LON_AND_LAT_LEVEL, OpenAPI_loc_info_granularity_LON_LAT_ALT_LEVEL } OpenAPI_loc_info_granularity_e;

char* OpenAPI_loc_info_granularity_ToString(OpenAPI_loc_info_granularity_e loc_info_granularity);

OpenAPI_loc_info_granularity_e OpenAPI_loc_info_granularity_FromString(char* loc_info_granularity);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_loc_info_granularity_H_ */

