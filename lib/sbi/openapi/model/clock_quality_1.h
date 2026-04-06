/*
 * clock_quality_1.h
 *
 * Contains Clock Quality.
 */

#ifndef _OpenAPI_clock_quality_1_H_
#define _OpenAPI_clock_quality_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_clock_quality_1_s OpenAPI_clock_quality_1_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_clock_quality_1_s {
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

OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_create(
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
void OpenAPI_clock_quality_1_free(OpenAPI_clock_quality_1_t *clock_quality_1);
OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_parseFromJSON(cJSON *clock_quality_1JSON);
cJSON *OpenAPI_clock_quality_1_convertToJSON(OpenAPI_clock_quality_1_t *clock_quality_1);
OpenAPI_clock_quality_1_t *OpenAPI_clock_quality_1_copy(OpenAPI_clock_quality_1_t *dst, OpenAPI_clock_quality_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_1_H_ */

