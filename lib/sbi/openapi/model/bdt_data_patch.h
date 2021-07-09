/*
 * bdt_data_patch.h
 *
 * Contains the modified background data transfer data.
 */

#ifndef _OpenAPI_bdt_data_patch_H_
#define _OpenAPI_bdt_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bdt_policy_status.h"
#include "transfer_policy.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bdt_data_patch_s OpenAPI_bdt_data_patch_t;
typedef struct OpenAPI_bdt_data_patch_s {
    struct OpenAPI_transfer_policy_s *trans_policy;
    struct OpenAPI_bdt_policy_status_s *bdtp_status;
} OpenAPI_bdt_data_patch_t;

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_create(
    OpenAPI_transfer_policy_t *trans_policy,
    OpenAPI_bdt_policy_status_t *bdtp_status
);
void OpenAPI_bdt_data_patch_free(OpenAPI_bdt_data_patch_t *bdt_data_patch);
OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_parseFromJSON(cJSON *bdt_data_patchJSON);
cJSON *OpenAPI_bdt_data_patch_convertToJSON(OpenAPI_bdt_data_patch_t *bdt_data_patch);
OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_copy(OpenAPI_bdt_data_patch_t *dst, OpenAPI_bdt_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_data_patch_H_ */

