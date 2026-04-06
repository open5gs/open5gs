/*
 * vn_group_data.h
 *
 * Contains VN Group Data
 */

#ifndef _OpenAPI_vn_group_data_H_
#define _OpenAPI_vn_group_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_vn_group_data_s OpenAPI_vn_group_data_t;
#include "app_descriptor.h"
#include "ip_address.h"
#include "max_group_data_rate.h"
#include "model_5_gvn_group_communication_type.h"
#include "pdu_session_types.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_vn_group_data_s {
    struct OpenAPI_pdu_session_types_s *pdu_session_types;
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
    OpenAPI_list_t *app_descriptors;
    bool is_secondary_auth;
    int secondary_auth;
    bool is_dn_aaa_ip_address_allocation;
    int dn_aaa_ip_address_allocation;
    struct OpenAPI_ip_address_s *dn_aaa_address;
    OpenAPI_list_t *additional_dn_aaa_addresses;
    char *dn_aaa_fqdn;
    bool is__5g_vn_group_communication_ind;
    int _5g_vn_group_communication_ind;
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_type;
    struct OpenAPI_max_group_data_rate_s *max_group_data_rate;
};

OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_create(
    OpenAPI_pdu_session_types_t *pdu_session_types,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t *app_descriptors,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn,
    bool is__5g_vn_group_communication_ind,
    int _5g_vn_group_communication_ind,
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_type,
    OpenAPI_max_group_data_rate_t *max_group_data_rate
);
void OpenAPI_vn_group_data_free(OpenAPI_vn_group_data_t *vn_group_data);
OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_parseFromJSON(cJSON *vn_group_dataJSON);
cJSON *OpenAPI_vn_group_data_convertToJSON(OpenAPI_vn_group_data_t *vn_group_data);
OpenAPI_vn_group_data_t *OpenAPI_vn_group_data_copy(OpenAPI_vn_group_data_t *dst, OpenAPI_vn_group_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_vn_group_data_H_ */

