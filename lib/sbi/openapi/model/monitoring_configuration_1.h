/*
 * monitoring_configuration_1.h
 *
 * 
 */

#ifndef _OpenAPI_monitoring_configuration_1_H_
#define _OpenAPI_monitoring_configuration_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "association_type.h"
#include "datalink_reporting_configuration_1.h"
#include "event_type.h"
#include "location_reporting_configuration_1.h"
#include "loss_connectivity_cfg_1.h"
#include "pdu_session_status_cfg_1.h"
#include "reachability_for_data_configuration_1.h"
#include "reachability_for_sms_configuration.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_monitoring_configuration_1_s OpenAPI_monitoring_configuration_1_t;
typedef struct OpenAPI_monitoring_configuration_1_s {
    struct OpenAPI_event_type_s *event_type;
    bool is_immediate_flag;
    int immediate_flag;
    struct OpenAPI_location_reporting_configuration_1_s *location_reporting_configuration;
    struct OpenAPI_association_type_s *association_type;
    struct OpenAPI_datalink_reporting_configuration_1_s *datalink_report_cfg;
    struct OpenAPI_loss_connectivity_cfg_1_s *loss_connectivity_cfg;
    bool is_maximum_latency;
    int maximum_latency;
    bool is_maximum_response_time;
    int maximum_response_time;
    bool is_suggested_packet_num_dl;
    int suggested_packet_num_dl;
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
    struct OpenAPI_pdu_session_status_cfg_1_s *pdu_session_status_cfg;
    struct OpenAPI_reachability_for_sms_configuration_s *reachability_for_sms_cfg;
    char *mtc_provider_information;
    char *af_id;
    struct OpenAPI_reachability_for_data_configuration_1_s *reachability_for_data_cfg;
    bool is_idle_status_ind;
    int idle_status_ind;
} OpenAPI_monitoring_configuration_1_t;

OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_create(
    OpenAPI_event_type_t *event_type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_1_t *location_reporting_configuration,
    OpenAPI_association_type_t *association_type,
    OpenAPI_datalink_reporting_configuration_1_t *datalink_report_cfg,
    OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg,
    bool is_maximum_latency,
    int maximum_latency,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_suggested_packet_num_dl,
    int suggested_packet_num_dl,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg,
    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_cfg,
    char *mtc_provider_information,
    char *af_id,
    OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_cfg,
    bool is_idle_status_ind,
    int idle_status_ind
);
void OpenAPI_monitoring_configuration_1_free(OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1);
OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_parseFromJSON(cJSON *monitoring_configuration_1JSON);
cJSON *OpenAPI_monitoring_configuration_1_convertToJSON(OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1);
OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_copy(OpenAPI_monitoring_configuration_1_t *dst, OpenAPI_monitoring_configuration_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_monitoring_configuration_1_H_ */

