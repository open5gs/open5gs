/*
 * n1_n2_message_transfer_req_data.h
 *
 * Data within a N1/N2 message transfer request
 */

#ifndef _OpenAPI_n1_n2_message_transfer_req_data_H_
#define _OpenAPI_n1_n2_message_transfer_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "access_type.h"
#include "area_of_validity.h"
#include "arp.h"
#include "guami.h"
#include "n1_message_container.h"
#include "n2_info_container.h"
#include "ref_to_binary_data.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_n1_n2_message_transfer_req_data_s OpenAPI_n1_n2_message_transfer_req_data_t;
typedef struct OpenAPI_n1_n2_message_transfer_req_data_s {
    struct OpenAPI_n1_message_container_s *n1_message_container;
    struct OpenAPI_n2_info_container_s *n2_info_container;
    struct OpenAPI_ref_to_binary_data_s *mt_data;
    bool is_skip_ind;
    int skip_ind;
    bool is_last_msg_indication;
    int last_msg_indication;
    bool is_pdu_session_id;
    int pdu_session_id;
    char *lcs_correlation_id;
    bool is_ppi;
    int ppi;
    struct OpenAPI_arp_s *arp;
    bool is__5qi;
    int _5qi;
    char *n1n2_failure_txf_notif_uri;
    bool is_smf_reallocation_ind;
    int smf_reallocation_ind;
    struct OpenAPI_area_of_validity_s *area_of_validity;
    char *supported_features;
    struct OpenAPI_guami_s *old_guami;
    bool is_ma_accepted_ind;
    int ma_accepted_ind;
    bool is_ext_buf_support;
    int ext_buf_support;
    OpenAPI_access_type_e target_access;
    char *nf_id;
} OpenAPI_n1_n2_message_transfer_req_data_t;

OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_create(
    OpenAPI_n1_message_container_t *n1_message_container,
    OpenAPI_n2_info_container_t *n2_info_container,
    OpenAPI_ref_to_binary_data_t *mt_data,
    bool is_skip_ind,
    int skip_ind,
    bool is_last_msg_indication,
    int last_msg_indication,
    bool is_pdu_session_id,
    int pdu_session_id,
    char *lcs_correlation_id,
    bool is_ppi,
    int ppi,
    OpenAPI_arp_t *arp,
    bool is__5qi,
    int _5qi,
    char *n1n2_failure_txf_notif_uri,
    bool is_smf_reallocation_ind,
    int smf_reallocation_ind,
    OpenAPI_area_of_validity_t *area_of_validity,
    char *supported_features,
    OpenAPI_guami_t *old_guami,
    bool is_ma_accepted_ind,
    int ma_accepted_ind,
    bool is_ext_buf_support,
    int ext_buf_support,
    OpenAPI_access_type_e target_access,
    char *nf_id
);
void OpenAPI_n1_n2_message_transfer_req_data_free(OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data);
OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_parseFromJSON(cJSON *n1_n2_message_transfer_req_dataJSON);
cJSON *OpenAPI_n1_n2_message_transfer_req_data_convertToJSON(OpenAPI_n1_n2_message_transfer_req_data_t *n1_n2_message_transfer_req_data);
OpenAPI_n1_n2_message_transfer_req_data_t *OpenAPI_n1_n2_message_transfer_req_data_copy(OpenAPI_n1_n2_message_transfer_req_data_t *dst, OpenAPI_n1_n2_message_transfer_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_n1_n2_message_transfer_req_data_H_ */

