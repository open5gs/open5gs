/*
 * bdt_data.h
 *
 * Contains the background data transfer data.
 */

#ifndef _OpenAPI_bdt_data_H_
#define _OpenAPI_bdt_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "bdt_policy_status.h"
#include "network_area_info_2.h"
#include "snssai.h"
#include "transfer_policy.h"
#include "usage_threshold.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_bdt_data_s OpenAPI_bdt_data_t;
typedef struct OpenAPI_bdt_data_s {
    char *asp_id;
    struct OpenAPI_transfer_policy_s *trans_policy;
    char *bdt_ref_id;
    struct OpenAPI_network_area_info_2_s *nw_area_info;
    bool is_num_of_ues;
    int num_of_ues;
    struct OpenAPI_usage_threshold_s *vol_per_ue;
    char *dnn;
    struct OpenAPI_snssai_s *snssai;
    char *traffic_des;
    struct OpenAPI_bdt_policy_status_s *bdtp_status;
    char *supp_feat;
    OpenAPI_list_t *reset_ids;
} OpenAPI_bdt_data_t;

OpenAPI_bdt_data_t *OpenAPI_bdt_data_create(
    char *asp_id,
    OpenAPI_transfer_policy_t *trans_policy,
    char *bdt_ref_id,
    OpenAPI_network_area_info_2_t *nw_area_info,
    bool is_num_of_ues,
    int num_of_ues,
    OpenAPI_usage_threshold_t *vol_per_ue,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *traffic_des,
    OpenAPI_bdt_policy_status_t *bdtp_status,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
);
void OpenAPI_bdt_data_free(OpenAPI_bdt_data_t *bdt_data);
OpenAPI_bdt_data_t *OpenAPI_bdt_data_parseFromJSON(cJSON *bdt_dataJSON);
cJSON *OpenAPI_bdt_data_convertToJSON(OpenAPI_bdt_data_t *bdt_data);
OpenAPI_bdt_data_t *OpenAPI_bdt_data_copy(OpenAPI_bdt_data_t *dst, OpenAPI_bdt_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_bdt_data_H_ */

