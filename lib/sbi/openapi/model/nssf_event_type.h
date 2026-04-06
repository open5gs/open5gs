/*
 * nssf_event_type.h
 *
 * This contains the event for the subscription
 */

#ifndef _OpenAPI_nssf_event_type_H_
#define _OpenAPI_nssf_event_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_nssf_event_type_NULL = 0, OpenAPI_nssf_event_type_SNSSAI_STATUS_CHANGE_REPORT, OpenAPI_nssf_event_type_SNSSAI_REPLACEMENT_REPORT, OpenAPI_nssf_event_type_NSI_UNAVAILABILITY_REPORT, OpenAPI_nssf_event_type_SNSSAI_VALIDITY_TIME_REPORT } OpenAPI_nssf_event_type_e;

char* OpenAPI_nssf_event_type_ToString(OpenAPI_nssf_event_type_e nssf_event_type);

OpenAPI_nssf_event_type_e OpenAPI_nssf_event_type_FromString(char* nssf_event_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nssf_event_type_H_ */

