/*
 * location_privacy_ind.h
 *
 * 
 */

#ifndef _OpenAPI_location_privacy_ind_H_
#define _OpenAPI_location_privacy_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_location_privacy_ind_NULL = 0, OpenAPI_location_privacy_ind_LOCATION_DISALLOWED, OpenAPI_location_privacy_ind_LOCATION_ALLOWED } OpenAPI_location_privacy_ind_e;

char* OpenAPI_location_privacy_ind_ToString(OpenAPI_location_privacy_ind_e location_privacy_ind);

OpenAPI_location_privacy_ind_e OpenAPI_location_privacy_ind_FromString(char* location_privacy_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_location_privacy_ind_H_ */

