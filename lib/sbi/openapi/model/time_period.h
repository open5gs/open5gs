/*
 * time_period.h
 *
 * Contains the periodicity for the defined usage monitoring data limits.
 */

#ifndef _OpenAPI_time_period_H_
#define _OpenAPI_time_period_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "periodicity.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_time_period_s OpenAPI_time_period_t;
typedef struct OpenAPI_time_period_s {
    struct OpenAPI_periodicity_s *period;
    bool is_max_num_period;
    int max_num_period;
} OpenAPI_time_period_t;

OpenAPI_time_period_t *OpenAPI_time_period_create(
    OpenAPI_periodicity_t *period,
    bool is_max_num_period,
    int max_num_period
);
void OpenAPI_time_period_free(OpenAPI_time_period_t *time_period);
OpenAPI_time_period_t *OpenAPI_time_period_parseFromJSON(cJSON *time_periodJSON);
cJSON *OpenAPI_time_period_convertToJSON(OpenAPI_time_period_t *time_period);
OpenAPI_time_period_t *OpenAPI_time_period_copy(OpenAPI_time_period_t *dst, OpenAPI_time_period_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_period_H_ */

