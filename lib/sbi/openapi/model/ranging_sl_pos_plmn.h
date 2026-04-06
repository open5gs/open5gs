/*
 * ranging_sl_pos_plmn.h
 *
 * Contains the PLMN identities where the Ranging/SL Positioning services are authorised to use and the authorised Ranging/SL Positioning services on this given PLMNs. 
 */

#ifndef _OpenAPI_ranging_sl_pos_plmn_H_
#define _OpenAPI_ranging_sl_pos_plmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_pos_plmn_s OpenAPI_ranging_sl_pos_plmn_t;
#include "plmn_id.h"
#include "ranging_sl_pos_allowed.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_pos_plmn_s {
    struct OpenAPI_plmn_id_s *ranging_sl_pos_plmn;
    OpenAPI_list_t *ranging_sl_pos_allowed;
};

OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_create(
    OpenAPI_plmn_id_t *ranging_sl_pos_plmn,
    OpenAPI_list_t *ranging_sl_pos_allowed
);
void OpenAPI_ranging_sl_pos_plmn_free(OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn);
OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_parseFromJSON(cJSON *ranging_sl_pos_plmnJSON);
cJSON *OpenAPI_ranging_sl_pos_plmn_convertToJSON(OpenAPI_ranging_sl_pos_plmn_t *ranging_sl_pos_plmn);
OpenAPI_ranging_sl_pos_plmn_t *OpenAPI_ranging_sl_pos_plmn_copy(OpenAPI_ranging_sl_pos_plmn_t *dst, OpenAPI_ranging_sl_pos_plmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_pos_plmn_H_ */

