/*
 * ranging_sl_pos_subscription_data.h
 *
 * Contains the Ranging/SL positioning Subscription Data.
 */

#ifndef _OpenAPI_ranging_sl_pos_subscription_data_H_
#define _OpenAPI_ranging_sl_pos_subscription_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_pos_subscription_data_s OpenAPI_ranging_sl_pos_subscription_data_t;
#include "ranging_sl_pos_auth.h"
#include "ranging_sl_pos_plmn.h"
#include "ranging_sl_pos_qos.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_pos_subscription_data_s {
    struct OpenAPI_ranging_sl_pos_auth_s *ranging_sl_pos_auth;
    OpenAPI_list_t *ranging_sl_pos_plmn;
    struct OpenAPI_ranging_sl_pos_qos_s *ranging_sl_pos_qos;
};

OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_create(
    OpenAPI_ranging_sl_pos_auth_t *ranging_sl_pos_auth,
    OpenAPI_list_t *ranging_sl_pos_plmn,
    OpenAPI_ranging_sl_pos_qos_t *ranging_sl_pos_qos
);
void OpenAPI_ranging_sl_pos_subscription_data_free(OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data);
OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_parseFromJSON(cJSON *ranging_sl_pos_subscription_dataJSON);
cJSON *OpenAPI_ranging_sl_pos_subscription_data_convertToJSON(OpenAPI_ranging_sl_pos_subscription_data_t *ranging_sl_pos_subscription_data);
OpenAPI_ranging_sl_pos_subscription_data_t *OpenAPI_ranging_sl_pos_subscription_data_copy(OpenAPI_ranging_sl_pos_subscription_data_t *dst, OpenAPI_ranging_sl_pos_subscription_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_pos_subscription_data_H_ */

