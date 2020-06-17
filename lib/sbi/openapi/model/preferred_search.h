/*
 * preferred_search.h
 *
 * Contains information on whether the returned NFProfiles match the preferred query parameters
 */

#ifndef _OpenAPI_preferred_search_H_
#define _OpenAPI_preferred_search_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_preferred_search_s OpenAPI_preferred_search_t;
typedef struct OpenAPI_preferred_search_s {
    int preferred_tai_match_ind;
} OpenAPI_preferred_search_t;

OpenAPI_preferred_search_t *OpenAPI_preferred_search_create(
    int preferred_tai_match_ind
    );
void OpenAPI_preferred_search_free(OpenAPI_preferred_search_t *preferred_search);
OpenAPI_preferred_search_t *OpenAPI_preferred_search_parseFromJSON(cJSON *preferred_searchJSON);
cJSON *OpenAPI_preferred_search_convertToJSON(OpenAPI_preferred_search_t *preferred_search);
OpenAPI_preferred_search_t *OpenAPI_preferred_search_copy(OpenAPI_preferred_search_t *dst, OpenAPI_preferred_search_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_preferred_search_H_ */

