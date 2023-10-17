/*
 * bsf_event.h
 *
 * 
 */

#ifndef _OpenAPI_bsf_event_H_
#define _OpenAPI_bsf_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_bsf_event_NULL = 0, OpenAPI_bsf_event_PCF_PDU_SESSION_BINDING_REGISTRATION, OpenAPI_bsf_event_PCF_PDU_SESSION_BINDING_DEREGISTRATION, OpenAPI_bsf_event_PCF_UE_BINDING_REGISTRATION, OpenAPI_bsf_event_PCF_UE_BINDING_DEREGISTRATION, OpenAPI_bsf_event_SNSSAI_DNN_BINDING_REGISTRATION, OpenAPI_bsf_event_SNSSAI_DNN_BINDING_DEREGISTRATION } OpenAPI_bsf_event_e;

char* OpenAPI_bsf_event_ToString(OpenAPI_bsf_event_e bsf_event);

OpenAPI_bsf_event_e OpenAPI_bsf_event_FromString(char* bsf_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bsf_event_H_ */

