/*
 * tac_range.h
 *
 * Range of TACs (Tracking Area Codes)
 */

#ifndef _OpenAPI_tac_range_H_
#define _OpenAPI_tac_range_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_tac_range_s OpenAPI_tac_range_t;
typedef struct OpenAPI_tac_range_s {
    char *start;
    char *end;
    char *pattern;
} OpenAPI_tac_range_t;

OpenAPI_tac_range_t *OpenAPI_tac_range_create(
    char *start,
    char *end,
    char *pattern
    );
void OpenAPI_tac_range_free(OpenAPI_tac_range_t *tac_range);
OpenAPI_tac_range_t *OpenAPI_tac_range_parseFromJSON(cJSON *tac_rangeJSON);
cJSON *OpenAPI_tac_range_convertToJSON(OpenAPI_tac_range_t *tac_range);
OpenAPI_tac_range_t *OpenAPI_tac_range_copy(OpenAPI_tac_range_t *dst, OpenAPI_tac_range_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_tac_range_H_ */

