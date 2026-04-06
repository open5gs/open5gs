
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "vn_group_data_1.h"

OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_create(
    OpenAPI_pdu_session_types_1_t *pdu_session_types,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    OpenAPI_list_t *app_descriptors,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_1_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn,
    bool is__5g_vn_group_communication_ind,
    int _5g_vn_group_communication_ind,
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_type,
    OpenAPI_max_group_data_rate_t *max_group_data_rate
)
{
    OpenAPI_vn_group_data_1_t *vn_group_data_1_local_var = ogs_malloc(sizeof(OpenAPI_vn_group_data_1_t));
    ogs_assert(vn_group_data_1_local_var);

    vn_group_data_1_local_var->pdu_session_types = pdu_session_types;
    vn_group_data_1_local_var->dnn = dnn;
    vn_group_data_1_local_var->single_nssai = single_nssai;
    vn_group_data_1_local_var->app_descriptors = app_descriptors;
    vn_group_data_1_local_var->is_secondary_auth = is_secondary_auth;
    vn_group_data_1_local_var->secondary_auth = secondary_auth;
    vn_group_data_1_local_var->is_dn_aaa_ip_address_allocation = is_dn_aaa_ip_address_allocation;
    vn_group_data_1_local_var->dn_aaa_ip_address_allocation = dn_aaa_ip_address_allocation;
    vn_group_data_1_local_var->dn_aaa_address = dn_aaa_address;
    vn_group_data_1_local_var->additional_dn_aaa_addresses = additional_dn_aaa_addresses;
    vn_group_data_1_local_var->dn_aaa_fqdn = dn_aaa_fqdn;
    vn_group_data_1_local_var->is__5g_vn_group_communication_ind = is__5g_vn_group_communication_ind;
    vn_group_data_1_local_var->_5g_vn_group_communication_ind = _5g_vn_group_communication_ind;
    vn_group_data_1_local_var->_5g_vn_group_communication_type = _5g_vn_group_communication_type;
    vn_group_data_1_local_var->max_group_data_rate = max_group_data_rate;

    return vn_group_data_1_local_var;
}

void OpenAPI_vn_group_data_1_free(OpenAPI_vn_group_data_1_t *vn_group_data_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == vn_group_data_1) {
        return;
    }
    if (vn_group_data_1->pdu_session_types) {
        OpenAPI_pdu_session_types_1_free(vn_group_data_1->pdu_session_types);
        vn_group_data_1->pdu_session_types = NULL;
    }
    if (vn_group_data_1->dnn) {
        ogs_free(vn_group_data_1->dnn);
        vn_group_data_1->dnn = NULL;
    }
    if (vn_group_data_1->single_nssai) {
        OpenAPI_snssai_free(vn_group_data_1->single_nssai);
        vn_group_data_1->single_nssai = NULL;
    }
    if (vn_group_data_1->app_descriptors) {
        OpenAPI_list_for_each(vn_group_data_1->app_descriptors, node) {
            OpenAPI_app_descriptor_1_free(node->data);
        }
        OpenAPI_list_free(vn_group_data_1->app_descriptors);
        vn_group_data_1->app_descriptors = NULL;
    }
    if (vn_group_data_1->dn_aaa_address) {
        OpenAPI_ip_address_1_free(vn_group_data_1->dn_aaa_address);
        vn_group_data_1->dn_aaa_address = NULL;
    }
    if (vn_group_data_1->additional_dn_aaa_addresses) {
        OpenAPI_list_for_each(vn_group_data_1->additional_dn_aaa_addresses, node) {
            OpenAPI_ip_address_1_free(node->data);
        }
        OpenAPI_list_free(vn_group_data_1->additional_dn_aaa_addresses);
        vn_group_data_1->additional_dn_aaa_addresses = NULL;
    }
    if (vn_group_data_1->dn_aaa_fqdn) {
        ogs_free(vn_group_data_1->dn_aaa_fqdn);
        vn_group_data_1->dn_aaa_fqdn = NULL;
    }
    if (vn_group_data_1->max_group_data_rate) {
        OpenAPI_max_group_data_rate_free(vn_group_data_1->max_group_data_rate);
        vn_group_data_1->max_group_data_rate = NULL;
    }
    ogs_free(vn_group_data_1);
}

cJSON *OpenAPI_vn_group_data_1_convertToJSON(OpenAPI_vn_group_data_1_t *vn_group_data_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (vn_group_data_1 == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [VnGroupData_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (vn_group_data_1->pdu_session_types) {
    cJSON *pdu_session_types_local_JSON = OpenAPI_pdu_session_types_1_convertToJSON(vn_group_data_1->pdu_session_types);
    if (pdu_session_types_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionTypes", pdu_session_types_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    }

    if (vn_group_data_1->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", vn_group_data_1->dnn) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (vn_group_data_1->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(vn_group_data_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (vn_group_data_1->app_descriptors) {
    cJSON *app_descriptorsList = cJSON_AddArrayToObject(item, "appDescriptors");
    if (app_descriptorsList == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [app_descriptors]");
        goto end;
    }
    OpenAPI_list_for_each(vn_group_data_1->app_descriptors, node) {
        cJSON *itemLocal = OpenAPI_app_descriptor_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [app_descriptors]");
            goto end;
        }
        cJSON_AddItemToArray(app_descriptorsList, itemLocal);
    }
    }

    if (vn_group_data_1->is_secondary_auth) {
    if (cJSON_AddBoolToObject(item, "secondaryAuth", vn_group_data_1->secondary_auth) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [secondary_auth]");
        goto end;
    }
    }

    if (vn_group_data_1->is_dn_aaa_ip_address_allocation) {
    if (cJSON_AddBoolToObject(item, "dnAaaIpAddressAllocation", vn_group_data_1->dn_aaa_ip_address_allocation) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    if (vn_group_data_1->dn_aaa_address) {
    cJSON *dn_aaa_address_local_JSON = OpenAPI_ip_address_1_convertToJSON(vn_group_data_1->dn_aaa_address);
    if (dn_aaa_address_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnAaaAddress", dn_aaa_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    }

    if (vn_group_data_1->additional_dn_aaa_addresses) {
    cJSON *additional_dn_aaa_addressesList = cJSON_AddArrayToObject(item, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addressesList == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [additional_dn_aaa_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(vn_group_data_1->additional_dn_aaa_addresses, node) {
        cJSON *itemLocal = OpenAPI_ip_address_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(additional_dn_aaa_addressesList, itemLocal);
    }
    }

    if (vn_group_data_1->dn_aaa_fqdn) {
    if (cJSON_AddStringToObject(item, "dnAaaFqdn", vn_group_data_1->dn_aaa_fqdn) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

    if (vn_group_data_1->is__5g_vn_group_communication_ind) {
    if (cJSON_AddBoolToObject(item, "5gVnGroupCommunicationInd", vn_group_data_1->_5g_vn_group_communication_ind) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [_5g_vn_group_communication_ind]");
        goto end;
    }
    }

    if (vn_group_data_1->_5g_vn_group_communication_type != OpenAPI_model_5_gvn_group_communication_type_NULL) {
    if (cJSON_AddStringToObject(item, "5gVnGroupCommunicationType", OpenAPI_model_5_gvn_group_communication_type_ToString(vn_group_data_1->_5g_vn_group_communication_type)) == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [_5g_vn_group_communication_type]");
        goto end;
    }
    }

    if (vn_group_data_1->max_group_data_rate) {
    cJSON *max_group_data_rate_local_JSON = OpenAPI_max_group_data_rate_convertToJSON(vn_group_data_1->max_group_data_rate);
    if (max_group_data_rate_local_JSON == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [max_group_data_rate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "maxGroupDataRate", max_group_data_rate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed [max_group_data_rate]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_parseFromJSON(cJSON *vn_group_data_1JSON)
{
    OpenAPI_vn_group_data_1_t *vn_group_data_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_pdu_session_types_1_t *pdu_session_types_local_nonprim = NULL;
    cJSON *dnn = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *app_descriptors = NULL;
    OpenAPI_list_t *app_descriptorsList = NULL;
    cJSON *secondary_auth = NULL;
    cJSON *dn_aaa_ip_address_allocation = NULL;
    cJSON *dn_aaa_address = NULL;
    OpenAPI_ip_address_1_t *dn_aaa_address_local_nonprim = NULL;
    cJSON *additional_dn_aaa_addresses = NULL;
    OpenAPI_list_t *additional_dn_aaa_addressesList = NULL;
    cJSON *dn_aaa_fqdn = NULL;
    cJSON *_5g_vn_group_communication_ind = NULL;
    cJSON *_5g_vn_group_communication_type = NULL;
    OpenAPI_model_5_gvn_group_communication_type_e _5g_vn_group_communication_typeVariable = 0;
    cJSON *max_group_data_rate = NULL;
    OpenAPI_max_group_data_rate_t *max_group_data_rate_local_nonprim = NULL;
    pdu_session_types = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "pduSessionTypes");
    if (pdu_session_types) {
    pdu_session_types_local_nonprim = OpenAPI_pdu_session_types_1_parseFromJSON(pdu_session_types);
    if (!pdu_session_types_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_types_1_parseFromJSON failed [pdu_session_types]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    app_descriptors = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "appDescriptors");
    if (app_descriptors) {
        cJSON *app_descriptors_local = NULL;
        if (!cJSON_IsArray(app_descriptors)) {
            ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [app_descriptors]");
            goto end;
        }

        app_descriptorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_descriptors_local, app_descriptors) {
            if (!cJSON_IsObject(app_descriptors_local)) {
                ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [app_descriptors]");
                goto end;
            }
            OpenAPI_app_descriptor_1_t *app_descriptorsItem = OpenAPI_app_descriptor_1_parseFromJSON(app_descriptors_local);
            if (!app_descriptorsItem) {
                ogs_error("No app_descriptorsItem");
                goto end;
            }
            OpenAPI_list_add(app_descriptorsList, app_descriptorsItem);
        }
    }

    secondary_auth = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "secondaryAuth");
    if (secondary_auth) {
    if (!cJSON_IsBool(secondary_auth)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [secondary_auth]");
        goto end;
    }
    }

    dn_aaa_ip_address_allocation = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "dnAaaIpAddressAllocation");
    if (dn_aaa_ip_address_allocation) {
    if (!cJSON_IsBool(dn_aaa_ip_address_allocation)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    dn_aaa_address = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "dnAaaAddress");
    if (dn_aaa_address) {
    dn_aaa_address_local_nonprim = OpenAPI_ip_address_1_parseFromJSON(dn_aaa_address);
    if (!dn_aaa_address_local_nonprim) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON failed [dn_aaa_address]");
        goto end;
    }
    }

    additional_dn_aaa_addresses = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addresses) {
        cJSON *additional_dn_aaa_addresses_local = NULL;
        if (!cJSON_IsArray(additional_dn_aaa_addresses)) {
            ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }

        additional_dn_aaa_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_dn_aaa_addresses_local, additional_dn_aaa_addresses) {
            if (!cJSON_IsObject(additional_dn_aaa_addresses_local)) {
                ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [additional_dn_aaa_addresses]");
                goto end;
            }
            OpenAPI_ip_address_1_t *additional_dn_aaa_addressesItem = OpenAPI_ip_address_1_parseFromJSON(additional_dn_aaa_addresses_local);
            if (!additional_dn_aaa_addressesItem) {
                ogs_error("No additional_dn_aaa_addressesItem");
                goto end;
            }
            OpenAPI_list_add(additional_dn_aaa_addressesList, additional_dn_aaa_addressesItem);
        }
    }

    dn_aaa_fqdn = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "dnAaaFqdn");
    if (dn_aaa_fqdn) {
    if (!cJSON_IsString(dn_aaa_fqdn) && !cJSON_IsNull(dn_aaa_fqdn)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

    _5g_vn_group_communication_ind = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "5gVnGroupCommunicationInd");
    if (_5g_vn_group_communication_ind) {
    if (!cJSON_IsBool(_5g_vn_group_communication_ind)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [_5g_vn_group_communication_ind]");
        goto end;
    }
    }

    _5g_vn_group_communication_type = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "5gVnGroupCommunicationType");
    if (_5g_vn_group_communication_type) {
    if (!cJSON_IsString(_5g_vn_group_communication_type)) {
        ogs_error("OpenAPI_vn_group_data_1_parseFromJSON() failed [_5g_vn_group_communication_type]");
        goto end;
    }
    _5g_vn_group_communication_typeVariable = OpenAPI_model_5_gvn_group_communication_type_FromString(_5g_vn_group_communication_type->valuestring);
    }

    max_group_data_rate = cJSON_GetObjectItemCaseSensitive(vn_group_data_1JSON, "maxGroupDataRate");
    if (max_group_data_rate) {
    max_group_data_rate_local_nonprim = OpenAPI_max_group_data_rate_parseFromJSON(max_group_data_rate);
    if (!max_group_data_rate_local_nonprim) {
        ogs_error("OpenAPI_max_group_data_rate_parseFromJSON failed [max_group_data_rate]");
        goto end;
    }
    }

    vn_group_data_1_local_var = OpenAPI_vn_group_data_1_create (
        pdu_session_types ? pdu_session_types_local_nonprim : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        app_descriptors ? app_descriptorsList : NULL,
        secondary_auth ? true : false,
        secondary_auth ? secondary_auth->valueint : 0,
        dn_aaa_ip_address_allocation ? true : false,
        dn_aaa_ip_address_allocation ? dn_aaa_ip_address_allocation->valueint : 0,
        dn_aaa_address ? dn_aaa_address_local_nonprim : NULL,
        additional_dn_aaa_addresses ? additional_dn_aaa_addressesList : NULL,
        dn_aaa_fqdn && !cJSON_IsNull(dn_aaa_fqdn) ? ogs_strdup(dn_aaa_fqdn->valuestring) : NULL,
        _5g_vn_group_communication_ind ? true : false,
        _5g_vn_group_communication_ind ? _5g_vn_group_communication_ind->valueint : 0,
        _5g_vn_group_communication_type ? _5g_vn_group_communication_typeVariable : 0,
        max_group_data_rate ? max_group_data_rate_local_nonprim : NULL
    );

    return vn_group_data_1_local_var;
end:
    if (pdu_session_types_local_nonprim) {
        OpenAPI_pdu_session_types_1_free(pdu_session_types_local_nonprim);
        pdu_session_types_local_nonprim = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (app_descriptorsList) {
        OpenAPI_list_for_each(app_descriptorsList, node) {
            OpenAPI_app_descriptor_1_free(node->data);
        }
        OpenAPI_list_free(app_descriptorsList);
        app_descriptorsList = NULL;
    }
    if (dn_aaa_address_local_nonprim) {
        OpenAPI_ip_address_1_free(dn_aaa_address_local_nonprim);
        dn_aaa_address_local_nonprim = NULL;
    }
    if (additional_dn_aaa_addressesList) {
        OpenAPI_list_for_each(additional_dn_aaa_addressesList, node) {
            OpenAPI_ip_address_1_free(node->data);
        }
        OpenAPI_list_free(additional_dn_aaa_addressesList);
        additional_dn_aaa_addressesList = NULL;
    }
    if (max_group_data_rate_local_nonprim) {
        OpenAPI_max_group_data_rate_free(max_group_data_rate_local_nonprim);
        max_group_data_rate_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_vn_group_data_1_t *OpenAPI_vn_group_data_1_copy(OpenAPI_vn_group_data_1_t *dst, OpenAPI_vn_group_data_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_vn_group_data_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_vn_group_data_1_convertToJSON() failed");
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

    OpenAPI_vn_group_data_1_free(dst);
    dst = OpenAPI_vn_group_data_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

