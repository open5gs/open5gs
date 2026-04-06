/*
 * af_requested_qos_data.h
 *
 * 
 */

#ifndef _OpenAPI_af_requested_qos_data_H_
#define _OpenAPI_af_requested_qos_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_af_requested_qos_data_s OpenAPI_af_requested_qos_data_t;
#include "alternative_service_requirements_data.h"
#include "eth_flow_description_1.h"
#include "eth_flow_info.h"
#include "events_subsc_req_data.h"
#include "flow_info_1.h"
#include "qos_requirements.h"
#include "snssai.h"
#include "temporal_in_validity.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_af_requested_qos_data_s {
    char *supi;
    char *inter_group_id;
    char *af_app_id;
    char *dnn;
    struct OpenAPI_snssai_s *slice_info;
    OpenAPI_list_t *flow_info;
    OpenAPI_list_t *eth_flow_info;
    OpenAPI_list_t *en_eth_flow_info;
    struct OpenAPI_events_subsc_req_data_s *ev_subsc;
    char *qos_reference;
    struct OpenAPI_qos_requirements_s *qos_reqs;
    OpenAPI_list_t *alt_ser_reqs;
    OpenAPI_list_t *alt_ser_reqs_data;
    bool is_dis_ue_notif;
    int dis_ue_notif;
    struct OpenAPI_temporal_in_validity_s *temp_in_validity;
    OpenAPI_list_t *headers;
    char *supp_feat;
};

OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_create(
    char *supi,
    char *inter_group_id,
    char *af_app_id,
    char *dnn,
    OpenAPI_snssai_t *slice_info,
    OpenAPI_list_t *flow_info,
    OpenAPI_list_t *eth_flow_info,
    OpenAPI_list_t *en_eth_flow_info,
    OpenAPI_events_subsc_req_data_t *ev_subsc,
    char *qos_reference,
    OpenAPI_qos_requirements_t *qos_reqs,
    OpenAPI_list_t *alt_ser_reqs,
    OpenAPI_list_t *alt_ser_reqs_data,
    bool is_dis_ue_notif,
    int dis_ue_notif,
    OpenAPI_temporal_in_validity_t *temp_in_validity,
    OpenAPI_list_t *headers,
    char *supp_feat
);
void OpenAPI_af_requested_qos_data_free(OpenAPI_af_requested_qos_data_t *af_requested_qos_data);
OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_parseFromJSON(cJSON *af_requested_qos_dataJSON);
cJSON *OpenAPI_af_requested_qos_data_convertToJSON(OpenAPI_af_requested_qos_data_t *af_requested_qos_data);
OpenAPI_af_requested_qos_data_t *OpenAPI_af_requested_qos_data_copy(OpenAPI_af_requested_qos_data_t *dst, OpenAPI_af_requested_qos_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_af_requested_qos_data_H_ */

