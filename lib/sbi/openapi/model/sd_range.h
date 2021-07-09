/*
 * sd_range.h
 *
 * A range of SDs (Slice Differentiators)
 */

#ifndef _OpenAPI_sd_range_H_
#define _OpenAPI_sd_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sd_range_s OpenAPI_sd_range_t;
typedef struct OpenAPI_sd_range_s {
    char *start;
    char *end;
} OpenAPI_sd_range_t;

OpenAPI_sd_range_t *OpenAPI_sd_range_create(
    char *start,
    char *end
);
void OpenAPI_sd_range_free(OpenAPI_sd_range_t *sd_range);
OpenAPI_sd_range_t *OpenAPI_sd_range_parseFromJSON(cJSON *sd_rangeJSON);
cJSON *OpenAPI_sd_range_convertToJSON(OpenAPI_sd_range_t *sd_range);
OpenAPI_sd_range_t *OpenAPI_sd_range_copy(OpenAPI_sd_range_t *dst, OpenAPI_sd_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sd_range_H_ */

