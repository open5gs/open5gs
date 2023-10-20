
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
    OpenAPI_lnode_t *node = NULL;

    if (NULL == mdt_configuration) {
        return;
    }
    if (mdt_configuration->area_scope) {
        OpenAPI_area_scope_free(mdt_configuration->area_scope);
        mdt_configuration->area_scope = NULL;
    }
    if (mdt_configuration->measurement_lte_list) {
        OpenAPI_list_free(mdt_configuration->measurement_lte_list);
        mdt_configuration->measurement_lte_list = NULL;
    }
    if (mdt_configuration->measurement_nr_list) {
        OpenAPI_list_free(mdt_configuration->measurement_nr_list);
        mdt_configuration->measurement_nr_list = NULL;
    }
    if (mdt_configuration->sensor_measurement_list) {
        OpenAPI_list_free(mdt_configuration->sensor_measurement_list);
        mdt_configuration->sensor_measurement_list = NULL;
    }
    if (mdt_configuration->reporting_trigger_list) {
        OpenAPI_list_free(mdt_configuration->reporting_trigger_list);
        mdt_configuration->reporting_trigger_list = NULL;
    }
    if (mdt_configuration->event_list) {
        OpenAPI_list_free(mdt_configuration->event_list);
        mdt_configuration->event_list = NULL;
    }
    if (mdt_configuration->add_positioning_method_list) {
        OpenAPI_list_free(mdt_configuration->add_positioning_method_list);
        mdt_configuration->add_positioning_method_list = NULL;
    }
    if (mdt_configuration->mdt_allowed_plmn_id_list) {
        OpenAPI_list_for_each(mdt_configuration->mdt_allowed_plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(mdt_configuration->mdt_allowed_plmn_id_list);
        mdt_configuration->mdt_allowed_plmn_id_list = NULL;
    }
    if (mdt_configuration->mbsfn_area_list) {
        OpenAPI_list_for_each(mdt_configuration->mbsfn_area_list, node) {
            OpenAPI_mbsfn_area_free(node->data);
        }
        OpenAPI_list_free(mdt_configuration->mbsfn_area_list);
        mdt_configuration->mbsfn_area_list = NULL;
    }
    if (mdt_configuration->inter_freq_target_list) {
        OpenAPI_list_for_each(mdt_configuration->inter_freq_target_list, node) {
            OpenAPI_inter_freq_target_info_free(node->data);
        }
        OpenAPI_list_free(mdt_configuration->inter_freq_target_list);
        mdt_configuration->inter_freq_target_list = NULL;
    }
    ogs_free(mdt_configuration);
}

cJSON *OpenAPI_mdt_configuration_convertToJSON(OpenAPI_mdt_configuration_t *mdt_configuration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (mdt_configuration == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [MdtConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (mdt_configuration->job_type == OpenAPI_job_type_NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [job_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "jobType", OpenAPI_job_type_ToString(mdt_configuration->job_type)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [job_type]");
        goto end;
    }

    if (mdt_configuration->report_type != OpenAPI_report_type_mdt_NULL) {
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

    if (mdt_configuration->measurement_lte_list != OpenAPI_measurement_lte_for_mdt_NULL) {
    cJSON *measurement_lte_listList = cJSON_AddArrayToObject(item, "measurementLteList");
    if (measurement_lte_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_lte_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->measurement_lte_list, node) {
        if (cJSON_AddStringToObject(measurement_lte_listList, "", OpenAPI_measurement_lte_for_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_lte_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->measurement_nr_list != OpenAPI_measurement_nr_for_mdt_NULL) {
    cJSON *measurement_nr_listList = cJSON_AddArrayToObject(item, "measurementNrList");
    if (measurement_nr_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_nr_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->measurement_nr_list, node) {
        if (cJSON_AddStringToObject(measurement_nr_listList, "", OpenAPI_measurement_nr_for_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [measurement_nr_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->sensor_measurement_list != OpenAPI_sensor_measurement_NULL) {
    cJSON *sensor_measurement_listList = cJSON_AddArrayToObject(item, "sensorMeasurementList");
    if (sensor_measurement_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [sensor_measurement_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->sensor_measurement_list, node) {
        if (cJSON_AddStringToObject(sensor_measurement_listList, "", OpenAPI_sensor_measurement_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [sensor_measurement_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->reporting_trigger_list != OpenAPI_reporting_trigger_NULL) {
    cJSON *reporting_trigger_listList = cJSON_AddArrayToObject(item, "reportingTriggerList");
    if (reporting_trigger_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [reporting_trigger_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->reporting_trigger_list, node) {
        if (cJSON_AddStringToObject(reporting_trigger_listList, "", OpenAPI_reporting_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [reporting_trigger_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->report_interval != OpenAPI_report_interval_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "reportInterval", OpenAPI_report_interval_mdt_ToString(mdt_configuration->report_interval)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_interval]");
        goto end;
    }
    }

    if (mdt_configuration->report_interval_nr != OpenAPI_report_interval_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "reportIntervalNr", OpenAPI_report_interval_nr_mdt_ToString(mdt_configuration->report_interval_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [report_interval_nr]");
        goto end;
    }
    }

    if (mdt_configuration->report_amount != OpenAPI_report_amount_mdt_NULL) {
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

    if (mdt_configuration->event_list != OpenAPI_event_for_mdt_NULL) {
    cJSON *event_listList = cJSON_AddArrayToObject(item, "eventList");
    if (event_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->event_list, node) {
        if (cJSON_AddStringToObject(event_listList, "", OpenAPI_event_for_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [event_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->logging_interval != OpenAPI_logging_interval_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "loggingInterval", OpenAPI_logging_interval_mdt_ToString(mdt_configuration->logging_interval)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_interval]");
        goto end;
    }
    }

    if (mdt_configuration->logging_interval_nr != OpenAPI_logging_interval_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "loggingIntervalNr", OpenAPI_logging_interval_nr_mdt_ToString(mdt_configuration->logging_interval_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_interval_nr]");
        goto end;
    }
    }

    if (mdt_configuration->logging_duration != OpenAPI_logging_duration_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "loggingDuration", OpenAPI_logging_duration_mdt_ToString(mdt_configuration->logging_duration)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_duration]");
        goto end;
    }
    }

    if (mdt_configuration->logging_duration_nr != OpenAPI_logging_duration_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "loggingDurationNr", OpenAPI_logging_duration_nr_mdt_ToString(mdt_configuration->logging_duration_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [logging_duration_nr]");
        goto end;
    }
    }

    if (mdt_configuration->positioning_method != OpenAPI_positioning_method_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "positioningMethod", OpenAPI_positioning_method_mdt_ToString(mdt_configuration->positioning_method)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [positioning_method]");
        goto end;
    }
    }

    if (mdt_configuration->add_positioning_method_list != OpenAPI_positioning_method_mdt_NULL) {
    cJSON *add_positioning_method_listList = cJSON_AddArrayToObject(item, "addPositioningMethodList");
    if (add_positioning_method_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [add_positioning_method_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->add_positioning_method_list, node) {
        if (cJSON_AddStringToObject(add_positioning_method_listList, "", OpenAPI_positioning_method_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [add_positioning_method_list]");
            goto end;
        }
    }
    }

    if (mdt_configuration->collection_period_rmm_lte != OpenAPI_collection_period_rmm_lte_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmLte", OpenAPI_collection_period_rmm_lte_mdt_ToString(mdt_configuration->collection_period_rmm_lte)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    }

    if (mdt_configuration->collection_period_rmm_nr != OpenAPI_collection_period_rmm_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmNr", OpenAPI_collection_period_rmm_nr_mdt_ToString(mdt_configuration->collection_period_rmm_nr)) == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    }

    if (mdt_configuration->measurement_period_lte != OpenAPI_measurement_period_lte_mdt_NULL) {
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
    OpenAPI_list_for_each(mdt_configuration->mdt_allowed_plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mdt_allowed_plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(mdt_allowed_plmn_id_listList, itemLocal);
    }
    }

    if (mdt_configuration->mbsfn_area_list) {
    cJSON *mbsfn_area_listList = cJSON_AddArrayToObject(item, "mbsfnAreaList");
    if (mbsfn_area_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mbsfn_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->mbsfn_area_list, node) {
        cJSON *itemLocal = OpenAPI_mbsfn_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [mbsfn_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(mbsfn_area_listList, itemLocal);
    }
    }

    if (mdt_configuration->inter_freq_target_list) {
    cJSON *inter_freq_target_listList = cJSON_AddArrayToObject(item, "interFreqTargetList");
    if (inter_freq_target_listList == NULL) {
        ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [inter_freq_target_list]");
        goto end;
    }
    OpenAPI_list_for_each(mdt_configuration->inter_freq_target_list, node) {
        cJSON *itemLocal = OpenAPI_inter_freq_target_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_mdt_configuration_convertToJSON() failed [inter_freq_target_list]");
            goto end;
        }
        cJSON_AddItemToArray(inter_freq_target_listList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_mdt_configuration_t *OpenAPI_mdt_configuration_parseFromJSON(cJSON *mdt_configurationJSON)
{
    OpenAPI_mdt_configuration_t *mdt_configuration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *job_type = NULL;
    OpenAPI_job_type_e job_typeVariable = 0;
    cJSON *report_type = NULL;
    OpenAPI_report_type_mdt_e report_typeVariable = 0;
    cJSON *area_scope = NULL;
    OpenAPI_area_scope_t *area_scope_local_nonprim = NULL;
    cJSON *measurement_lte_list = NULL;
    OpenAPI_list_t *measurement_lte_listList = NULL;
    cJSON *measurement_nr_list = NULL;
    OpenAPI_list_t *measurement_nr_listList = NULL;
    cJSON *sensor_measurement_list = NULL;
    OpenAPI_list_t *sensor_measurement_listList = NULL;
    cJSON *reporting_trigger_list = NULL;
    OpenAPI_list_t *reporting_trigger_listList = NULL;
    cJSON *report_interval = NULL;
    OpenAPI_report_interval_mdt_e report_intervalVariable = 0;
    cJSON *report_interval_nr = NULL;
    OpenAPI_report_interval_nr_mdt_e report_interval_nrVariable = 0;
    cJSON *report_amount = NULL;
    OpenAPI_report_amount_mdt_e report_amountVariable = 0;
    cJSON *event_threshold_rsrp = NULL;
    cJSON *event_threshold_rsrp_nr = NULL;
    cJSON *event_threshold_rsrq = NULL;
    cJSON *event_threshold_rsrq_nr = NULL;
    cJSON *event_list = NULL;
    OpenAPI_list_t *event_listList = NULL;
    cJSON *logging_interval = NULL;
    OpenAPI_logging_interval_mdt_e logging_intervalVariable = 0;
    cJSON *logging_interval_nr = NULL;
    OpenAPI_logging_interval_nr_mdt_e logging_interval_nrVariable = 0;
    cJSON *logging_duration = NULL;
    OpenAPI_logging_duration_mdt_e logging_durationVariable = 0;
    cJSON *logging_duration_nr = NULL;
    OpenAPI_logging_duration_nr_mdt_e logging_duration_nrVariable = 0;
    cJSON *positioning_method = NULL;
    OpenAPI_positioning_method_mdt_e positioning_methodVariable = 0;
    cJSON *add_positioning_method_list = NULL;
    OpenAPI_list_t *add_positioning_method_listList = NULL;
    cJSON *collection_period_rmm_lte = NULL;
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lteVariable = 0;
    cJSON *collection_period_rmm_nr = NULL;
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nrVariable = 0;
    cJSON *measurement_period_lte = NULL;
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lteVariable = 0;
    cJSON *mdt_allowed_plmn_id_list = NULL;
    OpenAPI_list_t *mdt_allowed_plmn_id_listList = NULL;
    cJSON *mbsfn_area_list = NULL;
    OpenAPI_list_t *mbsfn_area_listList = NULL;
    cJSON *inter_freq_target_list = NULL;
    OpenAPI_list_t *inter_freq_target_listList = NULL;
    job_type = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "jobType");
    if (!job_type) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }
    if (!cJSON_IsString(job_type)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [job_type]");
        goto end;
    }
    job_typeVariable = OpenAPI_job_type_FromString(job_type->valuestring);

    report_type = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportType");
    if (report_type) {
    if (!cJSON_IsString(report_type)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_type]");
        goto end;
    }
    report_typeVariable = OpenAPI_report_type_mdt_FromString(report_type->valuestring);
    }

    area_scope = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "areaScope");
    if (area_scope) {
    area_scope_local_nonprim = OpenAPI_area_scope_parseFromJSON(area_scope);
    if (!area_scope_local_nonprim) {
        ogs_error("OpenAPI_area_scope_parseFromJSON failed [area_scope]");
        goto end;
    }
    }

    measurement_lte_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementLteList");
    if (measurement_lte_list) {
        cJSON *measurement_lte_list_local = NULL;
        if (!cJSON_IsArray(measurement_lte_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_lte_list]");
            goto end;
        }

        measurement_lte_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_lte_list_local, measurement_lte_list) {
            OpenAPI_measurement_lte_for_mdt_e localEnum = OpenAPI_measurement_lte_for_mdt_NULL;
            if (!cJSON_IsString(measurement_lte_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_lte_list]");
                goto end;
            }
            localEnum = OpenAPI_measurement_lte_for_mdt_FromString(measurement_lte_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"measurement_lte_list\" is not supported. Ignoring it ...",
                         measurement_lte_list_local->valuestring);
            } else {
                OpenAPI_list_add(measurement_lte_listList, (void *)localEnum);
            }
        }
        if (measurement_lte_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected measurement_lte_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    measurement_nr_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementNrList");
    if (measurement_nr_list) {
        cJSON *measurement_nr_list_local = NULL;
        if (!cJSON_IsArray(measurement_nr_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_nr_list]");
            goto end;
        }

        measurement_nr_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_nr_list_local, measurement_nr_list) {
            OpenAPI_measurement_nr_for_mdt_e localEnum = OpenAPI_measurement_nr_for_mdt_NULL;
            if (!cJSON_IsString(measurement_nr_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_nr_list]");
                goto end;
            }
            localEnum = OpenAPI_measurement_nr_for_mdt_FromString(measurement_nr_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"measurement_nr_list\" is not supported. Ignoring it ...",
                         measurement_nr_list_local->valuestring);
            } else {
                OpenAPI_list_add(measurement_nr_listList, (void *)localEnum);
            }
        }
        if (measurement_nr_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected measurement_nr_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    sensor_measurement_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "sensorMeasurementList");
    if (sensor_measurement_list) {
        cJSON *sensor_measurement_list_local = NULL;
        if (!cJSON_IsArray(sensor_measurement_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [sensor_measurement_list]");
            goto end;
        }

        sensor_measurement_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sensor_measurement_list_local, sensor_measurement_list) {
            OpenAPI_sensor_measurement_e localEnum = OpenAPI_sensor_measurement_NULL;
            if (!cJSON_IsString(sensor_measurement_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [sensor_measurement_list]");
                goto end;
            }
            localEnum = OpenAPI_sensor_measurement_FromString(sensor_measurement_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"sensor_measurement_list\" is not supported. Ignoring it ...",
                         sensor_measurement_list_local->valuestring);
            } else {
                OpenAPI_list_add(sensor_measurement_listList, (void *)localEnum);
            }
        }
        if (sensor_measurement_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected sensor_measurement_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    reporting_trigger_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportingTriggerList");
    if (reporting_trigger_list) {
        cJSON *reporting_trigger_list_local = NULL;
        if (!cJSON_IsArray(reporting_trigger_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [reporting_trigger_list]");
            goto end;
        }

        reporting_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(reporting_trigger_list_local, reporting_trigger_list) {
            OpenAPI_reporting_trigger_e localEnum = OpenAPI_reporting_trigger_NULL;
            if (!cJSON_IsString(reporting_trigger_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [reporting_trigger_list]");
                goto end;
            }
            localEnum = OpenAPI_reporting_trigger_FromString(reporting_trigger_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"reporting_trigger_list\" is not supported. Ignoring it ...",
                         reporting_trigger_list_local->valuestring);
            } else {
                OpenAPI_list_add(reporting_trigger_listList, (void *)localEnum);
            }
        }
        if (reporting_trigger_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected reporting_trigger_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    report_interval = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportInterval");
    if (report_interval) {
    if (!cJSON_IsString(report_interval)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_interval]");
        goto end;
    }
    report_intervalVariable = OpenAPI_report_interval_mdt_FromString(report_interval->valuestring);
    }

    report_interval_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportIntervalNr");
    if (report_interval_nr) {
    if (!cJSON_IsString(report_interval_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_interval_nr]");
        goto end;
    }
    report_interval_nrVariable = OpenAPI_report_interval_nr_mdt_FromString(report_interval_nr->valuestring);
    }

    report_amount = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "reportAmount");
    if (report_amount) {
    if (!cJSON_IsString(report_amount)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [report_amount]");
        goto end;
    }
    report_amountVariable = OpenAPI_report_amount_mdt_FromString(report_amount->valuestring);
    }

    event_threshold_rsrp = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrp");
    if (event_threshold_rsrp) {
    if (!cJSON_IsNumber(event_threshold_rsrp)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    event_threshold_rsrp_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrpNr");
    if (event_threshold_rsrp_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrp_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    event_threshold_rsrq = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrq");
    if (event_threshold_rsrq) {
    if (!cJSON_IsNumber(event_threshold_rsrq)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    event_threshold_rsrq_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventThresholdRsrqNr");
    if (event_threshold_rsrq_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrq_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    event_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "eventList");
    if (event_list) {
        cJSON *event_list_local = NULL;
        if (!cJSON_IsArray(event_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_list]");
            goto end;
        }

        event_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_list_local, event_list) {
            OpenAPI_event_for_mdt_e localEnum = OpenAPI_event_for_mdt_NULL;
            if (!cJSON_IsString(event_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [event_list]");
                goto end;
            }
            localEnum = OpenAPI_event_for_mdt_FromString(event_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"event_list\" is not supported. Ignoring it ...",
                         event_list_local->valuestring);
            } else {
                OpenAPI_list_add(event_listList, (void *)localEnum);
            }
        }
        if (event_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected event_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    logging_interval = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingInterval");
    if (logging_interval) {
    if (!cJSON_IsString(logging_interval)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_interval]");
        goto end;
    }
    logging_intervalVariable = OpenAPI_logging_interval_mdt_FromString(logging_interval->valuestring);
    }

    logging_interval_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingIntervalNr");
    if (logging_interval_nr) {
    if (!cJSON_IsString(logging_interval_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_interval_nr]");
        goto end;
    }
    logging_interval_nrVariable = OpenAPI_logging_interval_nr_mdt_FromString(logging_interval_nr->valuestring);
    }

    logging_duration = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingDuration");
    if (logging_duration) {
    if (!cJSON_IsString(logging_duration)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_duration]");
        goto end;
    }
    logging_durationVariable = OpenAPI_logging_duration_mdt_FromString(logging_duration->valuestring);
    }

    logging_duration_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "loggingDurationNr");
    if (logging_duration_nr) {
    if (!cJSON_IsString(logging_duration_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [logging_duration_nr]");
        goto end;
    }
    logging_duration_nrVariable = OpenAPI_logging_duration_nr_mdt_FromString(logging_duration_nr->valuestring);
    }

    positioning_method = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "positioningMethod");
    if (positioning_method) {
    if (!cJSON_IsString(positioning_method)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [positioning_method]");
        goto end;
    }
    positioning_methodVariable = OpenAPI_positioning_method_mdt_FromString(positioning_method->valuestring);
    }

    add_positioning_method_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "addPositioningMethodList");
    if (add_positioning_method_list) {
        cJSON *add_positioning_method_list_local = NULL;
        if (!cJSON_IsArray(add_positioning_method_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [add_positioning_method_list]");
            goto end;
        }

        add_positioning_method_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_positioning_method_list_local, add_positioning_method_list) {
            OpenAPI_positioning_method_mdt_e localEnum = OpenAPI_positioning_method_mdt_NULL;
            if (!cJSON_IsString(add_positioning_method_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [add_positioning_method_list]");
                goto end;
            }
            localEnum = OpenAPI_positioning_method_mdt_FromString(add_positioning_method_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"add_positioning_method_list\" is not supported. Ignoring it ...",
                         add_positioning_method_list_local->valuestring);
            } else {
                OpenAPI_list_add(add_positioning_method_listList, (void *)localEnum);
            }
        }
        if (add_positioning_method_listList->count == 0) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed: Expected add_positioning_method_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    collection_period_rmm_lte = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "collectionPeriodRmmLte");
    if (collection_period_rmm_lte) {
    if (!cJSON_IsString(collection_period_rmm_lte)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    collection_period_rmm_lteVariable = OpenAPI_collection_period_rmm_lte_mdt_FromString(collection_period_rmm_lte->valuestring);
    }

    collection_period_rmm_nr = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "collectionPeriodRmmNr");
    if (collection_period_rmm_nr) {
    if (!cJSON_IsString(collection_period_rmm_nr)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    collection_period_rmm_nrVariable = OpenAPI_collection_period_rmm_nr_mdt_FromString(collection_period_rmm_nr->valuestring);
    }

    measurement_period_lte = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "measurementPeriodLte");
    if (measurement_period_lte) {
    if (!cJSON_IsString(measurement_period_lte)) {
        ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [measurement_period_lte]");
        goto end;
    }
    measurement_period_lteVariable = OpenAPI_measurement_period_lte_mdt_FromString(measurement_period_lte->valuestring);
    }

    mdt_allowed_plmn_id_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "mdtAllowedPlmnIdList");
    if (mdt_allowed_plmn_id_list) {
        cJSON *mdt_allowed_plmn_id_list_local = NULL;
        if (!cJSON_IsArray(mdt_allowed_plmn_id_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
            goto end;
        }

        mdt_allowed_plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mdt_allowed_plmn_id_list_local, mdt_allowed_plmn_id_list) {
            if (!cJSON_IsObject(mdt_allowed_plmn_id_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
                goto end;
            }
            OpenAPI_plmn_id_t *mdt_allowed_plmn_id_listItem = OpenAPI_plmn_id_parseFromJSON(mdt_allowed_plmn_id_list_local);
            if (!mdt_allowed_plmn_id_listItem) {
                ogs_error("No mdt_allowed_plmn_id_listItem");
                goto end;
            }
            OpenAPI_list_add(mdt_allowed_plmn_id_listList, mdt_allowed_plmn_id_listItem);
        }
    }

    mbsfn_area_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "mbsfnAreaList");
    if (mbsfn_area_list) {
        cJSON *mbsfn_area_list_local = NULL;
        if (!cJSON_IsArray(mbsfn_area_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mbsfn_area_list]");
            goto end;
        }

        mbsfn_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mbsfn_area_list_local, mbsfn_area_list) {
            if (!cJSON_IsObject(mbsfn_area_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [mbsfn_area_list]");
                goto end;
            }
            OpenAPI_mbsfn_area_t *mbsfn_area_listItem = OpenAPI_mbsfn_area_parseFromJSON(mbsfn_area_list_local);
            if (!mbsfn_area_listItem) {
                ogs_error("No mbsfn_area_listItem");
                goto end;
            }
            OpenAPI_list_add(mbsfn_area_listList, mbsfn_area_listItem);
        }
    }

    inter_freq_target_list = cJSON_GetObjectItemCaseSensitive(mdt_configurationJSON, "interFreqTargetList");
    if (inter_freq_target_list) {
        cJSON *inter_freq_target_list_local = NULL;
        if (!cJSON_IsArray(inter_freq_target_list)) {
            ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [inter_freq_target_list]");
            goto end;
        }

        inter_freq_target_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(inter_freq_target_list_local, inter_freq_target_list) {
            if (!cJSON_IsObject(inter_freq_target_list_local)) {
                ogs_error("OpenAPI_mdt_configuration_parseFromJSON() failed [inter_freq_target_list]");
                goto end;
            }
            OpenAPI_inter_freq_target_info_t *inter_freq_target_listItem = OpenAPI_inter_freq_target_info_parseFromJSON(inter_freq_target_list_local);
            if (!inter_freq_target_listItem) {
                ogs_error("No inter_freq_target_listItem");
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
    if (area_scope_local_nonprim) {
        OpenAPI_area_scope_free(area_scope_local_nonprim);
        area_scope_local_nonprim = NULL;
    }
    if (measurement_lte_listList) {
        OpenAPI_list_free(measurement_lte_listList);
        measurement_lte_listList = NULL;
    }
    if (measurement_nr_listList) {
        OpenAPI_list_free(measurement_nr_listList);
        measurement_nr_listList = NULL;
    }
    if (sensor_measurement_listList) {
        OpenAPI_list_free(sensor_measurement_listList);
        sensor_measurement_listList = NULL;
    }
    if (reporting_trigger_listList) {
        OpenAPI_list_free(reporting_trigger_listList);
        reporting_trigger_listList = NULL;
    }
    if (event_listList) {
        OpenAPI_list_free(event_listList);
        event_listList = NULL;
    }
    if (add_positioning_method_listList) {
        OpenAPI_list_free(add_positioning_method_listList);
        add_positioning_method_listList = NULL;
    }
    if (mdt_allowed_plmn_id_listList) {
        OpenAPI_list_for_each(mdt_allowed_plmn_id_listList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(mdt_allowed_plmn_id_listList);
        mdt_allowed_plmn_id_listList = NULL;
    }
    if (mbsfn_area_listList) {
        OpenAPI_list_for_each(mbsfn_area_listList, node) {
            OpenAPI_mbsfn_area_free(node->data);
        }
        OpenAPI_list_free(mbsfn_area_listList);
        mbsfn_area_listList = NULL;
    }
    if (inter_freq_target_listList) {
        OpenAPI_list_for_each(inter_freq_target_listList, node) {
            OpenAPI_inter_freq_target_info_free(node->data);
        }
        OpenAPI_list_free(inter_freq_target_listList);
        inter_freq_target_listList = NULL;
    }
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

