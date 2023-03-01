/*
 * npcf_atsss_capability.h
 *
 * 
 */

#ifndef _OpenAPI_npcf_atsss_capability_H_
#define _OpenAPI_npcf_atsss_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_npcf_atsss_capability_NULL = 0, OpenAPI_npcf_atsss_capability_MPTCP_ATSSS_LL_WITH_ASMODE_UL, OpenAPI_npcf_atsss_capability_MPTCP_ATSSS_LL_WITH_EXSDMODE_DL_ASMODE_UL, OpenAPI_npcf_atsss_capability_MPTCP_ATSSS_LL_WITH_ASMODE_DLUL, OpenAPI_npcf_atsss_capability_ATSSS_LL, OpenAPI_npcf_atsss_capability_MPTCP_ATSSS_LL } OpenAPI_npcf_atsss_capability_e;

char* OpenAPI_npcf_atsss_capability_ToString(OpenAPI_npcf_atsss_capability_e npcf_atsss_capability);

OpenAPI_npcf_atsss_capability_e OpenAPI_npcf_atsss_capability_FromString(char* npcf_atsss_capability);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_npcf_atsss_capability_H_ */

