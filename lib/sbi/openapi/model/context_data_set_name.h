/*
 * context_data_set_name.h
 *
 * 
 */

#ifndef _OpenAPI_context_data_set_name_H_
#define _OpenAPI_context_data_set_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_context_data_set_name_NULL = 0, OpenAPI_context_data_set_name_AMF_3GPP, OpenAPI_context_data_set_name_AMF_NON_3GPP, OpenAPI_context_data_set_name_SDM_SUBSCRIPTIONS, OpenAPI_context_data_set_name_EE_SUBSCRIPTIONS, OpenAPI_context_data_set_name_SMSF_3GPP, OpenAPI_context_data_set_name_SMSF_NON_3GPP, OpenAPI_context_data_set_name_SUBS_TO_NOTIFY, OpenAPI_context_data_set_name_SMF_REG, OpenAPI_context_data_set_name_IP_SM_GW, OpenAPI_context_data_set_name_ROAMING_INFO, OpenAPI_context_data_set_name_PEI_INFO } OpenAPI_context_data_set_name_e;

char* OpenAPI_context_data_set_name_ToString(OpenAPI_context_data_set_name_e context_data_set_name);

OpenAPI_context_data_set_name_e OpenAPI_context_data_set_name_FromString(char* context_data_set_name);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_context_data_set_name_H_ */

