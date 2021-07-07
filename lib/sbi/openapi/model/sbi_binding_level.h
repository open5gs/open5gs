/*
 * sbi_binding_level.h
 *
 * 
 */

#ifndef _OpenAPI_sbi_binding_level_H_
#define _OpenAPI_sbi_binding_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sbi_binding_level_NULL = 0, OpenAPI_sbi_binding_level_NF_INSTANCE_BINDING, OpenAPI_sbi_binding_level_NF_SET_BINDING, OpenAPI_sbi_binding_level_NF_SERVICE_SET_BINDING, OpenAPI_sbi_binding_level_NF_SERVICE_INSTANCE_BINDING } OpenAPI_sbi_binding_level_e;

char* OpenAPI_sbi_binding_level_ToString(OpenAPI_sbi_binding_level_e sbi_binding_level);

OpenAPI_sbi_binding_level_e OpenAPI_sbi_binding_level_FromString(char* sbi_binding_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sbi_binding_level_H_ */

