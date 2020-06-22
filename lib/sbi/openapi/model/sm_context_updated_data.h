/*
 * sm_context_updated_data.h
 *
 *
 */

#ifndef _OpenAPI_sm_context_updated_data_H_
#define _OpenAPI_sm_context_updated_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "cause.h"
#include "ebi_arp_mapping.h"
#include "ho_state.h"
#include "indirect_data_forwarding_tunnel_info.h"
#include "n2_sm_info_type.h"
#include "ref_to_binary_data.h"
#include "up_cnx_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_updated_data_s OpenAPI_sm_context_updated_data_t;
typedef struct OpenAPI_sm_context_updated_data_s {
    OpenAPI_up_cnx_state_e up_cnx_state;
    OpenAPI_ho_state_e ho_state;
    OpenAPI_list_t *release_ebi_list;
    OpenAPI_list_t *allocated_ebi_list;
    OpenAPI_list_t *modified_ebi_list;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_msg;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type;
    OpenAPI_list_t *eps_bearer_setup;
    int data_forwarding;
    OpenAPI_list_t *n3_dl_forwarding_tnl_list;
    OpenAPI_list_t *n3_ul_forwarding_tnl_list;
    OpenAPI_cause_e cause;
    int ma_accepted_ind;
    char *supported_features;
    char forwarding_f_teid;
    OpenAPI_list_t *forwarding_bearer_contexts;
} OpenAPI_sm_context_updated_data_t;

OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_create(
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_ho_state_e ho_state,
    OpenAPI_list_t *release_ebi_list,
    OpenAPI_list_t *allocated_ebi_list,
    OpenAPI_list_t *modified_ebi_list,
    OpenAPI_ref_to_binary_data_t *n1_sm_msg,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_list_t *eps_bearer_setup,
    int data_forwarding,
    OpenAPI_list_t *n3_dl_forwarding_tnl_list,
    OpenAPI_list_t *n3_ul_forwarding_tnl_list,
    OpenAPI_cause_e cause,
    int ma_accepted_ind,
    char *supported_features,
    char forwarding_f_teid,
    OpenAPI_list_t *forwarding_bearer_contexts
    );
void OpenAPI_sm_context_updated_data_free(OpenAPI_sm_context_updated_data_t *sm_context_updated_data);
OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_parseFromJSON(cJSON *sm_context_updated_dataJSON);
cJSON *OpenAPI_sm_context_updated_data_convertToJSON(OpenAPI_sm_context_updated_data_t *sm_context_updated_data);
OpenAPI_sm_context_updated_data_t *OpenAPI_sm_context_updated_data_copy(OpenAPI_sm_context_updated_data_t *dst, OpenAPI_sm_context_updated_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_updated_data_H_ */

