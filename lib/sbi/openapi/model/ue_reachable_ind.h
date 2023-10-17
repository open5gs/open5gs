/*
 * ue_reachable_ind.h
 *
 * 
 */

#ifndef _OpenAPI_ue_reachable_ind_H_
#define _OpenAPI_ue_reachable_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ue_reachable_ind_NULL = 0, OpenAPI_ue_reachable_ind_REACHABLE, OpenAPI_ue_reachable_ind_NOT_REACHABLE, OpenAPI_ue_reachable_ind_UNKNOWN } OpenAPI_ue_reachable_ind_e;

char* OpenAPI_ue_reachable_ind_ToString(OpenAPI_ue_reachable_ind_e ue_reachable_ind);

OpenAPI_ue_reachable_ind_e OpenAPI_ue_reachable_ind_FromString(char* ue_reachable_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_reachable_ind_H_ */

