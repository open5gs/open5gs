/*
 * plmn_range.h
 *
 * Range of PLMN IDs
 */

#ifndef _OpenAPI_plmn_range_H_
#define _OpenAPI_plmn_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_plmn_range_s OpenAPI_plmn_range_t;
typedef struct OpenAPI_plmn_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_plmn_range_t;

OpenAPI_plmn_range_t *OpenAPI_plmn_range_create(
    char *start,
    char *end,
    char *pattern
    );
void OpenAPI_plmn_range_free(OpenAPI_plmn_range_t *plmn_range);
OpenAPI_plmn_range_t *OpenAPI_plmn_range_parseFromJSON(cJSON *plmn_rangeJSON);
cJSON *OpenAPI_plmn_range_convertToJSON(OpenAPI_plmn_range_t *plmn_range);
OpenAPI_plmn_range_t *OpenAPI_plmn_range_copy(OpenAPI_plmn_range_t *dst, OpenAPI_plmn_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_plmn_range_H_ */

