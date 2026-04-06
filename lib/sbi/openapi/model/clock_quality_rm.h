/*
 * clock_quality_rm.h
 *
 * ClockQuality with &#39;nullable: true&#39; property
 */

#ifndef _OpenAPI_clock_quality_rm_H_
#define _OpenAPI_clock_quality_rm_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_rm_s OpenAPI_clock_quality_rm_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_rm_s {
    bool is_traceability_to_gnss;
    int traceability_to_gnss;
    bool is_traceability_to_utc;
    int traceability_to_utc;
    bool is_frequency_stability_null;
    bool is_frequency_stability;
    int frequency_stability;
    bool is_clock_accuracy_index_null;
    char *clock_accuracy_index;
    bool is_clock_accuracy_value_null;
    bool is_clock_accuracy_value;
    int clock_accuracy_value;
};

OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_create(
    bool is_traceability_to_gnss,
    int traceability_to_gnss,
    bool is_traceability_to_utc,
    int traceability_to_utc,
    bool is_frequency_stability_null,
    bool is_frequency_stability,
    int frequency_stability,
    bool is_clock_accuracy_index_null,
    char *clock_accuracy_index,
    bool is_clock_accuracy_value_null,
    bool is_clock_accuracy_value,
    int clock_accuracy_value
);
void OpenAPI_clock_quality_rm_free(OpenAPI_clock_quality_rm_t *clock_quality_rm);
OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_parseFromJSON(cJSON *clock_quality_rmJSON);
cJSON *OpenAPI_clock_quality_rm_convertToJSON(OpenAPI_clock_quality_rm_t *clock_quality_rm);
OpenAPI_clock_quality_rm_t *OpenAPI_clock_quality_rm_copy(OpenAPI_clock_quality_rm_t *dst, OpenAPI_clock_quality_rm_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_rm_H_ */

