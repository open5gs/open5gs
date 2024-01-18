
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "model_5_gvn_group_data.h"

OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_create(
    char *dnn,
    OpenAPI_snssai_t *s_nssai,
    OpenAPI_list_t *pdu_session_types,
    OpenAPI_list_t *app_descriptors,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_1_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn
)
{
    OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data_local_var = ogs_malloc(sizeof(OpenAPI_model_5_gvn_group_data_t));
    ogs_assert(model_5_gvn_group_data_local_var);

    model_5_gvn_group_data_local_var->dnn = dnn;
    model_5_gvn_group_data_local_var->s_nssai = s_nssai;
    model_5_gvn_group_data_local_var->pdu_session_types = pdu_session_types;
    model_5_gvn_group_data_local_var->app_descriptors = app_descriptors;
    model_5_gvn_group_data_local_var->is_secondary_auth = is_secondary_auth;
    model_5_gvn_group_data_local_var->secondary_auth = secondary_auth;
    model_5_gvn_group_data_local_var->is_dn_aaa_ip_address_allocation = is_dn_aaa_ip_address_allocation;
    model_5_gvn_group_data_local_var->dn_aaa_ip_address_allocation = dn_aaa_ip_address_allocation;
    model_5_gvn_group_data_local_var->dn_aaa_address = dn_aaa_address;
    model_5_gvn_group_data_local_var->additional_dn_aaa_addresses = additional_dn_aaa_addresses;
    model_5_gvn_group_data_local_var->dn_aaa_fqdn = dn_aaa_fqdn;

    return model_5_gvn_group_data_local_var;
}

void OpenAPI_model_5_gvn_group_data_free(OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == model_5_gvn_group_data) {
        return;
    }
    if (model_5_gvn_group_data->dnn) {
        ogs_free(model_5_gvn_group_data->dnn);
        model_5_gvn_group_data->dnn = NULL;
    }
    if (model_5_gvn_group_data->s_nssai) {
        OpenAPI_snssai_free(model_5_gvn_group_data->s_nssai);
        model_5_gvn_group_data->s_nssai = NULL;
    }
    if (model_5_gvn_group_data->pdu_session_types) {
        OpenAPI_list_free(model_5_gvn_group_data->pdu_session_types);
        model_5_gvn_group_data->pdu_session_types = NULL;
    }
    if (model_5_gvn_group_data->app_descriptors) {
        OpenAPI_list_for_each(model_5_gvn_group_data->app_descriptors, node) {
            OpenAPI_app_descriptor_free(node->data);
        }
        OpenAPI_list_free(model_5_gvn_group_data->app_descriptors);
        model_5_gvn_group_data->app_descriptors = NULL;
    }
    if (model_5_gvn_group_data->dn_aaa_address) {
        OpenAPI_ip_address_1_free(model_5_gvn_group_data->dn_aaa_address);
        model_5_gvn_group_data->dn_aaa_address = NULL;
    }
    if (model_5_gvn_group_data->additional_dn_aaa_addresses) {
        OpenAPI_list_for_each(model_5_gvn_group_data->additional_dn_aaa_addresses, node) {
            OpenAPI_ip_address_1_free(node->data);
        }
        OpenAPI_list_free(model_5_gvn_group_data->additional_dn_aaa_addresses);
        model_5_gvn_group_data->additional_dn_aaa_addresses = NULL;
    }
    if (model_5_gvn_group_data->dn_aaa_fqdn) {
        ogs_free(model_5_gvn_group_data->dn_aaa_fqdn);
        model_5_gvn_group_data->dn_aaa_fqdn = NULL;
    }
    ogs_free(model_5_gvn_group_data);
}

cJSON *OpenAPI_model_5_gvn_group_data_convertToJSON(OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (model_5_gvn_group_data == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [5GVnGroupData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!model_5_gvn_group_data->dnn) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dnn]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "dnn", model_5_gvn_group_data->dnn) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dnn]");
        goto end;
    }

    if (!model_5_gvn_group_data->s_nssai) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [s_nssai]");
        return NULL;
    }
    cJSON *s_nssai_local_JSON = OpenAPI_snssai_convertToJSON(model_5_gvn_group_data->s_nssai);
    if (s_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [s_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNssai", s_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [s_nssai]");
        goto end;
    }

    if (model_5_gvn_group_data->pdu_session_types != OpenAPI_pdu_session_type_NULL) {
    cJSON *pdu_session_typesList = cJSON_AddArrayToObject(item, "pduSessionTypes");
    if (pdu_session_typesList == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [pdu_session_types]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_gvn_group_data->pdu_session_types, node) {
        if (cJSON_AddStringToObject(pdu_session_typesList, "", OpenAPI_pdu_session_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [pdu_session_types]");
            goto end;
        }
    }
    }

    if (model_5_gvn_group_data->app_descriptors) {
    cJSON *app_descriptorsList = cJSON_AddArrayToObject(item, "appDescriptors");
    if (app_descriptorsList == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [app_descriptors]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_gvn_group_data->app_descriptors, node) {
        cJSON *itemLocal = OpenAPI_app_descriptor_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [app_descriptors]");
            goto end;
        }
        cJSON_AddItemToArray(app_descriptorsList, itemLocal);
    }
    }

    if (model_5_gvn_group_data->is_secondary_auth) {
    if (cJSON_AddBoolToObject(item, "secondaryAuth", model_5_gvn_group_data->secondary_auth) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [secondary_auth]");
        goto end;
    }
    }

    if (model_5_gvn_group_data->is_dn_aaa_ip_address_allocation) {
    if (cJSON_AddBoolToObject(item, "dnAaaIpAddressAllocation", model_5_gvn_group_data->dn_aaa_ip_address_allocation) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    if (model_5_gvn_group_data->dn_aaa_address) {
    cJSON *dn_aaa_address_local_JSON = OpenAPI_ip_address_1_convertToJSON(model_5_gvn_group_data->dn_aaa_address);
    if (dn_aaa_address_local_JSON == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dnAaaAddress", dn_aaa_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dn_aaa_address]");
        goto end;
    }
    }

    if (model_5_gvn_group_data->additional_dn_aaa_addresses) {
    cJSON *additional_dn_aaa_addressesList = cJSON_AddArrayToObject(item, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addressesList == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [additional_dn_aaa_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(model_5_gvn_group_data->additional_dn_aaa_addresses, node) {
        cJSON *itemLocal = OpenAPI_ip_address_1_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }
        cJSON_AddItemToArray(additional_dn_aaa_addressesList, itemLocal);
    }
    }

    if (model_5_gvn_group_data->dn_aaa_fqdn) {
    if (cJSON_AddStringToObject(item, "dnAaaFqdn", model_5_gvn_group_data->dn_aaa_fqdn) == NULL) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_parseFromJSON(cJSON *model_5_gvn_group_dataJSON)
{
    OpenAPI_model_5_gvn_group_data_t *model_5_gvn_group_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn = NULL;
    cJSON *s_nssai = NULL;
    OpenAPI_snssai_t *s_nssai_local_nonprim = NULL;
    cJSON *pdu_session_types = NULL;
    OpenAPI_list_t *pdu_session_typesList = NULL;
    cJSON *app_descriptors = NULL;
    OpenAPI_list_t *app_descriptorsList = NULL;
    cJSON *secondary_auth = NULL;
    cJSON *dn_aaa_ip_address_allocation = NULL;
    cJSON *dn_aaa_address = NULL;
    OpenAPI_ip_address_1_t *dn_aaa_address_local_nonprim = NULL;
    cJSON *additional_dn_aaa_addresses = NULL;
    OpenAPI_list_t *additional_dn_aaa_addressesList = NULL;
    cJSON *dn_aaa_fqdn = NULL;
    dnn = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "dnn");
    if (!dnn) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [dnn]");
        goto end;
    }

    s_nssai = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "sNssai");
    if (!s_nssai) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [s_nssai]");
        goto end;
    }
    s_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(s_nssai);
    if (!s_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [s_nssai]");
        goto end;
    }

    pdu_session_types = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "pduSessionTypes");
    if (pdu_session_types) {
        cJSON *pdu_session_types_local = NULL;
        if (!cJSON_IsArray(pdu_session_types)) {
            ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [pdu_session_types]");
            goto end;
        }

        pdu_session_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(pdu_session_types_local, pdu_session_types) {
            OpenAPI_pdu_session_type_e localEnum = OpenAPI_pdu_session_type_NULL;
            if (!cJSON_IsString(pdu_session_types_local)) {
                ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [pdu_session_types]");
                goto end;
            }
            localEnum = OpenAPI_pdu_session_type_FromString(pdu_session_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"pdu_session_types\" is not supported. Ignoring it ...",
                         pdu_session_types_local->valuestring);
            } else {
                OpenAPI_list_add(pdu_session_typesList, (void *)localEnum);
            }
        }
        if (pdu_session_typesList->count == 0) {
            ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed: Expected pdu_session_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    app_descriptors = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "appDescriptors");
    if (app_descriptors) {
        cJSON *app_descriptors_local = NULL;
        if (!cJSON_IsArray(app_descriptors)) {
            ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [app_descriptors]");
            goto end;
        }

        app_descriptorsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_descriptors_local, app_descriptors) {
            if (!cJSON_IsObject(app_descriptors_local)) {
                ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [app_descriptors]");
                goto end;
            }
            OpenAPI_app_descriptor_t *app_descriptorsItem = OpenAPI_app_descriptor_parseFromJSON(app_descriptors_local);
            if (!app_descriptorsItem) {
                ogs_error("No app_descriptorsItem");
                goto end;
            }
            OpenAPI_list_add(app_descriptorsList, app_descriptorsItem);
        }
    }

    secondary_auth = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "secondaryAuth");
    if (secondary_auth) {
    if (!cJSON_IsBool(secondary_auth)) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [secondary_auth]");
        goto end;
    }
    }

    dn_aaa_ip_address_allocation = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "dnAaaIpAddressAllocation");
    if (dn_aaa_ip_address_allocation) {
    if (!cJSON_IsBool(dn_aaa_ip_address_allocation)) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [dn_aaa_ip_address_allocation]");
        goto end;
    }
    }

    dn_aaa_address = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "dnAaaAddress");
    if (dn_aaa_address) {
    dn_aaa_address_local_nonprim = OpenAPI_ip_address_1_parseFromJSON(dn_aaa_address);
    if (!dn_aaa_address_local_nonprim) {
        ogs_error("OpenAPI_ip_address_1_parseFromJSON failed [dn_aaa_address]");
        goto end;
    }
    }

    additional_dn_aaa_addresses = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "additionalDnAaaAddresses");
    if (additional_dn_aaa_addresses) {
        cJSON *additional_dn_aaa_addresses_local = NULL;
        if (!cJSON_IsArray(additional_dn_aaa_addresses)) {
            ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [additional_dn_aaa_addresses]");
            goto end;
        }

        additional_dn_aaa_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(additional_dn_aaa_addresses_local, additional_dn_aaa_addresses) {
            if (!cJSON_IsObject(additional_dn_aaa_addresses_local)) {
                ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [additional_dn_aaa_addresses]");
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

    dn_aaa_fqdn = cJSON_GetObjectItemCaseSensitive(model_5_gvn_group_dataJSON, "dnAaaFqdn");
    if (dn_aaa_fqdn) {
    if (!cJSON_IsString(dn_aaa_fqdn) && !cJSON_IsNull(dn_aaa_fqdn)) {
        ogs_error("OpenAPI_model_5_gvn_group_data_parseFromJSON() failed [dn_aaa_fqdn]");
        goto end;
    }
    }

    model_5_gvn_group_data_local_var = OpenAPI_model_5_gvn_group_data_create (
        ogs_strdup(dnn->valuestring),
        s_nssai_local_nonprim,
        pdu_session_types ? pdu_session_typesList : NULL,
        app_descriptors ? app_descriptorsList : NULL,
        secondary_auth ? true : false,
        secondary_auth ? secondary_auth->valueint : 0,
        dn_aaa_ip_address_allocation ? true : false,
        dn_aaa_ip_address_allocation ? dn_aaa_ip_address_allocation->valueint : 0,
        dn_aaa_address ? dn_aaa_address_local_nonprim : NULL,
        additional_dn_aaa_addresses ? additional_dn_aaa_addressesList : NULL,
        dn_aaa_fqdn && !cJSON_IsNull(dn_aaa_fqdn) ? ogs_strdup(dn_aaa_fqdn->valuestring) : NULL
    );

    return model_5_gvn_group_data_local_var;
end:
    if (s_nssai_local_nonprim) {
        OpenAPI_snssai_free(s_nssai_local_nonprim);
        s_nssai_local_nonprim = NULL;
    }
    if (pdu_session_typesList) {
        OpenAPI_list_free(pdu_session_typesList);
        pdu_session_typesList = NULL;
    }
    if (app_descriptorsList) {
        OpenAPI_list_for_each(app_descriptorsList, node) {
            OpenAPI_app_descriptor_free(node->data);
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
    return NULL;
}

OpenAPI_model_5_gvn_group_data_t *OpenAPI_model_5_gvn_group_data_copy(OpenAPI_model_5_gvn_group_data_t *dst, OpenAPI_model_5_gvn_group_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_model_5_gvn_group_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_model_5_gvn_group_data_convertToJSON() failed");
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

    OpenAPI_model_5_gvn_group_data_free(dst);
    dst = OpenAPI_model_5_gvn_group_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

