/*
 * vn_group_data_1.h
 *
 * Contains VN Group Data
 */

#ifndef _OpenAPI_vn_group_data_1_H_
#define _OpenAPI_vn_group_data_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vn_group_data_1_s OpenAPI_vn_group_data_1_t;
#include "app_descriptor_1.h"
#include "ip_address_1.h"
#include "max_group_data_rate.h"
#include "model_5_gvn_group_communication_type.h"
#include "pdu_session_types_1.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vn_group_data_1_s {
    struct OpenAPI_pdu_session_types_1_s *pdu_session_types;
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t *app_descriptors;
    bool is_secondary_auth;
    int secondary_auth;
    bool is_dn_aaa_ip_address_allocation;
    int dn_aaa_ip_address_allocation;
    struct OpenAPI_ip_address_1_s *dn_aaa_address;
    OpenAPI_list_t *additional_dn_aaa_addresses;
    char *dn_aaa_fqdn;
    bool is__5g_vn_group_communication_ind;
    int _5g_vn_group_communication_ind;
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_type;
    struct OpenAPI_max_group_data_rate_s *max_group_data_rate;
};

OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_create(
    OpenAPI_pdu_session_types_1_t *pdu_session_types,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t *app_descriptors,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_1_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn,
    bool is__5g_vn_group_communication_ind,
    int _5g_vn_group_communication_ind,
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_type,
    OpenAPI_max_group_data_rate_t *max_group_data_rate
);
void OpenAPI_vn_group_data_1_free(OpenAPI_vn_group_data_1_t *vn_group_data_1);
OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_parseFromJSON(cJSON *vn_group_data_1JSON);
cJSON *OpenAPI_vn_group_data_1_convertToJSON(OpenAPI_vn_group_data_1_t *vn_group_data_1);
OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_copy(OpenAPI_vn_group_data_1_t *dst, OpenAPI_vn_group_data_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vn_group_data_1_H_ */

