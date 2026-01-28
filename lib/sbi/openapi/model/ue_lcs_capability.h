/*
 * ue_lcs_capability.h
 *
 * Indicates the LCS capability supported by the UE..
 */

#ifndef _OpenAPI_ue_lcs_capability_H_
#define _OpenAPI_ue_lcs_capability_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_ue_lcs_capability_s OpenAPI_ue_lcs_capability_t;
typedef struct OpenAPI_ue_lcs_capability_s {
    bool is_lpp_support;
    int lpp_support;
    bool is_ciot_optimisation;
    int ciot_optimisation;
} OpenAPI_ue_lcs_capability_t;

OpenAPI_ue_lcs_capability_t *OpenAPI_ue_lcs_capability_create(
    bool is_lpp_support,
    int lpp_support,
    bool is_ciot_optimisation,
    int ciot_optimisation
);
void OpenAPI_ue_lcs_capability_free(OpenAPI_ue_lcs_capability_t *ue_lcs_capability);
OpenAPI_ue_lcs_capability_t *OpenAPI_ue_lcs_capability_parseFromJSON(cJSON *ue_lcs_capabilityJSON);
cJSON *OpenAPI_ue_lcs_capability_convertToJSON(OpenAPI_ue_lcs_capability_t *ue_lcs_capability);
OpenAPI_ue_lcs_capability_t *OpenAPI_ue_lcs_capability_copy(OpenAPI_ue_lcs_capability_t *dst, OpenAPI_ue_lcs_capability_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_ue_lcs_capability_H_ */

