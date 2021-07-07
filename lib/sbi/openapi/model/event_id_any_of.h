/*
 * event_id_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_event_id_any_of_H_
#define _OpenAPI_event_id_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_event_id_any_of_NULL = 0, OpenAPI_event_id_any_of_LOAD_LEVEL_INFORMATION, OpenAPI_event_id_any_of_NETWORK_PERFORMANCE, OpenAPI_event_id_any_of_NF_LOAD, OpenAPI_event_id_any_of_SERVICE_EXPERIENCE, OpenAPI_event_id_any_of_UE_MOBILITY, OpenAPI_event_id_any_of_UE_COMMUNICATION, OpenAPI_event_id_any_of_QOS_SUSTAINABILITY, OpenAPI_event_id_any_of_ABNORMAL_BEHAVIOUR, OpenAPI_event_id_any_of_USER_DATA_CONGESTION, OpenAPI_event_id_any_of_NSI_LOAD_LEVEL } OpenAPI_event_id_any_of_e;

char* OpenAPI_event_id_any_of_ToString(OpenAPI_event_id_any_of_e event_id_any_of);

OpenAPI_event_id_any_of_e OpenAPI_event_id_any_of_FromString(char* event_id_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_id_any_of_H_ */

