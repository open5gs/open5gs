
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_data.h"

OpenAPI_pp_data_t *OpenAPI_pp_data_create(
    OpenAPI_communication_characteristics_t *communication_characteristics,
    char *supported_features,
    OpenAPI_expected_ue_behaviour_t *expected_ue_behaviour_parameters,
    OpenAPI_ec_restriction_t *ec_restriction,
    OpenAPI_acs_info_rm_t *acs_info,
    char *stn_sr,
    OpenAPI_lcs_privacy_t *lcs_privacy,
    OpenAPI_sor_info_t *sor_info,
    OpenAPI_model_5_mbs_authorization_info_t *_5mbs_authorization_info
)
{
    OpenAPI_pp_data_t *pp_data_local_var = ogs_malloc(sizeof(OpenAPI_pp_data_t));
    ogs_assert(pp_data_local_var);

    pp_data_local_var->communication_characteristics = communication_characteristics;
    pp_data_local_var->supported_features = supported_features;
    pp_data_local_var->expected_ue_behaviour_parameters = expected_ue_behaviour_parameters;
    pp_data_local_var->ec_restriction = ec_restriction;
    pp_data_local_var->acs_info = acs_info;
    pp_data_local_var->stn_sr = stn_sr;
    pp_data_local_var->lcs_privacy = lcs_privacy;
    pp_data_local_var->sor_info = sor_info;
    pp_data_local_var->_5mbs_authorization_info = _5mbs_authorization_info;

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
    }

    if (pp_data->stn_sr) {
    if (cJSON_AddStringToObject(item, "stnSr", pp_data->stn_sr) == NULL) {
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
    communication_characteristics = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "communicationCharacteristics");
    if (communication_characteristics) {
    communication_characteristics_local_nonprim = OpenAPI_communication_characteristics_parseFromJSON(communication_characteristics);
    if (!communication_characteristics_local_nonprim) {
        ogs_error("OpenAPI_communication_characteristics_parseFromJSON failed [communication_characteristics]");
        goto end;
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
    expected_ue_behaviour_parameters_local_nonprim = OpenAPI_expected_ue_behaviour_parseFromJSON(expected_ue_behaviour_parameters);
    if (!expected_ue_behaviour_parameters_local_nonprim) {
        ogs_error("OpenAPI_expected_ue_behaviour_parseFromJSON failed [expected_ue_behaviour_parameters]");
        goto end;
    }
    }

    ec_restriction = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "ecRestriction");
    if (ec_restriction) {
    ec_restriction_local_nonprim = OpenAPI_ec_restriction_parseFromJSON(ec_restriction);
    if (!ec_restriction_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_parseFromJSON failed [ec_restriction]");
        goto end;
    }
    }

    acs_info = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "acsInfo");
    if (acs_info) {
    acs_info_local_nonprim = OpenAPI_acs_info_rm_parseFromJSON(acs_info);
    if (!acs_info_local_nonprim) {
        ogs_error("OpenAPI_acs_info_rm_parseFromJSON failed [acs_info]");
        goto end;
    }
    }

    stn_sr = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "stnSr");
    if (stn_sr) {
    if (!cJSON_IsString(stn_sr) && !cJSON_IsNull(stn_sr)) {
        ogs_error("OpenAPI_pp_data_parseFromJSON() failed [stn_sr]");
        goto end;
    }
    }

    lcs_privacy = cJSON_GetObjectItemCaseSensitive(pp_dataJSON, "lcsPrivacy");
    if (lcs_privacy) {
    lcs_privacy_local_nonprim = OpenAPI_lcs_privacy_parseFromJSON(lcs_privacy);
    if (!lcs_privacy_local_nonprim) {
        ogs_error("OpenAPI_lcs_privacy_parseFromJSON failed [lcs_privacy]");
        goto end;
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
    _5mbs_authorization_info_local_nonprim = OpenAPI_model_5_mbs_authorization_info_parseFromJSON(_5mbs_authorization_info);
    if (!_5mbs_authorization_info_local_nonprim) {
        ogs_error("OpenAPI_model_5_mbs_authorization_info_parseFromJSON failed [_5mbs_authorization_info]");
        goto end;
    }
    }

    pp_data_local_var = OpenAPI_pp_data_create (
        communication_characteristics ? communication_characteristics_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        expected_ue_behaviour_parameters ? expected_ue_behaviour_parameters_local_nonprim : NULL,
        ec_restriction ? ec_restriction_local_nonprim : NULL,
        acs_info ? acs_info_local_nonprim : NULL,
        stn_sr && !cJSON_IsNull(stn_sr) ? ogs_strdup(stn_sr->valuestring) : NULL,
        lcs_privacy ? lcs_privacy_local_nonprim : NULL,
        sor_info ? sor_info_local_nonprim : NULL,
        _5mbs_authorization_info ? _5mbs_authorization_info_local_nonprim : NULL
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

