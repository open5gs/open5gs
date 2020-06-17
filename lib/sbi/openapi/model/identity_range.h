/*
 * identity_range.h
 *
 * A range of GPSIs (subscriber identities), either based on a numeric range, or based on regular-expression matching
 */

#ifndef _OpenAPI_identity_range_H_
#define _OpenAPI_identity_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_identity_range_s OpenAPI_identity_range_t;
typedef struct OpenAPI_identity_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_identity_range_t;

OpenAPI_identity_range_t *OpenAPI_identity_range_create(
    char *start,
    char *end,
    char *pattern
    );
void OpenAPI_identity_range_free(OpenAPI_identity_range_t *identity_range);
OpenAPI_identity_range_t *OpenAPI_identity_range_parseFromJSON(cJSON *identity_rangeJSON);
cJSON *OpenAPI_identity_range_convertToJSON(OpenAPI_identity_range_t *identity_range);
OpenAPI_identity_range_t *OpenAPI_identity_range_copy(OpenAPI_identity_range_t *dst, OpenAPI_identity_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_identity_range_H_ */

