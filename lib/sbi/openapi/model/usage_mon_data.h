/*
 * usage_mon_data.h
 *
 * Contains remain allowed usage data for a subscriber.
 */

#ifndef _OpenAPI_usage_mon_data_H_
#define _OpenAPI_usage_mon_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "time_period.h"
#include "usage_mon_data_scope.h"
#include "usage_mon_level.h"
#include "usage_threshold.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_usage_mon_data_s OpenAPI_usage_mon_data_t;
typedef struct OpenAPI_usage_mon_data_s {
    char *limit_id;
    OpenAPI_list_t* scopes;
    struct OpenAPI_usage_mon_level_s *um_level;
    struct OpenAPI_usage_threshold_s *allowed_usage;
    struct OpenAPI_time_period_s *reset_time;
    char *supp_feat;
} OpenAPI_usage_mon_data_t;

OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_create(
    char *limit_id,
    OpenAPI_list_t* scopes,
    OpenAPI_usage_mon_level_t *um_level,
    OpenAPI_usage_threshold_t *allowed_usage,
    OpenAPI_time_period_t *reset_time,
    char *supp_feat
);
void OpenAPI_usage_mon_data_free(OpenAPI_usage_mon_data_t *usage_mon_data);
OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_parseFromJSON(cJSON *usage_mon_dataJSON);
cJSON *OpenAPI_usage_mon_data_convertToJSON(OpenAPI_usage_mon_data_t *usage_mon_data);
OpenAPI_usage_mon_data_t *OpenAPI_usage_mon_data_copy(OpenAPI_usage_mon_data_t *dst, OpenAPI_usage_mon_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_usage_mon_data_H_ */

