/*
 * congestion_type.h
 *
 * Possible values are: - USER_PLANE: The congestion analytics type is User Plane.  - CONTROL_PLANE: The congestion analytics type is Control Plane. - USER_AND_CONTROL_PLANE: The congestion analytics type is User Plane and Control Plane. 
 */

#ifndef _OpenAPI_congestion_type_H_
#define _OpenAPI_congestion_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "congestion_type_any_of.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_congestion_type_s OpenAPI_congestion_type_t;
typedef struct OpenAPI_congestion_type_s {
} OpenAPI_congestion_type_t;

OpenAPI_congestion_type_t *OpenAPI_congestion_type_create(
);
void OpenAPI_congestion_type_free(OpenAPI_congestion_type_t *congestion_type);
OpenAPI_congestion_type_t *OpenAPI_congestion_type_parseFromJSON(cJSON *congestion_typeJSON);
cJSON *OpenAPI_congestion_type_convertToJSON(OpenAPI_congestion_type_t *congestion_type);
OpenAPI_congestion_type_t *OpenAPI_congestion_type_copy(OpenAPI_congestion_type_t *dst, OpenAPI_congestion_type_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_congestion_type_H_ */

