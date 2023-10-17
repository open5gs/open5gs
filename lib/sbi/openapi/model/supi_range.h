/*
 * supi_range.h
 *
 * A range of SUPIs (subscriber identities), either based on a numeric range, or based on regular-expression matching 
 */

#ifndef _OpenAPI_supi_range_H_
#define _OpenAPI_supi_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_supi_range_s OpenAPI_supi_range_t;
typedef struct OpenAPI_supi_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_supi_range_t;

OpenAPI_supi_range_t *OpenAPI_supi_range_create(
    char *start,
    char *end,
    char *pattern
);
void OpenAPI_supi_range_free(OpenAPI_supi_range_t *supi_range);
OpenAPI_supi_range_t *OpenAPI_supi_range_parseFromJSON(cJSON *supi_rangeJSON);
cJSON *OpenAPI_supi_range_convertToJSON(OpenAPI_supi_range_t *supi_range);
OpenAPI_supi_range_t *OpenAPI_supi_range_copy(OpenAPI_supi_range_t *dst, OpenAPI_supi_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_supi_range_H_ */

