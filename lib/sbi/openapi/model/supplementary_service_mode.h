/*
 * supplementary_service_mode.h
 *
 * 
 */

#ifndef _OpenAPI_supplementary_service_mode_H_
#define _OpenAPI_supplementary_service_mode_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_supplementary_service_mode_NULL = 0, OpenAPI_supplementary_service_mode_CFU, OpenAPI_supplementary_service_mode_CFB, OpenAPI_supplementary_service_mode_CFNR, OpenAPI_supplementary_service_mode_CFNL, OpenAPI_supplementary_service_mode_CD, OpenAPI_supplementary_service_mode_CFNRC, OpenAPI_supplementary_service_mode_ICB, OpenAPI_supplementary_service_mode_OCB, OpenAPI_supplementary_service_mode_ACR, OpenAPI_supplementary_service_mode_BLIND_TRANFER, OpenAPI_supplementary_service_mode_CONSULTATIVE_TRANFER } OpenAPI_supplementary_service_mode_e;

char* OpenAPI_supplementary_service_mode_ToString(OpenAPI_supplementary_service_mode_e supplementary_service_mode);

OpenAPI_supplementary_service_mode_e OpenAPI_supplementary_service_mode_FromString(char* supplementary_service_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supplementary_service_mode_H_ */

