/*
 * av_type.h
 *
 * 
 */

#ifndef _OpenAPI_av_type_H_
#define _OpenAPI_av_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_av_type_NULL = 0, OpenAPI_av_type_5G_HE_AKA, OpenAPI_av_type_EAP_AKA_PRIME } OpenAPI_av_type_e;

char* OpenAPI_av_type_ToString(OpenAPI_av_type_e av_type);

OpenAPI_av_type_e OpenAPI_av_type_FromString(char* av_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_av_type_H_ */

