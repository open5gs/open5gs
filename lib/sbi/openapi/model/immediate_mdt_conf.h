/*
 * immediate_mdt_conf.h
 *
 * Immediate MDT Configuration
 */

#ifndef _OpenAPI_immediate_mdt_conf_H_
#define _OpenAPI_immediate_mdt_conf_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "area_scope.h"
#include "collection_period_rmm_lte_mdt.h"
#include "collection_period_rmm_nr_mdt.h"
#include "job_type.h"
#include "measurement_lte_for_mdt.h"
#include "measurement_nr_for_mdt.h"
#include "measurement_period_lte_mdt.h"
#include "plmn_id.h"
#include "positioning_method_mdt.h"
#include "report_amount_mdt.h"
#include "report_interval_mdt.h"
#include "report_interval_nr_mdt.h"
#include "reporting_trigger.h"
#include "sensor_measurement.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_immediate_mdt_conf_s OpenAPI_immediate_mdt_conf_t;
typedef struct OpenAPI_immediate_mdt_conf_s {
    OpenAPI_job_type_e job_type;
    OpenAPI_list_t *measurement_lte_list;
    OpenAPI_list_t *measurement_nr_list;
    OpenAPI_list_t *reporting_trigger_list;
    OpenAPI_report_interval_mdt_e report_interval;
    OpenAPI_report_interval_nr_mdt_e report_interval_nr;
    OpenAPI_report_amount_mdt_e report_amount;
    bool is_event_threshold_rsrp;
    int event_threshold_rsrp;
    bool is_event_threshold_rsrq;
    int event_threshold_rsrq;
    bool is_event_threshold_rsrp_nr;
    int event_threshold_rsrp_nr;
    bool is_event_threshold_rsrq_nr;
    int event_threshold_rsrq_nr;
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte;
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr;
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lte;
    struct OpenAPI_area_scope_s *area_scope;
    OpenAPI_positioning_method_mdt_e positioning_method;
    OpenAPI_list_t *add_positioning_method_list;
    OpenAPI_list_t *mdt_allowed_plmn_id_list;
    OpenAPI_list_t *sensor_measurement_list;
} OpenAPI_immediate_mdt_conf_t;

OpenAPI_immediate_mdt_conf_t *OpenAPI_immediate_mdt_conf_create(
    OpenAPI_job_type_e job_type,
    OpenAPI_list_t *measurement_lte_list,
    OpenAPI_list_t *measurement_nr_list,
    OpenAPI_list_t *reporting_trigger_list,
    OpenAPI_report_interval_mdt_e report_interval,
    OpenAPI_report_interval_nr_mdt_e report_interval_nr,
    OpenAPI_report_amount_mdt_e report_amount,
    bool is_event_threshold_rsrp,
    int event_threshold_rsrp,
    bool is_event_threshold_rsrq,
    int event_threshold_rsrq,
    bool is_event_threshold_rsrp_nr,
    int event_threshold_rsrp_nr,
    bool is_event_threshold_rsrq_nr,
    int event_threshold_rsrq_nr,
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lte,
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nr,
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lte,
    OpenAPI_area_scope_t *area_scope,
    OpenAPI_positioning_method_mdt_e positioning_method,
    OpenAPI_list_t *add_positioning_method_list,
    OpenAPI_list_t *mdt_allowed_plmn_id_list,
    OpenAPI_list_t *sensor_measurement_list
);
void OpenAPI_immediate_mdt_conf_free(OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf);
OpenAPI_immediate_mdt_conf_t *OpenAPI_immediate_mdt_conf_parseFromJSON(cJSON *immediate_mdt_confJSON);
cJSON *OpenAPI_immediate_mdt_conf_convertToJSON(OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf);
OpenAPI_immediate_mdt_conf_t *OpenAPI_immediate_mdt_conf_copy(OpenAPI_immediate_mdt_conf_t *dst, OpenAPI_immediate_mdt_conf_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_immediate_mdt_conf_H_ */

