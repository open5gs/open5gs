/*
 * uc_purpose.h
 *
 * 
 */

#ifndef _OpenAPI_uc_purpose_H_
#define _OpenAPI_uc_purpose_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_uc_purpose_NULL = 0, OpenAPI_uc_purpose_ANALYTICS, OpenAPI_uc_purpose_MODEL_TRAINING, OpenAPI_uc_purpose_NW_CAP_EXPOSURE, OpenAPI_uc_purpose_EDGEAPP_UE_LOCATION } OpenAPI_uc_purpose_e;

char* OpenAPI_uc_purpose_ToString(OpenAPI_uc_purpose_e uc_purpose);

OpenAPI_uc_purpose_e OpenAPI_uc_purpose_FromString(char* uc_purpose);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uc_purpose_H_ */

