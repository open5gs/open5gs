/*
 * candidate_for_replacement.h
 *
 * 
 */

#ifndef _OpenAPI_candidate_for_replacement_H_
#define _OpenAPI_candidate_for_replacement_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_candidate_for_replacement_s OpenAPI_candidate_for_replacement_t;
typedef struct OpenAPI_candidate_for_replacement_s {
    struct OpenAPI_snssai_s *snssai;
    OpenAPI_list_t *dnns;
} OpenAPI_candidate_for_replacement_t;

OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t *dnns
);
void OpenAPI_candidate_for_replacement_free(OpenAPI_candidate_for_replacement_t *candidate_for_replacement);
OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_parseFromJSON(cJSON *candidate_for_replacementJSON);
cJSON *OpenAPI_candidate_for_replacement_convertToJSON(OpenAPI_candidate_for_replacement_t *candidate_for_replacement);
OpenAPI_candidate_for_replacement_t *OpenAPI_candidate_for_replacement_copy(OpenAPI_candidate_for_replacement_t *dst, OpenAPI_candidate_for_replacement_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_candidate_for_replacement_H_ */

