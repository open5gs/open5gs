
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qfi_container_information.h"

OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_create(
    bool is_q_fi,
    int q_fi,
    char *report_time,
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_qos_characteristics_t *qo_s_characteristics,
    OpenAPI_charging_user_location_t *user_location_information,
    char *uetime_zone,
    OpenAPI_list_t* presence_reporting_area_information,
    OpenAPI_rat_type_e r_at_type,
    OpenAPI_list_t *serving_network_function_id,
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status,
    bool is__3gpp_charging_id,
    int _3gpp_charging_id,
    bool is_diagnostics,
    int diagnostics,
    OpenAPI_list_t *enhanced_diagnostics
)
{
    OpenAPI_qfi_container_information_t *qfi_container_information_local_var = ogs_malloc(sizeof(OpenAPI_qfi_container_information_t));
    ogs_assert(qfi_container_information_local_var);

    qfi_container_information_local_var->is_q_fi = is_q_fi;
    qfi_container_information_local_var->q_fi = q_fi;
    qfi_container_information_local_var->report_time = report_time;
    qfi_container_information_local_var->timeof_first_usage = timeof_first_usage;
    qfi_container_information_local_var->timeof_last_usage = timeof_last_usage;
    qfi_container_information_local_var->is_qo_s_information_null = is_qo_s_information_null;
    qfi_container_information_local_var->qo_s_information = qo_s_information;
    qfi_container_information_local_var->qo_s_characteristics = qo_s_characteristics;
    qfi_container_information_local_var->user_location_information = user_location_information;
    qfi_container_information_local_var->uetime_zone = uetime_zone;
    qfi_container_information_local_var->presence_reporting_area_information = presence_reporting_area_information;
    qfi_container_information_local_var->r_at_type = r_at_type;
    qfi_container_information_local_var->serving_network_function_id = serving_network_function_id;
    qfi_container_information_local_var->_3gpp_ps_data_off_status = _3gpp_ps_data_off_status;
    qfi_container_information_local_var->is__3gpp_charging_id = is__3gpp_charging_id;
    qfi_container_information_local_var->_3gpp_charging_id = _3gpp_charging_id;
    qfi_container_information_local_var->is_diagnostics = is_diagnostics;
    qfi_container_information_local_var->diagnostics = diagnostics;
    qfi_container_information_local_var->enhanced_diagnostics = enhanced_diagnostics;

    return qfi_container_information_local_var;
}

void OpenAPI_qfi_container_information_free(OpenAPI_qfi_container_information_t *qfi_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qfi_container_information) {
        return;
    }
    if (qfi_container_information->report_time) {
        ogs_free(qfi_container_information->report_time);
        qfi_container_information->report_time = NULL;
    }
    if (qfi_container_information->timeof_first_usage) {
        ogs_free(qfi_container_information->timeof_first_usage);
        qfi_container_information->timeof_first_usage = NULL;
    }
    if (qfi_container_information->timeof_last_usage) {
        ogs_free(qfi_container_information->timeof_last_usage);
        qfi_container_information->timeof_last_usage = NULL;
    }
    if (qfi_container_information->qo_s_information) {
        OpenAPI_qos_data_free(qfi_container_information->qo_s_information);
        qfi_container_information->qo_s_information = NULL;
    }
    if (qfi_container_information->qo_s_characteristics) {
        OpenAPI_qos_characteristics_free(qfi_container_information->qo_s_characteristics);
        qfi_container_information->qo_s_characteristics = NULL;
    }
    if (qfi_container_information->user_location_information) {
        OpenAPI_charging_user_location_free(qfi_container_information->user_location_information);
        qfi_container_information->user_location_information = NULL;
    }
    if (qfi_container_information->uetime_zone) {
        ogs_free(qfi_container_information->uetime_zone);
        qfi_container_information->uetime_zone = NULL;
    }
    if (qfi_container_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(qfi_container_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(qfi_container_information->presence_reporting_area_information);
        qfi_container_information->presence_reporting_area_information = NULL;
    }
    if (qfi_container_information->serving_network_function_id) {
        OpenAPI_list_for_each(qfi_container_information->serving_network_function_id, node) {
            OpenAPI_serving_network_function_id_free(node->data);
        }
        OpenAPI_list_free(qfi_container_information->serving_network_function_id);
        qfi_container_information->serving_network_function_id = NULL;
    }
    if (qfi_container_information->enhanced_diagnostics) {
        OpenAPI_list_for_each(qfi_container_information->enhanced_diagnostics, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(qfi_container_information->enhanced_diagnostics);
        qfi_container_information->enhanced_diagnostics = NULL;
    }
    ogs_free(qfi_container_information);
}

cJSON *OpenAPI_qfi_container_information_convertToJSON(OpenAPI_qfi_container_information_t *qfi_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qfi_container_information == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [QFIContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qfi_container_information->is_q_fi) {
    if (cJSON_AddNumberToObject(item, "qFI", qfi_container_information->q_fi) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [q_fi]");
        goto end;
    }
    }

    if (!qfi_container_information->report_time) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [report_time]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "reportTime", qfi_container_information->report_time) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [report_time]");
        goto end;
    }

    if (qfi_container_information->timeof_first_usage) {
    if (cJSON_AddStringToObject(item, "timeofFirstUsage", qfi_container_information->timeof_first_usage) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    if (qfi_container_information->timeof_last_usage) {
    if (cJSON_AddStringToObject(item, "timeofLastUsage", qfi_container_information->timeof_last_usage) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    if (qfi_container_information->qo_s_information) {
    cJSON *qo_s_information_local_JSON = OpenAPI_qos_data_convertToJSON(qfi_container_information->qo_s_information);
    if (qo_s_information_local_JSON == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoSInformation", qo_s_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [qo_s_information]");
        goto end;
    }
    } else if (qfi_container_information->is_qo_s_information_null) {
        if (cJSON_AddNullToObject(item, "qoSInformation") == NULL) {
            ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [qo_s_information]");
            goto end;
        }
    }

    if (qfi_container_information->qo_s_characteristics) {
    cJSON *qo_s_characteristics_local_JSON = OpenAPI_qos_characteristics_convertToJSON(qfi_container_information->qo_s_characteristics);
    if (qo_s_characteristics_local_JSON == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [qo_s_characteristics]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qoSCharacteristics", qo_s_characteristics_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [qo_s_characteristics]");
        goto end;
    }
    }

    if (qfi_container_information->user_location_information) {
    cJSON *user_location_information_local_JSON = OpenAPI_charging_user_location_convertToJSON(qfi_container_information->user_location_information);
    if (user_location_information_local_JSON == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [user_location_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationInformation", user_location_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [user_location_information]");
        goto end;
    }
    }

    if (qfi_container_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", qfi_container_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (qfi_container_information->presence_reporting_area_information) {
    cJSON *presence_reporting_area_information = cJSON_AddObjectToObject(item, "presenceReportingAreaInformation");
    if (presence_reporting_area_information == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
        goto end;
    }
    cJSON *localMapObject = presence_reporting_area_information;
    if (qfi_container_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(qfi_container_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (qfi_container_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(qfi_container_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (qfi_container_information->serving_network_function_id) {
    cJSON *serving_network_function_idList = cJSON_AddArrayToObject(item, "servingNetworkFunctionID");
    if (serving_network_function_idList == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [serving_network_function_id]");
        goto end;
    }
    OpenAPI_list_for_each(qfi_container_information->serving_network_function_id, node) {
        cJSON *itemLocal = OpenAPI_serving_network_function_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [serving_network_function_id]");
            goto end;
        }
        cJSON_AddItemToArray(serving_network_function_idList, itemLocal);
    }
    }

    if (qfi_container_information->_3gpp_ps_data_off_status != OpenAPI_model_3_gppps_data_off_status_NULL) {
    if (cJSON_AddStringToObject(item, "3gppPSDataOffStatus", OpenAPI_model_3_gppps_data_off_status_ToString(qfi_container_information->_3gpp_ps_data_off_status)) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    }

    if (qfi_container_information->is__3gpp_charging_id) {
    if (cJSON_AddNumberToObject(item, "3gppChargingId", qfi_container_information->_3gpp_charging_id) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [_3gpp_charging_id]");
        goto end;
    }
    }

    if (qfi_container_information->is_diagnostics) {
    if (cJSON_AddNumberToObject(item, "diagnostics", qfi_container_information->diagnostics) == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [diagnostics]");
        goto end;
    }
    }

    if (qfi_container_information->enhanced_diagnostics) {
    cJSON *enhanced_diagnosticsList = cJSON_AddArrayToObject(item, "enhancedDiagnostics");
    if (enhanced_diagnosticsList == NULL) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [enhanced_diagnostics]");
        goto end;
    }
    OpenAPI_list_for_each(qfi_container_information->enhanced_diagnostics, node) {
        if (cJSON_AddStringToObject(enhanced_diagnosticsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed [enhanced_diagnostics]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_parseFromJSON(cJSON *qfi_container_informationJSON)
{
    OpenAPI_qfi_container_information_t *qfi_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *q_fi = NULL;
    cJSON *report_time = NULL;
    cJSON *timeof_first_usage = NULL;
    cJSON *timeof_last_usage = NULL;
    cJSON *qo_s_information = NULL;
    OpenAPI_qos_data_t *qo_s_information_local_nonprim = NULL;
    cJSON *qo_s_characteristics = NULL;
    OpenAPI_qos_characteristics_t *qo_s_characteristics_local_nonprim = NULL;
    cJSON *user_location_information = NULL;
    OpenAPI_charging_user_location_t *user_location_information_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *presence_reporting_area_information = NULL;
    OpenAPI_list_t *presence_reporting_area_informationList = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *serving_network_function_id = NULL;
    OpenAPI_list_t *serving_network_function_idList = NULL;
    cJSON *_3gpp_ps_data_off_status = NULL;
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_statusVariable = 0;
    cJSON *_3gpp_charging_id = NULL;
    cJSON *diagnostics = NULL;
    cJSON *enhanced_diagnostics = NULL;
    OpenAPI_list_t *enhanced_diagnosticsList = NULL;
    q_fi = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "qFI");
    if (q_fi) {
    if (!cJSON_IsNumber(q_fi)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [q_fi]");
        goto end;
    }
    }

    report_time = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "reportTime");
    if (!report_time) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [report_time]");
        goto end;
    }
    if (!cJSON_IsString(report_time) && !cJSON_IsNull(report_time)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [report_time]");
        goto end;
    }

    timeof_first_usage = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "timeofFirstUsage");
    if (timeof_first_usage) {
    if (!cJSON_IsString(timeof_first_usage) && !cJSON_IsNull(timeof_first_usage)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [timeof_first_usage]");
        goto end;
    }
    }

    timeof_last_usage = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "timeofLastUsage");
    if (timeof_last_usage) {
    if (!cJSON_IsString(timeof_last_usage) && !cJSON_IsNull(timeof_last_usage)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [timeof_last_usage]");
        goto end;
    }
    }

    qo_s_information = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "qoSInformation");
    if (qo_s_information) {
    if (!cJSON_IsNull(qo_s_information)) {
    qo_s_information_local_nonprim = OpenAPI_qos_data_parseFromJSON(qo_s_information);
    if (!qo_s_information_local_nonprim) {
        ogs_error("OpenAPI_qos_data_parseFromJSON failed [qo_s_information]");
        goto end;
    }
    }
    }

    qo_s_characteristics = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "qoSCharacteristics");
    if (qo_s_characteristics) {
    qo_s_characteristics_local_nonprim = OpenAPI_qos_characteristics_parseFromJSON(qo_s_characteristics);
    if (!qo_s_characteristics_local_nonprim) {
        ogs_error("OpenAPI_qos_characteristics_parseFromJSON failed [qo_s_characteristics]");
        goto end;
    }
    }

    user_location_information = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "userLocationInformation");
    if (user_location_information) {
    user_location_information_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_location_information);
    if (!user_location_information_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_location_information]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    presence_reporting_area_information = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "presenceReportingAreaInformation");
    if (presence_reporting_area_information) {
        cJSON *presence_reporting_area_information_local_map = NULL;
        if (!cJSON_IsObject(presence_reporting_area_information) && !cJSON_IsNull(presence_reporting_area_information)) {
            ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [presence_reporting_area_information]");
            goto end;
        }
        if (cJSON_IsObject(presence_reporting_area_information)) {
            presence_reporting_area_informationList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(presence_reporting_area_information_local_map, presence_reporting_area_information) {
                cJSON *localMapObject = presence_reporting_area_information_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                if (localMapKeyPair == NULL) {
                    ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [presence_reporting_area_information]");
                    goto end;
                }
                OpenAPI_list_add(presence_reporting_area_informationList, localMapKeyPair);
            }
        }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    serving_network_function_id = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "servingNetworkFunctionID");
    if (serving_network_function_id) {
        cJSON *serving_network_function_id_local = NULL;
        if (!cJSON_IsArray(serving_network_function_id)) {
            ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [serving_network_function_id]");
            goto end;
        }

        serving_network_function_idList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_network_function_id_local, serving_network_function_id) {
            if (!cJSON_IsObject(serving_network_function_id_local)) {
                ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [serving_network_function_id]");
                goto end;
            }
            OpenAPI_serving_network_function_id_t *serving_network_function_idItem = OpenAPI_serving_network_function_id_parseFromJSON(serving_network_function_id_local);
            if (!serving_network_function_idItem) {
                ogs_error("No serving_network_function_idItem");
                goto end;
            }
            OpenAPI_list_add(serving_network_function_idList, serving_network_function_idItem);
        }
    }

    _3gpp_ps_data_off_status = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "3gppPSDataOffStatus");
    if (_3gpp_ps_data_off_status) {
    if (!cJSON_IsString(_3gpp_ps_data_off_status)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [_3gpp_ps_data_off_status]");
        goto end;
    }
    _3gpp_ps_data_off_statusVariable = OpenAPI_model_3_gppps_data_off_status_FromString(_3gpp_ps_data_off_status->valuestring);
    }

    _3gpp_charging_id = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "3gppChargingId");
    if (_3gpp_charging_id) {
    if (!cJSON_IsNumber(_3gpp_charging_id)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [_3gpp_charging_id]");
        goto end;
    }
    }

    diagnostics = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "diagnostics");
    if (diagnostics) {
    if (!cJSON_IsNumber(diagnostics)) {
        ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [diagnostics]");
        goto end;
    }
    }

    enhanced_diagnostics = cJSON_GetObjectItemCaseSensitive(qfi_container_informationJSON, "enhancedDiagnostics");
    if (enhanced_diagnostics) {
        cJSON *enhanced_diagnostics_local = NULL;
        if (!cJSON_IsArray(enhanced_diagnostics)) {
            ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [enhanced_diagnostics]");
            goto end;
        }

        enhanced_diagnosticsList = OpenAPI_list_create();

        cJSON_ArrayForEach(enhanced_diagnostics_local, enhanced_diagnostics) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(enhanced_diagnostics_local)) {
                ogs_error("OpenAPI_qfi_container_information_parseFromJSON() failed [enhanced_diagnostics]");
                goto end;
            }
            OpenAPI_list_add(enhanced_diagnosticsList, ogs_strdup(enhanced_diagnostics_local->valuestring));
        }
    }

    qfi_container_information_local_var = OpenAPI_qfi_container_information_create (
        q_fi ? true : false,
        q_fi ? q_fi->valuedouble : 0,
        ogs_strdup(report_time->valuestring),
        timeof_first_usage && !cJSON_IsNull(timeof_first_usage) ? ogs_strdup(timeof_first_usage->valuestring) : NULL,
        timeof_last_usage && !cJSON_IsNull(timeof_last_usage) ? ogs_strdup(timeof_last_usage->valuestring) : NULL,
        qo_s_information && cJSON_IsNull(qo_s_information) ? true : false,
        qo_s_information ? qo_s_information_local_nonprim : NULL,
        qo_s_characteristics ? qo_s_characteristics_local_nonprim : NULL,
        user_location_information ? user_location_information_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        presence_reporting_area_information ? presence_reporting_area_informationList : NULL,
        r_at_type ? r_at_typeVariable : 0,
        serving_network_function_id ? serving_network_function_idList : NULL,
        _3gpp_ps_data_off_status ? _3gpp_ps_data_off_statusVariable : 0,
        _3gpp_charging_id ? true : false,
        _3gpp_charging_id ? _3gpp_charging_id->valuedouble : 0,
        diagnostics ? true : false,
        diagnostics ? diagnostics->valuedouble : 0,
        enhanced_diagnostics ? enhanced_diagnosticsList : NULL
    );

    return qfi_container_information_local_var;
end:
    if (qo_s_information_local_nonprim) {
        OpenAPI_qos_data_free(qo_s_information_local_nonprim);
        qo_s_information_local_nonprim = NULL;
    }
    if (qo_s_characteristics_local_nonprim) {
        OpenAPI_qos_characteristics_free(qo_s_characteristics_local_nonprim);
        qo_s_characteristics_local_nonprim = NULL;
    }
    if (user_location_information_local_nonprim) {
        OpenAPI_charging_user_location_free(user_location_information_local_nonprim);
        user_location_information_local_nonprim = NULL;
    }
    if (presence_reporting_area_informationList) {
        OpenAPI_list_for_each(presence_reporting_area_informationList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(presence_reporting_area_informationList);
        presence_reporting_area_informationList = NULL;
    }
    if (serving_network_function_idList) {
        OpenAPI_list_for_each(serving_network_function_idList, node) {
            OpenAPI_serving_network_function_id_free(node->data);
        }
        OpenAPI_list_free(serving_network_function_idList);
        serving_network_function_idList = NULL;
    }
    if (enhanced_diagnosticsList) {
        OpenAPI_list_for_each(enhanced_diagnosticsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(enhanced_diagnosticsList);
        enhanced_diagnosticsList = NULL;
    }
    return NULL;
}

OpenAPI_qfi_container_information_t *OpenAPI_qfi_container_information_copy(OpenAPI_qfi_container_information_t *dst, OpenAPI_qfi_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qfi_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qfi_container_information_convertToJSON() failed");
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

    OpenAPI_qfi_container_information_free(dst);
    dst = OpenAPI_qfi_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

