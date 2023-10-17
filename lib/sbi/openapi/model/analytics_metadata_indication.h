/*
 * analytics_metadata_indication.h
 *
 * Contains analytics metadata information requested to be used during analytics generation. 
 */

#ifndef _OpenAPI_analytics_metadata_indication_H_
#define _OpenAPI_analytics_metadata_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dataset_statistical_property.h"
#include "output_strategy.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_analytics_metadata_indication_s OpenAPI_analytics_metadata_indication_t;
typedef struct OpenAPI_analytics_metadata_indication_s {
    struct OpenAPI_time_window_s *data_window;
    OpenAPI_list_t *data_stat_props;
    struct OpenAPI_output_strategy_s *strategy;
    OpenAPI_list_t *aggr_nwdaf_ids;
} OpenAPI_analytics_metadata_indication_t;

OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_create(
    OpenAPI_time_window_t *data_window,
    OpenAPI_list_t *data_stat_props,
    OpenAPI_output_strategy_t *strategy,
    OpenAPI_list_t *aggr_nwdaf_ids
);
void OpenAPI_analytics_metadata_indication_free(OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication);
OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_parseFromJSON(cJSON *analytics_metadata_indicationJSON);
cJSON *OpenAPI_analytics_metadata_indication_convertToJSON(OpenAPI_analytics_metadata_indication_t *analytics_metadata_indication);
OpenAPI_analytics_metadata_indication_t *OpenAPI_analytics_metadata_indication_copy(OpenAPI_analytics_metadata_indication_t *dst, OpenAPI_analytics_metadata_indication_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_metadata_indication_H_ */

