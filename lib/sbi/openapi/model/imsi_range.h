/*
 * imsi_range.h
 *
 * A range of IMSIs (subscriber identities), either based on a numeric range, or based on regular-expression matching 
 */

#ifndef _OpenAPI_imsi_range_H_
#define _OpenAPI_imsi_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_imsi_range_s OpenAPI_imsi_range_t;
typedef struct OpenAPI_imsi_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_imsi_range_t;

OpenAPI_imsi_range_t *OpenAPI_imsi_range_create(
    char *start,
    char *end,
    char *pattern
);
void OpenAPI_imsi_range_free(OpenAPI_imsi_range_t *imsi_range);
OpenAPI_imsi_range_t *OpenAPI_imsi_range_parseFromJSON(cJSON *imsi_rangeJSON);
cJSON *OpenAPI_imsi_range_convertToJSON(OpenAPI_imsi_range_t *imsi_range);
OpenAPI_imsi_range_t *OpenAPI_imsi_range_copy(OpenAPI_imsi_range_t *dst, OpenAPI_imsi_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_imsi_range_H_ */

