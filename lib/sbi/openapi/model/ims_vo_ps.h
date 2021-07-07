/*
 * ims_vo_ps.h
 *
 * 
 */

#ifndef _OpenAPI_ims_vo_ps_H_
#define _OpenAPI_ims_vo_ps_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ims_vo_ps_NULL = 0, OpenAPI_ims_vo_ps_HOMOGENEOUS_SUPPORT, OpenAPI_ims_vo_ps_HOMOGENEOUS_NON_SUPPORT, OpenAPI_ims_vo_ps_NON_HOMOGENEOUS_OR_UNKNOWN } OpenAPI_ims_vo_ps_e;

char* OpenAPI_ims_vo_ps_ToString(OpenAPI_ims_vo_ps_e ims_vo_ps);

OpenAPI_ims_vo_ps_e OpenAPI_ims_vo_ps_FromString(char* ims_vo_ps);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_vo_ps_H_ */

