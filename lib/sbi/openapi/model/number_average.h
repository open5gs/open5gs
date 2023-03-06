/*
 * number_average.h
 *
 * Represents average and variance information.
 */

#ifndef _OpenAPI_number_average_H_
#define _OpenAPI_number_average_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_number_average_s OpenAPI_number_average_t;
typedef struct OpenAPI_number_average_s {
    float number;
    float variance;
    bool is_skewness;
    float skewness;
} OpenAPI_number_average_t;

OpenAPI_number_average_t *OpenAPI_number_average_create(
    float number,
    float variance,
    bool is_skewness,
    float skewness
);
void OpenAPI_number_average_free(OpenAPI_number_average_t *number_average);
OpenAPI_number_average_t *OpenAPI_number_average_parseFromJSON(cJSON *number_averageJSON);
cJSON *OpenAPI_number_average_convertToJSON(OpenAPI_number_average_t *number_average);
OpenAPI_number_average_t *OpenAPI_number_average_copy(OpenAPI_number_average_t *dst, OpenAPI_number_average_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_number_average_H_ */

