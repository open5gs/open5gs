/*
 * aerial_ue_indication.h
 *
 * 
 */

#ifndef _OpenAPI_aerial_ue_indication_H_
#define _OpenAPI_aerial_ue_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_aerial_ue_indication_NULL = 0, OpenAPI_aerial_ue_indication_AERIAL_UE_ALLOWED, OpenAPI_aerial_ue_indication_AERIAL_UE_NOT_ALLOWED } OpenAPI_aerial_ue_indication_e;

char* OpenAPI_aerial_ue_indication_ToString(OpenAPI_aerial_ue_indication_e aerial_ue_indication);

OpenAPI_aerial_ue_indication_e OpenAPI_aerial_ue_indication_FromString(char* aerial_ue_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aerial_ue_indication_H_ */

