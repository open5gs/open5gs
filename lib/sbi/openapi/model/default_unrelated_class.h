/*
 * default_unrelated_class.h
 *
 * 
 */

#ifndef _OpenAPI_default_unrelated_class_H_
#define _OpenAPI_default_unrelated_class_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "code_word_ind.h"
#include "geographic_area.h"
#include "privacy_check_related_action.h"
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_default_unrelated_class_s OpenAPI_default_unrelated_class_t;
typedef struct OpenAPI_default_unrelated_class_s {
    OpenAPI_list_t *allowed_geographic_area;
    OpenAPI_privacy_check_related_action_e privacy_check_related_action;
    OpenAPI_code_word_ind_e code_word_ind;
    struct OpenAPI_valid_time_period_s *valid_time_period;
    OpenAPI_list_t *code_word_list;
} OpenAPI_default_unrelated_class_t;

OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_create(
    OpenAPI_list_t *allowed_geographic_area,
    OpenAPI_privacy_check_related_action_e privacy_check_related_action,
    OpenAPI_code_word_ind_e code_word_ind,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t *code_word_list
);
void OpenAPI_default_unrelated_class_free(OpenAPI_default_unrelated_class_t *default_unrelated_class);
OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_parseFromJSON(cJSON *default_unrelated_classJSON);
cJSON *OpenAPI_default_unrelated_class_convertToJSON(OpenAPI_default_unrelated_class_t *default_unrelated_class);
OpenAPI_default_unrelated_class_t *OpenAPI_default_unrelated_class_copy(OpenAPI_default_unrelated_class_t *dst, OpenAPI_default_unrelated_class_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_default_unrelated_class_H_ */

