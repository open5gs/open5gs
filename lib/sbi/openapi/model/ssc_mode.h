/*
 * ssc_mode.h
 *
 * 
 */

#ifndef _OpenAPI_ssc_mode_H_
#define _OpenAPI_ssc_mode_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ssc_mode_NULL = 0, OpenAPI_ssc_mode_SSC_MODE_1, OpenAPI_ssc_mode_SSC_MODE_2, OpenAPI_ssc_mode_SSC_MODE_3 } OpenAPI_ssc_mode_e;

char* OpenAPI_ssc_mode_ToString(OpenAPI_ssc_mode_e ssc_mode);

OpenAPI_ssc_mode_e OpenAPI_ssc_mode_FromString(char* ssc_mode);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ssc_mode_H_ */

