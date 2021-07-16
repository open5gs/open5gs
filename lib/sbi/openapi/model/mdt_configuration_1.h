/*
 * mdt_configuration_1.h
 *
 * 
 */

#ifndef _OpenAPI_mdt_configuration_1_H_
#define _OpenAPI_mdt_configuration_1_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "area_scope.h"
#include "collection_period_rmm_lte_mdt.h"
#include "collection_period_rmm_nr_mdt.h"
#include "event_for_mdt.h"
#include "inter_freq_target_info_1.h"
#include "job_type.h"
#include "logging_duration_mdt.h"
#include "logging_duration_nr_mdt.h"
#include "logging_interval_mdt.h"
#include "logging_interval_nr_mdt.h"
#include "mbsfn_area.h"
#include "measurement_lte_for_mdt.h"
#include "measurement_nr_for_mdt.h"
#include "measurement_period_lte_mdt.h"
#include "plmn_id_1.h"
#include "positioning_method_mdt.h"
#include "report_amount_mdt.h"
#include "report_interval_mdt.h"
#include "report_interval_nr_mdt.h"
#include "report_type_mdt.h"
#include "reporting_trigger.h"
#include "sensor_measurement.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_mdt_configuration_1_s OpenAPI_mdt_configuration_1_t;
typedef struct OpenAPI_mdt_configuration_1_s {
    OpenAPI_job_type_e job_type;
    OpenAPI_report_type_mdt_e report_type;
    struct OpenAPI_area_scope_s *area_scope;
    OpenAPI_list_t *measurement_lte_list;
    OpenAPI_list_t *measurement_nr_list;
    OpenAPI_list_t *sensor_measurement_list;
    OpenAPI_list_t *reporting_trigger_list;
    OpenAPI_report_interval_mdt_e report_interval;
    OpenAPI_report_interval_nr_mdt_e report_interval_nr;
    OpenAPI_report_amount_mdt_e report_amount;
    bool is_event_threshold_rsrp;
    int event_threshold_rsrp;
    bool is_event_threshold_rsrp_nr;
    int event_threshold_rsrp_nr;
    bool is_event_threshold_rsrq;
    int event_threshold_rsrq;
    bool is_event_threshold_rsrq_nr;
    int event_threshold_rsrq_nr;
    OpenAPI_list_t *event_list;
    OpenAPI_logging_interval_mdt_e logging_interval;
    OpenAPI_logging_interval_nr_mdt_e logging_interval_nr;
    OpenAPI_logging_duration_mdt_e logging_duration;
    OpenAPI_logging_duration_nr_mdt_e logging_duration_nr;
    OpenAPI_positioning_method_mdt_e positioning_method;
    OpenAPI_list_t *add_positioning_method_list;
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte;
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr;
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lte;
    OpenAPI_list_t *mdt_allowed_plmn_id_list;
    OpenAPI_list_t *mbsfn_area_list;
    OpenAPI_list_t *inter_freq_target_list;
} OpenAPI_mdt_configuration_1_t;

OpenAPI_mdt_configuration_1_t *OpenAPI_mdt_configuration_1_create(
    OpenAPI_job_type_e job_type,
    OpenAPI_report_type_mdt_e report_type,
    OpenAPI_area_scope_t *area_scope,
    OpenAPI_list_t *measurement_lte_list,
    OpenAPI_list_t *measurement_nr_list,
    OpenAPI_list_t *sensor_measurement_list,
    OpenAPI_list_t *reporting_trigger_list,
    OpenAPI_report_interval_mdt_e report_interval,
    OpenAPI_report_interval_nr_mdt_e report_interval_nr,
    OpenAPI_report_amount_mdt_e report_amount,
    bool is_event_threshold_rsrp,
    int event_threshold_rsrp,
    bool is_event_threshold_rsrp_nr,
    int event_threshold_rsrp_nr,
    bool is_event_threshold_rsrq,
    int event_threshold_rsrq,
    bool is_event_threshold_rsrq_nr,
    int event_threshold_rsrq_nr,
    OpenAPI_list_t *event_list,
    OpenAPI_logging_interval_mdt_e logging_interval,
    OpenAPI_logging_interval_nr_mdt_e logging_interval_nr,
    OpenAPI_logging_duration_mdt_e logging_duration,
    OpenAPI_logging_duration_nr_mdt_e logging_duration_nr,
    OpenAPI_positioning_method_mdt_e positioning_method,
    OpenAPI_list_t *add_positioning_method_list,
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte,
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr,
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lte,
    OpenAPI_list_t *mdt_allowed_plmn_id_list,
    OpenAPI_list_t *mbsfn_area_list,
    OpenAPI_list_t *inter_freq_target_list
);
void OpenAPI_mdt_configuration_1_free(OpenAPI_mdt_configuration_1_t *mdt_configuration_1);
OpenAPI_mdt_configuration_1_t *OpenAPI_mdt_configuration_1_parseFromJSON(cJSON *mdt_configuration_1JSON);
cJSON *OpenAPI_mdt_configuration_1_convertToJSON(OpenAPI_mdt_configuration_1_t *mdt_configuration_1);
OpenAPI_mdt_configuration_1_t *OpenAPI_mdt_configuration_1_copy(OpenAPI_mdt_configuration_1_t *dst, OpenAPI_mdt_configuration_1_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_mdt_configuration_1_H_ */

