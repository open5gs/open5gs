
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_gvn_group_configuration.h"

OpenAPI_model_5_gvn_group_configuration_t *OpenAPI_model_5_gvn_group_configuration_create(
    OpenAPI_model_5_gvn_group_data_t *_5g_vn_group_data,
    OpenAPI_list_t *members,
    bool is_reference_id,
    int reference_id,
    char *af_instance_id,
    char *internal_group_identifier,
    char *mtc_provider_information
)
{
    OpenAPI_model_5_gvn_group_configuration_t *model_5_gvn_group_configuration_local_var = ogs_malloc(sizeof(OpenAPI_model_5_gvn_group_configuration_t));
    ogs_assert(model_5_gvn_group_configuration_local_var);

    model_5_gvn_group_configuration_local_var->_5g_vn_group_data = _5g_vn_group_data;
    model_5_gvn_group_configuration_local_var->members = members;
    model_5_gvn_group_configuration_local_var->is_reference_id = is_reference_id;
    model_5_gvn_group_configuration_local_var->reference_id = reference_id;
    model_5_gvn_group_configuration_local_var->af_instance_id = af_instance_id;
    model_5_gvn_group_configuration_local_var->internal_group_identifier = internal_group_identifier;
    model_5_gvn_group_configuration_local_var->mtc_provider_information = mtc_provider_information;

    return model_5_gvn_group_configuration_local_var;
}

void OpenAPI_model_5_gvn_group_configuration_free(OpenAPI_model_5_gvn_group_configuration_t *model_5_gvn_group_configuration)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_gvn_group_configuration) {
        return;
    }
    if (model_5_gvn_group_configuration->_5g_vn_group_data) {
        OpenAPI_model_5_gvn_group_data_free(model_5_gvn_group_configuration->_5g_vn_group_data);
        model_5_gvn_group_configuration->_5g_vn_group_data = NULL;
    }
    if (model_5_gvn_group_configuration->members) {
        OpenAPI_list_for_each(model_5_gvn_group_configuration->members, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(model_5_gvn_group_configuration->members);
        model_5_gvn_group_configuration->members = NULL;
    }
    if (model_5_gvn_group_configuration->af_instance_id) {
        ogs_free(model_5_gvn_group_configuration->af_instance_id);
        model_5_gvn_group_configuration->af_instance_id = NULL;
    }
    if (model_5_gvn_group_configuration->internal_group_identifier) {
        ogs_free(model_5_gvn_group_configuration->internal_group_identifier);
        model_5_gvn_group_configuration->internal_group_identifier = NULL;
    }
    if (model_5_gvn_group_configuration->mtc_provider_information) {
        ogs_free(model_5_gvn_group_configuration->mtc_provider_information);
        model_5_gvn_group_configuration->mtc_provider_information = NULL;
    }
    ogs_free(model_5_gvn_group_configuration);
}

cJSON *OpenAPI_model_5_gvn_group_configuration_convertToJSON(OpenAPI_model_5_gvn_group_configuration_t *model_5_gvn_group_configuration)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_gvn_group_configuration == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [5GVnGroupConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (model_5_gvn_group_configuration->_5g_vn_group_data) {
    cJSON *_5g_vn_group_data_local_JSON = OpenAPI_model_5_gvn_group_data_convertToJSON(model_5_gvn_group_configuration->_5g_vn_group_data);
    if (_5g_vn_group_data_local_JSON == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [_5g_vn_group_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "5gVnGroupData", _5g_vn_group_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [_5g_vn_group_data]");
        goto end;
    }
    }

    if (model_5_gvn_group_configuration->members) {
    cJSON *membersList = cJSON_AddArrayToObject(item, "members");
    if (membersList == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [members]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_gvn_group_configuration->members, node) {
        if (cJSON_AddStringToObject(membersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [members]");
            goto end;
        }
    }
    }

    if (model_5_gvn_group_configuration->is_reference_id) {
    if (cJSON_AddNumberToObject(item, "referenceId", model_5_gvn_group_configuration->reference_id) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [reference_id]");
        goto end;
    }
    }

    if (model_5_gvn_group_configuration->af_instance_id) {
    if (cJSON_AddStringToObject(item, "afInstanceId", model_5_gvn_group_configuration->af_instance_id) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [af_instance_id]");
        goto end;
    }
    }

    if (model_5_gvn_group_configuration->internal_group_identifier) {
    if (cJSON_AddStringToObject(item, "internalGroupIdentifier", model_5_gvn_group_configuration->internal_group_identifier) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    if (model_5_gvn_group_configuration->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", model_5_gvn_group_configuration->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_5_gvn_group_configuration_t *OpenAPI_model_5_gvn_group_configuration_parseFromJSON(cJSON *model_5_gvn_group_configurationJSON)
{
    OpenAPI_model_5_gvn_group_configuration_t *model_5_gvn_group_configuration_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *_5g_vn_group_data = NULL;
    OpenAPI_model_5_gvn_group_data_t *_5g_vn_group_data_local_nonprim = NULL;
    cJSON *members = NULL;
    OpenAPI_list_t *membersList = NULL;
    cJSON *reference_id = NULL;
    cJSON *af_instance_id = NULL;
    cJSON *internal_group_identifier = NULL;
    cJSON *mtc_provider_information = NULL;
    _5g_vn_group_data = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "5gVnGroupData");
    if (_5g_vn_group_data) {
    _5g_vn_group_data_local_nonprim = OpenAPI_model_5_gvn_group_data_parseFromJSON(_5g_vn_group_data);
    if (!_5g_vn_group_data_local_nonprim) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON failed [_5g_vn_group_data]");
        goto end;
    }
    }

    members = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "members");
    if (members) {
        cJSON *members_local = NULL;
        if (!cJSON_IsArray(members)) {
            ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [members]");
            goto end;
        }

        membersList = OpenAPI_list_create();

        cJSON_ArrayForEach(members_local, members) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(members_local)) {
                ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [members]");
                goto end;
            }
            OpenAPI_list_add(membersList, ogs_strdup(members_local->valuestring));
        }
    }

    reference_id = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "referenceId");
    if (reference_id) {
    if (!cJSON_IsNumber(reference_id)) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [reference_id]");
        goto end;
    }
    }

    af_instance_id = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "afInstanceId");
    if (af_instance_id) {
    if (!cJSON_IsString(af_instance_id) && !cJSON_IsNull(af_instance_id)) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [af_instance_id]");
        goto end;
    }
    }

    internal_group_identifier = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "internalGroupIdentifier");
    if (internal_group_identifier) {
    if (!cJSON_IsString(internal_group_identifier) && !cJSON_IsNull(internal_group_identifier)) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [internal_group_identifier]");
        goto end;
    }
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_configurationJSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    model_5_gvn_group_configuration_local_var = OpenAPI_model_5_gvn_group_configuration_create (
        _5g_vn_group_data ? _5g_vn_group_data_local_nonprim : NULL,
        members ? membersList : NULL,
        reference_id ? true : false,
        reference_id ? reference_id->valuedouble : 0,
        af_instance_id && !cJSON_IsNull(af_instance_id) ? ogs_strdup(af_instance_id->valuestring) : NULL,
        internal_group_identifier && !cJSON_IsNull(internal_group_identifier) ? ogs_strdup(internal_group_identifier->valuestring) : NULL,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL
    );

    return model_5_gvn_group_configuration_local_var;
end:
    if (_5g_vn_group_data_local_nonprim) {
        OpenAPI_model_5_gvn_group_data_free(_5g_vn_group_data_local_nonprim);
        _5g_vn_group_data_local_nonprim = NULL;
    }
    if (membersList) {
        OpenAPI_list_for_each(membersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(membersList);
        membersList = NULL;
    }
    return NULL;
}

OpenAPI_model_5_gvn_group_configuration_t *OpenAPI_model_5_gvn_group_configuration_copy(OpenAPI_model_5_gvn_group_configuration_t *dst, OpenAPI_model_5_gvn_group_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_gvn_group_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_gvn_group_configuration_convertToJSON() failed");
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

    OpenAPI_model_5_gvn_group_configuration_free(dst);
    dst = OpenAPI_model_5_gvn_group_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

