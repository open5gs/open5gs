/*
 * data_set_name.h
 *
 * 
 */

#ifndef _OpenAPI_data_set_name_H_
#define _OpenAPI_data_set_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_data_set_name_NULL = 0, OpenAPI_data_set_name_AM, OpenAPI_data_set_name_SMF_SEL, OpenAPI_data_set_name_UEC_SMF, OpenAPI_data_set_name_UEC_SMSF, OpenAPI_data_set_name_SMS_SUB, OpenAPI_data_set_name_SM, OpenAPI_data_set_name_TRACE, OpenAPI_data_set_name_SMS_MNG, OpenAPI_data_set_name_LCS_PRIVACY, OpenAPI_data_set_name_LCS_MO, OpenAPI_data_set_name_UEC_AMF, OpenAPI_data_set_name_V2X, OpenAPI_data_set_name_LCS_BCA, OpenAPI_data_set_name_PROSE, OpenAPI_data_set_name_UC, OpenAPI_data_set_name_MBS } OpenAPI_data_set_name_e;

char* OpenAPI_data_set_name_ToString(OpenAPI_data_set_name_e data_set_name);

OpenAPI_data_set_name_e OpenAPI_data_set_name_FromString(char* data_set_name);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_data_set_name_H_ */

