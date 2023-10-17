/*
 * monitoring_configuration.h
 *
 * 
 */

#ifndef _OpenAPI_monitoring_configuration_H_
#define _OpenAPI_monitoring_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "association_type.h"
#include "datalink_reporting_configuration.h"
#include "event_type.h"
#include "location_reporting_configuration.h"
#include "loss_connectivity_cfg.h"
#include "pdu_session_status_cfg.h"
#include "reachability_for_data_configuration.h"
#include "reachability_for_sms_configuration.h"
#include "snssai.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_monitoring_configuration_s OpenAPI_monitoring_configuration_t;
typedef struct OpenAPI_monitoring_configuration_s {
    struct OpenAPI_event_type_s *event_type;
    bool is_immediate_flag;
    int immediate_flag;
    struct OpenAPI_location_reporting_configuration_s *location_reporting_configuration;
    struct OpenAPI_association_type_s *association_type;
    struct OpenAPI_datalink_reporting_configuration_s *datalink_report_cfg;
    struct OpenAPI_loss_connectivity_cfg_s *loss_connectivity_cfg;
    bool is_maximum_latency;
    int maximum_latency;
    bool is_maximum_response_time;
    int maximum_response_time;
    bool is_suggested_packet_num_dl;
    int suggested_packet_num_dl;
    char *dnn;
    struct OpenAPI_snssai_s *single_nssai;
    struct OpenAPI_pdu_session_status_cfg_s *pdu_session_status_cfg;
    struct OpenAPI_reachability_for_sms_configuration_s *reachability_for_sms_cfg;
    char *mtc_provider_information;
    char *af_id;
    struct OpenAPI_reachability_for_data_configuration_s *reachability_for_data_cfg;
    bool is_idle_status_ind;
    int idle_status_ind;
} OpenAPI_monitoring_configuration_t;

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_create(
    OpenAPI_event_type_t *event_type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration,
    OpenAPI_association_type_t *association_type,
    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg,
    OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg,
    bool is_maximum_latency,
    int maximum_latency,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_suggested_packet_num_dl,
    int suggested_packet_num_dl,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg,
    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_cfg,
    char *mtc_provider_information,
    char *af_id,
    OpenAPI_reachability_for_data_configuration_t *reachability_for_data_cfg,
    bool is_idle_status_ind,
    int idle_status_ind
);
void OpenAPI_monitoring_configuration_free(OpenAPI_monitoring_configuration_t *monitoring_configuration);
OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_parseFromJSON(cJSON *monitoring_configurationJSON);
cJSON *OpenAPI_monitoring_configuration_convertToJSON(OpenAPI_monitoring_configuration_t *monitoring_configuration);
OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_copy(OpenAPI_monitoring_configuration_t *dst, OpenAPI_monitoring_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_monitoring_configuration_H_ */

