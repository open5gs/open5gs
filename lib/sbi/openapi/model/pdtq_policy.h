/*
 * pdtq_policy.h
 *
 * Describes a PDTQ policy.
 */

#ifndef _OpenAPI_pdtq_policy_H_
#define _OpenAPI_pdtq_policy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdtq_policy_s OpenAPI_pdtq_policy_t;
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdtq_policy_s {
    int pdtq_policy_id;
    struct OpenAPI_time_window_s *rec_time_int;
};

OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_create(
    int pdtq_policy_id,
    OpenAPI_time_window_t *rec_time_int
);
void OpenAPI_pdtq_policy_free(OpenAPI_pdtq_policy_t *pdtq_policy);
OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_parseFromJSON(cJSON *pdtq_policyJSON);
cJSON *OpenAPI_pdtq_policy_convertToJSON(OpenAPI_pdtq_policy_t *pdtq_policy);
OpenAPI_pdtq_policy_t *OpenAPI_pdtq_policy_copy(OpenAPI_pdtq_policy_t *dst, OpenAPI_pdtq_policy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdtq_policy_H_ */

