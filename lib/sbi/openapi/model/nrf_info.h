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
#include "amf_info.h"
#include "ausf_info.h"
#include "bsf_info.h"
#include "chf_info.h"
#include "gmlc_info.h"
#include "hss_info.h"
#include "lmf_info.h"
#include "nef_info.h"
#include "nf_info.h"
#include "nwdaf_info.h"
#include "pcf_info.h"
#include "pcscf_info.h"
#include "smf_info.h"
#include "udm_info.h"
#include "udr_info.h"
#include "upf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_s OpenAPI_nrf_info_t;
typedef struct OpenAPI_nrf_info_s {
    OpenAPI_list_t* served_udr_info;
    OpenAPI_list_t* served_udm_info;
    OpenAPI_list_t* served_ausf_info;
    OpenAPI_list_t* served_amf_info;
    OpenAPI_list_t* served_smf_info;
    OpenAPI_list_t* served_upf_info;
    OpenAPI_list_t* served_pcf_info;
    OpenAPI_list_t* served_bsf_info;
    OpenAPI_list_t* served_chf_info;
    OpenAPI_list_t* served_nef_info;
    OpenAPI_list_t* served_nwdaf_info;
    OpenAPI_list_t* served_pcscf_info;
    OpenAPI_list_t* served_gmlc_info;
    OpenAPI_list_t* served_lmf_info;
    OpenAPI_list_t* served_nf_info;
    OpenAPI_list_t* served_hss_info;
} OpenAPI_nrf_info_t;

OpenAPI_nrf_info_t *OpenAPI_nrf_info_create(
    OpenAPI_list_t* served_udr_info,
    OpenAPI_list_t* served_udm_info,
    OpenAPI_list_t* served_ausf_info,
    OpenAPI_list_t* served_amf_info,
    OpenAPI_list_t* served_smf_info,
    OpenAPI_list_t* served_upf_info,
    OpenAPI_list_t* served_pcf_info,
    OpenAPI_list_t* served_bsf_info,
    OpenAPI_list_t* served_chf_info,
    OpenAPI_list_t* served_nef_info,
    OpenAPI_list_t* served_nwdaf_info,
    OpenAPI_list_t* served_pcscf_info,
    OpenAPI_list_t* served_gmlc_info,
    OpenAPI_list_t* served_lmf_info,
    OpenAPI_list_t* served_nf_info,
    OpenAPI_list_t* served_hss_info
    );
void OpenAPI_nrf_info_free(OpenAPI_nrf_info_t *nrf_info);
OpenAPI_nrf_info_t *OpenAPI_nrf_info_parseFromJSON(cJSON *nrf_infoJSON);
cJSON *OpenAPI_nrf_info_convertToJSON(OpenAPI_nrf_info_t *nrf_info);
OpenAPI_nrf_info_t *OpenAPI_nrf_info_copy(OpenAPI_nrf_info_t *dst, OpenAPI_nrf_info_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_H_ */

