
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pp_data_entry.h"

OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_create(
    bool is_communication_characteristics_null,
    OpenAPI_communication_characteristics_af_t *communication_characteristics,
    bool is_reference_id,
    int reference_id,
    char *validity_time,
    char *mtc_provider_information,
    char *supported_features,
    bool is_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_1_t *ecs_addr_config_info,
    OpenAPI_list_t *additional_ecs_addr_config_infos,
    bool is_ec_restriction_null,
    OpenAPI_ec_restriction_1_t *ec_restriction
)
{
    OpenAPI_pp_data_entry_t *pp_data_entry_local_var = ogs_malloc(sizeof(OpenAPI_pp_data_entry_t));
    ogs_assert(pp_data_entry_local_var);

    pp_data_entry_local_var->is_communication_characteristics_null = is_communication_characteristics_null;
    pp_data_entry_local_var->communication_characteristics = communication_characteristics;
    pp_data_entry_local_var->is_reference_id = is_reference_id;
    pp_data_entry_local_var->reference_id = reference_id;
    pp_data_entry_local_var->validity_time = validity_time;
    pp_data_entry_local_var->mtc_provider_information = mtc_provider_information;
    pp_data_entry_local_var->supported_features = supported_features;
    pp_data_entry_local_var->is_ecs_addr_config_info_null = is_ecs_addr_config_info_null;
    pp_data_entry_local_var->ecs_addr_config_info = ecs_addr_config_info;
    pp_data_entry_local_var->additional_ecs_addr_config_infos = additional_ecs_addr_config_infos;
    pp_data_entry_local_var->is_ec_restriction_null = is_ec_restriction_null;
    pp_data_entry_local_var->ec_restriction = ec_restriction;

    return pp_data_entry_local_var;
}

void OpenAPI_pp_data_entry_free(OpenAPI_pp_data_entry_t *pp_data_entry)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pp_data_entry) {
        return;
    }
    if (pp_data_entry->communication_characteristics) {
        OpenAPI_communication_characteristics_af_free(pp_data_entry->communication_characteristics);
        pp_data_entry->communication_characteristics = NULL;
    }
    if (pp_data_entry->validity_time) {
        ogs_free(pp_data_entry->validity_time);
        pp_data_entry->validity_time = NULL;
    }
    if (pp_data_entry->mtc_provider_information) {
        ogs_free(pp_data_entry->mtc_provider_information);
        pp_data_entry->mtc_provider_information = NULL;
    }
    if (pp_data_entry->supported_features) {
        ogs_free(pp_data_entry->supported_features);
        pp_data_entry->supported_features = NULL;
    }
    if (pp_data_entry->ecs_addr_config_info) {
        OpenAPI_ecs_addr_config_info_1_free(pp_data_entry->ecs_addr_config_info);
        pp_data_entry->ecs_addr_config_info = NULL;
    }
    if (pp_data_entry->additional_ecs_addr_config_infos) {
        OpenAPI_list_for_each(pp_data_entry->additional_ecs_addr_config_infos, node) {
            OpenAPI_ecs_addr_config_info_1_free(node->data);
        }
        OpenAPI_list_free(pp_data_entry->additional_ecs_addr_config_infos);
        pp_data_entry->additional_ecs_addr_config_infos = NULL;
    }
    if (pp_data_entry->ec_restriction) {
        OpenAPI_ec_restriction_1_free(pp_data_entry->ec_restriction);
        pp_data_entry->ec_restriction = NULL;
    }
    ogs_free(pp_data_entry);
}

cJSON *OpenAPI_pp_data_entry_convertToJSON(OpenAPI_pp_data_entry_t *pp_data_entry)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pp_data_entry == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [PpDataEntry]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pp_data_entry->communication_characteristics) {
    cJSON *communication_characteristics_local_JSON = OpenAPI_communication_characteristics_af_convertToJSON(pp_data_entry->communication_characteristics);
    if (communication_characteristics_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [communication_characteristics]");
        goto end;
    }
    cJSON_AddItemToObject(item, "communicationCharacteristics", communication_characteristics_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [communication_characteristics]");
        goto end;
    }
    } else if (pp_data_entry->is_communication_characteristics_null) {
        if (cJSON_AddNullToObject(item, "communicationCharacteristics") == NULL) {
            ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [communication_characteristics]");
            goto end;
        }
    }

    if (pp_data_entry->is_reference_id) {
    if (cJSON_AddNumberToObject(item, "referenceId", pp_data_entry->reference_id) == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [reference_id]");
        goto end;
    }
    }

    if (pp_data_entry->validity_time) {
    if (cJSON_AddStringToObject(item, "validityTime", pp_data_entry->validity_time) == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [validity_time]");
        goto end;
    }
    }

    if (pp_data_entry->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", pp_data_entry->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    if (pp_data_entry->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", pp_data_entry->supported_features) == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (pp_data_entry->ecs_addr_config_info) {
    cJSON *ecs_addr_config_info_local_JSON = OpenAPI_ecs_addr_config_info_1_convertToJSON(pp_data_entry->ecs_addr_config_info);
    if (ecs_addr_config_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ecs_addr_config_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecsAddrConfigInfo", ecs_addr_config_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ecs_addr_config_info]");
        goto end;
    }
    } else if (pp_data_entry->is_ecs_addr_config_info_null) {
        if (cJSON_AddNullToObject(item, "ecsAddrConfigInfo") == NULL) {
            ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ecs_addr_config_info]");
            goto end;
        }
    }

    if (pp_data_entry->additional_ecs_addr_config_infos) {
    cJSON *additional_ecs_addr_config_infosList = cJSON_AddArrayToObject(item, "additionalEcsAddrConfigInfos");
    if (additional_ecs_addr_config_infosList == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [additional_ecs_addr_config_infos]");
        goto end;
    }
    OpenAPI_list_for_each(pp_data_entry->additional_ecs_addr_config_infos, node) {
        cJSON *itemLocal = OpenAPI_ecs_addr_config_info_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [additional_ecs_addr_config_infos]");
            goto end;
        }
        cJSON_AddItemToArray(additional_ecs_addr_config_infosList, itemLocal);
    }
    }

    if (pp_data_entry->ec_restriction) {
    cJSON *ec_restriction_local_JSON = OpenAPI_ec_restriction_1_convertToJSON(pp_data_entry->ec_restriction);
    if (ec_restriction_local_JSON == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ec_restriction]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecRestriction", ec_restriction_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ec_restriction]");
        goto end;
    }
    } else if (pp_data_entry->is_ec_restriction_null) {
        if (cJSON_AddNullToObject(item, "ecRestriction") == NULL) {
            ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed [ec_restriction]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_parseFromJSON(cJSON *pp_data_entryJSON)
{
    OpenAPI_pp_data_entry_t *pp_data_entry_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *communication_characteristics = NULL;
    OpenAPI_communication_characteristics_af_t *communication_characteristics_local_nonprim = NULL;
    cJSON *reference_id = NULL;
    cJSON *validity_time = NULL;
    cJSON *mtc_provider_information = NULL;
    cJSON *supported_features = NULL;
    cJSON *ecs_addr_config_info = NULL;
    OpenAPI_ecs_addr_config_info_1_t *ecs_addr_config_info_local_nonprim = NULL;
    cJSON *additional_ecs_addr_config_infos = NULL;
    OpenAPI_list_t *additional_ecs_addr_config_infosList = NULL;
    cJSON *ec_restriction = NULL;
    OpenAPI_ec_restriction_1_t *ec_restriction_local_nonprim = NULL;
    communication_characteristics = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "communicationCharacteristics");
    if (communication_characteristics) {
    if (!cJSON_IsNull(communication_characteristics)) {
    communication_characteristics_local_nonprim = OpenAPI_communication_characteristics_af_parseFromJSON(communication_characteristics);
    if (!communication_characteristics_local_nonprim) {
        ogs_error("OpenAPI_communication_characteristics_af_parseFromJSON failed [communication_characteristics]");
        goto end;
    }
    }
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "referenceId");
    if (reference_id) {
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [reference_id]");
        goto end;
    }
    }

    validity_time = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "validityTime");
    if (validity_time) {
    if (!cJSON_IsString(validity_time) && !cJSON_IsNull(validity_time)) {
        ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [validity_time]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ecs_addr_config_info = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "ecsAddrConfigInfo");
    if (ecs_addr_config_info) {
    if (!cJSON_IsNull(ecs_addr_config_info)) {
    ecs_addr_config_info_local_nonprim = OpenAPI_ecs_addr_config_info_1_parseFromJSON(ecs_addr_config_info);
    if (!ecs_addr_config_info_local_nonprim) {
        ogs_error("OpenAPI_ecs_addr_config_info_1_parseFromJSON failed [ecs_addr_config_info]");
        goto end;
    }
    }
    }

    additional_ecs_addr_config_infos = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "additionalEcsAddrConfigInfos");
    if (additional_ecs_addr_config_infos) {
        cJSON *additional_ecs_addr_config_infos_local = NULL;
        if (!cJSON_IsArray(additional_ecs_addr_config_infos)) {
            ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [additional_ecs_addr_config_infos]");
            goto end;
        }

        additional_ecs_addr_config_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_ecs_addr_config_infos_local, additional_ecs_addr_config_infos) {
            if (!cJSON_IsObject(additional_ecs_addr_config_infos_local)) {
                ogs_error("OpenAPI_pp_data_entry_parseFromJSON() failed [additional_ecs_addr_config_infos]");
                goto end;
            }
            OpenAPI_ecs_addr_config_info_1_t *additional_ecs_addr_config_infosItem = OpenAPI_ecs_addr_config_info_1_parseFromJSON(additional_ecs_addr_config_infos_local);
            if (!additional_ecs_addr_config_infosItem) {
                ogs_error("No additional_ecs_addr_config_infosItem");
                goto end;
            }
            OpenAPI_list_add(additional_ecs_addr_config_infosList, additional_ecs_addr_config_infosItem);
        }
    }

    ec_restriction = cJSON_GetObjectItemCaseSensitive(pp_data_entryJSON, "ecRestriction");
    if (ec_restriction) {
    if (!cJSON_IsNull(ec_restriction)) {
    ec_restriction_local_nonprim = OpenAPI_ec_restriction_1_parseFromJSON(ec_restriction);
    if (!ec_restriction_local_nonprim) {
        ogs_error("OpenAPI_ec_restriction_1_parseFromJSON failed [ec_restriction]");
        goto end;
    }
    }
    }

    pp_data_entry_local_var = OpenAPI_pp_data_entry_create (
        communication_characteristics && cJSON_IsNull(communication_characteristics) ? true : false,
        communication_characteristics ? communication_characteristics_local_nonprim : NULL,
        reference_id ? true : false,
        reference_id ? reference_id->valuedouble : 0,
        validity_time && !cJSON_IsNull(validity_time) ? ogs_strdup(validity_time->valuestring) : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        ecs_addr_config_info && cJSON_IsNull(ecs_addr_config_info) ? true : false,
        ecs_addr_config_info ? ecs_addr_config_info_local_nonprim : NULL,
        additional_ecs_addr_config_infos ? additional_ecs_addr_config_infosList : NULL,
        ec_restriction && cJSON_IsNull(ec_restriction) ? true : false,
        ec_restriction ? ec_restriction_local_nonprim : NULL
    );

    return pp_data_entry_local_var;
end:
    if (communication_characteristics_local_nonprim) {
        OpenAPI_communication_characteristics_af_free(communication_characteristics_local_nonprim);
        communication_characteristics_local_nonprim = NULL;
    }
    if (ecs_addr_config_info_local_nonprim) {
        OpenAPI_ecs_addr_config_info_1_free(ecs_addr_config_info_local_nonprim);
        ecs_addr_config_info_local_nonprim = NULL;
    }
    if (additional_ecs_addr_config_infosList) {
        OpenAPI_list_for_each(additional_ecs_addr_config_infosList, node) {
            OpenAPI_ecs_addr_config_info_1_free(node->data);
        }
        OpenAPI_list_free(additional_ecs_addr_config_infosList);
        additional_ecs_addr_config_infosList = NULL;
    }
    if (ec_restriction_local_nonprim) {
        OpenAPI_ec_restriction_1_free(ec_restriction_local_nonprim);
        ec_restriction_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pp_data_entry_t *OpenAPI_pp_data_entry_copy(OpenAPI_pp_data_entry_t *dst, OpenAPI_pp_data_entry_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pp_data_entry_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pp_data_entry_convertToJSON() failed");
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

    OpenAPI_pp_data_entry_free(dst);
    dst = OpenAPI_pp_data_entry_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

