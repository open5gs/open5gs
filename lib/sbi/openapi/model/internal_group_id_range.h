/*
 * internal_group_id_range.h
 *
 * A range of Group IDs (internal group identities), either based on a numeric range, or based on regular-expression matching
 */

#ifndef _OpenAPI_internal_group_id_range_H_
#define _OpenAPI_internal_group_id_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_internal_group_id_range_s OpenAPI_internal_group_id_range_t;
typedef struct OpenAPI_internal_group_id_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_internal_group_id_range_t;

OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_create(
    char *start,
    char *end,
    char *pattern
);
void OpenAPI_internal_group_id_range_free(OpenAPI_internal_group_id_range_t *internal_group_id_range);
OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_parseFromJSON(cJSON *internal_group_id_rangeJSON);
cJSON *OpenAPI_internal_group_id_range_convertToJSON(OpenAPI_internal_group_id_range_t *internal_group_id_range);
OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_copy(OpenAPI_internal_group_id_range_t *dst, OpenAPI_internal_group_id_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_internal_group_id_range_H_ */

