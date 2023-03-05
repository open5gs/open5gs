/*
 * n32_purpose.h
 *
 * 
 */

#ifndef _OpenAPI_n32_purpose_H_
#define _OpenAPI_n32_purpose_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n32_purpose_NULL = 0, OpenAPI_n32_purpose_ROAMING, OpenAPI_n32_purpose_INTER_PLMN_MOBILITY, OpenAPI_n32_purpose_SMS_INTERCONNECT, OpenAPI_n32_purpose_ROAMING_TEST, OpenAPI_n32_purpose_INTER_PLMN_MOBILITY_TEST, OpenAPI_n32_purpose_SMS_INTERCONNECT_TEST, OpenAPI_n32_purpose_SNPN_INTERCONNECT, OpenAPI_n32_purpose_SNPN_INTERCONNECT_TEST, OpenAPI_n32_purpose_DISASTER_ROAMING, OpenAPI_n32_purpose_DISASTER_ROAMING_TEST } OpenAPI_n32_purpose_e;

char* OpenAPI_n32_purpose_ToString(OpenAPI_n32_purpose_e n32_purpose);

OpenAPI_n32_purpose_e OpenAPI_n32_purpose_FromString(char* n32_purpose);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32_purpose_H_ */

