/*
 * protection_policy.h
 *
 * The protection policy to be negotiated between the SEPPs
 */

#ifndef _OpenAPI_protection_policy_H_
#define _OpenAPI_protection_policy_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "api_ie_mapping.h"
#include "ie_type.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_protection_policy_s OpenAPI_protection_policy_t;
typedef struct OpenAPI_protection_policy_s {
    OpenAPI_list_t *api_ie_mapping_list;
    OpenAPI_list_t *data_type_enc_policy;
} OpenAPI_protection_policy_t;

OpenAPI_protection_policy_t *OpenAPI_protection_policy_create(
    OpenAPI_list_t *api_ie_mapping_list,
    OpenAPI_list_t *data_type_enc_policy
);
void OpenAPI_protection_policy_free(OpenAPI_protection_policy_t *protection_policy);
OpenAPI_protection_policy_t *OpenAPI_protection_policy_parseFromJSON(cJSON *protection_policyJSON);
cJSON *OpenAPI_protection_policy_convertToJSON(OpenAPI_protection_policy_t *protection_policy);
OpenAPI_protection_policy_t *OpenAPI_protection_policy_copy(OpenAPI_protection_policy_t *dst, OpenAPI_protection_policy_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_protection_policy_H_ */

