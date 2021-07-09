/*
 * hss_av_type.h
 *
 * 
 */

#ifndef _OpenAPI_hss_av_type_H_
#define _OpenAPI_hss_av_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_hss_av_type_NULL = 0, OpenAPI_hss_av_type_EPS_AKA, OpenAPI_hss_av_type_EAP_AKA, OpenAPI_hss_av_type_IMS_AKA, OpenAPI_hss_av_type_GBA_AKA, OpenAPI_hss_av_type_UMTS_AKA } OpenAPI_hss_av_type_e;

char* OpenAPI_hss_av_type_ToString(OpenAPI_hss_av_type_e hss_av_type);

OpenAPI_hss_av_type_e OpenAPI_hss_av_type_FromString(char* hss_av_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hss_av_type_H_ */

