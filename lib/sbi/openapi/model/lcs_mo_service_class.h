/*
 * lcs_mo_service_class.h
 *
 *
 */

#ifndef _OpenAPI_lcs_mo_service_class_H_
#define _OpenAPI_lcs_mo_service_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lcs_mo_service_class_NULL = 0, OpenAPI_lcs_mo_service_class_BASIC_SELF_LOCATION, OpenAPI_lcs_mo_service_class_AUTONOMOUS_SELF_LOCATION, OpenAPI_lcs_mo_service_class_TRANSFER_TO_THIRD_PARTY } OpenAPI_lcs_mo_service_class_e;

char* OpenAPI_lcs_mo_service_class_ToString(OpenAPI_lcs_mo_service_class_e lcs_mo_service_class);

OpenAPI_lcs_mo_service_class_e OpenAPI_lcs_mo_service_class_FromString(char* lcs_mo_service_class);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_mo_service_class_H_ */

