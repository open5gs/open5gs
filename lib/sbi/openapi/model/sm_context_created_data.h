/*
 * sm_context_created_data.h
 *
 *
 */

#ifndef _OpenAPI_sm_context_created_data_H_
#define _OpenAPI_sm_context_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ebi_arp_mapping.h"
#include "ho_state.h"
#include "n2_sm_info_type.h"
#include "ref_to_binary_data.h"
#include "snssai.h"
#include "up_cnx_state.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sm_context_created_data_s OpenAPI_sm_context_created_data_t;
typedef struct OpenAPI_sm_context_created_data_s {
    char *h_smf_uri;
    char *smf_uri;
    int pdu_session_id;
    struct OpenAPI_snssai_s *s_nssai;
    OpenAPI_up_cnx_state_e up_cnx_state;
    struct OpenAPI_ref_to_binary_data_s *n2_sm_info;
    OpenAPI_n2_sm_info_type_e n2_sm_info_type;
    OpenAPI_list_t *allocated_ebi_list;
    OpenAPI_ho_state_e ho_state;
    char *gpsi;
    char *smf_service_instance_id;
    char *recovery_time;
    char *supported_features;
} OpenAPI_sm_context_created_data_t;

OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_create(
    char *h_smf_uri,
    char *smf_uri,
    int pdu_session_id,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_up_cnx_state_e up_cnx_state,
    OpenAPI_ref_to_binary_data_t *n2_sm_info,
    OpenAPI_n2_sm_info_type_e n2_sm_info_type,
    OpenAPI_list_t *allocated_ebi_list,
    OpenAPI_ho_state_e ho_state,
    char *gpsi,
    char *smf_service_instance_id,
    char *recovery_time,
    char *supported_features
    );
void OpenAPI_sm_context_created_data_free(OpenAPI_sm_context_created_data_t *sm_context_created_data);
OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_parseFromJSON(cJSON *sm_context_created_dataJSON);
cJSON *OpenAPI_sm_context_created_data_convertToJSON(OpenAPI_sm_context_created_data_t *sm_context_created_data);
OpenAPI_sm_context_created_data_t *OpenAPI_sm_context_created_data_copy(OpenAPI_sm_context_created_data_t *dst, OpenAPI_sm_context_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sm_context_created_data_H_ */

