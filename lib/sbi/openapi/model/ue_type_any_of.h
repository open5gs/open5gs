/*
 * ue_type_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_ue_type_any_of_H_
#define _OpenAPI_ue_type_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_type_any_of_NULL = 0, OpenAPI_ue_type_any_of_AERIAL_UE } OpenAPI_ue_type_any_of_e;

char* OpenAPI_ue_type_any_of_ToString(OpenAPI_ue_type_any_of_e ue_type_any_of);

OpenAPI_ue_type_any_of_e OpenAPI_ue_type_any_of_FromString(char* ue_type_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_type_any_of_H_ */

