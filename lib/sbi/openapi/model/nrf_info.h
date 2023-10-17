/*
 * nrf_info.h
 *
 * Information of an NRF NF Instance, used in hierarchical NRF deployments
 */

#ifndef _OpenAPI_nrf_info_H_
#define _OpenAPI_nrf_info_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "dccf_info.h"
#include "easdf_info.h"
#include "mb_upf_info.h"
#include "mfaf_info.h"
#include "model_5_g_ddnmf_info.h"
#include "nf_info.h"
#include "nrf_info_served_aanf_info_list_value_value.h"
#include "nrf_info_served_amf_info_value.h"
#include "nrf_info_served_ausf_info_value.h"
#include "nrf_info_served_bsf_info_value.h"
#include "nrf_info_served_chf_info_value.h"
#include "nrf_info_served_gmlc_info_value.h"
#include "nrf_info_served_hss_info_list_value_value.h"
#include "nrf_info_served_lmf_info_value.h"
#include "nrf_info_served_mb_smf_info_list_value_value.h"
#include "nrf_info_served_nef_info_value.h"
#include "nrf_info_served_nwdaf_info_value.h"
#include "nrf_info_served_pcf_info_value.h"
#include "nrf_info_served_pcscf_info_list_value_value.h"
#include "nrf_info_served_scp_info_list_value.h"
#include "nrf_info_served_sepp_info_list_value.h"
#include "nrf_info_served_smf_info_value.h"
#include "nrf_info_served_udm_info_value.h"
#include "nrf_info_served_udr_info_value.h"
#include "nrf_info_served_udsf_info_value.h"
#include "nrf_info_served_upf_info_value.h"
#include "nssaaf_info.h"
#include "nwdaf_info.h"
#include "trust_af_info.h"
#include "tsctsf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_s OpenAPI_nrf_info_t;
typedef struct OpenAPI_nrf_info_s {
    OpenAPI_list_t* served_udr_info;
    OpenAPI_list_t* served_udr_info_list;
    OpenAPI_list_t* served_udm_info;
    OpenAPI_list_t* served_udm_info_list;
    OpenAPI_list_t* served_ausf_info;
    OpenAPI_list_t* served_ausf_info_list;
    OpenAPI_list_t* served_amf_info;
    OpenAPI_list_t* served_amf_info_list;
    OpenAPI_list_t* served_smf_info;
    OpenAPI_list_t* served_smf_info_list;
    OpenAPI_list_t* served_upf_info;
    OpenAPI_list_t* served_upf_info_list;
    OpenAPI_list_t* served_pcf_info;
    OpenAPI_list_t* served_pcf_info_list;
    OpenAPI_list_t* served_bsf_info;
    OpenAPI_list_t* served_bsf_info_list;
    OpenAPI_list_t* served_chf_info;
    OpenAPI_list_t* served_chf_info_list;
    OpenAPI_list_t* served_nef_info;
    OpenAPI_list_t* served_nwdaf_info;
    OpenAPI_list_t* served_nwdaf_info_list;
    OpenAPI_list_t* served_pcscf_info_list;
    OpenAPI_list_t* served_gmlc_info;
    OpenAPI_list_t* served_lmf_info;
    OpenAPI_list_t* served_nf_info;
    OpenAPI_list_t* served_hss_info_list;
    OpenAPI_list_t* served_udsf_info;
    OpenAPI_list_t* served_udsf_info_list;
    OpenAPI_list_t* served_scp_info_list;
    OpenAPI_list_t* served_sepp_info_list;
    OpenAPI_list_t* served_aanf_info_list;
    OpenAPI_list_t* served5g_ddnmf_info;
    OpenAPI_list_t* served_mfaf_info_list;
    OpenAPI_list_t* served_easdf_info_list;
    OpenAPI_list_t* served_dccf_info_list;
    OpenAPI_list_t* served_mb_smf_info_list;
    OpenAPI_list_t* served_tsctsf_info_list;
    OpenAPI_list_t* served_mb_upf_info_list;
    OpenAPI_list_t* served_trust_af_info;
    OpenAPI_list_t* served_nssaaf_info;
} OpenAPI_nrf_info_t;

OpenAPI_nrf_info_t *OpenAPI_nrf_info_create(
    OpenAPI_list_t* served_udr_info,
    OpenAPI_list_t* served_udr_info_list,
    OpenAPI_list_t* served_udm_info,
    OpenAPI_list_t* served_udm_info_list,
    OpenAPI_list_t* served_ausf_info,
    OpenAPI_list_t* served_ausf_info_list,
    OpenAPI_list_t* served_amf_info,
    OpenAPI_list_t* served_amf_info_list,
    OpenAPI_list_t* served_smf_info,
    OpenAPI_list_t* served_smf_info_list,
    OpenAPI_list_t* served_upf_info,
    OpenAPI_list_t* served_upf_info_list,
    OpenAPI_list_t* served_pcf_info,
    OpenAPI_list_t* served_pcf_info_list,
    OpenAPI_list_t* served_bsf_info,
    OpenAPI_list_t* served_bsf_info_list,
    OpenAPI_list_t* served_chf_info,
    OpenAPI_list_t* served_chf_info_list,
    OpenAPI_list_t* served_nef_info,
    OpenAPI_list_t* served_nwdaf_info,
    OpenAPI_list_t* served_nwdaf_info_list,
    OpenAPI_list_t* served_pcscf_info_list,
    OpenAPI_list_t* served_gmlc_info,
    OpenAPI_list_t* served_lmf_info,
    OpenAPI_list_t* served_nf_info,
    OpenAPI_list_t* served_hss_info_list,
    OpenAPI_list_t* served_udsf_info,
    OpenAPI_list_t* served_udsf_info_list,
    OpenAPI_list_t* served_scp_info_list,
    OpenAPI_list_t* served_sepp_info_list,
    OpenAPI_list_t* served_aanf_info_list,
    OpenAPI_list_t* served5g_ddnmf_info,
    OpenAPI_list_t* served_mfaf_info_list,
    OpenAPI_list_t* served_easdf_info_list,
    OpenAPI_list_t* served_dccf_info_list,
    OpenAPI_list_t* served_mb_smf_info_list,
    OpenAPI_list_t* served_tsctsf_info_list,
    OpenAPI_list_t* served_mb_upf_info_list,
    OpenAPI_list_t* served_trust_af_info,
    OpenAPI_list_t* served_nssaaf_info
);
void OpenAPI_nrf_info_free(OpenAPI_nrf_info_t *nrf_info);
OpenAPI_nrf_info_t *OpenAPI_nrf_info_parseFromJSON(cJSON *nrf_infoJSON);
cJSON *OpenAPI_nrf_info_convertToJSON(OpenAPI_nrf_info_t *nrf_info);
OpenAPI_nrf_info_t *OpenAPI_nrf_info_copy(OpenAPI_nrf_info_t *dst, OpenAPI_nrf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_H_ */

