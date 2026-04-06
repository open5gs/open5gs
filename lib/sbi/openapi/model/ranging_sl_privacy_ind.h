/*
 * ranging_sl_privacy_ind.h
 *
 * Indicates one of the mutually exclusive global settings (if the ranging and sidelink positioning is allowed or disallowed). 
 */

#ifndef _OpenAPI_ranging_sl_privacy_ind_H_
#define _OpenAPI_ranging_sl_privacy_ind_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ranging_sl_privacy_ind_NULL = 0, OpenAPI_ranging_sl_privacy_ind_RANGINGSL_DISALLOWED, OpenAPI_ranging_sl_privacy_ind_RANGINGSL_ALLOWED } OpenAPI_ranging_sl_privacy_ind_e;

char* OpenAPI_ranging_sl_privacy_ind_ToString(OpenAPI_ranging_sl_privacy_ind_e ranging_sl_privacy_ind);

OpenAPI_ranging_sl_privacy_ind_e OpenAPI_ranging_sl_privacy_ind_FromString(char* ranging_sl_privacy_ind);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_privacy_ind_H_ */

