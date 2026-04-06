/*
 * provisioned_data_set_name.h
 *
 * The name of data set.
 */

#ifndef _OpenAPI_provisioned_data_set_name_H_
#define _OpenAPI_provisioned_data_set_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_provisioned_data_set_name_NULL = 0, OpenAPI_provisioned_data_set_name_AM, OpenAPI_provisioned_data_set_name_SMF_SEL, OpenAPI_provisioned_data_set_name_SMS_SUB, OpenAPI_provisioned_data_set_name_SM, OpenAPI_provisioned_data_set_name_TRACE, OpenAPI_provisioned_data_set_name_SMS_MNG, OpenAPI_provisioned_data_set_name_LCS_PRIVACY, OpenAPI_provisioned_data_set_name_LCS_MO, OpenAPI_provisioned_data_set_name_LCS_BCA, OpenAPI_provisioned_data_set_name_LCS_SUB, OpenAPI_provisioned_data_set_name_V2X, OpenAPI_provisioned_data_set_name_PROSE, OpenAPI_provisioned_data_set_name_ODB, OpenAPI_provisioned_data_set_name_EE_PROF, OpenAPI_provisioned_data_set_name_PP_PROF, OpenAPI_provisioned_data_set_name_NIDD_AUTH, OpenAPI_provisioned_data_set_name_USER_CONSENT, OpenAPI_provisioned_data_set_name_MBS, OpenAPI_provisioned_data_set_name_PP_DATA, OpenAPI_provisioned_data_set_name_A2X, OpenAPI_provisioned_data_set_name_RANGINGSL_PRIVACY } OpenAPI_provisioned_data_set_name_e;

char* OpenAPI_provisioned_data_set_name_ToString(OpenAPI_provisioned_data_set_name_e provisioned_data_set_name);

OpenAPI_provisioned_data_set_name_e OpenAPI_provisioned_data_set_name_FromString(char* provisioned_data_set_name);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_provisioned_data_set_name_H_ */

