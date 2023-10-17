/*
 * ranking_criterion.h
 *
 * Indicates the usage ranking criterion between the high, medium and low usage UE.
 */

#ifndef _OpenAPI_ranking_criterion_H_
#define _OpenAPI_ranking_criterion_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ranking_criterion_s OpenAPI_ranking_criterion_t;
typedef struct OpenAPI_ranking_criterion_s {
    int high_base;
    int low_base;
} OpenAPI_ranking_criterion_t;

OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_create(
    int high_base,
    int low_base
);
void OpenAPI_ranking_criterion_free(OpenAPI_ranking_criterion_t *ranking_criterion);
OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_parseFromJSON(cJSON *ranking_criterionJSON);
cJSON *OpenAPI_ranking_criterion_convertToJSON(OpenAPI_ranking_criterion_t *ranking_criterion);
OpenAPI_ranking_criterion_t *OpenAPI_ranking_criterion_copy(OpenAPI_ranking_criterion_t *dst, OpenAPI_ranking_criterion_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ranking_criterion_H_ */

