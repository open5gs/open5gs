/*
 * atsss_capability_ext.h
 *
 * 
 */

#ifndef _OpenAPI_atsss_capability_ext_H_
#define _OpenAPI_atsss_capability_ext_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_atsss_capability_ext_NULL = 0, OpenAPI_atsss_capability_ext_MPTCP, OpenAPI_atsss_capability_ext_MPQUIC_UDP, OpenAPI_atsss_capability_ext_MPQUIC_IP, OpenAPI_atsss_capability_ext_MPQUIC_E, OpenAPI_atsss_capability_ext_ATSSS_LL, OpenAPI_atsss_capability_ext_ATSSS_LL_WITH_ASMODE_UL, OpenAPI_atsss_capability_ext_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL, OpenAPI_atsss_capability_ext_ATSSS_LL_WITH_ASMODE_DLUL } OpenAPI_atsss_capability_ext_e;

char* OpenAPI_atsss_capability_ext_ToString(OpenAPI_atsss_capability_ext_e atsss_capability_ext);

OpenAPI_atsss_capability_ext_e OpenAPI_atsss_capability_ext_FromString(char* atsss_capability_ext);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_atsss_capability_ext_H_ */

