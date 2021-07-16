/*
 * traffic_influ_data_patch.h
 *
 * 
 */

#ifndef _OpenAPI_traffic_influ_data_patch_H_
#define _OpenAPI_traffic_influ_data_patch_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "eth_flow_description.h"
#include "flow_info.h"
#include "network_area_info_1.h"
#include "route_to_location.h"
#include "snssai.h"
#include "temporal_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_traffic_influ_data_patch_s OpenAPI_traffic_influ_data_patch_t;
typedef struct OpenAPI_traffic_influ_data_patch_s {
    char *up_path_chg_notif_corre_id;
    bool is_app_relo_ind;
    int app_relo_ind;
    char *dnn;
    OpenAPI_list_t *eth_traffic_filters;
    struct OpenAPI_snssai_s *snssai;
    char *internal_group_id;
    char *supi;
    OpenAPI_list_t *traffic_filters;
    OpenAPI_list_t *traffic_routes;
    bool is_traff_corre_ind;
    int traff_corre_ind;
    char *valid_start_time;
    char *valid_end_time;
    OpenAPI_list_t *temp_validities;
    struct OpenAPI_network_area_info_1_s *nw_area_info;
    char *up_path_chg_notif_uri;
    OpenAPI_list_t *headers;
    bool is_af_ack_ind;
    int af_ack_ind;
    bool is_addr_preser_ind;
    int addr_preser_ind;
} OpenAPI_traffic_influ_data_patch_t;

OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_create(
    char *up_path_chg_notif_corre_id,
    bool is_app_relo_ind,
    int app_relo_ind,
    char *dnn,
    OpenAPI_list_t *eth_traffic_filters,
    OpenAPI_snssai_t *snssai,
    char *internal_group_id,
    char *supi,
    OpenAPI_list_t *traffic_filters,
    OpenAPI_list_t *traffic_routes,
    bool is_traff_corre_ind,
    int traff_corre_ind,
    char *valid_start_time,
    char *valid_end_time,
    OpenAPI_list_t *temp_validities,
    OpenAPI_network_area_info_1_t *nw_area_info,
    char *up_path_chg_notif_uri,
    OpenAPI_list_t *headers,
    bool is_af_ack_ind,
    int af_ack_ind,
    bool is_addr_preser_ind,
    int addr_preser_ind
);
void OpenAPI_traffic_influ_data_patch_free(OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch);
OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_parseFromJSON(cJSON *traffic_influ_data_patchJSON);
cJSON *OpenAPI_traffic_influ_data_patch_convertToJSON(OpenAPI_traffic_influ_data_patch_t *traffic_influ_data_patch);
OpenAPI_traffic_influ_data_patch_t *OpenAPI_traffic_influ_data_patch_copy(OpenAPI_traffic_influ_data_patch_t *dst, OpenAPI_traffic_influ_data_patch_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_traffic_influ_data_patch_H_ */

