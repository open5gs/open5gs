/*
 * smf_change_indication.h
 *
 * 
 */

#ifndef _OpenAPI_smf_change_indication_H_
#define _OpenAPI_smf_change_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_smf_change_indication_NULL = 0, OpenAPI_smf_change_indication_CHANGED, OpenAPI_smf_change_indication_REMOVED } OpenAPI_smf_change_indication_e;

char* OpenAPI_smf_change_indication_ToString(OpenAPI_smf_change_indication_e smf_change_indication);

OpenAPI_smf_change_indication_e OpenAPI_smf_change_indication_FromString(char* smf_change_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_smf_change_indication_H_ */

