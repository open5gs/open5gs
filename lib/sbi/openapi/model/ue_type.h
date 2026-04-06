/*
 * ue_type.h
 *
 * Describes the type of UEs
 */

#ifndef _OpenAPI_ue_type_H_
#define _OpenAPI_ue_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_type_NULL = 0, OpenAPI_ue_type_AERIAL_UE } OpenAPI_ue_type_e;

char* OpenAPI_ue_type_ToString(OpenAPI_ue_type_e ue_type);

OpenAPI_ue_type_e OpenAPI_ue_type_FromString(char* ue_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_type_H_ */

