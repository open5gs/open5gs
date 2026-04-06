/*
 * ranging_sl_app_id_unrelated_class_1.h
 *
 * Describes Call/Session unrelated Class subscriptions for ranging and sidelink positioning  service for identified service type for UE. 
 */

#ifndef _OpenAPI_ranging_sl_app_id_unrelated_class_1_H_
#define _OpenAPI_ranging_sl_app_id_unrelated_class_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_app_id_unrelated_class_1_s OpenAPI_ranging_sl_app_id_unrelated_class_1_t;
#include "code_word_ind.h"
#include "geographic_area.h"
#include "ranging_sl_privacy_check_related_action.h"
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_app_id_unrelated_class_1_s {
    char *ranging_sl_app_id;
    OpenAPI_list_t *allowed_geographic_area;
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action;
    OpenAPI_code_word_ind_e code_word_ind;
    struct OpenAPI_valid_time_period_1_s *valid_time_period;
    OpenAPI_list_t *code_word_list;
};

OpenAPI_ranging_sl_app_id_unrelated_class_1_t *OpenAPI_ranging_sl_app_id_unrelated_class_1_create(
    char *ranging_sl_app_id,
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_1_t *valid_time_period,
    OpenAPI_list_t *code_word_list
);
void OpenAPI_ranging_sl_app_id_unrelated_class_1_free(OpenAPI_ranging_sl_app_id_unrelated_class_1_t *ranging_sl_app_id_unrelated_class_1);
OpenAPI_ranging_sl_app_id_unrelated_class_1_t *OpenAPI_ranging_sl_app_id_unrelated_class_1_parseFromJSON(cJSON *ranging_sl_app_id_unrelated_class_1JSON);
cJSON *OpenAPI_ranging_sl_app_id_unrelated_class_1_convertToJSON(OpenAPI_ranging_sl_app_id_unrelated_class_1_t *ranging_sl_app_id_unrelated_class_1);
OpenAPI_ranging_sl_app_id_unrelated_class_1_t *OpenAPI_ranging_sl_app_id_unrelated_class_1_copy(OpenAPI_ranging_sl_app_id_unrelated_class_1_t *dst, OpenAPI_ranging_sl_app_id_unrelated_class_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_app_id_unrelated_class_1_H_ */

