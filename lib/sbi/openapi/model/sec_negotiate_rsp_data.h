/*
 * sec_negotiate_rsp_data.h
 *
 * Defines the selected security capabilities by a SEPP
 */

#ifndef _OpenAPI_sec_negotiate_rsp_data_H_
#define _OpenAPI_sec_negotiate_rsp_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_sec_negotiate_rsp_data_s OpenAPI_sec_negotiate_rsp_data_t;
#include "intended_n32_purpose.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "security_capability.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_sec_negotiate_rsp_data_s {
    char *sender;
    OpenAPI_security_capability_e selected_sec_capability;
    char *n32_handshake_id;
    bool is__3_gpp_sbi_target_api_root_supported;
    int _3_gpp_sbi_target_api_root_supported;
    OpenAPI_list_t *plmn_id_list;
    OpenAPI_list_t *snpn_id_list;
    OpenAPI_list_t *allowed_usage_purpose;
    OpenAPI_list_t *rejected_usage_purpose;
    char *supported_features;
    char *sender_n32f_fqdn;
    bool is_sender_n32f_port;
    int sender_n32f_port;
    bool is_n32_keepalive_timer;
    int n32_keepalive_timer;
};

OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_create(
    char *sender,
    OpenAPI_security_capability_e selected_sec_capability,
    char *n32_handshake_id,
    bool is__3_gpp_sbi_target_api_root_supported,
    int _3_gpp_sbi_target_api_root_supported,
    OpenAPI_list_t *plmn_id_list,
    OpenAPI_list_t *snpn_id_list,
    OpenAPI_list_t *allowed_usage_purpose,
    OpenAPI_list_t *rejected_usage_purpose,
    char *supported_features,
    char *sender_n32f_fqdn,
    bool is_sender_n32f_port,
    int sender_n32f_port,
    bool is_n32_keepalive_timer,
    int n32_keepalive_timer
);
void OpenAPI_sec_negotiate_rsp_data_free(OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data);
OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_parseFromJSON(cJSON *sec_negotiate_rsp_dataJSON);
cJSON *OpenAPI_sec_negotiate_rsp_data_convertToJSON(OpenAPI_sec_negotiate_rsp_data_t *sec_negotiate_rsp_data);
OpenAPI_sec_negotiate_rsp_data_t *OpenAPI_sec_negotiate_rsp_data_copy(OpenAPI_sec_negotiate_rsp_data_t *dst, OpenAPI_sec_negotiate_rsp_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_sec_negotiate_rsp_data_H_ */

