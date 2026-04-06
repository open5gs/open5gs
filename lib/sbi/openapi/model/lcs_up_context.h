/*
 * lcs_up_context.h
 *
 * Represents the LCS UP related parameters
 */

#ifndef _OpenAPI_lcs_up_context_H_
#define _OpenAPI_lcs_up_context_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_up_context_s OpenAPI_lcs_up_context_t;
#include "up_connection_status.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_up_context_s {
    OpenAPI_up_connection_status_e up_connection_status;
    char *serving_lmf_identification;
};

OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_create(
    OpenAPI_up_connection_status_e up_connection_status,
    char *serving_lmf_identification
);
void OpenAPI_lcs_up_context_free(OpenAPI_lcs_up_context_t *lcs_up_context);
OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_parseFromJSON(cJSON *lcs_up_contextJSON);
cJSON *OpenAPI_lcs_up_context_convertToJSON(OpenAPI_lcs_up_context_t *lcs_up_context);
OpenAPI_lcs_up_context_t *OpenAPI_lcs_up_context_copy(OpenAPI_lcs_up_context_t *dst, OpenAPI_lcs_up_context_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_up_context_H_ */

