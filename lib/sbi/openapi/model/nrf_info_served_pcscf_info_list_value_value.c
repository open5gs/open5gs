
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nrf_info_served_pcscf_info_list_value_value.h"

OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_create(
    OpenAPI_list_t *access_type,
    OpenAPI_list_t *dnn_list,
    char *gm_fqdn,
    OpenAPI_list_t *gm_ipv4_addresses,
    OpenAPI_list_t *gm_ipv6_addresses,
    char *mw_fqdn,
    OpenAPI_list_t *mw_ipv4_addresses,
    OpenAPI_list_t *mw_ipv6_addresses,
    OpenAPI_list_t *served_ipv4_address_ranges,
    OpenAPI_list_t *served_ipv6_prefix_ranges,
    OpenAPI_list_t *supi_ranges,
    OpenAPI_list_t *gpsi_ranges,
    char *group_id,
    OpenAPI_list_t *serving_plmns
)
{
    OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value_local_var = ogs_malloc(sizeof(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t));
    ogs_assert(nrf_info_served_pcscf_info_list_value_value_local_var);

    nrf_info_served_pcscf_info_list_value_value_local_var->access_type = access_type;
    nrf_info_served_pcscf_info_list_value_value_local_var->dnn_list = dnn_list;
    nrf_info_served_pcscf_info_list_value_value_local_var->gm_fqdn = gm_fqdn;
    nrf_info_served_pcscf_info_list_value_value_local_var->gm_ipv4_addresses = gm_ipv4_addresses;
    nrf_info_served_pcscf_info_list_value_value_local_var->gm_ipv6_addresses = gm_ipv6_addresses;
    nrf_info_served_pcscf_info_list_value_value_local_var->mw_fqdn = mw_fqdn;
    nrf_info_served_pcscf_info_list_value_value_local_var->mw_ipv4_addresses = mw_ipv4_addresses;
    nrf_info_served_pcscf_info_list_value_value_local_var->mw_ipv6_addresses = mw_ipv6_addresses;
    nrf_info_served_pcscf_info_list_value_value_local_var->served_ipv4_address_ranges = served_ipv4_address_ranges;
    nrf_info_served_pcscf_info_list_value_value_local_var->served_ipv6_prefix_ranges = served_ipv6_prefix_ranges;
    nrf_info_served_pcscf_info_list_value_value_local_var->supi_ranges = supi_ranges;
    nrf_info_served_pcscf_info_list_value_value_local_var->gpsi_ranges = gpsi_ranges;
    nrf_info_served_pcscf_info_list_value_value_local_var->group_id = group_id;
    nrf_info_served_pcscf_info_list_value_value_local_var->serving_plmns = serving_plmns;

    return nrf_info_served_pcscf_info_list_value_value_local_var;
}

void OpenAPI_nrf_info_served_pcscf_info_list_value_value_free(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nrf_info_served_pcscf_info_list_value_value) {
        return;
    }
    if (nrf_info_served_pcscf_info_list_value_value->access_type) {
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->access_type);
        nrf_info_served_pcscf_info_list_value_value->access_type = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->dnn_list) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->dnn_list);
        nrf_info_served_pcscf_info_list_value_value->dnn_list = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->gm_fqdn) {
        ogs_free(nrf_info_served_pcscf_info_list_value_value->gm_fqdn);
        nrf_info_served_pcscf_info_list_value_value->gm_fqdn = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses);
        nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses);
        nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->mw_fqdn) {
        ogs_free(nrf_info_served_pcscf_info_list_value_value->mw_fqdn);
        nrf_info_served_pcscf_info_list_value_value->mw_fqdn = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses);
        nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses);
        nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges);
        nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges);
        nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->supi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->supi_ranges, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->supi_ranges);
        nrf_info_served_pcscf_info_list_value_value->supi_ranges = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->gpsi_ranges) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gpsi_ranges, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->gpsi_ranges);
        nrf_info_served_pcscf_info_list_value_value->gpsi_ranges = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->group_id) {
        ogs_free(nrf_info_served_pcscf_info_list_value_value->group_id);
        nrf_info_served_pcscf_info_list_value_value->group_id = NULL;
    }
    if (nrf_info_served_pcscf_info_list_value_value->serving_plmns) {
        OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->serving_plmns, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(nrf_info_served_pcscf_info_list_value_value->serving_plmns);
        nrf_info_served_pcscf_info_list_value_value->serving_plmns = NULL;
    }
    ogs_free(nrf_info_served_pcscf_info_list_value_value);
}

cJSON *OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nrf_info_served_pcscf_info_list_value_value == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [NrfInfo_servedPcscfInfoList_value_value]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nrf_info_served_pcscf_info_list_value_value->access_type != OpenAPI_access_type_NULL) {
    cJSON *access_typeList = cJSON_AddArrayToObject(item, "accessType");
    if (access_typeList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [access_type]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->access_type, node) {
        if (cJSON_AddStringToObject(access_typeList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [access_type]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->gm_fqdn) {
    if (cJSON_AddStringToObject(item, "gmFqdn", nrf_info_served_pcscf_info_list_value_value->gm_fqdn) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gm_fqdn]");
        goto end;
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses) {
    cJSON *gm_ipv4_addressesList = cJSON_AddArrayToObject(item, "gmIpv4Addresses");
    if (gm_ipv4_addressesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gm_ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gm_ipv4_addresses, node) {
        if (cJSON_AddStringToObject(gm_ipv4_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gm_ipv4_addresses]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses) {
    cJSON *gm_ipv6_addressesList = cJSON_AddArrayToObject(item, "gmIpv6Addresses");
    if (gm_ipv6_addressesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gm_ipv6_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gm_ipv6_addresses, node) {
        if (cJSON_AddStringToObject(gm_ipv6_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gm_ipv6_addresses]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->mw_fqdn) {
    if (cJSON_AddStringToObject(item, "mwFqdn", nrf_info_served_pcscf_info_list_value_value->mw_fqdn) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [mw_fqdn]");
        goto end;
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses) {
    cJSON *mw_ipv4_addressesList = cJSON_AddArrayToObject(item, "mwIpv4Addresses");
    if (mw_ipv4_addressesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [mw_ipv4_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->mw_ipv4_addresses, node) {
        if (cJSON_AddStringToObject(mw_ipv4_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [mw_ipv4_addresses]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses) {
    cJSON *mw_ipv6_addressesList = cJSON_AddArrayToObject(item, "mwIpv6Addresses");
    if (mw_ipv6_addressesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [mw_ipv6_addresses]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->mw_ipv6_addresses, node) {
        if (cJSON_AddStringToObject(mw_ipv6_addressesList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [mw_ipv6_addresses]");
            goto end;
        }
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges) {
    cJSON *served_ipv4_address_rangesList = cJSON_AddArrayToObject(item, "servedIpv4AddressRanges");
    if (served_ipv4_address_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [served_ipv4_address_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->served_ipv4_address_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv4_address_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [served_ipv4_address_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(served_ipv4_address_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges) {
    cJSON *served_ipv6_prefix_rangesList = cJSON_AddArrayToObject(item, "servedIpv6PrefixRanges");
    if (served_ipv6_prefix_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [served_ipv6_prefix_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->served_ipv6_prefix_ranges, node) {
        cJSON *itemLocal = OpenAPI_ipv6_prefix_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [served_ipv6_prefix_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(served_ipv6_prefix_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->supi_ranges) {
    cJSON *supi_rangesList = cJSON_AddArrayToObject(item, "supiRanges");
    if (supi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [supi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->supi_ranges, node) {
        cJSON *itemLocal = OpenAPI_supi_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [supi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(supi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->gpsi_ranges) {
    cJSON *gpsi_rangesList = cJSON_AddArrayToObject(item, "gpsiRanges");
    if (gpsi_rangesList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gpsi_ranges]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->gpsi_ranges, node) {
        cJSON *itemLocal = OpenAPI_identity_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [gpsi_ranges]");
            goto end;
        }
        cJSON_AddItemToArray(gpsi_rangesList, itemLocal);
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->group_id) {
    if (cJSON_AddStringToObject(item, "groupId", nrf_info_served_pcscf_info_list_value_value->group_id) == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [group_id]");
        goto end;
    }
    }

    if (nrf_info_served_pcscf_info_list_value_value->serving_plmns) {
    cJSON *serving_plmnsList = cJSON_AddArrayToObject(item, "servingPlmns");
    if (serving_plmnsList == NULL) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [serving_plmns]");
        goto end;
    }
    OpenAPI_list_for_each(nrf_info_served_pcscf_info_list_value_value->serving_plmns, node) {
        cJSON *itemLocal = OpenAPI_plmn_id_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed [serving_plmns]");
            goto end;
        }
        cJSON_AddItemToArray(serving_plmnsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON(cJSON *nrf_info_served_pcscf_info_list_value_valueJSON)
{
    OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *nrf_info_served_pcscf_info_list_value_value_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *access_type = NULL;
    OpenAPI_list_t *access_typeList = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    cJSON *gm_fqdn = NULL;
    cJSON *gm_ipv4_addresses = NULL;
    OpenAPI_list_t *gm_ipv4_addressesList = NULL;
    cJSON *gm_ipv6_addresses = NULL;
    OpenAPI_list_t *gm_ipv6_addressesList = NULL;
    cJSON *mw_fqdn = NULL;
    cJSON *mw_ipv4_addresses = NULL;
    OpenAPI_list_t *mw_ipv4_addressesList = NULL;
    cJSON *mw_ipv6_addresses = NULL;
    OpenAPI_list_t *mw_ipv6_addressesList = NULL;
    cJSON *served_ipv4_address_ranges = NULL;
    OpenAPI_list_t *served_ipv4_address_rangesList = NULL;
    cJSON *served_ipv6_prefix_ranges = NULL;
    OpenAPI_list_t *served_ipv6_prefix_rangesList = NULL;
    cJSON *supi_ranges = NULL;
    OpenAPI_list_t *supi_rangesList = NULL;
    cJSON *gpsi_ranges = NULL;
    OpenAPI_list_t *gpsi_rangesList = NULL;
    cJSON *group_id = NULL;
    cJSON *serving_plmns = NULL;
    OpenAPI_list_t *serving_plmnsList = NULL;
    access_type = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "accessType");
    if (access_type) {
        cJSON *access_type_local = NULL;
        if (!cJSON_IsArray(access_type)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [access_type]");
            goto end;
        }

        access_typeList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_type_local, access_type) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_type_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [access_type]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_type_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_type\" is not supported. Ignoring it ...",
                         access_type_local->valuestring);
            } else {
                OpenAPI_list_add(access_typeList, (void *)localEnum);
            }
        }
        if (access_typeList->count == 0) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed: Expected access_typeList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    dnn_list = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    gm_fqdn = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "gmFqdn");
    if (gm_fqdn) {
    if (!cJSON_IsString(gm_fqdn) && !cJSON_IsNull(gm_fqdn)) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gm_fqdn]");
        goto end;
    }
    }

    gm_ipv4_addresses = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "gmIpv4Addresses");
    if (gm_ipv4_addresses) {
        cJSON *gm_ipv4_addresses_local = NULL;
        if (!cJSON_IsArray(gm_ipv4_addresses)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gm_ipv4_addresses]");
            goto end;
        }

        gm_ipv4_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gm_ipv4_addresses_local, gm_ipv4_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gm_ipv4_addresses_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gm_ipv4_addresses]");
                goto end;
            }
            OpenAPI_list_add(gm_ipv4_addressesList, ogs_strdup(gm_ipv4_addresses_local->valuestring));
        }
    }

    gm_ipv6_addresses = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "gmIpv6Addresses");
    if (gm_ipv6_addresses) {
        cJSON *gm_ipv6_addresses_local = NULL;
        if (!cJSON_IsArray(gm_ipv6_addresses)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gm_ipv6_addresses]");
            goto end;
        }

        gm_ipv6_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gm_ipv6_addresses_local, gm_ipv6_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(gm_ipv6_addresses_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gm_ipv6_addresses]");
                goto end;
            }
            OpenAPI_list_add(gm_ipv6_addressesList, ogs_strdup(gm_ipv6_addresses_local->valuestring));
        }
    }

    mw_fqdn = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "mwFqdn");
    if (mw_fqdn) {
    if (!cJSON_IsString(mw_fqdn) && !cJSON_IsNull(mw_fqdn)) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [mw_fqdn]");
        goto end;
    }
    }

    mw_ipv4_addresses = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "mwIpv4Addresses");
    if (mw_ipv4_addresses) {
        cJSON *mw_ipv4_addresses_local = NULL;
        if (!cJSON_IsArray(mw_ipv4_addresses)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [mw_ipv4_addresses]");
            goto end;
        }

        mw_ipv4_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(mw_ipv4_addresses_local, mw_ipv4_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mw_ipv4_addresses_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [mw_ipv4_addresses]");
                goto end;
            }
            OpenAPI_list_add(mw_ipv4_addressesList, ogs_strdup(mw_ipv4_addresses_local->valuestring));
        }
    }

    mw_ipv6_addresses = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "mwIpv6Addresses");
    if (mw_ipv6_addresses) {
        cJSON *mw_ipv6_addresses_local = NULL;
        if (!cJSON_IsArray(mw_ipv6_addresses)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [mw_ipv6_addresses]");
            goto end;
        }

        mw_ipv6_addressesList = OpenAPI_list_create();

        cJSON_ArrayForEach(mw_ipv6_addresses_local, mw_ipv6_addresses) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(mw_ipv6_addresses_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [mw_ipv6_addresses]");
                goto end;
            }
            OpenAPI_list_add(mw_ipv6_addressesList, ogs_strdup(mw_ipv6_addresses_local->valuestring));
        }
    }

    served_ipv4_address_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "servedIpv4AddressRanges");
    if (served_ipv4_address_ranges) {
        cJSON *served_ipv4_address_ranges_local = NULL;
        if (!cJSON_IsArray(served_ipv4_address_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [served_ipv4_address_ranges]");
            goto end;
        }

        served_ipv4_address_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(served_ipv4_address_ranges_local, served_ipv4_address_ranges) {
            if (!cJSON_IsObject(served_ipv4_address_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [served_ipv4_address_ranges]");
                goto end;
            }
            OpenAPI_ipv4_address_range_t *served_ipv4_address_rangesItem = OpenAPI_ipv4_address_range_parseFromJSON(served_ipv4_address_ranges_local);
            if (!served_ipv4_address_rangesItem) {
                ogs_error("No served_ipv4_address_rangesItem");
                goto end;
            }
            OpenAPI_list_add(served_ipv4_address_rangesList, served_ipv4_address_rangesItem);
        }
    }

    served_ipv6_prefix_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "servedIpv6PrefixRanges");
    if (served_ipv6_prefix_ranges) {
        cJSON *served_ipv6_prefix_ranges_local = NULL;
        if (!cJSON_IsArray(served_ipv6_prefix_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [served_ipv6_prefix_ranges]");
            goto end;
        }

        served_ipv6_prefix_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(served_ipv6_prefix_ranges_local, served_ipv6_prefix_ranges) {
            if (!cJSON_IsObject(served_ipv6_prefix_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [served_ipv6_prefix_ranges]");
                goto end;
            }
            OpenAPI_ipv6_prefix_range_t *served_ipv6_prefix_rangesItem = OpenAPI_ipv6_prefix_range_parseFromJSON(served_ipv6_prefix_ranges_local);
            if (!served_ipv6_prefix_rangesItem) {
                ogs_error("No served_ipv6_prefix_rangesItem");
                goto end;
            }
            OpenAPI_list_add(served_ipv6_prefix_rangesList, served_ipv6_prefix_rangesItem);
        }
    }

    supi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "supiRanges");
    if (supi_ranges) {
        cJSON *supi_ranges_local = NULL;
        if (!cJSON_IsArray(supi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [supi_ranges]");
            goto end;
        }

        supi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supi_ranges_local, supi_ranges) {
            if (!cJSON_IsObject(supi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [supi_ranges]");
                goto end;
            }
            OpenAPI_supi_range_t *supi_rangesItem = OpenAPI_supi_range_parseFromJSON(supi_ranges_local);
            if (!supi_rangesItem) {
                ogs_error("No supi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(supi_rangesList, supi_rangesItem);
        }
    }

    gpsi_ranges = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "gpsiRanges");
    if (gpsi_ranges) {
        cJSON *gpsi_ranges_local = NULL;
        if (!cJSON_IsArray(gpsi_ranges)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gpsi_ranges]");
            goto end;
        }

        gpsi_rangesList = OpenAPI_list_create();

        cJSON_ArrayForEach(gpsi_ranges_local, gpsi_ranges) {
            if (!cJSON_IsObject(gpsi_ranges_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [gpsi_ranges]");
                goto end;
            }
            OpenAPI_identity_range_t *gpsi_rangesItem = OpenAPI_identity_range_parseFromJSON(gpsi_ranges_local);
            if (!gpsi_rangesItem) {
                ogs_error("No gpsi_rangesItem");
                goto end;
            }
            OpenAPI_list_add(gpsi_rangesList, gpsi_rangesItem);
        }
    }

    group_id = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "groupId");
    if (group_id) {
    if (!cJSON_IsString(group_id) && !cJSON_IsNull(group_id)) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [group_id]");
        goto end;
    }
    }

    serving_plmns = cJSON_GetObjectItemCaseSensitive(nrf_info_served_pcscf_info_list_value_valueJSON, "servingPlmns");
    if (serving_plmns) {
        cJSON *serving_plmns_local = NULL;
        if (!cJSON_IsArray(serving_plmns)) {
            ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [serving_plmns]");
            goto end;
        }

        serving_plmnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_plmns_local, serving_plmns) {
            if (!cJSON_IsObject(serving_plmns_local)) {
                ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON() failed [serving_plmns]");
                goto end;
            }
            OpenAPI_plmn_id_t *serving_plmnsItem = OpenAPI_plmn_id_parseFromJSON(serving_plmns_local);
            if (!serving_plmnsItem) {
                ogs_error("No serving_plmnsItem");
                goto end;
            }
            OpenAPI_list_add(serving_plmnsList, serving_plmnsItem);
        }
    }

    nrf_info_served_pcscf_info_list_value_value_local_var = OpenAPI_nrf_info_served_pcscf_info_list_value_value_create (
        access_type ? access_typeList : NULL,
        dnn_list ? dnn_listList : NULL,
        gm_fqdn && !cJSON_IsNull(gm_fqdn) ? ogs_strdup(gm_fqdn->valuestring) : NULL,
        gm_ipv4_addresses ? gm_ipv4_addressesList : NULL,
        gm_ipv6_addresses ? gm_ipv6_addressesList : NULL,
        mw_fqdn && !cJSON_IsNull(mw_fqdn) ? ogs_strdup(mw_fqdn->valuestring) : NULL,
        mw_ipv4_addresses ? mw_ipv4_addressesList : NULL,
        mw_ipv6_addresses ? mw_ipv6_addressesList : NULL,
        served_ipv4_address_ranges ? served_ipv4_address_rangesList : NULL,
        served_ipv6_prefix_ranges ? served_ipv6_prefix_rangesList : NULL,
        supi_ranges ? supi_rangesList : NULL,
        gpsi_ranges ? gpsi_rangesList : NULL,
        group_id && !cJSON_IsNull(group_id) ? ogs_strdup(group_id->valuestring) : NULL,
        serving_plmns ? serving_plmnsList : NULL
    );

    return nrf_info_served_pcscf_info_list_value_value_local_var;
end:
    if (access_typeList) {
        OpenAPI_list_free(access_typeList);
        access_typeList = NULL;
    }
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    if (gm_ipv4_addressesList) {
        OpenAPI_list_for_each(gm_ipv4_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gm_ipv4_addressesList);
        gm_ipv4_addressesList = NULL;
    }
    if (gm_ipv6_addressesList) {
        OpenAPI_list_for_each(gm_ipv6_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(gm_ipv6_addressesList);
        gm_ipv6_addressesList = NULL;
    }
    if (mw_ipv4_addressesList) {
        OpenAPI_list_for_each(mw_ipv4_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mw_ipv4_addressesList);
        mw_ipv4_addressesList = NULL;
    }
    if (mw_ipv6_addressesList) {
        OpenAPI_list_for_each(mw_ipv6_addressesList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(mw_ipv6_addressesList);
        mw_ipv6_addressesList = NULL;
    }
    if (served_ipv4_address_rangesList) {
        OpenAPI_list_for_each(served_ipv4_address_rangesList, node) {
            OpenAPI_ipv4_address_range_free(node->data);
        }
        OpenAPI_list_free(served_ipv4_address_rangesList);
        served_ipv4_address_rangesList = NULL;
    }
    if (served_ipv6_prefix_rangesList) {
        OpenAPI_list_for_each(served_ipv6_prefix_rangesList, node) {
            OpenAPI_ipv6_prefix_range_free(node->data);
        }
        OpenAPI_list_free(served_ipv6_prefix_rangesList);
        served_ipv6_prefix_rangesList = NULL;
    }
    if (supi_rangesList) {
        OpenAPI_list_for_each(supi_rangesList, node) {
            OpenAPI_supi_range_free(node->data);
        }
        OpenAPI_list_free(supi_rangesList);
        supi_rangesList = NULL;
    }
    if (gpsi_rangesList) {
        OpenAPI_list_for_each(gpsi_rangesList, node) {
            OpenAPI_identity_range_free(node->data);
        }
        OpenAPI_list_free(gpsi_rangesList);
        gpsi_rangesList = NULL;
    }
    if (serving_plmnsList) {
        OpenAPI_list_for_each(serving_plmnsList, node) {
            OpenAPI_plmn_id_free(node->data);
        }
        OpenAPI_list_free(serving_plmnsList);
        serving_plmnsList = NULL;
    }
    return NULL;
}

OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *OpenAPI_nrf_info_served_pcscf_info_list_value_value_copy(OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *dst, OpenAPI_nrf_info_served_pcscf_info_list_value_value_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nrf_info_served_pcscf_info_list_value_value_convertToJSON() failed");
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

    OpenAPI_nrf_info_served_pcscf_info_list_value_value_free(dst);
    dst = OpenAPI_nrf_info_served_pcscf_info_list_value_value_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

