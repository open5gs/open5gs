/*
 * supplementary_service_type.h
 *
 * 
 */

#ifndef _OpenAPI_supplementary_service_type_H_
#define _OpenAPI_supplementary_service_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_supplementary_service_type_NULL = 0, OpenAPI_supplementary_service_type_OIP, OpenAPI_supplementary_service_type_OIR, OpenAPI_supplementary_service_type_TIP, OpenAPI_supplementary_service_type_TIR, OpenAPI_supplementary_service_type_HOLD, OpenAPI_supplementary_service_type_CB, OpenAPI_supplementary_service_type_CDIV, OpenAPI_supplementary_service_type_CW, OpenAPI_supplementary_service_type_MWI, OpenAPI_supplementary_service_type_CONF, OpenAPI_supplementary_service_type_FA, OpenAPI_supplementary_service_type_CCBS, OpenAPI_supplementary_service_type_CCNR, OpenAPI_supplementary_service_type_MCID, OpenAPI_supplementary_service_type_CAT, OpenAPI_supplementary_service_type_CUG, OpenAPI_supplementary_service_type_PNM, OpenAPI_supplementary_service_type_CRS, OpenAPI_supplementary_service_type_ECT } OpenAPI_supplementary_service_type_e;

char* OpenAPI_supplementary_service_type_ToString(OpenAPI_supplementary_service_type_e supplementary_service_type);

OpenAPI_supplementary_service_type_e OpenAPI_supplementary_service_type_FromString(char* supplementary_service_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supplementary_service_type_H_ */

