/*
 * ranging_sl_af_external.h
 *
 * Describes list of Call/session Unrelated Class for ranging and sidelink positioning  identified by AF in the external LCS client list. 
 */

#ifndef _OpenAPI_ranging_sl_af_external_H_
#define _OpenAPI_ranging_sl_af_external_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_af_external_s OpenAPI_ranging_sl_af_external_t;
#include "geographic_area.h"
#include "ranging_sl_privacy_check_related_action.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_af_external_s {
    char *af_id;
    OpenAPI_list_t *allowed_geographic_area;
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action;
    struct OpenAPI_valid_time_period_s *valid_time_period;
};

OpenAPI_ranging_sl_af_external_t *OpenAPI_ranging_sl_af_external_create(
    char *af_id,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action,
    OpenAPI_valid_time_period_t *valid_time_period
);
void OpenAPI_ranging_sl_af_external_free(OpenAPI_ranging_sl_af_external_t *ranging_sl_af_external);
OpenAPI_ranging_sl_af_external_t *OpenAPI_ranging_sl_af_external_parseFromJSON(cJSON *ranging_sl_af_externalJSON);
cJSON *OpenAPI_ranging_sl_af_external_convertToJSON(OpenAPI_ranging_sl_af_external_t *ranging_sl_af_external);
OpenAPI_ranging_sl_af_external_t *OpenAPI_ranging_sl_af_external_copy(OpenAPI_ranging_sl_af_external_t *dst, OpenAPI_ranging_sl_af_external_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_af_external_H_ */

