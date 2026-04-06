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
typedef struct OpenAPI_bdt_data_patch_s OpenAPI_bdt_data_patch_t;
#include "bdt_policy_status.h"
#include "transfer_policy.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_bdt_data_patch_s {
    struct OpenAPI_transfer_policy_s *trans_policy;
    OpenAPI_bdt_policy_status_e bdtp_status;
    bool is_warn_notif_enabled;
    int warn_notif_enabled;
    char *notif_uri;
    bool is_energy_ind;
    int energy_ind;
};

OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_create(
    OpenAPI_transfer_policy_t *trans_policy,
    OpenAPI_bdt_policy_status_e bdtp_status,
    bool is_warn_notif_enabled,
    int warn_notif_enabled,
    char *notif_uri,
    bool is_energy_ind,
    int energy_ind
);
void OpenAPI_bdt_data_patch_free(OpenAPI_bdt_data_patch_t *bdt_data_patch);
OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_parseFromJSON(cJSON *bdt_data_patchJSON);
cJSON *OpenAPI_bdt_data_patch_convertToJSON(OpenAPI_bdt_data_patch_t *bdt_data_patch);
OpenAPI_bdt_data_patch_t *OpenAPI_bdt_data_patch_copy(OpenAPI_bdt_data_patch_t *dst, OpenAPI_bdt_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_data_patch_H_ */

