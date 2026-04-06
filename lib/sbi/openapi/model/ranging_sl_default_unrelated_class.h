/*
 * ranging_sl_default_unrelated_class.h
 *
 * Describes default Call/Session unrelated Class subscription for ranging and sidelink  positioning for unidentified value added LCS clients or AFs. 
 */

#ifndef _OpenAPI_ranging_sl_default_unrelated_class_H_
#define _OpenAPI_ranging_sl_default_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_ranging_sl_default_unrelated_class_s OpenAPI_ranging_sl_default_unrelated_class_t;
#include "code_word_ind.h"
#include "geographic_area.h"
#include "ranging_sl_privacy_check_related_action.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_ranging_sl_default_unrelated_class_s {
    OpenAPI_list_t *allowed_geographic_area;
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action;
    OpenAPI_code_word_ind_e code_word_ind;
    struct OpenAPI_valid_time_period_s *valid_time_period;
    OpenAPI_list_t *code_word_list;
};

OpenAPI_ranging_sl_default_unrelated_class_t *OpenAPI_ranging_sl_default_unrelated_class_create(
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_ranging_sl_privacy_check_related_action_e ranging_sl_privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t *code_word_list
);
void OpenAPI_ranging_sl_default_unrelated_class_free(OpenAPI_ranging_sl_default_unrelated_class_t *ranging_sl_default_unrelated_class);
OpenAPI_ranging_sl_default_unrelated_class_t *OpenAPI_ranging_sl_default_unrelated_class_parseFromJSON(cJSON *ranging_sl_default_unrelated_classJSON);
cJSON *OpenAPI_ranging_sl_default_unrelated_class_convertToJSON(OpenAPI_ranging_sl_default_unrelated_class_t *ranging_sl_default_unrelated_class);
OpenAPI_ranging_sl_default_unrelated_class_t *OpenAPI_ranging_sl_default_unrelated_class_copy(OpenAPI_ranging_sl_default_unrelated_class_t *dst, OpenAPI_ranging_sl_default_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranging_sl_default_unrelated_class_H_ */

