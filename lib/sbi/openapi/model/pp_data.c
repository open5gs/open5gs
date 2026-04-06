
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_data.h"

OpenAPI_pp_data_t *OpenAPI_pp_data_create(
    bool is_communication_characteristics_null,
    OpenAPI_communication_characteristics_t *communication_characteristics,
    char *supported_features,
    bool is_expected_ue_behaviour_parameters_null,
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_parameters,
    bool is_expected_ue_behaviour_extension_null,
    OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension,
    bool is_ec_restriction_null,
    OpenAPI_ec_restriction_t *ec_restriction,
    bool is_acs_info_null,
    OpenAPI_acs_info_rm_t *acs_info,
    bool is_stn_sr_null,
    char *stn_sr,
    bool is_lcs_privacy_null,
    OpenAPI_lcs_privacy_t *lcs_privacy,
    OpenAPI_sor_info_t *sor_info,
    bool is__5mbs_authorization_info_null,
    OpenAPI_model_5_mbs_authorization_info_t *_5mbs_authorization_info,
    bool is_dnn_snssai_specific_group_null,
    OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group,
    OpenAPI_mbs_assistance_info_t *mbs_assistance_info,
    bool is_app_specific_expected_ue_behaviour_null,
    OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour,
    OpenAPI_list_t *slice_usage_control_infos,
    bool is_ranging_sl_privacy_null,
    OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy,
    OpenAPI_cag_provision_information_t *cag_provision_info,
    OpenAPI_list_t *static_ue_ip_address_params
)
{
    OpenAPI_pp_data_t *pp_data_local_var = ogs_malloc(sizeof(OpenAPI_pp_data_t));
    ogs_assert(pp_data_local_var);

    pp_data_local_var->is_communication_characteristics_null = is_communication_characteristics_null;
    pp_data_local_var->communication_characteristics = communication_characteristics;
    pp_data_local_var->supported_features = supported_features;
    pp_data_local_var->is_expected_ue_behaviour_parameters_null = is_expected_ue_behaviour_parameters_null;
    pp_data_local_var->expected_ue_behaviour_parameters = expected_ue_behaviour_parameters;
    pp_data_local_var->is_expected_ue_behaviour_extension_null = is_expected_ue_behaviour_extension_null;
    pp_data_local_var->expected_ue_behaviour_extension = expected_ue_behaviour_extension;
    pp_data_local_var->is_ec_restriction_null = is_ec_restriction_null;
    pp_data_local_var->ec_restriction = ec_restriction;
    pp_data_local_var->is_acs_info_null = is_acs_info_null;
    pp_data_local_var->acs_info = acs_info;
    pp_data_local_var->is_stn_sr_null = is_stn_sr_null;
    pp_data_local_var->stn_sr = stn_sr;
    pp_data_local_var->is_lcs_privacy_null = is_lcs_privacy_null;
    pp_data_local_var->lcs_privacy = lcs_privacy;
    pp_data_local_var->sor_info = sor_info;
    pp_data_local_var->is__5mbs_authorization_info_null = is__5mbs_authorization_info_null;
    pp_data_local_var->_5mbs_authorization_info = _5mbs_authorization_info;
    pp_data_local_var->is_dnn_snssai_specific_group_null = is_dnn_snssai_specific_group_null;
    pp_data_local_var->dnn_snssai_specific_group = dnn_snssai_specific_group;
    pp_data_local_var->mbs_assistance_info = mbs_assistance_info;
    pp_data_local_var->is_app_specific_expected_ue_behaviour_null = is_app_specific_expected_ue_behaviour_null;
    pp_data_local_var->app_specific_expected_ue_behaviour = app_specific_expected_ue_behaviour;
    pp_data_local_var->slice_usage_control_infos = slice_usage_control_infos;
    pp_data_local_var->is_ranging_sl_privacy_null = is_ranging_sl_privacy_null;
    pp_data_local_var->ranging_sl_privacy = ranging_sl_privacy;
    pp_data_local_var->cag_provision_info = cag_provision_info;
    pp_data_local_var->static_ue_ip_address_params = static_ue_ip_address_params;

    return pp_data_local_var;
}

void OpenAPI_pp_data_free(OpenAPI_pp_data_t *pp_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_data) {
        return;
    }
    if (pp_data->communication_characteristics) {
        OpenAPI_communication_characteristics_free(pp_data->communication_characteristics);
        pp_data->communication_characteristics = NULL;
    }
    if (pp_data->supported_features) {
        ogs_free(pp_data->supported_features);
        pp_data->supported_features = NULL;
    }
    if (pp_data->expected_ue_behaviour_parameters) {
        OpenAPI_expected_ue_behaviour_free(pp_data->expected_ue_behaviour_parameters);
        pp_data->expected_ue_behaviour_parameters = NULL;
    }
    if (pp_data->expected_ue_behaviour_extension) {
        OpenAPI_expected_ue_behaviour_extension_free(pp_data->expected_ue_behaviour_extension);
        pp_data->expected_ue_behaviour_extension = NULL;
    }
    if (pp_data->ec_restriction) {
        OpenAPI_ec_restriction_free(pp_data->ec_restriction);
        pp_data->ec_restriction = NULL;
    }
    if (pp_data->acs_info) {
        OpenAPI_acs_info_rm_free(pp_data->acs_info);
        pp_data->acs_info = NULL;
    }
    if (pp_data->stn_sr) {
        ogs_free(pp_data->stn_sr);
        pp_data->stn_sr = NULL;
    }
    if (pp_data->lcs_privacy) {
        OpenAPI_lcs_privacy_free(pp_data->lcs_privacy);
        pp_data->lcs_privacy = NULL;
    }
    if (pp_data->sor_info) {
        OpenAPI_sor_info_free(pp_data->sor_info);
        pp_data->sor_info = NULL;
    }
    if (pp_data->_5mbs_authorization_info) {
        OpenAPI_model_5_mbs_authorization_info_free(pp_data->_5mbs_authorization_info);
        pp_data->_5mbs_authorization_info = NULL;
    }
    if (pp_data->dnn_snssai_specific_group) {
        OpenAPI_dnn_snssai_specific_group_free(pp_data->dnn_snssai_specific_group);
        pp_data->dnn_snssai_specific_group = NULL;
    }
    if (pp_data->mbs_assistance_info) {
        OpenAPI_mbs_assistance_info_free(pp_data->mbs_assistance_info);
        pp_data->mbs_assistance_info = NULL;
    }
    if (pp_data->app_specific_expected_ue_behaviour) {
        OpenAPI_app_specific_expected_ue_behaviour_free(pp_data->app_specific_expected_ue_behaviour);
        pp_data->app_specific_expected_ue_behaviour = NULL;
    }
    if (pp_data->slice_usage_control_infos) {
        OpenAPI_list_for_each(pp_data->slice_usage_control_infos, node) {
            OpenAPI_slice_usage_control_info_free(node->data);
        }
        OpenAPI_list_free(pp_data->slice_usage_control_infos);
        pp_data->slice_usage_control_infos = NULL;
    }
    if (pp_data->ranging_sl_privacy) {
        OpenAPI_ranging_sl_privacy_free(pp_data->ranging_sl_privacy);
        pp_data->ranging_sl_privacy = NULL;
    }
    if (pp_data->cag_provision_info) {
        OpenAPI_cag_provision_information_free(pp_data->cag_provision_info);
        pp_data->cag_provision_info = NULL;
    }
    if (pp_data->static_ue_ip_address_params) {
        OpenAPI_list_for_each(pp_data->static_ue_ip_address_params, node) {
            OpenAPI_static_ue_ip_address_params_free(node->data);
        }
        OpenAPI_list_free(pp_data->static_ue_ip_address_params);
        pp_data->static_ue_ip_address_params = NULL;
    }
    ogs_free(pp_data);
}

cJSON *OpenAPI_pp_data_convertToJSON(OpenAPI_pp_data_t *pp_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_data == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [PpData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp_data->communication_characteristics) {
    cJSON *communication_characteristics_local_JSON = OpenAPI_communication_characteristics_convertToJSON(pp_data->communication_characteristics);
    if (communication_characteristics_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [communication_characteristics]");
        goto end;
    }
    cJSON_AddItemToObject(item, "communicationCharacteristics", communication_characteristics_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [communication_characteristics]");
        goto end;
    }
    } else if (pp_data->is_communication_characteristics_null) {
        if (cJSON_AddNullToObject(item, "communicationCharacteristics") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [communication_characteristics]");
            goto end;
        }
    }

    if (pp_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp_data->supported_features) == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (pp_data->expected_ue_behaviour_parameters) {
    cJSON *expected_ue_behaviour_parameters_local_JSON = OpenAPI_expected_ue_behaviour_convertToJSON(pp_data->expected_ue_behaviour_parameters);
    if (expected_ue_behaviour_parameters_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_parameters]");
        goto end;
    }
    cJSON_AddItemToObject(item, "expectedUeBehaviourParameters", expected_ue_behaviour_parameters_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_parameters]");
        goto end;
    }
    } else if (pp_data->is_expected_ue_behaviour_parameters_null) {
        if (cJSON_AddNullToObject(item, "expectedUeBehaviourParameters") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_parameters]");
            goto end;
        }
    }

    if (pp_data->expected_ue_behaviour_extension) {
    cJSON *expected_ue_behaviour_extension_local_JSON = OpenAPI_expected_ue_behaviour_extension_convertToJSON(pp_data->expected_ue_behaviour_extension);
    if (expected_ue_behaviour_extension_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_extension]");
        goto end;
    }
    cJSON_AddItemToObject(item, "expectedUeBehaviourExtension", expected_ue_behaviour_extension_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_extension]");
        goto end;
    }
    } else if (pp_data->is_expected_ue_behaviour_extension_null) {
        if (cJSON_AddNullToObject(item, "expectedUeBehaviourExtension") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [expected_ue_behaviour_extension]");
            goto end;
        }
    }

    if (pp_data->ec_restriction) {
    cJSON *ec_restriction_local_JSON = OpenAPI_ec_restriction_convertToJSON(pp_data->ec_restriction);
    if (ec_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [ec_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecRestriction", ec_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [ec_restriction]");
        goto end;
    }
    } else if (pp_data->is_ec_restriction_null) {
        if (cJSON_AddNullToObject(item, "ecRestriction") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [ec_restriction]");
            goto end;
        }
    }

    if (pp_data->acs_info) {
    cJSON *acs_info_local_JSON = OpenAPI_acs_info_rm_convertToJSON(pp_data->acs_info);
    if (acs_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [acs_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "acsInfo", acs_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [acs_info]");
        goto end;
    }
    } else if (pp_data->is_acs_info_null) {
        if (cJSON_AddNullToObject(item, "acsInfo") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [acs_info]");
            goto end;
        }
    }

    if (pp_data->stn_sr) {
    if (cJSON_AddStringToObject(item, "stnSr", pp_data->stn_sr) == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [stn_sr]");
        goto end;
    }
    } else if (pp_data->is_stn_sr_null) {
        if (cJSON_AddNullToObject(item, "stnSr") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [stn_sr]");
            goto end;
        }
    }

    if (pp_data->lcs_privacy) {
    cJSON *lcs_privacy_local_JSON = OpenAPI_lcs_privacy_convertToJSON(pp_data->lcs_privacy);
    if (lcs_privacy_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [lcs_privacy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsPrivacy", lcs_privacy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [lcs_privacy]");
        goto end;
    }
    } else if (pp_data->is_lcs_privacy_null) {
        if (cJSON_AddNullToObject(item, "lcsPrivacy") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [lcs_privacy]");
            goto end;
        }
    }

    if (pp_data->sor_info) {
    cJSON *sor_info_local_JSON = OpenAPI_sor_info_convertToJSON(pp_data->sor_info);
    if (sor_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [sor_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sorInfo", sor_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [sor_info]");
        goto end;
    }
    }

    if (pp_data->_5mbs_authorization_info) {
    cJSON *_5mbs_authorization_info_local_JSON = OpenAPI_model_5_mbs_authorization_info_convertToJSON(pp_data->_5mbs_authorization_info);
    if (_5mbs_authorization_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [_5mbs_authorization_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5mbsAuthorizationInfo", _5mbs_authorization_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [_5mbs_authorization_info]");
        goto end;
    }
    } else if (pp_data->is__5mbs_authorization_info_null) {
        if (cJSON_AddNullToObject(item, "5mbsAuthorizationInfo") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [_5mbs_authorization_info]");
            goto end;
        }
    }

    if (pp_data->dnn_snssai_specific_group) {
    cJSON *dnn_snssai_specific_group_local_JSON = OpenAPI_dnn_snssai_specific_group_convertToJSON(pp_data->dnn_snssai_specific_group);
    if (dnn_snssai_specific_group_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [dnn_snssai_specific_group]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnnSnssaiSpecificGroup", dnn_snssai_specific_group_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [dnn_snssai_specific_group]");
        goto end;
    }
    } else if (pp_data->is_dnn_snssai_specific_group_null) {
        if (cJSON_AddNullToObject(item, "dnnSnssaiSpecificGroup") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [dnn_snssai_specific_group]");
            goto end;
        }
    }

    if (pp_data->mbs_assistance_info) {
    cJSON *mbs_assistance_info_local_JSON = OpenAPI_mbs_assistance_info_convertToJSON(pp_data->mbs_assistance_info);
    if (mbs_assistance_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [mbs_assistance_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mbsAssistanceInfo", mbs_assistance_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [mbs_assistance_info]");
        goto end;
    }
    }

    if (pp_data->app_specific_expected_ue_behaviour) {
    cJSON *app_specific_expected_ue_behaviour_local_JSON = OpenAPI_app_specific_expected_ue_behaviour_convertToJSON(pp_data->app_specific_expected_ue_behaviour);
    if (app_specific_expected_ue_behaviour_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [app_specific_expected_ue_behaviour]");
        goto end;
    }
    cJSON_AddItemToObject(item, "appSpecificExpectedUeBehaviour", app_specific_expected_ue_behaviour_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [app_specific_expected_ue_behaviour]");
        goto end;
    }
    } else if (pp_data->is_app_specific_expected_ue_behaviour_null) {
        if (cJSON_AddNullToObject(item, "appSpecificExpectedUeBehaviour") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [app_specific_expected_ue_behaviour]");
            goto end;
        }
    }

    if (pp_data->slice_usage_control_infos) {
    cJSON *slice_usage_control_infosList = cJSON_AddArrayToObject(item, "sliceUsageControlInfos");
    if (slice_usage_control_infosList == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [slice_usage_control_infos]");
        goto end;
    }
    OpenAPI_list_for_each(pp_data->slice_usage_control_infos, node) {
        cJSON *itemLocal = OpenAPI_slice_usage_control_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [slice_usage_control_infos]");
            goto end;
        }
        cJSON_AddItemToArray(slice_usage_control_infosList, itemLocal);
    }
    }

    if (pp_data->ranging_sl_privacy) {
    cJSON *ranging_sl_privacy_local_JSON = OpenAPI_ranging_sl_privacy_convertToJSON(pp_data->ranging_sl_privacy);
    if (ranging_sl_privacy_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [ranging_sl_privacy]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSlPrivacy", ranging_sl_privacy_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [ranging_sl_privacy]");
        goto end;
    }
    } else if (pp_data->is_ranging_sl_privacy_null) {
        if (cJSON_AddNullToObject(item, "rangingSlPrivacy") == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [ranging_sl_privacy]");
            goto end;
        }
    }

    if (pp_data->cag_provision_info) {
    cJSON *cag_provision_info_local_JSON = OpenAPI_cag_provision_information_convertToJSON(pp_data->cag_provision_info);
    if (cag_provision_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [cag_provision_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cagProvisionInfo", cag_provision_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [cag_provision_info]");
        goto end;
    }
    }

    if (pp_data->static_ue_ip_address_params) {
    cJSON *static_ue_ip_address_paramsList = cJSON_AddArrayToObject(item, "staticUeIpAddressParams");
    if (static_ue_ip_address_paramsList == NULL) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed [static_ue_ip_address_params]");
        goto end;
    }
    OpenAPI_list_for_each(pp_data->static_ue_ip_address_params, node) {
        cJSON *itemLocal = OpenAPI_static_ue_ip_address_params_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pp_data_convertToJSON() failed [static_ue_ip_address_params]");
            goto end;
        }
        cJSON_AddItemToArray(static_ue_ip_address_paramsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_pp_data_t *OpenAPI_pp_data_parseFromJSON(cJSON *pp_dataJSON)
{
    OpenAPI_pp_data_t *pp_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *communication_characteristics = NULL;
    OpenAPI_communication_characteristics_t *communication_characteristics_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *expected_ue_behaviour_parameters = NULL;
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_parameters_local_nonprim = NULL;
    cJSON *expected_ue_behaviour_extension = NULL;
    OpenAPI_expected_ue_behaviour_extension_t *expected_ue_behaviour_extension_local_nonprim = NULL;
    cJSON *ec_restriction = NULL;
    OpenAPI_ec_restriction_t *ec_restriction_local_nonprim = NULL;
    cJSON *acs_info = NULL;
    OpenAPI_acs_info_rm_t *acs_info_local_nonprim = NULL;
    cJSON *stn_sr = NULL;
    cJSON *lcs_privacy = NULL;
    OpenAPI_lcs_privacy_t *lcs_privacy_local_nonprim = NULL;
    cJSON *sor_info = NULL;
    OpenAPI_sor_info_t *sor_info_local_nonprim = NULL;
    cJSON *_5mbs_authorization_info = NULL;
    OpenAPI_model_5_mbs_authorization_info_t *_5mbs_authorization_info_local_nonprim = NULL;
    cJSON *dnn_snssai_specific_group = NULL;
    OpenAPI_dnn_snssai_specific_group_t *dnn_snssai_specific_group_local_nonprim = NULL;
    cJSON *mbs_assistance_info = NULL;
    OpenAPI_mbs_assistance_info_t *mbs_assistance_info_local_nonprim = NULL;
    cJSON *app_specific_expected_ue_behaviour = NULL;
    OpenAPI_app_specific_expected_ue_behaviour_t *app_specific_expected_ue_behaviour_local_nonprim = NULL;
    cJSON *slice_usage_control_infos = NULL;
    OpenAPI_list_t *slice_usage_control_infosList = NULL;
    cJSON *ranging_sl_privacy = NULL;
    OpenAPI_ranging_sl_privacy_t *ranging_sl_privacy_local_nonprim = NULL;
    cJSON *cag_provision_info = NULL;
    OpenAPI_cag_provision_information_t *cag_provision_info_local_nonprim = NULL;
    cJSON *static_ue_ip_address_params = NULL;
    OpenAPI_list_t *static_ue_ip_address_paramsList = NULL;
    communication_characteristics = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "communicationCharacteristics");
    if (communication_characteristics) {
    if (!cJSON_IsNull(communication_characteristics)) {
    communication_characteristics_local_nonprim = OpenAPI_communication_characteristics_parseFromJSON(communication_characteristics);
    if (!communication_characteristics_local_nonprim) {
        ogs_error("OpenAPI_communication_characteristics_parseFromJSON failed [communication_characteristics]");
        goto end;
    }
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pp_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    expected_ue_behaviour_parameters = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "expectedUeBehaviourParameters");
    if (expected_ue_behaviour_parameters) {
    if (!cJSON_IsNull(expected_ue_behaviour_parameters)) {
    expected_ue_behaviour_parameters_local_nonprim = OpenAPI_expected_ue_behaviour_parseFromJSON(expected_ue_behaviour_parameters);
    if (!expected_ue_behaviour_parameters_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON failed [expected_ue_behaviour_parameters]");
        goto end;
    }
    }
    }

    expected_ue_behaviour_extension = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "expectedUeBehaviourExtension");
    if (expected_ue_behaviour_extension) {
    if (!cJSON_IsNull(expected_ue_behaviour_extension)) {
    expected_ue_behaviour_extension_local_nonprim = OpenAPI_expected_ue_behaviour_extension_parseFromJSON(expected_ue_behaviour_extension);
    if (!expected_ue_behaviour_extension_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behaviour_extension_parseFromJSON failed [expected_ue_behaviour_extension]");
        goto end;
    }
    }
    }

    ec_restriction = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "ecRestriction");
    if (ec_restriction) {
    if (!cJSON_IsNull(ec_restriction)) {
    ec_restriction_local_nonprim = OpenAPI_ec_restriction_parseFromJSON(ec_restriction);
    if (!ec_restriction_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON failed [ec_restriction]");
        goto end;
    }
    }
    }

    acs_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "acsInfo");
    if (acs_info) {
    if (!cJSON_IsNull(acs_info)) {
    acs_info_local_nonprim = OpenAPI_acs_info_rm_parseFromJSON(acs_info);
    if (!acs_info_local_nonprim) {
        ogs_error("OpenAPI_acs_info_rm_parseFromJSON failed [acs_info]");
        goto end;
    }
    }
    }

    stn_sr = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "stnSr");
    if (stn_sr) {
    if (!cJSON_IsNull(stn_sr)) {
    if (!cJSON_IsString(stn_sr) && !cJSON_IsNull(stn_sr)) {
        ogs_error("OpenAPI_pp_data_parseFromJSON() failed [stn_sr]");
        goto end;
    }
    }
    }

    lcs_privacy = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "lcsPrivacy");
    if (lcs_privacy) {
    if (!cJSON_IsNull(lcs_privacy)) {
    lcs_privacy_local_nonprim = OpenAPI_lcs_privacy_parseFromJSON(lcs_privacy);
    if (!lcs_privacy_local_nonprim) {
        ogs_error("OpenAPI_lcs_privacy_parseFromJSON failed [lcs_privacy]");
        goto end;
    }
    }
    }

    sor_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "sorInfo");
    if (sor_info) {
    sor_info_local_nonprim = OpenAPI_sor_info_parseFromJSON(sor_info);
    if (!sor_info_local_nonprim) {
        ogs_error("OpenAPI_sor_info_parseFromJSON failed [sor_info]");
        goto end;
    }
    }

    _5mbs_authorization_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "5mbsAuthorizationInfo");
    if (_5mbs_authorization_info) {
    if (!cJSON_IsNull(_5mbs_authorization_info)) {
    _5mbs_authorization_info_local_nonprim = OpenAPI_model_5_mbs_authorization_info_parseFromJSON(_5mbs_authorization_info);
    if (!_5mbs_authorization_info_local_nonprim) {
        ogs_error("OpenAPI_model_5_mbs_authorization_info_parseFromJSON failed [_5mbs_authorization_info]");
        goto end;
    }
    }
    }

    dnn_snssai_specific_group = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "dnnSnssaiSpecificGroup");
    if (dnn_snssai_specific_group) {
    if (!cJSON_IsNull(dnn_snssai_specific_group)) {
    dnn_snssai_specific_group_local_nonprim = OpenAPI_dnn_snssai_specific_group_parseFromJSON(dnn_snssai_specific_group);
    if (!dnn_snssai_specific_group_local_nonprim) {
        ogs_error("OpenAPI_dnn_snssai_specific_group_parseFromJSON failed [dnn_snssai_specific_group]");
        goto end;
    }
    }
    }

    mbs_assistance_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "mbsAssistanceInfo");
    if (mbs_assistance_info) {
    mbs_assistance_info_local_nonprim = OpenAPI_mbs_assistance_info_parseFromJSON(mbs_assistance_info);
    if (!mbs_assistance_info_local_nonprim) {
        ogs_error("OpenAPI_mbs_assistance_info_parseFromJSON failed [mbs_assistance_info]");
        goto end;
    }
    }

    app_specific_expected_ue_behaviour = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "appSpecificExpectedUeBehaviour");
    if (app_specific_expected_ue_behaviour) {
    if (!cJSON_IsNull(app_specific_expected_ue_behaviour)) {
    app_specific_expected_ue_behaviour_local_nonprim = OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON(app_specific_expected_ue_behaviour);
    if (!app_specific_expected_ue_behaviour_local_nonprim) {
        ogs_error("OpenAPI_app_specific_expected_ue_behaviour_parseFromJSON failed [app_specific_expected_ue_behaviour]");
        goto end;
    }
    }
    }

    slice_usage_control_infos = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "sliceUsageControlInfos");
    if (slice_usage_control_infos) {
        cJSON *slice_usage_control_infos_local = NULL;
        if (!cJSON_IsArray(slice_usage_control_infos)) {
            ogs_error("OpenAPI_pp_data_parseFromJSON() failed [slice_usage_control_infos]");
            goto end;
        }

        slice_usage_control_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(slice_usage_control_infos_local, slice_usage_control_infos) {
            if (!cJSON_IsObject(slice_usage_control_infos_local)) {
                ogs_error("OpenAPI_pp_data_parseFromJSON() failed [slice_usage_control_infos]");
                goto end;
            }
            OpenAPI_slice_usage_control_info_t *slice_usage_control_infosItem = OpenAPI_slice_usage_control_info_parseFromJSON(slice_usage_control_infos_local);
            if (!slice_usage_control_infosItem) {
                ogs_error("No slice_usage_control_infosItem");
                goto end;
            }
            OpenAPI_list_add(slice_usage_control_infosList, slice_usage_control_infosItem);
        }
    }

    ranging_sl_privacy = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "rangingSlPrivacy");
    if (ranging_sl_privacy) {
    if (!cJSON_IsNull(ranging_sl_privacy)) {
    ranging_sl_privacy_local_nonprim = OpenAPI_ranging_sl_privacy_parseFromJSON(ranging_sl_privacy);
    if (!ranging_sl_privacy_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_privacy_parseFromJSON failed [ranging_sl_privacy]");
        goto end;
    }
    }
    }

    cag_provision_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "cagProvisionInfo");
    if (cag_provision_info) {
    cag_provision_info_local_nonprim = OpenAPI_cag_provision_information_parseFromJSON(cag_provision_info);
    if (!cag_provision_info_local_nonprim) {
        ogs_error("OpenAPI_cag_provision_information_parseFromJSON failed [cag_provision_info]");
        goto end;
    }
    }

    static_ue_ip_address_params = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "staticUeIpAddressParams");
    if (static_ue_ip_address_params) {
        cJSON *static_ue_ip_address_params_local = NULL;
        if (!cJSON_IsArray(static_ue_ip_address_params)) {
            ogs_error("OpenAPI_pp_data_parseFromJSON() failed [static_ue_ip_address_params]");
            goto end;
        }

        static_ue_ip_address_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(static_ue_ip_address_params_local, static_ue_ip_address_params) {
            if (!cJSON_IsObject(static_ue_ip_address_params_local)) {
                ogs_error("OpenAPI_pp_data_parseFromJSON() failed [static_ue_ip_address_params]");
                goto end;
            }
            OpenAPI_static_ue_ip_address_params_t *static_ue_ip_address_paramsItem = OpenAPI_static_ue_ip_address_params_parseFromJSON(static_ue_ip_address_params_local);
            if (!static_ue_ip_address_paramsItem) {
                ogs_error("No static_ue_ip_address_paramsItem");
                goto end;
            }
            OpenAPI_list_add(static_ue_ip_address_paramsList, static_ue_ip_address_paramsItem);
        }
    }

    pp_data_local_var = OpenAPI_pp_data_create (
        communication_characteristics && cJSON_IsNull(communication_characteristics) ? true : false,
        communication_characteristics ? communication_characteristics_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        expected_ue_behaviour_parameters && cJSON_IsNull(expected_ue_behaviour_parameters) ? true : false,
        expected_ue_behaviour_parameters ? expected_ue_behaviour_parameters_local_nonprim : NULL,
        expected_ue_behaviour_extension && cJSON_IsNull(expected_ue_behaviour_extension) ? true : false,
        expected_ue_behaviour_extension ? expected_ue_behaviour_extension_local_nonprim : NULL,
        ec_restriction && cJSON_IsNull(ec_restriction) ? true : false,
        ec_restriction ? ec_restriction_local_nonprim : NULL,
        acs_info && cJSON_IsNull(acs_info) ? true : false,
        acs_info ? acs_info_local_nonprim : NULL,
        stn_sr && cJSON_IsNull(stn_sr) ? true : false,
        stn_sr && !cJSON_IsNull(stn_sr) ? ogs_strdup(stn_sr->valuestring) : NULL,
        lcs_privacy && cJSON_IsNull(lcs_privacy) ? true : false,
        lcs_privacy ? lcs_privacy_local_nonprim : NULL,
        sor_info ? sor_info_local_nonprim : NULL,
        _5mbs_authorization_info && cJSON_IsNull(_5mbs_authorization_info) ? true : false,
        _5mbs_authorization_info ? _5mbs_authorization_info_local_nonprim : NULL,
        dnn_snssai_specific_group && cJSON_IsNull(dnn_snssai_specific_group) ? true : false,
        dnn_snssai_specific_group ? dnn_snssai_specific_group_local_nonprim : NULL,
        mbs_assistance_info ? mbs_assistance_info_local_nonprim : NULL,
        app_specific_expected_ue_behaviour && cJSON_IsNull(app_specific_expected_ue_behaviour) ? true : false,
        app_specific_expected_ue_behaviour ? app_specific_expected_ue_behaviour_local_nonprim : NULL,
        slice_usage_control_infos ? slice_usage_control_infosList : NULL,
        ranging_sl_privacy && cJSON_IsNull(ranging_sl_privacy) ? true : false,
        ranging_sl_privacy ? ranging_sl_privacy_local_nonprim : NULL,
        cag_provision_info ? cag_provision_info_local_nonprim : NULL,
        static_ue_ip_address_params ? static_ue_ip_address_paramsList : NULL
    );

    return pp_data_local_var;
end:
    if (communication_characteristics_local_nonprim) {
        OpenAPI_communication_characteristics_free(communication_characteristics_local_nonprim);
        communication_characteristics_local_nonprim = NULL;
    }
    if (expected_ue_behaviour_parameters_local_nonprim) {
        OpenAPI_expected_ue_behaviour_free(expected_ue_behaviour_parameters_local_nonprim);
        expected_ue_behaviour_parameters_local_nonprim = NULL;
    }
    if (expected_ue_behaviour_extension_local_nonprim) {
        OpenAPI_expected_ue_behaviour_extension_free(expected_ue_behaviour_extension_local_nonprim);
        expected_ue_behaviour_extension_local_nonprim = NULL;
    }
    if (ec_restriction_local_nonprim) {
        OpenAPI_ec_restriction_free(ec_restriction_local_nonprim);
        ec_restriction_local_nonprim = NULL;
    }
    if (acs_info_local_nonprim) {
        OpenAPI_acs_info_rm_free(acs_info_local_nonprim);
        acs_info_local_nonprim = NULL;
    }
    if (lcs_privacy_local_nonprim) {
        OpenAPI_lcs_privacy_free(lcs_privacy_local_nonprim);
        lcs_privacy_local_nonprim = NULL;
    }
    if (sor_info_local_nonprim) {
        OpenAPI_sor_info_free(sor_info_local_nonprim);
        sor_info_local_nonprim = NULL;
    }
    if (_5mbs_authorization_info_local_nonprim) {
        OpenAPI_model_5_mbs_authorization_info_free(_5mbs_authorization_info_local_nonprim);
        _5mbs_authorization_info_local_nonprim = NULL;
    }
    if (dnn_snssai_specific_group_local_nonprim) {
        OpenAPI_dnn_snssai_specific_group_free(dnn_snssai_specific_group_local_nonprim);
        dnn_snssai_specific_group_local_nonprim = NULL;
    }
    if (mbs_assistance_info_local_nonprim) {
        OpenAPI_mbs_assistance_info_free(mbs_assistance_info_local_nonprim);
        mbs_assistance_info_local_nonprim = NULL;
    }
    if (app_specific_expected_ue_behaviour_local_nonprim) {
        OpenAPI_app_specific_expected_ue_behaviour_free(app_specific_expected_ue_behaviour_local_nonprim);
        app_specific_expected_ue_behaviour_local_nonprim = NULL;
    }
    if (slice_usage_control_infosList) {
        OpenAPI_list_for_each(slice_usage_control_infosList, node) {
            OpenAPI_slice_usage_control_info_free(node->data);
        }
        OpenAPI_list_free(slice_usage_control_infosList);
        slice_usage_control_infosList = NULL;
    }
    if (ranging_sl_privacy_local_nonprim) {
        OpenAPI_ranging_sl_privacy_free(ranging_sl_privacy_local_nonprim);
        ranging_sl_privacy_local_nonprim = NULL;
    }
    if (cag_provision_info_local_nonprim) {
        OpenAPI_cag_provision_information_free(cag_provision_info_local_nonprim);
        cag_provision_info_local_nonprim = NULL;
    }
    if (static_ue_ip_address_paramsList) {
        OpenAPI_list_for_each(static_ue_ip_address_paramsList, node) {
            OpenAPI_static_ue_ip_address_params_free(node->data);
        }
        OpenAPI_list_free(static_ue_ip_address_paramsList);
        static_ue_ip_address_paramsList = NULL;
    }
    return NULL;
}

OpenAPI_pp_data_t *OpenAPI_pp_data_copy(OpenAPI_pp_data_t *dst, OpenAPI_pp_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_data_convertToJSON() failed");
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

    OpenAPI_pp_data_free(dst);
    dst = OpenAPI_pp_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

