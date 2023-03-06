/*
 * authorized_network_slice_info.h
 *
 * Contains the authorized network slice information
 */

#ifndef _OpenAPI_authorized_network_slice_info_H_
#define _OpenAPI_authorized_network_slice_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "allowed_nssai.h"
#include "configured_snssai.h"
#include "nsag_info.h"
#include "nsi_information.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_authorized_network_slice_info_s OpenAPI_authorized_network_slice_info_t;
typedef struct OpenAPI_authorized_network_slice_info_s {
    OpenAPI_list_t *allowed_nssai_list;
    OpenAPI_list_t *configured_nssai;
    char *target_amf_set;
    OpenAPI_list_t *candidate_amf_list;
    OpenAPI_list_t *rejected_nssai_in_plmn;
    OpenAPI_list_t *rejected_nssai_in_ta;
    struct OpenAPI_nsi_information_s *nsi_information;
    char *supported_features;
    char *nrf_amf_set;
    char *nrf_amf_set_nf_mgt_uri;
    char *nrf_amf_set_access_token_uri;
    OpenAPI_list_t* nrf_oauth2_required;
    char *target_amf_service_set;
    OpenAPI_list_t *target_nssai;
    OpenAPI_list_t *nsag_infos;
} OpenAPI_authorized_network_slice_info_t;

OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_create(
    OpenAPI_list_t *allowed_nssai_list,
    OpenAPI_list_t *configured_nssai,
    char *target_amf_set,
    OpenAPI_list_t *candidate_amf_list,
    OpenAPI_list_t *rejected_nssai_in_plmn,
    OpenAPI_list_t *rejected_nssai_in_ta,
    OpenAPI_nsi_information_t *nsi_information,
    char *supported_features,
    char *nrf_amf_set,
    char *nrf_amf_set_nf_mgt_uri,
    char *nrf_amf_set_access_token_uri,
    OpenAPI_list_t* nrf_oauth2_required,
    char *target_amf_service_set,
    OpenAPI_list_t *target_nssai,
    OpenAPI_list_t *nsag_infos
);
void OpenAPI_authorized_network_slice_info_free(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info);
OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_parseFromJSON(cJSON *authorized_network_slice_infoJSON);
cJSON *OpenAPI_authorized_network_slice_info_convertToJSON(OpenAPI_authorized_network_slice_info_t *authorized_network_slice_info);
OpenAPI_authorized_network_slice_info_t *OpenAPI_authorized_network_slice_info_copy(OpenAPI_authorized_network_slice_info_t *dst, OpenAPI_authorized_network_slice_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_authorized_network_slice_info_H_ */

