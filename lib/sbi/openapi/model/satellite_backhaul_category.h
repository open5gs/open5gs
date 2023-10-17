/*
 * satellite_backhaul_category.h
 *
 * 
 */

#ifndef _OpenAPI_satellite_backhaul_category_H_
#define _OpenAPI_satellite_backhaul_category_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_satellite_backhaul_category_NULL = 0, OpenAPI_satellite_backhaul_category_GEO, OpenAPI_satellite_backhaul_category_MEO, OpenAPI_satellite_backhaul_category_LEO, OpenAPI_satellite_backhaul_category_OTHER_SAT, OpenAPI_satellite_backhaul_category_NON_SATELLITE } OpenAPI_satellite_backhaul_category_e;

char* OpenAPI_satellite_backhaul_category_ToString(OpenAPI_satellite_backhaul_category_e satellite_backhaul_category);

OpenAPI_satellite_backhaul_category_e OpenAPI_satellite_backhaul_category_FromString(char* satellite_backhaul_category);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_satellite_backhaul_category_H_ */

