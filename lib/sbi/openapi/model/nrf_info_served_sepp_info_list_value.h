/*
 * nrf_info_served_sepp_info_list_value.h
 *
 * 
 */

#ifndef _OpenAPI_nrf_info_served_sepp_info_list_value_H_
#define _OpenAPI_nrf_info_served_sepp_info_list_value_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "sepp_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nrf_info_served_sepp_info_list_value_s OpenAPI_nrf_info_served_sepp_info_list_value_t;
typedef struct OpenAPI_nrf_info_served_sepp_info_list_value_s {
    char *sepp_prefix;
    OpenAPI_list_t* sepp_ports;
    OpenAPI_list_t *remote_plmn_list;
    OpenAPI_list_t *remote_snpn_list;
} OpenAPI_nrf_info_served_sepp_info_list_value_t;

OpenAPI_nrf_info_served_sepp_info_list_value_t *OpenAPI_nrf_info_served_sepp_info_list_value_create(
    char *sepp_prefix,
    OpenAPI_list_t* sepp_ports,
    OpenAPI_list_t *remote_plmn_list,
    OpenAPI_list_t *remote_snpn_list
);
void OpenAPI_nrf_info_served_sepp_info_list_value_free(OpenAPI_nrf_info_served_sepp_info_list_value_t *nrf_info_served_sepp_info_list_value);
OpenAPI_nrf_info_served_sepp_info_list_value_t *OpenAPI_nrf_info_served_sepp_info_list_value_parseFromJSON(cJSON *nrf_info_served_sepp_info_list_valueJSON);
cJSON *OpenAPI_nrf_info_served_sepp_info_list_value_convertToJSON(OpenAPI_nrf_info_served_sepp_info_list_value_t *nrf_info_served_sepp_info_list_value);
OpenAPI_nrf_info_served_sepp_info_list_value_t *OpenAPI_nrf_info_served_sepp_info_list_value_copy(OpenAPI_nrf_info_served_sepp_info_list_value_t *dst, OpenAPI_nrf_info_served_sepp_info_list_value_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nrf_info_served_sepp_info_list_value_H_ */

