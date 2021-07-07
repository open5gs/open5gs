/*
 * max_integrity_protected_data_rate.h
 *
 * 
 */

#ifndef _OpenAPI_max_integrity_protected_data_rate_H_
#define _OpenAPI_max_integrity_protected_data_rate_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_max_integrity_protected_data_rate_NULL = 0, OpenAPI_max_integrity_protected_data_rate__64_KBPS, OpenAPI_max_integrity_protected_data_rate_MAX_UE_RATE } OpenAPI_max_integrity_protected_data_rate_e;

char* OpenAPI_max_integrity_protected_data_rate_ToString(OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate);

OpenAPI_max_integrity_protected_data_rate_e OpenAPI_max_integrity_protected_data_rate_FromString(char* max_integrity_protected_data_rate);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_max_integrity_protected_data_rate_H_ */

