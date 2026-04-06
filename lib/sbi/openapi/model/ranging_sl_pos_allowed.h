/*
 * ranging_sl_pos_allowed.h
 *
 * Indicates the Ranging/SL positioning services that can be authorised to  use in the given PLMN for the UE. 
 */

#ifndef _OpenAPI_ranging_sl_pos_allowed_H_
#define _OpenAPI_ranging_sl_pos_allowed_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_ranging_sl_pos_allowed_NULL = 0, OpenAPI_ranging_sl_pos_allowed_PC5, OpenAPI_ranging_sl_pos_allowed_ANNOUNCE, OpenAPI_ranging_sl_pos_allowed_MONITOR, OpenAPI_ranging_sl_pos_allowed_DISCOVERER, OpenAPI_ranging_sl_pos_allowed_DISCOVEREE, OpenAPI_ranging_sl_pos_allowed_LOCATED_UE, OpenAPI_ranging_sl_pos_allowed_SL_SERVER_UE, OpenAPI_ranging_sl_pos_allowed_SL_CLIENT_UE } OpenAPI_ranging_sl_pos_allowed_e;

char* OpenAPI_ranging_sl_pos_allowed_ToString(OpenAPI_ranging_sl_pos_allowed_e ranging_sl_pos_allowed);

OpenAPI_ranging_sl_pos_allowed_e OpenAPI_ranging_sl_pos_allowed_FromString(char* ranging_sl_pos_allowed);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_pos_allowed_H_ */

