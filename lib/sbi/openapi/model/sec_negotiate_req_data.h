/*
 * sec_negotiate_req_data.h
 *
 * Defines the security capabilities of a SEPP sent to a receiving SEPP
 */

#ifndef _OpenAPI_sec_negotiate_req_data_H_
#define _OpenAPI_sec_negotiate_req_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "intended_n32_purpose.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "security_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_sec_negotiate_req_data_s OpenAPI_sec_negotiate_req_data_t;
typedef struct OpenAPI_sec_negotiate_req_data_s {
    char *sender;
    OpenAPI_list_t *supported_sec_capability_list;
    bool is__3_gpp_sbi_target_api_root_supported;
    int _3_gpp_sbi_target_api_root_supported;
    OpenAPI_list_t *plmn_id_list;
    OpenAPI_list_t *snpn_id_list;
    struct OpenAPI_plmn_id_s *target_plmn_id;
    struct OpenAPI_plmn_id_nid_s *target_snpn_id;
    OpenAPI_list_t *intended_usage_purpose;
    char *supported_features;
} OpenAPI_sec_negotiate_req_data_t;

OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_create(
    char *sender,
    OpenAPI_list_t *supported_sec_capability_list,
    bool is__3_gpp_sbi_target_api_root_supported,
    int _3_gpp_sbi_target_api_root_supported,
    OpenAPI_list_t *plmn_id_list,
    OpenAPI_list_t *snpn_id_list,
    OpenAPI_plmn_id_t *target_plmn_id,
    OpenAPI_plmn_id_nid_t *target_snpn_id,
    OpenAPI_list_t *intended_usage_purpose,
    char *supported_features
);
void OpenAPI_sec_negotiate_req_data_free(OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data);
OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_parseFromJSON(cJSON *sec_negotiate_req_dataJSON);
cJSON *OpenAPI_sec_negotiate_req_data_convertToJSON(OpenAPI_sec_negotiate_req_data_t *sec_negotiate_req_data);
OpenAPI_sec_negotiate_req_data_t *OpenAPI_sec_negotiate_req_data_copy(OpenAPI_sec_negotiate_req_data_t *dst, OpenAPI_sec_negotiate_req_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sec_negotiate_req_data_H_ */

