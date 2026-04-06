/*
 * nsac_admission_mode.h
 *
 * Indicates the NSAC admission mode applied in roaming case. 
 */

#ifndef _OpenAPI_nsac_admission_mode_H_
#define _OpenAPI_nsac_admission_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nsac_admission_mode_NULL = 0, OpenAPI_nsac_admission_mode_VPLMN_ADMISSION, OpenAPI_nsac_admission_mode_VPLMN_WITH_HPLMN_ASSISTANCE, OpenAPI_nsac_admission_mode_HPLMN_ADMISSION } OpenAPI_nsac_admission_mode_e;

char* OpenAPI_nsac_admission_mode_ToString(OpenAPI_nsac_admission_mode_e nsac_admission_mode);

OpenAPI_nsac_admission_mode_e OpenAPI_nsac_admission_mode_FromString(char* nsac_admission_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nsac_admission_mode_H_ */

