/*
 * ims_event.h
 *
 * The IMS events
 */

#ifndef _OpenAPI_ims_event_H_
#define _OpenAPI_ims_event_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ims_event_NULL = 0, OpenAPI_ims_event_ADC_ESTABLISHMENT, OpenAPI_ims_event_ADC_RELEASE, OpenAPI_ims_event_BDC_ESTABLISHMENT, OpenAPI_ims_event_BDC_RELEASE, OpenAPI_ims_event_ADC_IWK_ESTABLISHMENT } OpenAPI_ims_event_e;

char* OpenAPI_ims_event_ToString(OpenAPI_ims_event_e ims_event);

OpenAPI_ims_event_e OpenAPI_ims_event_FromString(char* ims_event);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ims_event_H_ */

