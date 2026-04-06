/*
 * ranging_sl_pos_auth.h
 *
 * Indicates whether the UE is authorized to use related services. 
 */

#ifndef _OpenAPI_ranging_sl_pos_auth_H_
#define _OpenAPI_ranging_sl_pos_auth_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_pos_auth_s OpenAPI_ranging_sl_pos_auth_t;
#include "ue_auth.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_pos_auth_s {
    OpenAPI_ue_auth_e rg_sl_pos_pc5_auth;
    OpenAPI_ue_auth_e rg_sl_pos_loc_auth;
    OpenAPI_ue_auth_e rg_sl_pos_client_auth;
    OpenAPI_ue_auth_e rg_sl_pos_server_auth;
};

OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_create(
    OpenAPI_ue_auth_e rg_sl_pos_pc5_auth,
    OpenAPI_ue_auth_e rg_sl_pos_loc_auth,
    OpenAPI_ue_auth_e rg_sl_pos_client_auth,
    OpenAPI_ue_auth_e rg_sl_pos_server_auth
);
void OpenAPI_ranging_sl_pos_auth_free(OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth);
OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_parseFromJSON(cJSON *ranging_sl_pos_authJSON);
cJSON *OpenAPI_ranging_sl_pos_auth_convertToJSON(OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth);
OpenAPI_ranging_sl_pos_auth_t *OpenAPI_ranging_sl_pos_auth_copy(OpenAPI_ranging_sl_pos_auth_t *dst, OpenAPI_ranging_sl_pos_auth_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_pos_auth_H_ */

