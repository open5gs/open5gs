
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
    OpenAPI_list_t *served_ipv6_prefix_ranges
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
        served_ipv6_prefix_ranges ? served_ipv6_prefix_rangesList : NULL
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

