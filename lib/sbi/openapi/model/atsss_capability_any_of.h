/*
 * atsss_capability_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_atsss_capability_any_of_H_
#define _OpenAPI_atsss_capability_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_atsss_capability_any_of_NULL = 0, OpenAPI_atsss_capability_any_of_MPTCP_ATSSS_LL_WITH_ASMODE_UL, OpenAPI_atsss_capability_any_of_MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL, OpenAPI_atsss_capability_any_of_MPTCP_ATSSS_LL_WITH_ASMODE_DLUL, OpenAPI_atsss_capability_any_of_ATSSS_LL, OpenAPI_atsss_capability_any_of_MPTCP_ATSSS_LL } OpenAPI_atsss_capability_any_of_e;

char* OpenAPI_atsss_capability_any_of_ToString(OpenAPI_atsss_capability_any_of_e atsss_capability_any_of);

OpenAPI_atsss_capability_any_of_e OpenAPI_atsss_capability_any_of_FromString(char* atsss_capability_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_atsss_capability_any_of_H_ */

