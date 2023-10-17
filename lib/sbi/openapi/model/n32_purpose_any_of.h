/*
 * n32_purpose_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_n32_purpose_any_of_H_
#define _OpenAPI_n32_purpose_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_n32_purpose_any_of_NULL = 0, OpenAPI_n32_purpose_any_of_ROAMING, OpenAPI_n32_purpose_any_of_INTER_PLMN_MOBILITY, OpenAPI_n32_purpose_any_of_SMS_INTERCONNECT, OpenAPI_n32_purpose_any_of_ROAMING_TEST, OpenAPI_n32_purpose_any_of_INTER_PLMN_MOBILITY_TEST, OpenAPI_n32_purpose_any_of_SMS_INTERCONNECT_TEST, OpenAPI_n32_purpose_any_of_SNPN_INTERCONNECT, OpenAPI_n32_purpose_any_of_SNPN_INTERCONNECT_TEST, OpenAPI_n32_purpose_any_of_DISASTER_ROAMING, OpenAPI_n32_purpose_any_of_DISASTER_ROAMING_TEST } OpenAPI_n32_purpose_any_of_e;

char* OpenAPI_n32_purpose_any_of_ToString(OpenAPI_n32_purpose_any_of_e n32_purpose_any_of);

OpenAPI_n32_purpose_any_of_e OpenAPI_n32_purpose_any_of_FromString(char* n32_purpose_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n32_purpose_any_of_H_ */

