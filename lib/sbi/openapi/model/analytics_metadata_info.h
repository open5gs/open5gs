/*
 * analytics_metadata_info.h
 *
 * Contains analytics metadata information required for analytics aggregation.
 */

#ifndef _OpenAPI_analytics_metadata_info_H_
#define _OpenAPI_analytics_metadata_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_analytics_metadata_info_s OpenAPI_analytics_metadata_info_t;
#include "accuracy.h"
#include "dataset_statistical_property.h"
#include "output_strategy.h"
#include "processing_instruction.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_analytics_metadata_info_s {
    bool is_num_samples;
    int num_samples;
    struct OpenAPI_time_window_s *data_window;
    OpenAPI_list_t *data_stat_props;
    OpenAPI_output_strategy_e strategy;
    OpenAPI_accuracy_e accuracy;
    OpenAPI_list_t *nf_ids;
    OpenAPI_list_t *nf_set_ids;
    OpenAPI_list_t *proc_instructs;
};

OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_create(
    bool is_num_samples,
    int num_samples,
    OpenAPI_time_window_t *data_window,
    OpenAPI_list_t *data_stat_props,
    OpenAPI_output_strategy_e strategy,
    OpenAPI_accuracy_e accuracy,
    OpenAPI_list_t *nf_ids,
    OpenAPI_list_t *nf_set_ids,
    OpenAPI_list_t *proc_instructs
);
void OpenAPI_analytics_metadata_info_free(OpenAPI_analytics_metadata_info_t *analytics_metadata_info);
OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_parseFromJSON(cJSON *analytics_metadata_infoJSON);
cJSON *OpenAPI_analytics_metadata_info_convertToJSON(OpenAPI_analytics_metadata_info_t *analytics_metadata_info);
OpenAPI_analytics_metadata_info_t *OpenAPI_analytics_metadata_info_copy(OpenAPI_analytics_metadata_info_t *dst, OpenAPI_analytics_metadata_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_metadata_info_H_ */

