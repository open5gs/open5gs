/*
 * clock_quality.h
 *
 * Contains Clock Quality.
 */

#ifndef _OpenAPI_clock_quality_H_
#define _OpenAPI_clock_quality_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_s OpenAPI_clock_quality_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_s {
    bool is_traceability_to_gnss;
    int traceability_to_gnss;
    bool is_traceability_to_utc;
    int traceability_to_utc;
    bool is_frequency_stability;
    int frequency_stability;
    char *clock_accuracy_index;
    bool is_clock_accuracy_value;
    int clock_accuracy_value;
};

OpenAPI_clock_quality_t *OpenAPI_clock_quality_create(
    bool is_traceability_to_gnss,
    int traceability_to_gnss,
    bool is_traceability_to_utc,
    int traceability_to_utc,
    bool is_frequency_stability,
    int frequency_stability,
    char *clock_accuracy_index,
    bool is_clock_accuracy_value,
    int clock_accuracy_value
);
void OpenAPI_clock_quality_free(OpenAPI_clock_quality_t *clock_quality);
OpenAPI_clock_quality_t *OpenAPI_clock_quality_parseFromJSON(cJSON *clock_qualityJSON);
cJSON *OpenAPI_clock_quality_convertToJSON(OpenAPI_clock_quality_t *clock_quality);
OpenAPI_clock_quality_t *OpenAPI_clock_quality_copy(OpenAPI_clock_quality_t *dst, OpenAPI_clock_quality_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_H_ */

