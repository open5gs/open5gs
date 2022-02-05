
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mdt_configuration.h"

OpenAPI_mdt_configuration_t *OpenAPI_mdt_configuration_create(
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
)
{
    OpenAPI_mdt_configuration_t *mdt_configuration_local_var = ogs_malloc(sizeof(OpenAPI_mdt_configuration_t));
    ogs_assert(mdt_configuration_local_var);

    mdt_configuration_local_var->job_type = job_type;
    mdt_configuration_local_var->report_type = report_type;
    mdt_configuration_local_var->area_scope = area_scope;
    mdt_configuration_local_var->measurement_lte_list = measurement_lte_list;
    mdt_configuration_local_var->measurement_nr_list = measurement_nr_list;
    mdt_configuration_local_var->sensor_measurement_list = sensor_measurement_list;
    mdt_configuration_local_var->reporting_trigger_list = reporting_trigger_list;
    mdt_configuration_local_var->report_interval = report_interval;
    mdt_configuration_local_var->report_interval_nr = report_interval_nr;
    mdt_configuration_local_var->report_amount = report_amount;
    mdt_configuration_local_var->is_event_threshold_rsrp = is_event_threshold_rsrp;
    mdt_configuration_local_var->event_threshold_rsrp = event_threshold_rsrp;
    mdt_configuration_local_var->is_event_threshold_rsrp_nr = is_event_threshold_rsrp_nr;
    mdt_configuration_local_var->event_threshold_rsrp_nr = event_threshold_rsrp_nr;
    mdt_configuration_local_var->is_event_threshold_rsrq = is_event_threshold_rsrq;
    mdt_configuration_local_var->event_threshold_rsrq = event_threshold_rsrq;
    mdt_configuration_local_var->is_event_threshold_rsrq_nr = is_event_threshold_rsrq_nr;
    mdt_configuration_local_var->event_threshold_rsrq_nr = event_threshold_rsrq_nr;
    mdt_configuration_local_var->event_list = event_list;
    mdt_configuration_local_var->logging_interval = logging_interval;
    mdt_configuration_local_var->logging_interval_nr = logging_interval_nr;
    mdt_configuration_local_var->logging_duration = logging_duration;
    mdt_configuration_local_var->logging_duration_nr = logging_duration_nr;
    mdt_configuration_local_var->positioning_method = positioning_method;
    mdt_configuration_local_var->add_positioning_method_list = add_positioning_method_list;
    mdt_configuration_local_var->collection_period_rmm_lte = collection_period_rmm_lte;
    mdt_configuration_local_var->collection_period_rmm_nr = collection_period_rmm_nr;
    mdt_configuration_local_var->measurement_period_lte = measurement_period_lte;
    mdt_configuration_local_var->mdt_allowed_plmn_id_list = mdt_allowed_plmn_id_list;
    mdt_configuration_local_var->mbsfn_area_list = mbsfn_area_list;
    mdt_configuration_local_var->inter_freq_target_list = inter_freq_target_list;

    return mdt_configuration_local_var;
}

void OpenAPI_mdt_configuration_free(OpenAPI_mdt_configuration_t *mdt_configuration)
{
    if (NULL == mdt_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_area_scope_free(mdt_configuration->area_scope);
    OpenAPI_list_free(mdt_configuration->measurement_lte_list);
    OpenAPI_list_free(mdt_configuration->measurement_nr_list);
    OpenAPI_list_free(mdt_configuration->sensor_measurement_list);
    OpenAPI_list_free(mdt_configuration->reporting_trigger_list);
    OpenAPI_list_free(mdt_configuration->event_list);
    OpenAPI_list_free(mdt_configuration->add_positioning_method_list);
    OpenAPI_list_for_each(mdt_configuration->mdt_allowed_plmn_id_list, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(mdt_configuration->mdt_allowed_plmn_id_list);
    OpenAPI_list_for_each(mdt_configuration->mbsfn_area_list, node) {
        OpenAPI_mbsfn_area_free(node->data);
    }
    OpenAPI_list_free(mdt_configuration->mbsfn_area_list);
    OpenAPI_list_for_each(mdt_configuration->inter_freq_target_list, node) {
        OpenAPI_inter_freq_target_info_free(node->data);
    }
    OpenAPI_list_free(mdt_configuration->inter_freq_target_list);
    ogs_free(mdt_configuration);
}

cJSON *OpenAPI_mdt_configuration_convertToJSON(OpenAPI_mdt_configuration_t *mdt_configuration)
{
    cJSON *item = NULL;

    if (mdt_configuration == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [MdtConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "jobType", OpenAPI_job_type_ToString(mdt_configuration->job_type)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [job_type]");
        goto end;
    }

    if (mdt_configuration->report_type) {
    if (cJSON_AddStringToObject(item, "reportType", OpenAPI_report_type_mdt_ToString(mdt_configuration->report_type)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_type]");
        goto end;
    }
    }

    if (mdt_configuration->area_scope) {
    cJSON *area_scope_local_JSON = OpenAPI_area_scope_convertToJSON(mdt_configuration->area_scope);
    if (area_scope_local_JSON == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [area_scope]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaScope", area_scope_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [area_scope]");
        goto end;
    }
    }

    if (mdt_configuration->measurement_lte_list) {
    cJSON *measurement_lte_list = cJSON_AddArrayToObject(item, "measurementLteList");
    if (measurement_lte_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_lte_list]");
        goto end;
    }
    OpenAPI_lnode_t *measurement_lte_list_node;
    OpenAPI_list_for_each(mdt_configuration->measurement_lte_list, measurement_lte_list_node) {
        if (cJSON_AddStringToObject(measurement_lte_list, "", OpenAPI_measurement_lte_for_mdt_ToString((intptr_t)measurement_lte_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_lte_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->measurement_nr_list) {
    cJSON *measurement_nr_list = cJSON_AddArrayToObject(item, "measurementNrList");
    if (measurement_nr_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_nr_list]");
        goto end;
    }
    OpenAPI_lnode_t *measurement_nr_list_node;
    OpenAPI_list_for_each(mdt_configuration->measurement_nr_list, measurement_nr_list_node) {
        if (cJSON_AddStringToObject(measurement_nr_list, "", OpenAPI_measurement_nr_for_mdt_ToString((intptr_t)measurement_nr_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_nr_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->sensor_measurement_list) {
    cJSON *sensor_measurement_list = cJSON_AddArrayToObject(item, "sensorMeasurementList");
    if (sensor_measurement_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [sensor_measurement_list]");
        goto end;
    }
    OpenAPI_lnode_t *sensor_measurement_list_node;
    OpenAPI_list_for_each(mdt_configuration->sensor_measurement_list, sensor_measurement_list_node) {
        if (cJSON_AddStringToObject(sensor_measurement_list, "", OpenAPI_sensor_measurement_ToString((intptr_t)sensor_measurement_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [sensor_measurement_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->reporting_trigger_list) {
    cJSON *reporting_trigger_list = cJSON_AddArrayToObject(item, "reportingTriggerList");
    if (reporting_trigger_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [reporting_trigger_list]");
        goto end;
    }
    OpenAPI_lnode_t *reporting_trigger_list_node;
    OpenAPI_list_for_each(mdt_configuration->reporting_trigger_list, reporting_trigger_list_node) {
        if (cJSON_AddStringToObject(reporting_trigger_list, "", OpenAPI_reporting_trigger_ToString((intptr_t)reporting_trigger_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [reporting_trigger_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->report_interval) {
    if (cJSON_AddStringToObject(item, "reportInterval", OpenAPI_report_interval_mdt_ToString(mdt_configuration->report_interval)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_interval]");
        goto end;
    }
    }

    if (mdt_configuration->report_interval_nr) {
    if (cJSON_AddStringToObject(item, "reportIntervalNr", OpenAPI_report_interval_nr_mdt_ToString(mdt_configuration->report_interval_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_interval_nr]");
        goto end;
    }
    }

    if (mdt_configuration->report_amount) {
    if (cJSON_AddStringToObject(item, "reportAmount", OpenAPI_report_amount_mdt_ToString(mdt_configuration->report_amount)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_amount]");
        goto end;
    }
    }

    if (mdt_configuration->is_event_threshold_rsrp) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrp", mdt_configuration->event_threshold_rsrp) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    if (mdt_configuration->is_event_threshold_rsrp_nr) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrpNr", mdt_configuration->event_threshold_rsrp_nr) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    if (mdt_configuration->is_event_threshold_rsrq) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrq", mdt_configuration->event_threshold_rsrq) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    if (mdt_configuration->is_event_threshold_rsrq_nr) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrqNr", mdt_configuration->event_threshold_rsrq_nr) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    if (mdt_configuration->event_list) {
    cJSON *event_list = cJSON_AddArrayToObject(item, "eventList");
    if (event_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_list]");
        goto end;
    }
    OpenAPI_lnode_t *event_list_node;
    OpenAPI_list_for_each(mdt_configuration->event_list, event_list_node) {
        if (cJSON_AddStringToObject(event_list, "", OpenAPI_event_for_mdt_ToString((intptr_t)event_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->logging_interval) {
    if (cJSON_AddStringToObject(item, "loggingInterval", OpenAPI_logging_interval_mdt_ToString(mdt_configuration->logging_interval)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_interval]");
        goto end;
    }
    }

    if (mdt_configuration->logging_interval_nr) {
    if (cJSON_AddStringToObject(item, "loggingIntervalNr", OpenAPI_logging_interval_nr_mdt_ToString(mdt_configuration->logging_interval_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_interval_nr]");
        goto end;
    }
    }

    if (mdt_configuration->logging_duration) {
    if (cJSON_AddStringToObject(item, "loggingDuration", OpenAPI_logging_duration_mdt_ToString(mdt_configuration->logging_duration)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_duration]");
        goto end;
    }
    }

    if (mdt_configuration->logging_duration_nr) {
    if (cJSON_AddStringToObject(item, "loggingDurationNr", OpenAPI_logging_duration_nr_mdt_ToString(mdt_configuration->logging_duration_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_duration_nr]");
        goto end;
    }
    }

    if (mdt_configuration->positioning_method) {
    if (cJSON_AddStringToObject(item, "positioningMethod", OpenAPI_positioning_method_mdt_ToString(mdt_configuration->positioning_method)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [positioning_method]");
        goto end;
    }
    }

    if (mdt_configuration->add_positioning_method_list) {
    cJSON *add_positioning_method_list = cJSON_AddArrayToObject(item, "addPositioningMethodList");
    if (add_positioning_method_list == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [add_positioning_method_list]");
        goto end;
    }
    OpenAPI_lnode_t *add_positioning_method_list_node;
    OpenAPI_list_for_each(mdt_configuration->add_positioning_method_list, add_positioning_method_list_node) {
        if (cJSON_AddStringToObject(add_positioning_method_list, "", OpenAPI_positioning_method_mdt_ToString((intptr_t)add_positioning_method_list_node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [add_positioning_method_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->collection_period_rmm_lte) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmLte", OpenAPI_collection_period_rmm_lte_mdt_ToString(mdt_configuration->collection_period_rmm_lte)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    }

    if (mdt_configuration->collection_period_rmm_nr) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmNr", OpenAPI_collection_period_rmm_nr_mdt_ToString(mdt_configuration->collection_period_rmm_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    }

    if (mdt_configuration->measurement_period_lte) {
    if (cJSON_AddStringToObject(item, "measurementPeriodLte", OpenAPI_measurement_period_lte_mdt_ToString(mdt_configuration->measurement_period_lte)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_period_lte]");
        goto end;
    }
    }

    if (mdt_configuration->mdt_allowed_plmn_id_list) {
    cJSON *mdt_allowed_plmn_id_listList = cJSON_AddArrayToObject(item, "mdtAllowedPlmnIdList");
    if (mdt_allowed_plmn_id_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mdt_allowed_plmn_id_list]");
        goto end;
    }

    OpenAPI_lnode_t *mdt_allowed_plmn_id_list_node;
    if (mdt_configuration->mdt_allowed_plmn_id_list) {
        OpenAPI_list_for_each(mdt_configuration->mdt_allowed_plmn_id_list, mdt_allowed_plmn_id_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(mdt_allowed_plmn_id_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mdt_allowed_plmn_id_list]");
                goto end;
            }
            cJSON_AddItemToArray(mdt_allowed_plmn_id_listList, itemLocal);
        }
    }
    }

    if (mdt_configuration->mbsfn_area_list) {
    cJSON *mbsfn_area_listList = cJSON_AddArrayToObject(item, "mbsfnAreaList");
    if (mbsfn_area_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mbsfn_area_list]");
        goto end;
    }

    OpenAPI_lnode_t *mbsfn_area_list_node;
    if (mdt_configuration->mbsfn_area_list) {
        OpenAPI_list_for_each(mdt_configuration->mbsfn_area_list, mbsfn_area_list_node) {
            cJSON *itemLocal = OpenAPI_mbsfn_area_convertToJSON(mbsfn_area_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mbsfn_area_list]");
                goto end;
            }
            cJSON_AddItemToArray(mbsfn_area_listList, itemLocal);
        }
    }
    }

    if (mdt_configuration->inter_freq_target_list) {
    cJSON *inter_freq_target_listList = cJSON_AddArrayToObject(item, "interFreqTargetList");
    if (inter_freq_target_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [inter_freq_target_list]");
        goto end;
    }

    OpenAPI_lnode_t *inter_freq_target_list_node;
    if (mdt_configuration->inter_freq_target_list) {
        OpenAPI_list_for_each(mdt_configuration->inter_freq_target_list, inter_freq_target_list_node) {
            cJSON *itemLocal = OpenAPI_inter_freq_target_info_convertToJSON(inter_freq_target_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [inter_freq_target_list]");
                goto end;
            }
            cJSON_AddItemToArray(inter_freq_target_listList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_mdt_configuration_t *OpenAPI_mdt_configuration_parseFromJSON(cJSON *mdt_configurationJSON)
{
    OpenAPI_mdt_configuration_t *mdt_configuration_local_var = NULL;
    cJSON *job_type = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "jobType");
    if (!job_type) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }

    OpenAPI_job_type_e job_typeVariable;
    if (!cJSON_IsString(job_type)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }
    job_typeVariable = OpenAPI_job_type_FromString(job_type->valuestring);

    cJSON *report_type = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportType");

    OpenAPI_report_type_mdt_e report_typeVariable;
    if (report_type) {
    if (!cJSON_IsString(report_type)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_type]");
        goto end;
    }
    report_typeVariable = OpenAPI_report_type_mdt_FromString(report_type->valuestring);
    }

    cJSON *area_scope = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "areaScope");

    OpenAPI_area_scope_t *area_scope_local_nonprim = NULL;
    if (area_scope) {
    area_scope_local_nonprim = OpenAPI_area_scope_parseFromJSON(area_scope);
    }

    cJSON *measurement_lte_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementLteList");

    OpenAPI_list_t *measurement_lte_listList;
    if (measurement_lte_list) {
    cJSON *measurement_lte_list_local_nonprimitive;
    if (!cJSON_IsArray(measurement_lte_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_lte_list]");
        goto end;
    }

    measurement_lte_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(measurement_lte_list_local_nonprimitive, measurement_lte_list ) {
        if (!cJSON_IsString(measurement_lte_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_lte_list]");
            goto end;
        }

        OpenAPI_list_add(measurement_lte_listList, (void *)OpenAPI_measurement_lte_for_mdt_FromString(measurement_lte_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *measurement_nr_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementNrList");

    OpenAPI_list_t *measurement_nr_listList;
    if (measurement_nr_list) {
    cJSON *measurement_nr_list_local_nonprimitive;
    if (!cJSON_IsArray(measurement_nr_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_nr_list]");
        goto end;
    }

    measurement_nr_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(measurement_nr_list_local_nonprimitive, measurement_nr_list ) {
        if (!cJSON_IsString(measurement_nr_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_nr_list]");
            goto end;
        }

        OpenAPI_list_add(measurement_nr_listList, (void *)OpenAPI_measurement_nr_for_mdt_FromString(measurement_nr_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *sensor_measurement_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "sensorMeasurementList");

    OpenAPI_list_t *sensor_measurement_listList;
    if (sensor_measurement_list) {
    cJSON *sensor_measurement_list_local_nonprimitive;
    if (!cJSON_IsArray(sensor_measurement_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [sensor_measurement_list]");
        goto end;
    }

    sensor_measurement_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(sensor_measurement_list_local_nonprimitive, sensor_measurement_list ) {
        if (!cJSON_IsString(sensor_measurement_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [sensor_measurement_list]");
            goto end;
        }

        OpenAPI_list_add(sensor_measurement_listList, (void *)OpenAPI_sensor_measurement_FromString(sensor_measurement_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *reporting_trigger_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportingTriggerList");

    OpenAPI_list_t *reporting_trigger_listList;
    if (reporting_trigger_list) {
    cJSON *reporting_trigger_list_local_nonprimitive;
    if (!cJSON_IsArray(reporting_trigger_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [reporting_trigger_list]");
        goto end;
    }

    reporting_trigger_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(reporting_trigger_list_local_nonprimitive, reporting_trigger_list ) {
        if (!cJSON_IsString(reporting_trigger_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [reporting_trigger_list]");
            goto end;
        }

        OpenAPI_list_add(reporting_trigger_listList, (void *)OpenAPI_reporting_trigger_FromString(reporting_trigger_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *report_interval = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportInterval");

    OpenAPI_report_interval_mdt_e report_intervalVariable;
    if (report_interval) {
    if (!cJSON_IsString(report_interval)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_interval]");
        goto end;
    }
    report_intervalVariable = OpenAPI_report_interval_mdt_FromString(report_interval->valuestring);
    }

    cJSON *report_interval_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportIntervalNr");

    OpenAPI_report_interval_nr_mdt_e report_interval_nrVariable;
    if (report_interval_nr) {
    if (!cJSON_IsString(report_interval_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_interval_nr]");
        goto end;
    }
    report_interval_nrVariable = OpenAPI_report_interval_nr_mdt_FromString(report_interval_nr->valuestring);
    }

    cJSON *report_amount = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportAmount");

    OpenAPI_report_amount_mdt_e report_amountVariable;
    if (report_amount) {
    if (!cJSON_IsString(report_amount)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_amount]");
        goto end;
    }
    report_amountVariable = OpenAPI_report_amount_mdt_FromString(report_amount->valuestring);
    }

    cJSON *event_threshold_rsrp = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrp");

    if (event_threshold_rsrp) {
    if (!cJSON_IsNumber(event_threshold_rsrp)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrp_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrpNr");

    if (event_threshold_rsrp_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrp_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrq = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrq");

    if (event_threshold_rsrq) {
    if (!cJSON_IsNumber(event_threshold_rsrq)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrq_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrqNr");

    if (event_threshold_rsrq_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrq_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    cJSON *event_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventList");

    OpenAPI_list_t *event_listList;
    if (event_list) {
    cJSON *event_list_local_nonprimitive;
    if (!cJSON_IsArray(event_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_list]");
        goto end;
    }

    event_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(event_list_local_nonprimitive, event_list ) {
        if (!cJSON_IsString(event_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_list]");
            goto end;
        }

        OpenAPI_list_add(event_listList, (void *)OpenAPI_event_for_mdt_FromString(event_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *logging_interval = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingInterval");

    OpenAPI_logging_interval_mdt_e logging_intervalVariable;
    if (logging_interval) {
    if (!cJSON_IsString(logging_interval)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_interval]");
        goto end;
    }
    logging_intervalVariable = OpenAPI_logging_interval_mdt_FromString(logging_interval->valuestring);
    }

    cJSON *logging_interval_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingIntervalNr");

    OpenAPI_logging_interval_nr_mdt_e logging_interval_nrVariable;
    if (logging_interval_nr) {
    if (!cJSON_IsString(logging_interval_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_interval_nr]");
        goto end;
    }
    logging_interval_nrVariable = OpenAPI_logging_interval_nr_mdt_FromString(logging_interval_nr->valuestring);
    }

    cJSON *logging_duration = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingDuration");

    OpenAPI_logging_duration_mdt_e logging_durationVariable;
    if (logging_duration) {
    if (!cJSON_IsString(logging_duration)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_duration]");
        goto end;
    }
    logging_durationVariable = OpenAPI_logging_duration_mdt_FromString(logging_duration->valuestring);
    }

    cJSON *logging_duration_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingDurationNr");

    OpenAPI_logging_duration_nr_mdt_e logging_duration_nrVariable;
    if (logging_duration_nr) {
    if (!cJSON_IsString(logging_duration_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_duration_nr]");
        goto end;
    }
    logging_duration_nrVariable = OpenAPI_logging_duration_nr_mdt_FromString(logging_duration_nr->valuestring);
    }

    cJSON *positioning_method = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "positioningMethod");

    OpenAPI_positioning_method_mdt_e positioning_methodVariable;
    if (positioning_method) {
    if (!cJSON_IsString(positioning_method)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [positioning_method]");
        goto end;
    }
    positioning_methodVariable = OpenAPI_positioning_method_mdt_FromString(positioning_method->valuestring);
    }

    cJSON *add_positioning_method_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "addPositioningMethodList");

    OpenAPI_list_t *add_positioning_method_listList;
    if (add_positioning_method_list) {
    cJSON *add_positioning_method_list_local_nonprimitive;
    if (!cJSON_IsArray(add_positioning_method_list)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [add_positioning_method_list]");
        goto end;
    }

    add_positioning_method_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(add_positioning_method_list_local_nonprimitive, add_positioning_method_list ) {
        if (!cJSON_IsString(add_positioning_method_list_local_nonprimitive)){
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [add_positioning_method_list]");
            goto end;
        }

        OpenAPI_list_add(add_positioning_method_listList, (void *)OpenAPI_positioning_method_mdt_FromString(add_positioning_method_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *collection_period_rmm_lte = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "collectionPeriodRmmLte");

    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lteVariable;
    if (collection_period_rmm_lte) {
    if (!cJSON_IsString(collection_period_rmm_lte)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    collection_period_rmm_lteVariable = OpenAPI_collection_period_rmm_lte_mdt_FromString(collection_period_rmm_lte->valuestring);
    }

    cJSON *collection_period_rmm_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "collectionPeriodRmmNr");

    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nrVariable;
    if (collection_period_rmm_nr) {
    if (!cJSON_IsString(collection_period_rmm_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    collection_period_rmm_nrVariable = OpenAPI_collection_period_rmm_nr_mdt_FromString(collection_period_rmm_nr->valuestring);
    }

    cJSON *measurement_period_lte = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementPeriodLte");

    OpenAPI_measurement_period_lte_mdt_e measurement_period_lteVariable;
    if (measurement_period_lte) {
    if (!cJSON_IsString(measurement_period_lte)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_period_lte]");
        goto end;
    }
    measurement_period_lteVariable = OpenAPI_measurement_period_lte_mdt_FromString(measurement_period_lte->valuestring);
    }

    cJSON *mdt_allowed_plmn_id_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "mdtAllowedPlmnIdList");

    OpenAPI_list_t *mdt_allowed_plmn_id_listList;
    if (mdt_allowed_plmn_id_list) {
    cJSON *mdt_allowed_plmn_id_list_local_nonprimitive;
    if (!cJSON_IsArray(mdt_allowed_plmn_id_list)){
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
        goto end;
    }

    mdt_allowed_plmn_id_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(mdt_allowed_plmn_id_list_local_nonprimitive, mdt_allowed_plmn_id_list ) {
        if (!cJSON_IsObject(mdt_allowed_plmn_id_list_local_nonprimitive)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
            goto end;
        }
        OpenAPI_plmn_id_t *mdt_allowed_plmn_id_listItem = OpenAPI_plmn_id_parseFromJSON(mdt_allowed_plmn_id_list_local_nonprimitive);

        if (!mdt_allowed_plmn_id_listItem) {
            ogs_error("No mdt_allowed_plmn_id_listItem");
            OpenAPI_list_free(mdt_allowed_plmn_id_listList);
            goto end;
        }

        OpenAPI_list_add(mdt_allowed_plmn_id_listList, mdt_allowed_plmn_id_listItem);
    }
    }

    cJSON *mbsfn_area_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "mbsfnAreaList");

    OpenAPI_list_t *mbsfn_area_listList;
    if (mbsfn_area_list) {
    cJSON *mbsfn_area_list_local_nonprimitive;
    if (!cJSON_IsArray(mbsfn_area_list)){
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mbsfn_area_list]");
        goto end;
    }

    mbsfn_area_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(mbsfn_area_list_local_nonprimitive, mbsfn_area_list ) {
        if (!cJSON_IsObject(mbsfn_area_list_local_nonprimitive)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mbsfn_area_list]");
            goto end;
        }
        OpenAPI_mbsfn_area_t *mbsfn_area_listItem = OpenAPI_mbsfn_area_parseFromJSON(mbsfn_area_list_local_nonprimitive);

        if (!mbsfn_area_listItem) {
            ogs_error("No mbsfn_area_listItem");
            OpenAPI_list_free(mbsfn_area_listList);
            goto end;
        }

        OpenAPI_list_add(mbsfn_area_listList, mbsfn_area_listItem);
    }
    }

    cJSON *inter_freq_target_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "interFreqTargetList");

    OpenAPI_list_t *inter_freq_target_listList;
    if (inter_freq_target_list) {
    cJSON *inter_freq_target_list_local_nonprimitive;
    if (!cJSON_IsArray(inter_freq_target_list)){
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [inter_freq_target_list]");
        goto end;
    }

    inter_freq_target_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(inter_freq_target_list_local_nonprimitive, inter_freq_target_list ) {
        if (!cJSON_IsObject(inter_freq_target_list_local_nonprimitive)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [inter_freq_target_list]");
            goto end;
        }
        OpenAPI_inter_freq_target_info_t *inter_freq_target_listItem = OpenAPI_inter_freq_target_info_parseFromJSON(inter_freq_target_list_local_nonprimitive);

        if (!inter_freq_target_listItem) {
            ogs_error("No inter_freq_target_listItem");
            OpenAPI_list_free(inter_freq_target_listList);
            goto end;
        }

        OpenAPI_list_add(inter_freq_target_listList, inter_freq_target_listItem);
    }
    }

    mdt_configuration_local_var = OpenAPI_mdt_configuration_create (
        job_typeVariable,
        report_type ? report_typeVariable : 0,
        area_scope ? area_scope_local_nonprim : NULL,
        measurement_lte_list ? measurement_lte_listList : NULL,
        measurement_nr_list ? measurement_nr_listList : NULL,
        sensor_measurement_list ? sensor_measurement_listList : NULL,
        reporting_trigger_list ? reporting_trigger_listList : NULL,
        report_interval ? report_intervalVariable : 0,
        report_interval_nr ? report_interval_nrVariable : 0,
        report_amount ? report_amountVariable : 0,
        event_threshold_rsrp ? true : false,
        event_threshold_rsrp ? event_threshold_rsrp->valuedouble : 0,
        event_threshold_rsrp_nr ? true : false,
        event_threshold_rsrp_nr ? event_threshold_rsrp_nr->valuedouble : 0,
        event_threshold_rsrq ? true : false,
        event_threshold_rsrq ? event_threshold_rsrq->valuedouble : 0,
        event_threshold_rsrq_nr ? true : false,
        event_threshold_rsrq_nr ? event_threshold_rsrq_nr->valuedouble : 0,
        event_list ? event_listList : NULL,
        logging_interval ? logging_intervalVariable : 0,
        logging_interval_nr ? logging_interval_nrVariable : 0,
        logging_duration ? logging_durationVariable : 0,
        logging_duration_nr ? logging_duration_nrVariable : 0,
        positioning_method ? positioning_methodVariable : 0,
        add_positioning_method_list ? add_positioning_method_listList : NULL,
        collection_period_rmm_lte ? collection_period_rmm_lteVariable : 0,
        collection_period_rmm_nr ? collection_period_rmm_nrVariable : 0,
        measurement_period_lte ? measurement_period_lteVariable : 0,
        mdt_allowed_plmn_id_list ? mdt_allowed_plmn_id_listList : NULL,
        mbsfn_area_list ? mbsfn_area_listList : NULL,
        inter_freq_target_list ? inter_freq_target_listList : NULL
    );

    return mdt_configuration_local_var;
end:
    return NULL;
}

OpenAPI_mdt_configuration_t *OpenAPI_mdt_configuration_copy(OpenAPI_mdt_configuration_t *dst, OpenAPI_mdt_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_mdt_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_mdt_configuration_free(dst);
    dst = OpenAPI_mdt_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

