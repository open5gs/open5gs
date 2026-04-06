/*
 * conditional_cag_info_1.h
 *
 * Conditional Closed Access Group Information
 */

#ifndef _OpenAPI_conditional_cag_info_1_H_
#define _OpenAPI_conditional_cag_info_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_conditional_cag_info_1_s OpenAPI_conditional_cag_info_1_t;
#include "valid_time_period_1.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_conditional_cag_info_1_s {
    OpenAPI_list_t *allowed_cag_list;
    bool is_cag_only_indicator;
    int cag_only_indicator;
    struct OpenAPI_valid_time_period_1_s *valid_time_period;
    OpenAPI_list_t* cag_specific_valid_time_period_list;
};

OpenAPI_conditional_cag_info_1_t *OpenAPI_conditional_cag_info_1_create(
    OpenAPI_list_t *allowed_cag_list,
    bool is_cag_only_indicator,
    int cag_only_indicator,
    OpenAPI_valid_time_period_1_t *valid_time_period,
    OpenAPI_list_t* cag_specific_valid_time_period_list
);
void OpenAPI_conditional_cag_info_1_free(OpenAPI_conditional_cag_info_1_t *conditional_cag_info_1);
OpenAPI_conditional_cag_info_1_t *OpenAPI_conditional_cag_info_1_parseFromJSON(cJSON *conditional_cag_info_1JSON);
cJSON *OpenAPI_conditional_cag_info_1_convertToJSON(OpenAPI_conditional_cag_info_1_t *conditional_cag_info_1);
OpenAPI_conditional_cag_info_1_t *OpenAPI_conditional_cag_info_1_copy(OpenAPI_conditional_cag_info_1_t *dst, OpenAPI_conditional_cag_info_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_conditional_cag_info_1_H_ */

