/*
 * sip_forking_indication.h
 *
 * 
 */

#ifndef _OpenAPI_sip_forking_indication_H_
#define _OpenAPI_sip_forking_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_sip_forking_indication_NULL = 0, OpenAPI_sip_forking_indication_SINGLE_DIALOGUE, OpenAPI_sip_forking_indication_SEVERAL_DIALOGUES } OpenAPI_sip_forking_indication_e;

char* OpenAPI_sip_forking_indication_ToString(OpenAPI_sip_forking_indication_e sip_forking_indication);

OpenAPI_sip_forking_indication_e OpenAPI_sip_forking_indication_FromString(char* sip_forking_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sip_forking_indication_H_ */

