/*
 * event_reporting_requirement.h
 *
 * Represents the type of reporting that the subscription requires.
 */

#ifndef _OpenAPI_event_reporting_requirement_H_
#define _OpenAPI_event_reporting_requirement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "accuracy.h"
#include "analytics_metadata.h"
#include "analytics_metadata_indication.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_event_reporting_requirement_s OpenAPI_event_reporting_requirement_t;
typedef struct OpenAPI_event_reporting_requirement_s {
    struct OpenAPI_accuracy_s *accuracy;
    OpenAPI_list_t *acc_per_subset;
    char *start_ts;
    char *end_ts;
    bool is_offset_period;
    int offset_period;
    bool is_samp_ratio;
    int samp_ratio;
    bool is_max_object_nbr;
    int max_object_nbr;
    bool is_max_supi_nbr;
    int max_supi_nbr;
    char *time_ana_needed;
    OpenAPI_list_t *ana_meta;
    struct OpenAPI_analytics_metadata_indication_s *ana_meta_ind;
} OpenAPI_event_reporting_requirement_t;

OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_create(
    OpenAPI_accuracy_t *accuracy,
    OpenAPI_list_t *acc_per_subset,
    char *start_ts,
    char *end_ts,
    bool is_offset_period,
    int offset_period,
    bool is_samp_ratio,
    int samp_ratio,
    bool is_max_object_nbr,
    int max_object_nbr,
    bool is_max_supi_nbr,
    int max_supi_nbr,
    char *time_ana_needed,
    OpenAPI_list_t *ana_meta,
    OpenAPI_analytics_metadata_indication_t *ana_meta_ind
);
void OpenAPI_event_reporting_requirement_free(OpenAPI_event_reporting_requirement_t *event_reporting_requirement);
OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_parseFromJSON(cJSON *event_reporting_requirementJSON);
cJSON *OpenAPI_event_reporting_requirement_convertToJSON(OpenAPI_event_reporting_requirement_t *event_reporting_requirement);
OpenAPI_event_reporting_requirement_t *OpenAPI_event_reporting_requirement_copy(OpenAPI_event_reporting_requirement_t *dst, OpenAPI_event_reporting_requirement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_event_reporting_requirement_H_ */

