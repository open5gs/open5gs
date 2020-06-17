/*
 * transfer_policy.h
 *
 * Describes a transfer policy.
 */

#ifndef _OpenAPI_transfer_policy_H_
#define _OpenAPI_transfer_policy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "time_window.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_transfer_policy_s OpenAPI_transfer_policy_t;
typedef struct OpenAPI_transfer_policy_s {
    char *max_bit_rate_dl;
    char *max_bit_rate_ul;
    int rating_group;
    struct OpenAPI_time_window_s *rec_time_int;
    int trans_policy_id;
} OpenAPI_transfer_policy_t;

OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_create(
    char *max_bit_rate_dl,
    char *max_bit_rate_ul,
    int rating_group,
    OpenAPI_time_window_t *rec_time_int,
    int trans_policy_id
    );
void OpenAPI_transfer_policy_free(OpenAPI_transfer_policy_t *transfer_policy);
OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_parseFromJSON(cJSON *transfer_policyJSON);
cJSON *OpenAPI_transfer_policy_convertToJSON(OpenAPI_transfer_policy_t *transfer_policy);
OpenAPI_transfer_policy_t *OpenAPI_transfer_policy_copy(OpenAPI_transfer_policy_t *dst, OpenAPI_transfer_policy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_transfer_policy_H_ */

