/*
 * lcs_subscription_data_1.h
 *
 * LSC Subscription Data
 */

#ifndef _OpenAPI_lcs_subscription_data_1_H_
#define _OpenAPI_lcs_subscription_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_lcs_subscription_data_1_s OpenAPI_lcs_subscription_data_1_t;
#include "lp_hap_type.h"
#include "pru_ind.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_lcs_subscription_data_1_s {
    char *configured_lmf_id;
    OpenAPI_pru_ind_e pru_ind;
    OpenAPI_lp_hap_type_e lp_hap_type;
    bool is_user_plane_pos_ind_lmf;
    int user_plane_pos_ind_lmf;
};

OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_create(
    char *configured_lmf_id,
    OpenAPI_pru_ind_e pru_ind,
    OpenAPI_lp_hap_type_e lp_hap_type,
    bool is_user_plane_pos_ind_lmf,
    int user_plane_pos_ind_lmf
);
void OpenAPI_lcs_subscription_data_1_free(OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1);
OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_parseFromJSON(cJSON *lcs_subscription_data_1JSON);
cJSON *OpenAPI_lcs_subscription_data_1_convertToJSON(OpenAPI_lcs_subscription_data_1_t *lcs_subscription_data_1);
OpenAPI_lcs_subscription_data_1_t *OpenAPI_lcs_subscription_data_1_copy(OpenAPI_lcs_subscription_data_1_t *dst, OpenAPI_lcs_subscription_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_subscription_data_1_H_ */

