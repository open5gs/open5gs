/*
 * target_cause_id.h
 *
 * 
 */

#ifndef _OpenAPI_target_cause_id_H_
#define _OpenAPI_target_cause_id_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_target_cause_id_NULL = 0, OpenAPI_target_cause_id_SIGNALLING_STORM_CAUSED_BY_UE, OpenAPI_target_cause_id_SIGNALLING_STORM_CAUSED_BY_NF } OpenAPI_target_cause_id_e;

char* OpenAPI_target_cause_id_ToString(OpenAPI_target_cause_id_e target_cause_id);

OpenAPI_target_cause_id_e OpenAPI_target_cause_id_FromString(char* target_cause_id);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_target_cause_id_H_ */

