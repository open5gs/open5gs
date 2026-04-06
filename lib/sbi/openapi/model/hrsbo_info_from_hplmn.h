/*
 * hrsbo_info_from_hplmn.h
 *
 * HR-SBO Information in response message
 */

#ifndef _OpenAPI_hrsbo_info_from_hplmn_H_
#define _OpenAPI_hrsbo_info_from_hplmn_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_hrsbo_info_from_hplmn_s OpenAPI_hrsbo_info_from_hplmn_t;
#include "ip_address.h"
#include "traffic_influence_info.h"
#include "vplmn_dl_ambr.h"
#include "vplmn_offloading_info.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_hrsbo_info_from_hplmn_s {
    bool is_hrsbo_auth_result;
    int hrsbo_auth_result;
    struct OpenAPI_ip_address_s *h_dns_addr;
    struct OpenAPI_ip_address_s *h_plmn_addr;
    OpenAPI_list_t *vplmn_offloading_info_list;
    bool is_vplmn_dl_ambr_null;
    struct OpenAPI_vplmn_dl_ambr_s *vplmn_dl_ambr;
    OpenAPI_list_t *offload_ids;
    OpenAPI_list_t *internal_group_ids;
    char *target_dnai;
    struct OpenAPI_traffic_influence_info_s *traffic_influ_info;
};

OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_create(
    bool is_hrsbo_auth_result,
    int hrsbo_auth_result,
    OpenAPI_ip_address_t *h_dns_addr,
    OpenAPI_ip_address_t *h_plmn_addr,
    OpenAPI_list_t *vplmn_offloading_info_list,
    bool is_vplmn_dl_ambr_null,
    OpenAPI_vplmn_dl_ambr_t *vplmn_dl_ambr,
    OpenAPI_list_t *offload_ids,
    OpenAPI_list_t *internal_group_ids,
    char *target_dnai,
    OpenAPI_traffic_influence_info_t *traffic_influ_info
);
void OpenAPI_hrsbo_info_from_hplmn_free(OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn);
OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_parseFromJSON(cJSON *hrsbo_info_from_hplmnJSON);
cJSON *OpenAPI_hrsbo_info_from_hplmn_convertToJSON(OpenAPI_hrsbo_info_from_hplmn_t *hrsbo_info_from_hplmn);
OpenAPI_hrsbo_info_from_hplmn_t *OpenAPI_hrsbo_info_from_hplmn_copy(OpenAPI_hrsbo_info_from_hplmn_t *dst, OpenAPI_hrsbo_info_from_hplmn_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hrsbo_info_from_hplmn_H_ */

