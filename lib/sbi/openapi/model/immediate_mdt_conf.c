
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
    if (NULL == immediate_mdt_conf) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_free(immediate_mdt_conf->measurement_lte_list);
    OpenAPI_list_free(immediate_mdt_conf->measurement_nr_list);
    OpenAPI_list_free(immediate_mdt_conf->reporting_trigger_list);
    OpenAPI_area_scope_free(immediate_mdt_conf->area_scope);
    OpenAPI_list_free(immediate_mdt_conf->add_positioning_method_list);
    OpenAPI_list_for_each(immediate_mdt_conf->mdt_allowed_plmn_id_list, node) {
        OpenAPI_plmn_id_free(node->data);
    }
    OpenAPI_list_free(immediate_mdt_conf->mdt_allowed_plmn_id_list);
    OpenAPI_list_free(immediate_mdt_conf->sensor_measurement_list);
    ogs_free(immediate_mdt_conf);
}

cJSON *OpenAPI_immediate_mdt_conf_convertToJSON(OpenAPI_immediate_mdt_conf_t *immediate_mdt_conf)
{
    cJSON *item = NULL;

    if (immediate_mdt_conf == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [ImmediateMdtConf]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "jobType", OpenAPI_job_type_ToString(immediate_mdt_conf->job_type)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [job_type]");
        goto end;
    }

    if (immediate_mdt_conf->measurement_lte_list) {
    cJSON *measurement_lte_list = cJSON_AddArrayToObject(item, "measurementLteList");
    if (measurement_lte_list == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_lte_list]");
        goto end;
    }
    OpenAPI_lnode_t *measurement_lte_list_node;
    OpenAPI_list_for_each(immediate_mdt_conf->measurement_lte_list, measurement_lte_list_node) {
        if (cJSON_AddStringToObject(measurement_lte_list, "", OpenAPI_measurement_lte_for_mdt_ToString((intptr_t)measurement_lte_list_node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_lte_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->measurement_nr_list) {
    cJSON *measurement_nr_list = cJSON_AddArrayToObject(item, "measurementNrList");
    if (measurement_nr_list == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_nr_list]");
        goto end;
    }
    OpenAPI_lnode_t *measurement_nr_list_node;
    OpenAPI_list_for_each(immediate_mdt_conf->measurement_nr_list, measurement_nr_list_node) {
        if (cJSON_AddStringToObject(measurement_nr_list, "", OpenAPI_measurement_nr_for_mdt_ToString((intptr_t)measurement_nr_list_node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [measurement_nr_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->reporting_trigger_list) {
    cJSON *reporting_trigger_list = cJSON_AddArrayToObject(item, "reportingTriggerList");
    if (reporting_trigger_list == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [reporting_trigger_list]");
        goto end;
    }
    OpenAPI_lnode_t *reporting_trigger_list_node;
    OpenAPI_list_for_each(immediate_mdt_conf->reporting_trigger_list, reporting_trigger_list_node) {
        if (cJSON_AddStringToObject(reporting_trigger_list, "", OpenAPI_reporting_trigger_ToString((intptr_t)reporting_trigger_list_node->data)) == NULL) {
            ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [reporting_trigger_list]");
            goto end;
        }
    }
    }

    if (immediate_mdt_conf->report_interval) {
    if (cJSON_AddStringToObject(item, "reportInterval", OpenAPI_report_interval_mdt_ToString(immediate_mdt_conf->report_interval)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [report_interval]");
        goto end;
    }
    }

    if (immediate_mdt_conf->report_interval_nr) {
    if (cJSON_AddStringToObject(item, "reportIntervalNr", OpenAPI_report_interval_nr_mdt_ToString(immediate_mdt_conf->report_interval_nr)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [report_interval_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->report_amount) {
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

    if (immediate_mdt_conf->collection_period_rmm_lte) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmLte", OpenAPI_collection_period_rmm_lte_mdt_ToString(immediate_mdt_conf->collection_period_rmm_lte)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    }

    if (immediate_mdt_conf->collection_period_rmm_nr) {
    if (cJSON_AddStringToObject(item, "collectionPeriodRmmNr", OpenAPI_collection_period_rmm_nr_mdt_ToString(immediate_mdt_conf->collection_period_rmm_nr)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    }

    if (immediate_mdt_conf->measurement_period_lte) {
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

    if (immediate_mdt_conf->positioning_method) {
    if (cJSON_AddStringToObject(item, "positioningMethod", OpenAPI_positioning_method_mdt_ToString(immediate_mdt_conf->positioning_method)) == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [positioning_method]");
        goto end;
    }
    }

    if (immediate_mdt_conf->add_positioning_method_list) {
    cJSON *add_positioning_method_list = cJSON_AddArrayToObject(item, "addPositioningMethodList");
    if (add_positioning_method_list == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [add_positioning_method_list]");
        goto end;
    }
    OpenAPI_lnode_t *add_positioning_method_list_node;
    OpenAPI_list_for_each(immediate_mdt_conf->add_positioning_method_list, add_positioning_method_list_node) {
        if (cJSON_AddStringToObject(add_positioning_method_list, "", OpenAPI_positioning_method_mdt_ToString((intptr_t)add_positioning_method_list_node->data)) == NULL) {
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

    OpenAPI_lnode_t *mdt_allowed_plmn_id_list_node;
    if (immediate_mdt_conf->mdt_allowed_plmn_id_list) {
        OpenAPI_list_for_each(immediate_mdt_conf->mdt_allowed_plmn_id_list, mdt_allowed_plmn_id_list_node) {
            cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(mdt_allowed_plmn_id_list_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [mdt_allowed_plmn_id_list]");
                goto end;
            }
            cJSON_AddItemToArray(mdt_allowed_plmn_id_listList, itemLocal);
        }
    }
    }

    if (immediate_mdt_conf->sensor_measurement_list) {
    cJSON *sensor_measurement_list = cJSON_AddArrayToObject(item, "sensorMeasurementList");
    if (sensor_measurement_list == NULL) {
        ogs_error("OpenAPI_immediate_mdt_conf_convertToJSON() failed [sensor_measurement_list]");
        goto end;
    }
    OpenAPI_lnode_t *sensor_measurement_list_node;
    OpenAPI_list_for_each(immediate_mdt_conf->sensor_measurement_list, sensor_measurement_list_node) {
        if (cJSON_AddStringToObject(sensor_measurement_list, "", OpenAPI_sensor_measurement_ToString((intptr_t)sensor_measurement_list_node->data)) == NULL) {
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
    cJSON *job_type = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "jobType");
    if (!job_type) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [job_type]");
        goto end;
    }

    OpenAPI_job_type_e job_typeVariable;
    if (!cJSON_IsString(job_type)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [job_type]");
        goto end;
    }
    job_typeVariable = OpenAPI_job_type_FromString(job_type->valuestring);

    cJSON *measurement_lte_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementLteList");

    OpenAPI_list_t *measurement_lte_listList;
    if (measurement_lte_list) {
    cJSON *measurement_lte_list_local_nonprimitive;
    if (!cJSON_IsArray(measurement_lte_list)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_lte_list]");
        goto end;
    }

    measurement_lte_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(measurement_lte_list_local_nonprimitive, measurement_lte_list ) {
        if (!cJSON_IsString(measurement_lte_list_local_nonprimitive)){
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_lte_list]");
            goto end;
        }

        OpenAPI_list_add(measurement_lte_listList, (void *)OpenAPI_measurement_lte_for_mdt_FromString(measurement_lte_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *measurement_nr_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementNrList");

    OpenAPI_list_t *measurement_nr_listList;
    if (measurement_nr_list) {
    cJSON *measurement_nr_list_local_nonprimitive;
    if (!cJSON_IsArray(measurement_nr_list)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_nr_list]");
        goto end;
    }

    measurement_nr_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(measurement_nr_list_local_nonprimitive, measurement_nr_list ) {
        if (!cJSON_IsString(measurement_nr_list_local_nonprimitive)){
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_nr_list]");
            goto end;
        }

        OpenAPI_list_add(measurement_nr_listList, (void *)OpenAPI_measurement_nr_for_mdt_FromString(measurement_nr_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *reporting_trigger_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportingTriggerList");

    OpenAPI_list_t *reporting_trigger_listList;
    if (reporting_trigger_list) {
    cJSON *reporting_trigger_list_local_nonprimitive;
    if (!cJSON_IsArray(reporting_trigger_list)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [reporting_trigger_list]");
        goto end;
    }

    reporting_trigger_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(reporting_trigger_list_local_nonprimitive, reporting_trigger_list ) {
        if (!cJSON_IsString(reporting_trigger_list_local_nonprimitive)){
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [reporting_trigger_list]");
            goto end;
        }

        OpenAPI_list_add(reporting_trigger_listList, (void *)OpenAPI_reporting_trigger_FromString(reporting_trigger_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *report_interval = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportInterval");

    OpenAPI_report_interval_mdt_e report_intervalVariable;
    if (report_interval) {
    if (!cJSON_IsString(report_interval)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_interval]");
        goto end;
    }
    report_intervalVariable = OpenAPI_report_interval_mdt_FromString(report_interval->valuestring);
    }

    cJSON *report_interval_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportIntervalNr");

    OpenAPI_report_interval_nr_mdt_e report_interval_nrVariable;
    if (report_interval_nr) {
    if (!cJSON_IsString(report_interval_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_interval_nr]");
        goto end;
    }
    report_interval_nrVariable = OpenAPI_report_interval_nr_mdt_FromString(report_interval_nr->valuestring);
    }

    cJSON *report_amount = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "reportAmount");

    OpenAPI_report_amount_mdt_e report_amountVariable;
    if (report_amount) {
    if (!cJSON_IsString(report_amount)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [report_amount]");
        goto end;
    }
    report_amountVariable = OpenAPI_report_amount_mdt_FromString(report_amount->valuestring);
    }

    cJSON *event_threshold_rsrp = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrp");

    if (event_threshold_rsrp) {
    if (!cJSON_IsNumber(event_threshold_rsrp)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrp]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrq = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrq");

    if (event_threshold_rsrq) {
    if (!cJSON_IsNumber(event_threshold_rsrq)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrq]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrp_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrpNr");

    if (event_threshold_rsrp_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrp_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrp_nr]");
        goto end;
    }
    }

    cJSON *event_threshold_rsrq_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "eventThresholdRsrqNr");

    if (event_threshold_rsrq_nr) {
    if (!cJSON_IsNumber(event_threshold_rsrq_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [event_threshold_rsrq_nr]");
        goto end;
    }
    }

    cJSON *collection_period_rmm_lte = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "collectionPeriodRmmLte");

    OpenAPI_collection_period_rmm_lte_mdt_e collection_period_rmm_lteVariable;
    if (collection_period_rmm_lte) {
    if (!cJSON_IsString(collection_period_rmm_lte)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [collection_period_rmm_lte]");
        goto end;
    }
    collection_period_rmm_lteVariable = OpenAPI_collection_period_rmm_lte_mdt_FromString(collection_period_rmm_lte->valuestring);
    }

    cJSON *collection_period_rmm_nr = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "collectionPeriodRmmNr");

    OpenAPI_collection_period_rmm_nr_mdt_e collection_period_rmm_nrVariable;
    if (collection_period_rmm_nr) {
    if (!cJSON_IsString(collection_period_rmm_nr)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [collection_period_rmm_nr]");
        goto end;
    }
    collection_period_rmm_nrVariable = OpenAPI_collection_period_rmm_nr_mdt_FromString(collection_period_rmm_nr->valuestring);
    }

    cJSON *measurement_period_lte = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "measurementPeriodLte");

    OpenAPI_measurement_period_lte_mdt_e measurement_period_lteVariable;
    if (measurement_period_lte) {
    if (!cJSON_IsString(measurement_period_lte)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [measurement_period_lte]");
        goto end;
    }
    measurement_period_lteVariable = OpenAPI_measurement_period_lte_mdt_FromString(measurement_period_lte->valuestring);
    }

    cJSON *area_scope = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "areaScope");

    OpenAPI_area_scope_t *area_scope_local_nonprim = NULL;
    if (area_scope) {
    area_scope_local_nonprim = OpenAPI_area_scope_parseFromJSON(area_scope);
    }

    cJSON *positioning_method = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "positioningMethod");

    OpenAPI_positioning_method_mdt_e positioning_methodVariable;
    if (positioning_method) {
    if (!cJSON_IsString(positioning_method)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [positioning_method]");
        goto end;
    }
    positioning_methodVariable = OpenAPI_positioning_method_mdt_FromString(positioning_method->valuestring);
    }

    cJSON *add_positioning_method_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "addPositioningMethodList");

    OpenAPI_list_t *add_positioning_method_listList;
    if (add_positioning_method_list) {
    cJSON *add_positioning_method_list_local_nonprimitive;
    if (!cJSON_IsArray(add_positioning_method_list)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [add_positioning_method_list]");
        goto end;
    }

    add_positioning_method_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(add_positioning_method_list_local_nonprimitive, add_positioning_method_list ) {
        if (!cJSON_IsString(add_positioning_method_list_local_nonprimitive)){
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [add_positioning_method_list]");
            goto end;
        }

        OpenAPI_list_add(add_positioning_method_listList, (void *)OpenAPI_positioning_method_mdt_FromString(add_positioning_method_list_local_nonprimitive->valuestring));
    }
    }

    cJSON *mdt_allowed_plmn_id_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "mdtAllowedPlmnIdList");

    OpenAPI_list_t *mdt_allowed_plmn_id_listList;
    if (mdt_allowed_plmn_id_list) {
    cJSON *mdt_allowed_plmn_id_list_local_nonprimitive;
    if (!cJSON_IsArray(mdt_allowed_plmn_id_list)){
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
        goto end;
    }

    mdt_allowed_plmn_id_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(mdt_allowed_plmn_id_list_local_nonprimitive, mdt_allowed_plmn_id_list ) {
        if (!cJSON_IsObject(mdt_allowed_plmn_id_list_local_nonprimitive)) {
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [mdt_allowed_plmn_id_list]");
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

    cJSON *sensor_measurement_list = cJSON_GetObjectItemCaseSensitive(immediate_mdt_confJSON, "sensorMeasurementList");

    OpenAPI_list_t *sensor_measurement_listList;
    if (sensor_measurement_list) {
    cJSON *sensor_measurement_list_local_nonprimitive;
    if (!cJSON_IsArray(sensor_measurement_list)) {
        ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [sensor_measurement_list]");
        goto end;
    }

    sensor_measurement_listList = OpenAPI_list_create();

    cJSON_ArrayForEach(sensor_measurement_list_local_nonprimitive, sensor_measurement_list ) {
        if (!cJSON_IsString(sensor_measurement_list_local_nonprimitive)){
            ogs_error("OpenAPI_immediate_mdt_conf_parseFromJSON() failed [sensor_measurement_list]");
            goto end;
        }

        OpenAPI_list_add(sensor_measurement_listList, (void *)OpenAPI_sensor_measurement_FromString(sensor_measurement_list_local_nonprimitive->valuestring));
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

