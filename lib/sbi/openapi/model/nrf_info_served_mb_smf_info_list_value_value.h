/*
 * nrf_info_served_mb_smf_info_list_value_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_mb_smf_info_list_value_value_H_
#define _OpenAPI_nrf_info_served_mb_smf_info_list_value_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "mb_smf_info.h"
#include "mbs_session.h"
#include "snssai_mb_smf_info_item.h"
#include "tai.h"
#include "tai_range.h"
#include "tmgi_range.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_mb_smf_info_list_value_value_s OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t;
typedef struct OpenAPI_nrf_info_served_mb_smf_info_list_value_value_s {
    OpenAPI_list_t* s_nssai_info_list;
    OpenAPI_list_t* tmgi_range_list;
    OpenAPI_list_t *tai_list;
    OpenAPI_list_t *tai_range_list;
    OpenAPI_list_t* mbs_session_list;
} OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t;

OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *OpenAPI_nrf_info_served_mb_smf_info_list_value_value_create(
    OpenAPI_list_t* s_nssai_info_list,
    OpenAPI_list_t* tmgi_range_list,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t* mbs_session_list
);
void OpenAPI_nrf_info_served_mb_smf_info_list_value_value_free(OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *nrf_info_served_mb_smf_info_list_value_value);
OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *OpenAPI_nrf_info_served_mb_smf_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_mb_smf_info_list_value_valueJSON);
cJSON *OpenAPI_nrf_info_served_mb_smf_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *nrf_info_served_mb_smf_info_list_value_value);
OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *OpenAPI_nrf_info_served_mb_smf_info_list_value_value_copy(OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *dst, OpenAPI_nrf_info_served_mb_smf_info_list_value_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_mb_smf_info_list_value_value_H_ */

