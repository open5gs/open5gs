/*
 * registration_data_set_name.h
 *
 * 
 */

#ifndef _OpenAPI_registration_data_set_name_H_
#define _OpenAPI_registration_data_set_name_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_registration_data_set_name_NULL = 0, OpenAPI_registration_data_set_name_AMF_3GPP, OpenAPI_registration_data_set_name_AMF_NON_3GPP, OpenAPI_registration_data_set_name_SMF_PDU_SESSIONS, OpenAPI_registration_data_set_name_SMSF_3GPP, OpenAPI_registration_data_set_name_SMSF_NON_3GPP } OpenAPI_registration_data_set_name_e;

char* OpenAPI_registration_data_set_name_ToString(OpenAPI_registration_data_set_name_e registration_data_set_name);

OpenAPI_registration_data_set_name_e OpenAPI_registration_data_set_name_FromString(char* registration_data_set_name);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_registration_data_set_name_H_ */

