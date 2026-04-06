/*
 * rate_limit_repo.h
 *
 * Contains the rate limit information for the non-GRB flows. 
 */

#ifndef _OpenAPI_rate_limit_repo_H_
#define _OpenAPI_rate_limit_repo_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_rate_limit_repo_s OpenAPI_rate_limit_repo_t;
#include "flows.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_rate_limit_repo_s {
    OpenAPI_list_t *rate_limit_repo_ul;
    OpenAPI_list_t *rate_limit_repo_dl;
    OpenAPI_list_t *flows;
};

OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_create(
    OpenAPI_list_t *rate_limit_repo_ul,
    OpenAPI_list_t *rate_limit_repo_dl,
    OpenAPI_list_t *flows
);
void OpenAPI_rate_limit_repo_free(OpenAPI_rate_limit_repo_t *rate_limit_repo);
OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_parseFromJSON(cJSON *rate_limit_repoJSON);
cJSON *OpenAPI_rate_limit_repo_convertToJSON(OpenAPI_rate_limit_repo_t *rate_limit_repo);
OpenAPI_rate_limit_repo_t *OpenAPI_rate_limit_repo_copy(OpenAPI_rate_limit_repo_t *dst, OpenAPI_rate_limit_repo_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_rate_limit_repo_H_ */

