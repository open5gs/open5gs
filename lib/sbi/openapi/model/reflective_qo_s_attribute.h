/*
 * reflective_qo_s_attribute.h
 *
 * 
 */

#ifndef _OpenAPI_reflective_qo_s_attribute_H_
#define _OpenAPI_reflective_qo_s_attribute_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_reflective_qo_s_attribute_NULL = 0, OpenAPI_reflective_qo_s_attribute_RQOS, OpenAPI_reflective_qo_s_attribute_NO_RQOS } OpenAPI_reflective_qo_s_attribute_e;

char* OpenAPI_reflective_qo_s_attribute_ToString(OpenAPI_reflective_qo_s_attribute_e reflective_qo_s_attribute);

OpenAPI_reflective_qo_s_attribute_e OpenAPI_reflective_qo_s_attribute_FromString(char* reflective_qo_s_attribute);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_reflective_qo_s_attribute_H_ */

