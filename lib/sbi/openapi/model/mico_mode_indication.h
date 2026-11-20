/*
 * mico_mode_indication.h
 *
 * 
 */

#ifndef _OpenAPI_mico_mode_indication_H_
#define _OpenAPI_mico_mode_indication_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_mico_mode_indication_NULL = 0, OpenAPI_mico_mode_indication_MICO_MODE, OpenAPI_mico_mode_indication_NO_MICO_MODE } OpenAPI_mico_mode_indication_e;

char* OpenAPI_mico_mode_indication_ToString(OpenAPI_mico_mode_indication_e mico_mode_indication);

OpenAPI_mico_mode_indication_e OpenAPI_mico_mode_indication_FromString(char* mico_mode_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mico_mode_indication_H_ */

