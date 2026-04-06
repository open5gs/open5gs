
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "session_management_subscription_data.h"

OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_create(
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t* dnn_configurations,
    OpenAPI_list_t *internal_group_ids,
    OpenAPI_list_t* shared_vn_group_data_ids,
    char *shared_dnn_configurations_id,
    OpenAPI_odb_packet_services_e odb_packet_services,
    OpenAPI_list_t* odb_exempted_dnn_data,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    char *shared_trace_data_id,
    OpenAPI_list_t* expected_ue_behaviours_list,
    OpenAPI_list_t* expected_ue_behaviour_data,
    OpenAPI_list_t* app_specific_expected_ue_behaviour_data,
    OpenAPI_list_t* suggested_packet_num_dl_list,
    char *_3gpp_charging_characteristics,
    char *chf_group_id,
    OpenAPI_nsac_admission_mode_e nsac_mode,
    bool is_sess_inact_timer,
    int sess_inact_timer,
    char *supported_features,
    OpenAPI_list_t *additional_shared_dnn_configurations_ids,
    OpenAPI_ue_level_measurements_configuration_t *ue_level_meas_config
)
{
    OpenAPI_session_management_subscription_data_t *session_management_subscription_data_local_var = ogs_malloc(sizeof(OpenAPI_session_management_subscription_data_t));
    ogs_assert(session_management_subscription_data_local_var);

    session_management_subscription_data_local_var->single_nssai = single_nssai;
    session_management_subscription_data_local_var->dnn_configurations = dnn_configurations;
    session_management_subscription_data_local_var->internal_group_ids = internal_group_ids;
    session_management_subscription_data_local_var->shared_vn_group_data_ids = shared_vn_group_data_ids;
    session_management_subscription_data_local_var->shared_dnn_configurations_id = shared_dnn_configurations_id;
    session_management_subscription_data_local_var->odb_packet_services = odb_packet_services;
    session_management_subscription_data_local_var->odb_exempted_dnn_data = odb_exempted_dnn_data;
    session_management_subscription_data_local_var->is_trace_data_null = is_trace_data_null;
    session_management_subscription_data_local_var->trace_data = trace_data;
    session_management_subscription_data_local_var->shared_trace_data_id = shared_trace_data_id;
    session_management_subscription_data_local_var->expected_ue_behaviours_list = expected_ue_behaviours_list;
    session_management_subscription_data_local_var->expected_ue_behaviour_data = expected_ue_behaviour_data;
    session_management_subscription_data_local_var->app_specific_expected_ue_behaviour_data = app_specific_expected_ue_behaviour_data;
    session_management_subscription_data_local_var->suggested_packet_num_dl_list = suggested_packet_num_dl_list;
    session_management_subscription_data_local_var->_3gpp_charging_characteristics = _3gpp_charging_characteristics;
    session_management_subscription_data_local_var->chf_group_id = chf_group_id;
    session_management_subscription_data_local_var->nsac_mode = nsac_mode;
    session_management_subscription_data_local_var->is_sess_inact_timer = is_sess_inact_timer;
    session_management_subscription_data_local_var->sess_inact_timer = sess_inact_timer;
    session_management_subscription_data_local_var->supported_features = supported_features;
    session_management_subscription_data_local_var->additional_shared_dnn_configurations_ids = additional_shared_dnn_configurations_ids;
    session_management_subscription_data_local_var->ue_level_meas_config = ue_level_meas_config;

    return session_management_subscription_data_local_var;
}

void OpenAPI_session_management_subscription_data_free(OpenAPI_session_management_subscription_data_t *session_management_subscription_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == session_management_subscription_data) {
        return;
    }
    if (session_management_subscription_data->single_nssai) {
        OpenAPI_snssai_free(session_management_subscription_data->single_nssai);
        session_management_subscription_data->single_nssai = NULL;
    }
    if (session_management_subscription_data->dnn_configurations) {
        OpenAPI_list_for_each(session_management_subscription_data->dnn_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dnn_configuration_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->dnn_configurations);
        session_management_subscription_data->dnn_configurations = NULL;
    }
    if (session_management_subscription_data->internal_group_ids) {
        OpenAPI_list_for_each(session_management_subscription_data->internal_group_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(session_management_subscription_data->internal_group_ids);
        session_management_subscription_data->internal_group_ids = NULL;
    }
    if (session_management_subscription_data->shared_vn_group_data_ids) {
        OpenAPI_list_for_each(session_management_subscription_data->shared_vn_group_data_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->shared_vn_group_data_ids);
        session_management_subscription_data->shared_vn_group_data_ids = NULL;
    }
    if (session_management_subscription_data->shared_dnn_configurations_id) {
        ogs_free(session_management_subscription_data->shared_dnn_configurations_id);
        session_management_subscription_data->shared_dnn_configurations_id = NULL;
    }
    if (session_management_subscription_data->odb_exempted_dnn_data) {
        OpenAPI_list_for_each(session_management_subscription_data->odb_exempted_dnn_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_odb_exempted_dnn_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->odb_exempted_dnn_data);
        session_management_subscription_data->odb_exempted_dnn_data = NULL;
    }
    if (session_management_subscription_data->trace_data) {
        OpenAPI_trace_data_free(session_management_subscription_data->trace_data);
        session_management_subscription_data->trace_data = NULL;
    }
    if (session_management_subscription_data->shared_trace_data_id) {
        ogs_free(session_management_subscription_data->shared_trace_data_id);
        session_management_subscription_data->shared_trace_data_id = NULL;
    }
    if (session_management_subscription_data->expected_ue_behaviours_list) {
        OpenAPI_list_for_each(session_management_subscription_data->expected_ue_behaviours_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->expected_ue_behaviours_list);
        session_management_subscription_data->expected_ue_behaviours_list = NULL;
    }
    if (session_management_subscription_data->expected_ue_behaviour_data) {
        OpenAPI_list_for_each(session_management_subscription_data->expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->expected_ue_behaviour_data);
        session_management_subscription_data->expected_ue_behaviour_data = NULL;
    }
    if (session_management_subscription_data->app_specific_expected_ue_behaviour_data) {
        OpenAPI_list_for_each(session_management_subscription_data->app_specific_expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_specific_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->app_specific_expected_ue_behaviour_data);
        session_management_subscription_data->app_specific_expected_ue_behaviour_data = NULL;
    }
    if (session_management_subscription_data->suggested_packet_num_dl_list) {
        OpenAPI_list_for_each(session_management_subscription_data->suggested_packet_num_dl_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_suggested_packet_num_dl_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(session_management_subscription_data->suggested_packet_num_dl_list);
        session_management_subscription_data->suggested_packet_num_dl_list = NULL;
    }
    if (session_management_subscription_data->_3gpp_charging_characteristics) {
        ogs_free(session_management_subscription_data->_3gpp_charging_characteristics);
        session_management_subscription_data->_3gpp_charging_characteristics = NULL;
    }
    if (session_management_subscription_data->chf_group_id) {
        ogs_free(session_management_subscription_data->chf_group_id);
        session_management_subscription_data->chf_group_id = NULL;
    }
    if (session_management_subscription_data->supported_features) {
        ogs_free(session_management_subscription_data->supported_features);
        session_management_subscription_data->supported_features = NULL;
    }
    if (session_management_subscription_data->additional_shared_dnn_configurations_ids) {
        OpenAPI_list_for_each(session_management_subscription_data->additional_shared_dnn_configurations_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(session_management_subscription_data->additional_shared_dnn_configurations_ids);
        session_management_subscription_data->additional_shared_dnn_configurations_ids = NULL;
    }
    if (session_management_subscription_data->ue_level_meas_config) {
        OpenAPI_ue_level_measurements_configuration_free(session_management_subscription_data->ue_level_meas_config);
        session_management_subscription_data->ue_level_meas_config = NULL;
    }
    ogs_free(session_management_subscription_data);
}

cJSON *OpenAPI_session_management_subscription_data_convertToJSON(OpenAPI_session_management_subscription_data_t *session_management_subscription_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (session_management_subscription_data == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [SessionManagementSubscriptionData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!session_management_subscription_data->single_nssai) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [single_nssai]");
        return NULL;
    }
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(session_management_subscription_data->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [single_nssai]");
        goto end;
    }

    if (session_management_subscription_data->dnn_configurations) {
    cJSON *dnn_configurations = cJSON_AddObjectToObject(item, "dnnConfigurations");
    if (dnn_configurations == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [dnn_configurations]");
        goto end;
    }
    cJSON *localMapObject = dnn_configurations;
    if (session_management_subscription_data->dnn_configurations) {
        OpenAPI_list_for_each(session_management_subscription_data->dnn_configurations, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [dnn_configurations]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [dnn_configurations]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_dnn_configuration_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->internal_group_ids) {
    cJSON *internal_group_idsList = cJSON_AddArrayToObject(item, "internalGroupIds");
    if (internal_group_idsList == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [internal_group_ids]");
        goto end;
    }
    OpenAPI_list_for_each(session_management_subscription_data->internal_group_ids, node) {
        if (cJSON_AddStringToObject(internal_group_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [internal_group_ids]");
            goto end;
        }
    }
    }

    if (session_management_subscription_data->shared_vn_group_data_ids) {
    cJSON *shared_vn_group_data_ids = cJSON_AddObjectToObject(item, "sharedVnGroupDataIds");
    if (shared_vn_group_data_ids == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [shared_vn_group_data_ids]");
        goto end;
    }
    cJSON *localMapObject = shared_vn_group_data_ids;
    if (session_management_subscription_data->shared_vn_group_data_ids) {
        OpenAPI_list_for_each(session_management_subscription_data->shared_vn_group_data_ids, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [shared_vn_group_data_ids]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [shared_vn_group_data_ids]");
                goto end;
            }
            if (cJSON_AddStringToObject(localMapObject, localKeyValue->key, (char*)localKeyValue->value) == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
        }
    }
    }

    if (session_management_subscription_data->shared_dnn_configurations_id) {
    if (cJSON_AddStringToObject(item, "sharedDnnConfigurationsId", session_management_subscription_data->shared_dnn_configurations_id) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [shared_dnn_configurations_id]");
        goto end;
    }
    }

    if (session_management_subscription_data->odb_packet_services != OpenAPI_odb_packet_services_NULL) {
    if (cJSON_AddStringToObject(item, "odbPacketServices", OpenAPI_odb_packet_services_ToString(session_management_subscription_data->odb_packet_services)) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [odb_packet_services]");
        goto end;
    }
    }

    if (session_management_subscription_data->odb_exempted_dnn_data) {
    cJSON *odb_exempted_dnn_data = cJSON_AddObjectToObject(item, "odbExemptedDnnData");
    if (odb_exempted_dnn_data == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [odb_exempted_dnn_data]");
        goto end;
    }
    cJSON *localMapObject = odb_exempted_dnn_data;
    if (session_management_subscription_data->odb_exempted_dnn_data) {
        OpenAPI_list_for_each(session_management_subscription_data->odb_exempted_dnn_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [odb_exempted_dnn_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [odb_exempted_dnn_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_odb_exempted_dnn_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(session_management_subscription_data->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (session_management_subscription_data->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (session_management_subscription_data->shared_trace_data_id) {
    if (cJSON_AddStringToObject(item, "sharedTraceDataId", session_management_subscription_data->shared_trace_data_id) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

    if (session_management_subscription_data->expected_ue_behaviours_list) {
    cJSON *expected_ue_behaviours_list = cJSON_AddObjectToObject(item, "expectedUeBehavioursList");
    if (expected_ue_behaviours_list == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviours_list]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviours_list;
    if (session_management_subscription_data->expected_ue_behaviours_list) {
        OpenAPI_list_for_each(session_management_subscription_data->expected_ue_behaviours_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviours_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviours_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_expected_ue_behaviour_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->expected_ue_behaviour_data) {
    cJSON *expected_ue_behaviour_data = cJSON_AddObjectToObject(item, "expectedUeBehaviourData");
    if (expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviour_data]");
        goto end;
    }
    cJSON *localMapObject = expected_ue_behaviour_data;
    if (session_management_subscription_data->expected_ue_behaviour_data) {
        OpenAPI_list_for_each(session_management_subscription_data->expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviour_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [expected_ue_behaviour_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_expected_ue_behaviour_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->app_specific_expected_ue_behaviour_data) {
    cJSON *app_specific_expected_ue_behaviour_data = cJSON_AddObjectToObject(item, "appSpecificExpectedUeBehaviourData");
    if (app_specific_expected_ue_behaviour_data == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
        goto end;
    }
    cJSON *localMapObject = app_specific_expected_ue_behaviour_data;
    if (session_management_subscription_data->app_specific_expected_ue_behaviour_data) {
        OpenAPI_list_for_each(session_management_subscription_data->app_specific_expected_ue_behaviour_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [app_specific_expected_ue_behaviour_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_app_specific_expected_ue_behaviour_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->suggested_packet_num_dl_list) {
    cJSON *suggested_packet_num_dl_list = cJSON_AddObjectToObject(item, "suggestedPacketNumDlList");
    if (suggested_packet_num_dl_list == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [suggested_packet_num_dl_list]");
        goto end;
    }
    cJSON *localMapObject = suggested_packet_num_dl_list;
    if (session_management_subscription_data->suggested_packet_num_dl_list) {
        OpenAPI_list_for_each(session_management_subscription_data->suggested_packet_num_dl_list, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [suggested_packet_num_dl_list]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [suggested_packet_num_dl_list]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_suggested_packet_num_dl_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (session_management_subscription_data->_3gpp_charging_characteristics) {
    if (cJSON_AddStringToObject(item, "3gppChargingCharacteristics", session_management_subscription_data->_3gpp_charging_characteristics) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    if (session_management_subscription_data->chf_group_id) {
    if (cJSON_AddStringToObject(item, "chfGroupId", session_management_subscription_data->chf_group_id) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [chf_group_id]");
        goto end;
    }
    }

    if (session_management_subscription_data->nsac_mode != OpenAPI_nsac_admission_mode_NULL) {
    if (cJSON_AddStringToObject(item, "nsacMode", OpenAPI_nsac_admission_mode_ToString(session_management_subscription_data->nsac_mode)) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [nsac_mode]");
        goto end;
    }
    }

    if (session_management_subscription_data->is_sess_inact_timer) {
    if (cJSON_AddNumberToObject(item, "sessInactTimer", session_management_subscription_data->sess_inact_timer) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [sess_inact_timer]");
        goto end;
    }
    }

    if (session_management_subscription_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", session_management_subscription_data->supported_features) == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (session_management_subscription_data->additional_shared_dnn_configurations_ids) {
    cJSON *additional_shared_dnn_configurations_idsList = cJSON_AddArrayToObject(item, "additionalSharedDnnConfigurationsIds");
    if (additional_shared_dnn_configurations_idsList == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [additional_shared_dnn_configurations_ids]");
        goto end;
    }
    OpenAPI_list_for_each(session_management_subscription_data->additional_shared_dnn_configurations_ids, node) {
        if (cJSON_AddStringToObject(additional_shared_dnn_configurations_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [additional_shared_dnn_configurations_ids]");
            goto end;
        }
    }
    }

    if (session_management_subscription_data->ue_level_meas_config) {
    cJSON *ue_level_meas_config_local_JSON = OpenAPI_ue_level_measurements_configuration_convertToJSON(session_management_subscription_data->ue_level_meas_config);
    if (ue_level_meas_config_local_JSON == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [ue_level_meas_config]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLevelMeasConfig", ue_level_meas_config_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed [ue_level_meas_config]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_parseFromJSON(cJSON *session_management_subscription_dataJSON)
{
    OpenAPI_session_management_subscription_data_t *session_management_subscription_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *dnn_configurations = NULL;
    OpenAPI_list_t *dnn_configurationsList = NULL;
    cJSON *internal_group_ids = NULL;
    OpenAPI_list_t *internal_group_idsList = NULL;
    cJSON *shared_vn_group_data_ids = NULL;
    OpenAPI_list_t *shared_vn_group_data_idsList = NULL;
    cJSON *shared_dnn_configurations_id = NULL;
    cJSON *odb_packet_services = NULL;
    OpenAPI_odb_packet_services_e odb_packet_servicesVariable = 0;
    cJSON *odb_exempted_dnn_data = NULL;
    OpenAPI_list_t *odb_exempted_dnn_dataList = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *shared_trace_data_id = NULL;
    cJSON *expected_ue_behaviours_list = NULL;
    OpenAPI_list_t *expected_ue_behaviours_listList = NULL;
    cJSON *expected_ue_behaviour_data = NULL;
    OpenAPI_list_t *expected_ue_behaviour_dataList = NULL;
    cJSON *app_specific_expected_ue_behaviour_data = NULL;
    OpenAPI_list_t *app_specific_expected_ue_behaviour_dataList = NULL;
    cJSON *suggested_packet_num_dl_list = NULL;
    OpenAPI_list_t *suggested_packet_num_dl_listList = NULL;
    cJSON *_3gpp_charging_characteristics = NULL;
    cJSON *chf_group_id = NULL;
    cJSON *nsac_mode = NULL;
    OpenAPI_nsac_admission_mode_e nsac_modeVariable = 0;
    cJSON *sess_inact_timer = NULL;
    cJSON *supported_features = NULL;
    cJSON *additional_shared_dnn_configurations_ids = NULL;
    OpenAPI_list_t *additional_shared_dnn_configurations_idsList = NULL;
    cJSON *ue_level_meas_config = NULL;
    OpenAPI_ue_level_measurements_configuration_t *ue_level_meas_config_local_nonprim = NULL;
    single_nssai = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "singleNssai");
    if (!single_nssai) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [single_nssai]");
        goto end;
    }
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }

    dnn_configurations = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "dnnConfigurations");
    if (dnn_configurations) {
        cJSON *dnn_configurations_local_map = NULL;
        if (!cJSON_IsObject(dnn_configurations) && !cJSON_IsNull(dnn_configurations)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [dnn_configurations]");
            goto end;
        }
        if (cJSON_IsObject(dnn_configurations)) {
            dnn_configurationsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(dnn_configurations_local_map, dnn_configurations) {
                cJSON *localMapObject = dnn_configurations_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_dnn_configuration_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(dnn_configurationsList, localMapKeyPair);
            }
        }
    }

    internal_group_ids = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "internalGroupIds");
    if (internal_group_ids) {
        cJSON *internal_group_ids_local = NULL;
        if (!cJSON_IsArray(internal_group_ids)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [internal_group_ids]");
            goto end;
        }

        internal_group_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(internal_group_ids_local, internal_group_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(internal_group_ids_local)) {
                ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [internal_group_ids]");
                goto end;
            }
            OpenAPI_list_add(internal_group_idsList, ogs_strdup(internal_group_ids_local->valuestring));
        }
    }

    shared_vn_group_data_ids = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "sharedVnGroupDataIds");
    if (shared_vn_group_data_ids) {
        cJSON *shared_vn_group_data_ids_local_map = NULL;
        if (!cJSON_IsObject(shared_vn_group_data_ids) && !cJSON_IsNull(shared_vn_group_data_ids)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [shared_vn_group_data_ids]");
            goto end;
        }
        if (cJSON_IsObject(shared_vn_group_data_ids)) {
            shared_vn_group_data_idsList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(shared_vn_group_data_ids_local_map, shared_vn_group_data_ids) {
                cJSON *localMapObject = shared_vn_group_data_ids_local_map;
                double *localDouble = NULL;
                int *localInt = NULL;
                if (!cJSON_IsString(localMapObject)) {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), ogs_strdup(localMapObject->valuestring));
                OpenAPI_list_add(shared_vn_group_data_idsList, localMapKeyPair);
            }
        }
    }

    shared_dnn_configurations_id = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "sharedDnnConfigurationsId");
    if (shared_dnn_configurations_id) {
    if (!cJSON_IsString(shared_dnn_configurations_id) && !cJSON_IsNull(shared_dnn_configurations_id)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [shared_dnn_configurations_id]");
        goto end;
    }
    }

    odb_packet_services = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "odbPacketServices");
    if (odb_packet_services) {
    if (!cJSON_IsString(odb_packet_services)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [odb_packet_services]");
        goto end;
    }
    odb_packet_servicesVariable = OpenAPI_odb_packet_services_FromString(odb_packet_services->valuestring);
    }

    odb_exempted_dnn_data = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "odbExemptedDnnData");
    if (odb_exempted_dnn_data) {
        cJSON *odb_exempted_dnn_data_local_map = NULL;
        if (!cJSON_IsObject(odb_exempted_dnn_data) && !cJSON_IsNull(odb_exempted_dnn_data)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [odb_exempted_dnn_data]");
            goto end;
        }
        if (cJSON_IsObject(odb_exempted_dnn_data)) {
            odb_exempted_dnn_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(odb_exempted_dnn_data_local_map, odb_exempted_dnn_data) {
                cJSON *localMapObject = odb_exempted_dnn_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_odb_exempted_dnn_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(odb_exempted_dnn_dataList, localMapKeyPair);
            }
        }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    shared_trace_data_id = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "sharedTraceDataId");
    if (shared_trace_data_id) {
    if (!cJSON_IsString(shared_trace_data_id) && !cJSON_IsNull(shared_trace_data_id)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [shared_trace_data_id]");
        goto end;
    }
    }

    expected_ue_behaviours_list = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "expectedUeBehavioursList");
    if (expected_ue_behaviours_list) {
        cJSON *expected_ue_behaviours_list_local_map = NULL;
        if (!cJSON_IsObject(expected_ue_behaviours_list) && !cJSON_IsNull(expected_ue_behaviours_list)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [expected_ue_behaviours_list]");
            goto end;
        }
        if (cJSON_IsObject(expected_ue_behaviours_list)) {
            expected_ue_behaviours_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(expected_ue_behaviours_list_local_map, expected_ue_behaviours_list) {
                cJSON *localMapObject = expected_ue_behaviours_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(expected_ue_behaviours_listList, localMapKeyPair);
            }
        }
    }

    expected_ue_behaviour_data = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "expectedUeBehaviourData");
    if (expected_ue_behaviour_data) {
        cJSON *expected_ue_behaviour_data_local_map = NULL;
        if (!cJSON_IsObject(expected_ue_behaviour_data) && !cJSON_IsNull(expected_ue_behaviour_data)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [expected_ue_behaviour_data]");
            goto end;
        }
        if (cJSON_IsObject(expected_ue_behaviour_data)) {
            expected_ue_behaviour_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(expected_ue_behaviour_data_local_map, expected_ue_behaviour_data) {
                cJSON *localMapObject = expected_ue_behaviour_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_expected_ue_behaviour_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(expected_ue_behaviour_dataList, localMapKeyPair);
            }
        }
    }

    app_specific_expected_ue_behaviour_data = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "appSpecificExpectedUeBehaviourData");
    if (app_specific_expected_ue_behaviour_data) {
        cJSON *app_specific_expected_ue_behaviour_data_local_map = NULL;
        if (!cJSON_IsObject(app_specific_expected_ue_behaviour_data) && !cJSON_IsNull(app_specific_expected_ue_behaviour_data)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [app_specific_expected_ue_behaviour_data]");
            goto end;
        }
        if (cJSON_IsObject(app_specific_expected_ue_behaviour_data)) {
            app_specific_expected_ue_behaviour_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(app_specific_expected_ue_behaviour_data_local_map, app_specific_expected_ue_behaviour_data) {
                cJSON *localMapObject = app_specific_expected_ue_behaviour_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_app_specific_expected_ue_behaviour_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(app_specific_expected_ue_behaviour_dataList, localMapKeyPair);
            }
        }
    }

    suggested_packet_num_dl_list = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "suggestedPacketNumDlList");
    if (suggested_packet_num_dl_list) {
        cJSON *suggested_packet_num_dl_list_local_map = NULL;
        if (!cJSON_IsObject(suggested_packet_num_dl_list) && !cJSON_IsNull(suggested_packet_num_dl_list)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [suggested_packet_num_dl_list]");
            goto end;
        }
        if (cJSON_IsObject(suggested_packet_num_dl_list)) {
            suggested_packet_num_dl_listList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(suggested_packet_num_dl_list_local_map, suggested_packet_num_dl_list) {
                cJSON *localMapObject = suggested_packet_num_dl_list_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_suggested_packet_num_dl_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(suggested_packet_num_dl_listList, localMapKeyPair);
            }
        }
    }

    _3gpp_charging_characteristics = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "3gppChargingCharacteristics");
    if (_3gpp_charging_characteristics) {
    if (!cJSON_IsString(_3gpp_charging_characteristics) && !cJSON_IsNull(_3gpp_charging_characteristics)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [_3gpp_charging_characteristics]");
        goto end;
    }
    }

    chf_group_id = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "chfGroupId");
    if (chf_group_id) {
    if (!cJSON_IsString(chf_group_id) && !cJSON_IsNull(chf_group_id)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [chf_group_id]");
        goto end;
    }
    }

    nsac_mode = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "nsacMode");
    if (nsac_mode) {
    if (!cJSON_IsString(nsac_mode)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [nsac_mode]");
        goto end;
    }
    nsac_modeVariable = OpenAPI_nsac_admission_mode_FromString(nsac_mode->valuestring);
    }

    sess_inact_timer = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "sessInactTimer");
    if (sess_inact_timer) {
    if (!cJSON_IsNumber(sess_inact_timer)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [sess_inact_timer]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    additional_shared_dnn_configurations_ids = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "additionalSharedDnnConfigurationsIds");
    if (additional_shared_dnn_configurations_ids) {
        cJSON *additional_shared_dnn_configurations_ids_local = NULL;
        if (!cJSON_IsArray(additional_shared_dnn_configurations_ids)) {
            ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [additional_shared_dnn_configurations_ids]");
            goto end;
        }

        additional_shared_dnn_configurations_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_shared_dnn_configurations_ids_local, additional_shared_dnn_configurations_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(additional_shared_dnn_configurations_ids_local)) {
                ogs_error("OpenAPI_session_management_subscription_data_parseFromJSON() failed [additional_shared_dnn_configurations_ids]");
                goto end;
            }
            OpenAPI_list_add(additional_shared_dnn_configurations_idsList, ogs_strdup(additional_shared_dnn_configurations_ids_local->valuestring));
        }
    }

    ue_level_meas_config = cJSON_GetObjectItemCaseSensitive(session_management_subscription_dataJSON, "ueLevelMeasConfig");
    if (ue_level_meas_config) {
    ue_level_meas_config_local_nonprim = OpenAPI_ue_level_measurements_configuration_parseFromJSON(ue_level_meas_config);
    if (!ue_level_meas_config_local_nonprim) {
        ogs_error("OpenAPI_ue_level_measurements_configuration_parseFromJSON failed [ue_level_meas_config]");
        goto end;
    }
    }

    session_management_subscription_data_local_var = OpenAPI_session_management_subscription_data_create (
        single_nssai_local_nonprim,
        dnn_configurations ? dnn_configurationsList : NULL,
        internal_group_ids ? internal_group_idsList : NULL,
        shared_vn_group_data_ids ? shared_vn_group_data_idsList : NULL,
        shared_dnn_configurations_id && !cJSON_IsNull(shared_dnn_configurations_id) ? ogs_strdup(shared_dnn_configurations_id->valuestring) : NULL,
        odb_packet_services ? odb_packet_servicesVariable : 0,
        odb_exempted_dnn_data ? odb_exempted_dnn_dataList : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        shared_trace_data_id && !cJSON_IsNull(shared_trace_data_id) ? ogs_strdup(shared_trace_data_id->valuestring) : NULL,
        expected_ue_behaviours_list ? expected_ue_behaviours_listList : NULL,
        expected_ue_behaviour_data ? expected_ue_behaviour_dataList : NULL,
        app_specific_expected_ue_behaviour_data ? app_specific_expected_ue_behaviour_dataList : NULL,
        suggested_packet_num_dl_list ? suggested_packet_num_dl_listList : NULL,
        _3gpp_charging_characteristics && !cJSON_IsNull(_3gpp_charging_characteristics) ? ogs_strdup(_3gpp_charging_characteristics->valuestring) : NULL,
        chf_group_id && !cJSON_IsNull(chf_group_id) ? ogs_strdup(chf_group_id->valuestring) : NULL,
        nsac_mode ? nsac_modeVariable : 0,
        sess_inact_timer ? true : false,
        sess_inact_timer ? sess_inact_timer->valuedouble : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        additional_shared_dnn_configurations_ids ? additional_shared_dnn_configurations_idsList : NULL,
        ue_level_meas_config ? ue_level_meas_config_local_nonprim : NULL
    );

    return session_management_subscription_data_local_var;
end:
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (dnn_configurationsList) {
        OpenAPI_list_for_each(dnn_configurationsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_dnn_configuration_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(dnn_configurationsList);
        dnn_configurationsList = NULL;
    }
    if (internal_group_idsList) {
        OpenAPI_list_for_each(internal_group_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(internal_group_idsList);
        internal_group_idsList = NULL;
    }
    if (shared_vn_group_data_idsList) {
        OpenAPI_list_for_each(shared_vn_group_data_idsList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            ogs_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(shared_vn_group_data_idsList);
        shared_vn_group_data_idsList = NULL;
    }
    if (odb_exempted_dnn_dataList) {
        OpenAPI_list_for_each(odb_exempted_dnn_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_odb_exempted_dnn_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(odb_exempted_dnn_dataList);
        odb_exempted_dnn_dataList = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (expected_ue_behaviours_listList) {
        OpenAPI_list_for_each(expected_ue_behaviours_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviours_listList);
        expected_ue_behaviours_listList = NULL;
    }
    if (expected_ue_behaviour_dataList) {
        OpenAPI_list_for_each(expected_ue_behaviour_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(expected_ue_behaviour_dataList);
        expected_ue_behaviour_dataList = NULL;
    }
    if (app_specific_expected_ue_behaviour_dataList) {
        OpenAPI_list_for_each(app_specific_expected_ue_behaviour_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_app_specific_expected_ue_behaviour_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(app_specific_expected_ue_behaviour_dataList);
        app_specific_expected_ue_behaviour_dataList = NULL;
    }
    if (suggested_packet_num_dl_listList) {
        OpenAPI_list_for_each(suggested_packet_num_dl_listList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_suggested_packet_num_dl_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(suggested_packet_num_dl_listList);
        suggested_packet_num_dl_listList = NULL;
    }
    if (additional_shared_dnn_configurations_idsList) {
        OpenAPI_list_for_each(additional_shared_dnn_configurations_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(additional_shared_dnn_configurations_idsList);
        additional_shared_dnn_configurations_idsList = NULL;
    }
    if (ue_level_meas_config_local_nonprim) {
        OpenAPI_ue_level_measurements_configuration_free(ue_level_meas_config_local_nonprim);
        ue_level_meas_config_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_session_management_subscription_data_t *OpenAPI_session_management_subscription_data_copy(OpenAPI_session_management_subscription_data_t *dst, OpenAPI_session_management_subscription_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_session_management_subscription_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_session_management_subscription_data_convertToJSON() failed");
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

    OpenAPI_session_management_subscription_data_free(dst);
    dst = OpenAPI_session_management_subscription_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

