/*
 * usage_mon_data_limit.h
 *
 * Contains usage monitoring control data for a subscriber.
 */

#ifndef _OpenAPI_usage_mon_data_limit_H_
#define _OpenAPI_usage_mon_data_limit_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "usage_mon_data_scope.h"
#include "usage_mon_level.h"
#include "usage_threshold.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_mon_data_limit_s OpenAPI_usage_mon_data_limit_t;
typedef struct OpenAPI_usage_mon_data_limit_s {
    char *limit_id;
    OpenAPI_list_t* scopes;
    struct OpenAPI_usage_mon_level_s *um_level;
    char *start_date;
    char *end_date;
    struct OpenAPI_usage_threshold_s *usage_limit;
    char *reset_period;
} OpenAPI_usage_mon_data_limit_t;

OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_create(
    char *limit_id,
    OpenAPI_list_t* scopes,
    OpenAPI_usage_mon_level_t *um_level,
    char *start_date,
    char *end_date,
    OpenAPI_usage_threshold_t *usage_limit,
    char *reset_period
);
void OpenAPI_usage_mon_data_limit_free(OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit);
OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_parseFromJSON(cJSON *usage_mon_data_limitJSON);
cJSON *OpenAPI_usage_mon_data_limit_convertToJSON(OpenAPI_usage_mon_data_limit_t *usage_mon_data_limit);
OpenAPI_usage_mon_data_limit_t *OpenAPI_usage_mon_data_limit_copy(OpenAPI_usage_mon_data_limit_t *dst, OpenAPI_usage_mon_data_limit_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_data_limit_H_ */

