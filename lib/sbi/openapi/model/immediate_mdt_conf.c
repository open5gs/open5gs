
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "immediate_mdt_conf.h"

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
)
{
    OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf_local_var = ogs_malloc(sizeof(OpenAPI_immediate_mdt_conf_t));
    ogs_assert(immediate_mdt_conf_local_var);

    immediate_mdt_conf_local_var->job_type = job_type;
    immediate_mdt_conf_local_var->measurement_lte_list = measurement_lte_list;
    immediate_mdt_conf_local_var->measurement_nr_list = measurement_nr_list;
    immediate_mdt_conf_local_var->reporting_trigger_list = reporting_trigger_list;
    immediate_mdt_conf_local_var->report_interval = report_interval;
    immediate_mdt_conf_local_var->report_interval_nr = report_interval_nr;
    immediate_mdt_conf_local_var->report_amount = report_amount;
    immediate_mdt_conf_local_var->is_event_threshold_rsrp = is_event_threshold_rsrp;
    immediate_mdt_conf_local_var->event_threshold_rsrp = event_threshold_rsrp;
    immediate_mdt_conf_local_var->is_event_threshold_rsrq = is_event_threshold_rsrq;
    immediate_mdt_conf_local_var->event_threshold_rsrq = event_threshold_rsrq;
    immediate_mdt_conf_local_var->is_event_threshold_rsrp_nr = is_event_threshold_rsrp_nr;
    immediate_mdt_conf_local_var->event_threshold_rsrp_nr = event_threshold_rsrp_nr;
    immediate_mdt_conf_local_var->is_event_threshold_rsrq_nr = is_event_threshold_rsrq_nr;
    immediate_mdt_conf_local_var->event_threshold_rsrq_nr = event_threshold_rsrq_nr;
    immediate_mdt_conf_local_var->collection_period_rmm_lte = collection_period_rmm_lte;
    immediate_mdt_conf_local_var->collection_period_rmm_nr = collection_period_rmm_nr;
    immediate_mdt_conf_local_var->measurement_period_lte = measurement_period_lte;
    immediate_mdt_conf_local_var->area_scope = area_scope;
    immediate_mdt_conf_local_var->positioning_method = positioning_method;
    immediate_mdt_conf_local_var->add_positioning_method_list = add_positioning_method_list;
    immediate_mdt_conf_local_var->mdt_allowed_plmn_id_list = mdt_allowed_plmn_id_list;
    immediate_mdt_conf_local_var->sensor_measurement_list = sensor_measurement_list;

    return immediate_mdt_conf_local_var;
}

void OpenAPI_immediate_mdt_conf_free(OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == immediate_mdt_conf) {
        return;
    }
    if (immediate_mdt_conf->measurement_lte_list) {
        OpenAPI_list_free(immediate_mdt_conf->measurement_lte_list);
        immediate_mdt_conf->measurement_lte_list = NULL;
    }
    if (immediate_mdt_conf->measurement_nr_list) {
        OpenAPI_list_free(immediate_mdt_conf->measurement_nr_list);
        immediate_mdt_conf->measurement_nr_list = NULL;
    }
    if (immediate_mdt_conf->reporting_trigger_list) {
        OpenAPI_list_free(immediate_mdt_conf->reporting_trigger_list);
        immediate_mdt_conf->reporting_trigger_list = NULL;
    }
    if (immediate_mdt_conf->area_scope) {
        OpenAPI_area_scope_free(immediate_mdt_conf->area_scope);
        immediate_mdt_conf->area_scope = NULL;
    }
    if (immediate_mdt_conf->add_positioning_method_list) {
        OpenAPI_list_free(immediate_mdt_conf->add_positioning_method_list);
        immediate_mdt_conf->add_positioning_method_list = NULL;
    }
    if (immediate_mdt_conf->mdt_allowed_plmn_id_list) {
        OpenAPI_list_for_each(immediate_mdt_conf->mdt_allowed_plmn_id_list, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(immediate_mdt_conf->mdt_allowed_plmn_id_list);
        immediate_mdt_conf->mdt_allowed_plmn_id_list = NULL;
    }
    if (immediate_mdt_conf->sensor_measurement_list) {
        OpenAPI_list_free(immediate_mdt_conf->sensor_measurement_list);
        immediate_mdt_conf->sensor_measurement_list = NULL;
    }
    ogs_free(immediate_mdt_conf);
}

cJSON *OpenAPI_immediate_mdt_conf_convertToJSON(OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (immediate_mdt_conf == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [ImmediateMdtConf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (immediate_mdt_conf->job_type == OpenAPI_job_type_NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [job_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "jobType", OpenAPI_job_type_ToString(immediate_mdt_conf->job_type)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [job_type]");
        goto end;
    }

    if (immediate_mdt_conf->measurement_lte_list != OpenAPI_measurement_lte_for_mdt_NULL) {
    cJSON *measurement_lte_listList = cJSON_AddArrayToObject(item, "measurementLteList");
    if (measurement_lte_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_lte_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->measurement_lte_list, node) {
        if (cJSON_AddStringToObject(measurement_lte_listList, "", OpenAPI_measurement_lte_for_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_lte_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->measurement_nr_list != OpenAPI_measurement_nr_for_mdt_NULL) {
    cJSON *measurement_nr_listList = cJSON_AddArrayToObject(item, "measurementNrList");
    if (measurement_nr_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_nr_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->measurement_nr_list, node) {
        if (cJSON_AddStringToObject(measurement_nr_listList, "", OpenAPI_measurement_nr_for_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_nr_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->reporting_trigger_list != OpenAPI_reporting_trigger_NULL) {
    cJSON *reporting_trigger_listList = cJSON_AddArrayToObject(item, "reportingTriggerList");
    if (reporting_trigger_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [reporting_trigger_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->reporting_trigger_list, node) {
        if (cJSON_AddStringToObject(reporting_trigger_listList, "", OpenAPI_reporting_trigger_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [reporting_trigger_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->report_interval != OpenAPI_report_interval_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "reportInterval", OpenAPI_report_interval_mdt_ToString(immediate_mdt_conf->report_interval)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [report_interval]");
        goto end;
    }
    }

    if (immediate_mdt_conf->report_interval_nr != OpenAPI_report_interval_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "reportIntervalNr", OpenAPI_report_interval_nr_mdt_ToString(immediate_mdt_conf->report_interval_nr)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [report_interval_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->report_amount != OpenAPI_report_amount_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "reportAmount", OpenAPI_report_amount_mdt_ToString(immediate_mdt_conf->report_amount)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [report_amount]");
        goto end;
    }
    }

    if (immediate_mdt_conf->is_event_threshold_rsrp) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrp", immediate_mdt_conf->event_threshold_rsrp) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    if (immediate_mdt_conf->is_event_threshold_rsrq) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrq", immediate_mdt_conf->event_threshold_rsrq) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    if (immediate_mdt_conf->is_event_threshold_rsrp_nr) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrpNr", immediate_mdt_conf->event_threshold_rsrp_nr) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->is_event_threshold_rsrq_nr) {
    if (cJSON_AddNumberToObject(item, "eventThresholdRsrqNr", immediate_mdt_conf->event_threshold_rsrq_nr) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->collection_period_rmm_lte != OpenAPI_collection_period_rmm_lte_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmLte", OpenAPI_collection_period_rmm_lte_mdt_ToString(immediate_mdt_conf->collection_period_rmm_lte)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    }

    if (immediate_mdt_conf->collection_period_rmm_nr != OpenAPI_collection_period_rmm_nr_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmNr", OpenAPI_collection_period_rmm_nr_mdt_ToString(immediate_mdt_conf->collection_period_rmm_nr)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->measurement_period_lte != OpenAPI_measurement_period_lte_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "measurementPeriodLte", OpenAPI_measurement_period_lte_mdt_ToString(immediate_mdt_conf->measurement_period_lte)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_period_lte]");
        goto end;
    }
    }

    if (immediate_mdt_conf->area_scope) {
    cJSON *area_scope_local_JSON = OpenAPI_area_scope_convertToJSON(immediate_mdt_conf->area_scope);
    if (area_scope_local_JSON == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [area_scope]");
        goto end;
    }
    cJSON_AddItemToObject(item, "areaScope", area_scope_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [area_scope]");
        goto end;
    }
    }

    if (immediate_mdt_conf->positioning_method != OpenAPI_positioning_method_mdt_NULL) {
    if (cJSON_AddStringToObject(item, "positioningMethod", OpenAPI_positioning_method_mdt_ToString(immediate_mdt_conf->positioning_method)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [positioning_method]");
        goto end;
    }
    }

    if (immediate_mdt_conf->add_positioning_method_list != OpenAPI_positioning_method_mdt_NULL) {
    cJSON *add_positioning_method_listList = cJSON_AddArrayToObject(item, "addPositioningMethodList");
    if (add_positioning_method_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [add_positioning_method_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->add_positioning_method_list, node) {
        if (cJSON_AddStringToObject(add_positioning_method_listList, "", OpenAPI_positioning_method_mdt_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [add_positioning_method_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->mdt_allowed_plmn_id_list) {
    cJSON *mdt_allowed_plmn_id_listList = cJSON_AddArrayToObject(item, "mdtAllowedPlmnIdList");
    if (mdt_allowed_plmn_id_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [mdt_allowed_plmn_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->mdt_allowed_plmn_id_list, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [mdt_allowed_plmn_id_list]");
            goto end;
        }
        cJSON_AddItemToArray(mdt_allowed_plmn_id_listList, itemLocal);
    }
    }

    if (immediate_mdt_conf->sensor_measurement_list != OpenAPI_sensor_measurement_NULL) {
    cJSON *sensor_measurement_listList = cJSON_AddArrayToObject(item, "sensorMeasurementList");
    if (sensor_measurement_listList == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [sensor_measurement_list]");
        goto end;
    }
    OpenAPI_list_for_each(immediate_mdt_conf->sensor_measurement_list, node) {
        if (cJSON_AddStringToObject(sensor_measurement_listList, "", OpenAPI_sensor_measurement_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [sensor_measurement_list]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_immediate_mdt_conf_t *OpenAPI_immediate_mdt_conf_parseFromJSON(cJSON *immediate_mdt_confJSON)
{
    OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *job_type = NULL;
    OpenAPI_job_type_e job_typeVariable = 0;
    cJSON *measurement_lte_list = NULL;
    OpenAPI_list_t *measurement_lte_listList = NULL;
    cJSON *measurement_nr_list = NULL;
    OpenAPI_list_t *measurement_nr_listList = NULL;
    cJSON *reporting_trigger_list = NULL;
    OpenAPI_list_t *reporting_trigger_listList = NULL;
    cJSON *report_interval = NULL;
    OpenAPI_report_interval_mdt_e report_intervalVariable = 0;
    cJSON *report_interval_nr = NULL;
    OpenAPI_report_interval_nr_mdt_e report_interval_nrVariable = 0;
    cJSON *report_amount = NULL;
    OpenAPI_report_amount_mdt_e report_amountVariable = 0;
    cJSON *event_threshold_rsrp = NULL;
    cJSON *event_threshold_rsrq = NULL;
    cJSON *event_threshold_rsrp_nr = NULL;
    cJSON *event_threshold_rsrq_nr = NULL;
    cJSON *collection_period_rmm_lte = NULL;
    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lteVariable = 0;
    cJSON *collection_period_rmm_nr = NULL;
    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nrVariable = 0;
    cJSON *measurement_period_lte = NULL;
    OpenAPI_measurement_period_lte_mdt_e measurement_period_lteVariable = 0;
    cJSON *area_scope = NULL;
    OpenAPI_area_scope_t *area_scope_local_nonprim = NULL;
    cJSON *positioning_method = NULL;
    OpenAPI_positioning_method_mdt_e positioning_methodVariable = 0;
    cJSON *add_positioning_method_list = NULL;
    OpenAPI_list_t *add_positioning_method_listList = NULL;
    cJSON *mdt_allowed_plmn_id_list = NULL;
    OpenAPI_list_t *mdt_allowed_plmn_id_listList = NULL;
    cJSON *sensor_measurement_list = NULL;
    OpenAPI_list_t *sensor_measurement_listList = NULL;
    job_type = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "jobType");
    if (!job_type) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [job_type]");
        goto end;
    }
    if (!cJSON_IsString(job_type)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [job_type]");
        goto end;
    }
    job_typeVariable = OpenAPI_job_type_FromString(job_type->valuestring);

    measurement_lte_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementLteList");
    if (measurement_lte_list) {
        cJSON *measurement_lte_list_local = NULL;
        if (!cJSON_IsArray(measurement_lte_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_lte_list]");
            goto end;
        }

        measurement_lte_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_lte_list_local, measurement_lte_list) {
            OpenAPI_measurement_lte_for_mdt_e localEnum = OpenAPI_measurement_lte_for_mdt_NULL;
            if (!cJSON_IsString(measurement_lte_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_lte_list]");
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
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed: Expected measurement_lte_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    measurement_nr_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementNrList");
    if (measurement_nr_list) {
        cJSON *measurement_nr_list_local = NULL;
        if (!cJSON_IsArray(measurement_nr_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_nr_list]");
            goto end;
        }

        measurement_nr_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(measurement_nr_list_local, measurement_nr_list) {
            OpenAPI_measurement_nr_for_mdt_e localEnum = OpenAPI_measurement_nr_for_mdt_NULL;
            if (!cJSON_IsString(measurement_nr_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_nr_list]");
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
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed: Expected measurement_nr_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    reporting_trigger_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportingTriggerList");
    if (reporting_trigger_list) {
        cJSON *reporting_trigger_list_local = NULL;
        if (!cJSON_IsArray(reporting_trigger_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [reporting_trigger_list]");
            goto end;
        }

        reporting_trigger_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(reporting_trigger_list_local, reporting_trigger_list) {
            OpenAPI_reporting_trigger_e localEnum = OpenAPI_reporting_trigger_NULL;
            if (!cJSON_IsString(reporting_trigger_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [reporting_trigger_list]");
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
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed: Expected reporting_trigger_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    report_interval = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportInterval");
    if (report_interval) {
    if (!cJSON_IsString(report_interval)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_interval]");
        goto end;
    }
    report_intervalVariable = OpenAPI_report_interval_mdt_FromString(report_interval->valuestring);
    }

    report_interval_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportIntervalNr");
    if (report_interval_nr) {
    if (!cJSON_IsString(report_interval_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_interval_nr]");
        goto end;
    }
    report_interval_nrVariable = OpenAPI_report_interval_nr_mdt_FromString(report_interval_nr->valuestring);
    }

    report_amount = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportAmount");
    if (report_amount) {
    if (!cJSON_IsString(report_amount)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_amount]");
        goto end;
    }
    report_amountVariable = OpenAPI_report_amount_mdt_FromString(report_amount->valuestring);
    }

    event_threshold_rsrp = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrp");
    if (event_threshold_rsrp) {
    if (!cJSON_IsNumber(event_threshold_rsrp)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    event_threshold_rsrq = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrq");
    if (event_threshold_rsrq) {
    if (!cJSON_IsNumber(event_threshold_rsrq)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    event_threshold_rsrp_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrpNr");
    if (event_threshold_rsrp_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrp_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    event_threshold_rsrq_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrqNr");
    if (event_threshold_rsrq_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrq_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    collection_period_rmm_lte = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "collectionPeriodRmmLte");
    if (collection_period_rmm_lte) {
    if (!cJSON_IsString(collection_period_rmm_lte)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    collection_period_rmm_lteVariable = OpenAPI_collection_period_rmm_lte_mdt_FromString(collection_period_rmm_lte->valuestring);
    }

    collection_period_rmm_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "collectionPeriodRmmNr");
    if (collection_period_rmm_nr) {
    if (!cJSON_IsString(collection_period_rmm_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    collection_period_rmm_nrVariable = OpenAPI_collection_period_rmm_nr_mdt_FromString(collection_period_rmm_nr->valuestring);
    }

    measurement_period_lte = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementPeriodLte");
    if (measurement_period_lte) {
    if (!cJSON_IsString(measurement_period_lte)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_period_lte]");
        goto end;
    }
    measurement_period_lteVariable = OpenAPI_measurement_period_lte_mdt_FromString(measurement_period_lte->valuestring);
    }

    area_scope = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "areaScope");
    if (area_scope) {
    area_scope_local_nonprim = OpenAPI_area_scope_parseFromJSON(area_scope);
    if (!area_scope_local_nonprim) {
        ogs_error("OpenAPI_area_scope_parseFromJSON failed [area_scope]");
        goto end;
    }
    }

    positioning_method = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "positioningMethod");
    if (positioning_method) {
    if (!cJSON_IsString(positioning_method)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [positioning_method]");
        goto end;
    }
    positioning_methodVariable = OpenAPI_positioning_method_mdt_FromString(positioning_method->valuestring);
    }

    add_positioning_method_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "addPositioningMethodList");
    if (add_positioning_method_list) {
        cJSON *add_positioning_method_list_local = NULL;
        if (!cJSON_IsArray(add_positioning_method_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [add_positioning_method_list]");
            goto end;
        }

        add_positioning_method_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(add_positioning_method_list_local, add_positioning_method_list) {
            OpenAPI_positioning_method_mdt_e localEnum = OpenAPI_positioning_method_mdt_NULL;
            if (!cJSON_IsString(add_positioning_method_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [add_positioning_method_list]");
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
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed: Expected add_positioning_method_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    mdt_allowed_plmn_id_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "mdtAllowedPlmnIdList");
    if (mdt_allowed_plmn_id_list) {
        cJSON *mdt_allowed_plmn_id_list_local = NULL;
        if (!cJSON_IsArray(mdt_allowed_plmn_id_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
            goto end;
        }

        mdt_allowed_plmn_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(mdt_allowed_plmn_id_list_local, mdt_allowed_plmn_id_list) {
            if (!cJSON_IsObject(mdt_allowed_plmn_id_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
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

    sensor_measurement_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "sensorMeasurementList");
    if (sensor_measurement_list) {
        cJSON *sensor_measurement_list_local = NULL;
        if (!cJSON_IsArray(sensor_measurement_list)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [sensor_measurement_list]");
            goto end;
        }

        sensor_measurement_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(sensor_measurement_list_local, sensor_measurement_list) {
            OpenAPI_sensor_measurement_e localEnum = OpenAPI_sensor_measurement_NULL;
            if (!cJSON_IsString(sensor_measurement_list_local)) {
                ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [sensor_measurement_list]");
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
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed: Expected sensor_measurement_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    immediate_mdt_conf_local_var = OpenAPI_immediate_mdt_conf_create (
        job_typeVariable,
        measurement_lte_list ? measurement_lte_listList : NULL,
        measurement_nr_list ? measurement_nr_listList : NULL,
        reporting_trigger_list ? reporting_trigger_listList : NULL,
        report_interval ? report_intervalVariable : 0,
        report_interval_nr ? report_interval_nrVariable : 0,
        report_amount ? report_amountVariable : 0,
        event_threshold_rsrp ? true : false,
        event_threshold_rsrp ? event_threshold_rsrp->valuedouble : 0,
        event_threshold_rsrq ? true : false,
        event_threshold_rsrq ? event_threshold_rsrq->valuedouble : 0,
        event_threshold_rsrp_nr ? true : false,
        event_threshold_rsrp_nr ? event_threshold_rsrp_nr->valuedouble : 0,
        event_threshold_rsrq_nr ? true : false,
        event_threshold_rsrq_nr ? event_threshold_rsrq_nr->valuedouble : 0,
        collection_period_rmm_lte ? collection_period_rmm_lteVariable : 0,
        collection_period_rmm_nr ? collection_period_rmm_nrVariable : 0,
        measurement_period_lte ? measurement_period_lteVariable : 0,
        area_scope ? area_scope_local_nonprim : NULL,
        positioning_method ? positioning_methodVariable : 0,
        add_positioning_method_list ? add_positioning_method_listList : NULL,
        mdt_allowed_plmn_id_list ? mdt_allowed_plmn_id_listList : NULL,
        sensor_measurement_list ? sensor_measurement_listList : NULL
    );

    return immediate_mdt_conf_local_var;
end:
    if (measurement_lte_listList) {
        OpenAPI_list_free(measurement_lte_listList);
        measurement_lte_listList = NULL;
    }
    if (measurement_nr_listList) {
        OpenAPI_list_free(measurement_nr_listList);
        measurement_nr_listList = NULL;
    }
    if (reporting_trigger_listList) {
        OpenAPI_list_free(reporting_trigger_listList);
        reporting_trigger_listList = NULL;
    }
    if (area_scope_local_nonprim) {
        OpenAPI_area_scope_free(area_scope_local_nonprim);
        area_scope_local_nonprim = NULL;
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
    if (sensor_measurement_listList) {
        OpenAPI_list_free(sensor_measurement_listList);
        sensor_measurement_listList = NULL;
    }
    return NULL;
}

OpenAPI_immediate_mdt_conf_t *OpenAPI_immediate_mdt_conf_copy(OpenAPI_immediate_mdt_conf_t *dst, OpenAPI_immediate_mdt_conf_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_immediate_mdt_conf_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed");
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

    OpenAPI_immediate_mdt_conf_free(dst);
    dst = OpenAPI_immediate_mdt_conf_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

