/*
 * sac_event_type.h
 *
 * Describes the supported slice event types
 */

#ifndef _OpenAPI_sac_event_type_H_
#define _OpenAPI_sac_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sac_event_type_NULL = 0, OpenAPI_sac_event_type_NUM_OF_REGD_UES, OpenAPI_sac_event_type_NUM_OF_ESTD_PDU_SESSIONS } OpenAPI_sac_event_type_e;

char* OpenAPI_sac_event_type_ToString(OpenAPI_sac_event_type_e sac_event_type);

OpenAPI_sac_event_type_e OpenAPI_sac_event_type_FromString(char* sac_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sac_event_type_H_ */

