/*
 * conditional_cag_info.h
 *
 * Conditional Closed Access Group Information
 */

#ifndef _OpenAPI_conditional_cag_info_H_
#define _OpenAPI_conditional_cag_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_conditional_cag_info_s OpenAPI_conditional_cag_info_t;
#include "valid_time_period.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_conditional_cag_info_s {
    OpenAPI_list_t *allowed_cag_list;
    bool is_cag_only_indicator;
    int cag_only_indicator;
    struct OpenAPI_valid_time_period_s *valid_time_period;
    OpenAPI_list_t* cag_specific_valid_time_period_list;
};

OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_create(
    OpenAPI_list_t *allowed_cag_list,
    bool is_cag_only_indicator,
    int cag_only_indicator,
    OpenAPI_valid_time_period_t *valid_time_period,
    OpenAPI_list_t* cag_specific_valid_time_period_list
);
void OpenAPI_conditional_cag_info_free(OpenAPI_conditional_cag_info_t *conditional_cag_info);
OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_parseFromJSON(cJSON *conditional_cag_infoJSON);
cJSON *OpenAPI_conditional_cag_info_convertToJSON(OpenAPI_conditional_cag_info_t *conditional_cag_info);
OpenAPI_conditional_cag_info_t *OpenAPI_conditional_cag_info_copy(OpenAPI_conditional_cag_info_t *dst, OpenAPI_conditional_cag_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_conditional_cag_info_H_ */

