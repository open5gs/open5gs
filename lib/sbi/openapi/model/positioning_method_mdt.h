/*
 * positioning_method_mdt.h
 *
 * 
 */

#ifndef _OpenAPI_positioning_method_mdt_H_
#define _OpenAPI_positioning_method_mdt_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_positioning_method_mdt_NULL = 0, OpenAPI_positioning_method_mdt_GNSS, OpenAPI_positioning_method_mdt_E_CELL_ID } OpenAPI_positioning_method_mdt_e;

char* OpenAPI_positioning_method_mdt_ToString(OpenAPI_positioning_method_mdt_e positioning_method_mdt);

OpenAPI_positioning_method_mdt_e OpenAPI_positioning_method_mdt_FromString(char* positioning_method_mdt);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_method_mdt_H_ */

