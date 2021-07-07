/*
 * lcs_client_class.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_client_class_H_
#define _OpenAPI_lcs_client_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lcs_client_class_NULL = 0, OpenAPI_lcs_client_class_BROADCAST_SERVICE, OpenAPI_lcs_client_class_OM_IN_HPLMN, OpenAPI_lcs_client_class_OM_IN_VPLMN, OpenAPI_lcs_client_class_ANONYMOUS_LOCATION_SERVICE, OpenAPI_lcs_client_class_SPECIFIC_SERVICE } OpenAPI_lcs_client_class_e;

char* OpenAPI_lcs_client_class_ToString(OpenAPI_lcs_client_class_e lcs_client_class);

OpenAPI_lcs_client_class_e OpenAPI_lcs_client_class_FromString(char* lcs_client_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_client_class_H_ */

