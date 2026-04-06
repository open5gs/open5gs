/*
 * model_5_gvn_group_communication_type.h
 *
 * indicates the type of the 5G VN group associated with 5G VN group communication.
 */

#ifndef _OpenAPI_model_5_gvn_group_communication_type_H_
#define _OpenAPI_model_5_gvn_group_communication_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_model_5_gvn_group_communication_type_NULL = 0, OpenAPI_model_5_gvn_group_communication_type_WITH_N6_BASED_FORWARDING, OpenAPI_model_5_gvn_group_communication_type_WITHOUT_N6_BASED_FORWARDING } OpenAPI_model_5_gvn_group_communication_type_e;

char* OpenAPI_model_5_gvn_group_communication_type_ToString(OpenAPI_model_5_gvn_group_communication_type_e model_5_gvn_group_communication_type);

OpenAPI_model_5_gvn_group_communication_type_e OpenAPI_model_5_gvn_group_communication_type_FromString(char* model_5_gvn_group_communication_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_model_5_gvn_group_communication_type_H_ */

